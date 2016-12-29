#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <unistd.h>
#include "../GameController.h"
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h> 
#include <mqueue.h>
#include <errno.h>

#define PRODUCTID 0x045e
#define VENDORID 0x028e
#define ENDPOINT2IN 0x81
#define ENDPOINT2OUT 0x01
#define BUFFERSIZE 20
#define RUMBLETIME 200000

const char* SHM_NAME = "controller";

libusb_device_handle * h;
GameController * controller;
int error, transferred, shm_fd;

static void * readMessageQueueThread (void * arg);
int prepareSharedMemory();
int prepareController();
void publishControllerState();
bool getBitAtIndex(int byte, int index);
void rumble(bool shouldRumble);
void rotateLeds(bool shouldRotate);

int main(int argc, char *argv[]) {
    
	// Make this process a daemon
	//daemon(0,0);
	
    // Get shared memory file descriptor.
    if ((shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666)) == -1){
        std::cout << "cannot open" << std::endl;
        return -1;
    }
    
    // Set the shared memory size to the size of GameController struct.
    if (ftruncate(shm_fd, sizeof(GameController)) != 0){
        std::cout << "cannot set size" << std::endl;
        return -1;
    }
    
    // Map shared memory in address space.
    if ((controller = (struct GameController *)mmap(0, sizeof(GameController), PROT_READ, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        std::cout << "cannot mmap" << std::endl;
        return -1;
    }
    
    // Lock the shared memory.
    if (mlock(controller, sizeof(GameController)) != 0){
        std::cout << "cannot mlock" << std::endl;
        return -1;
    }
    
    // Shared memory is ready for use.
    
    if(prepareController() != 0){
        return 1;
    }
    
    mq_unlink("/commandQueue");
    struct mq_attr attr;  
	attr.mq_maxmsg = 1;  
	attr.mq_msgsize = 10;  
    // Open the intial messageQueue
    mqd_t m = mq_open("/commandQueue", O_CREAT | O_RDONLY, 0644, &attr);
    std::cout << "Opened MQ with mqd_t: " << m << std::endl;
    
    // Start thread for messagequeue reading
    pthread_t threadID;
	if(pthread_create(&threadID, NULL, readMessageQueueThread, &m) != 0){
            std::cout << "Error creating thread. Exiting. : " << m << std::endl;
            exit(1);
    }
    
    // Publish the state to the shared memory struct indefinitely
    while(true) {
        publishControllerState();
    }
}

// Thread that will read messages from the MessageQueue and act accordingly/
static void * readMessageQueueThread (void * threadArgs){
    // Parse the parameter to the right type.
    mqd_t * fd_ptr = (mqd_t *) threadArgs;
    
    char message[10];
    
    while(true){
		int read = mq_receive(*fd_ptr, message, 11, 0);
			
		if(read > 0){
			std::string cppmessage(message);
			
			if(cppmessage.find("rumble") != std::string::npos){
				rumble(true);
				sleep(3);
				rumble(false);
			}
			
			if(cppmessage.find("rotate") != std::string::npos){
				rotateLeds(true);
				sleep(3);
				rotateLeds(false);
			}
		} 
    
	}
}

// Opens the shared memory.
int prepareSharedMemory(){
    // Get shared memory file descriptor.
    if ((shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666)) == -1){
        std::cout << "cannot open" << std::endl;
        return -1;
    }
    
    // Set the shared memory size to the size of GameController struct.
    if (ftruncate(shm_fd, sizeof(GameController)) != 0){
        std::cout << "cannot set size" << std::endl;
        return -1;
    }
    
    // Map shared memory in address space.
    if ((controller = (struct GameController *)mmap(0, sizeof(GameController), PROT_READ, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        std::cout << "cannot mmap" << std::endl;
        return -1;
    }
    
    // Lock the shared memory.
    if (mlock(controller, sizeof(GameController)) != 0){
        std::cout << "cannot mlock" << std::endl;
        return -1;
    }
    
    // Shared memory is ready for use.
    return 0;
}

// Opens a USB controller.
int prepareController(){
    libusb_init(NULL);
    h = libusb_open_device_with_vid_pid(NULL, PRODUCTID, VENDORID);
    
    if (h == NULL) {
        std::cout << "Failed to open device " << std::endl;
        return -1;
    }
    
    return 0;
}

// Reads the controller state and publishes it on the shared memory.
void publishControllerState(){
    u_int8_t inpData[BUFFERSIZE];
    
    if(libusb_interrupt_transfer(h, ENDPOINT2IN, inpData, BUFFERSIZE , &transferred, 0) == 0) {
        
        controller->dPad.up = getBitAtIndex(inpData[2], 0);
        controller->dPad.down = getBitAtIndex(inpData[2], 1);
        controller->dPad.left = getBitAtIndex(inpData[2], 2);
        controller->dPad.right = getBitAtIndex(inpData[2], 3);
        
        controller->startButton = getBitAtIndex(inpData[2], 4);
        controller->backButton = getBitAtIndex(inpData[2], 5);
        
        controller->leftStickPress = getBitAtIndex(inpData[2], 6);
        controller->rightStickPress = getBitAtIndex(inpData[2], 7);
        
        controller->leftBackButton = getBitAtIndex(inpData[3], 0);
        controller->rightBackButton = getBitAtIndex(inpData[3], 1);
        
        controller->xboxButton = getBitAtIndex(inpData[3], 2);
        
        controller->aButton = getBitAtIndex(inpData[3], 4);
        controller->bButton = getBitAtIndex(inpData[3], 5);
        controller->xButton = getBitAtIndex(inpData[3], 6);
        controller->yButton = getBitAtIndex(inpData[3], 7);
        
        controller->leftTrigger = inpData[4];
        controller->rightTrigger = inpData[5];
        
        controller->leftStick.X = (inpData[6] << 8) | inpData[7];
        controller->leftStick.Y = (inpData[8] << 8) | inpData[9];
        
        controller->rightStick.X = (inpData[10] << 8) | inpData[11];
        controller->rightStick.Y = (inpData[12] << 8) | inpData[13];
    }
}

// Gets the bit at the specified index in the specified byte. Its value will be either 0 or 1.
bool getBitAtIndex(int byte, int index) {
    return byte >> index & 1;
}

// Turn the xbox's controller rumble on or off, depending on the parameter being true or false respectively.
void rumble(bool shouldRumble){
    unsigned char data[] = { 0, 8, 0, 0xcc, 0xcc, 0, 0, 0  };
    
    if(!shouldRumble){
        data[3] = 0;
        data[4] = 0;
    }
    
    if ((error = libusb_interrupt_transfer(h, ENDPOINT2OUT, data, sizeof data, &transferred, 0)) != 0) {
        std::cout << "Transfer for 'rumble' failed: " << error << std::endl;
    }
}

// Turn the xbox's controller leds on (spinning) or off, depending on the parameter being true or false respectively.
void rotateLeds(bool shouldRotate){
    unsigned char data[] = {1,3, 0x0a};
    
    if(!shouldRotate) {
        data[2] = 0;
    }
    
    if ((error = libusb_interrupt_transfer(h, ENDPOINT2OUT, data, sizeof data, &transferred, 0)) != 0) {
        std::cout << "Transfer for 'rotateLeds' failed: " << error << std::endl;
    }
}
