#define PRODUCTID 0x045e
#define VENDORID 0x028e
#define ENDPOINT2IN 0x81
#define ENDPOINT2OUT 0x01
#define BUFFERSIZE 20
#define RUMBLETIME 200000

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <unistd.h>
#include "../GameController.h"
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h> 
#include <mqueue.h>
#include <errno.h>

static libusb_device_handle *h;
static int error, transferred, printStickOutputs, ignore;

const char* SHM_NAME = "controller";

struct GameController* controller;
int shm_fd;

//Gets the bit at the specified index in the specified byte. Its value will be either 0 or 1.
static int getBitAtIndex(int byte, int index) {
    return byte >> index & 1;
}

// Turn the xbox's controller rumble on or off, depending on the parameter being 1 or 0 respectively.
static int rumble(int shouldRumble){
    unsigned char data[] = { 0, 8, 0, 0xcc, 0xcc, 0, 0, 0  };
    
    if(shouldRumble == 0){
        data[3] = 0;
        data[4] = 0;
    }
    
    if ((error = libusb_interrupt_transfer(h, ENDPOINT2OUT, data, sizeof data, &transferred, 0)) != 0) {
        fprintf(stderr, "Transfer failed: %d\n", error);
        return (1);
    }
    return (0);
}

void printController(struct GameController c){
	std::cout << "========" << std::endl;
	std::cout << "DPad: {" << c.dPad.up << ":" << c.dPad.down << ":" << c.dPad.left << ":" << c.dPad.right << "}" << std::endl;
	std::cout << "leftStick: {" << c.leftStick.X << ":" << c.leftStick.Y << "}" << std::endl;
	std::cout << "rightStick: {" << c.rightStick.X << ":" << c.rightStick.Y << "}" << std::endl;
	std::cout << "startButton: " << c.startButton << std::endl;
	std::cout << "backButton: " << c.backButton << std::endl;
	std::cout << "leftStickPress: " << c.leftStickPress << std::endl;
	std::cout << "rightStickPress: " << c.rightStickPress << std::endl;
	std::cout << "leftBackButton: " << c.leftBackButton << std::endl;
	std::cout << "rightBackButton: " << c.rightBackButton << std::endl;
	std::cout << "xboxButton: " << c.xboxButton << std::endl;
	std::cout << "aButton: " << c.aButton << std::endl;
	std::cout << "bButton: " << c.bButton << std::endl;
	std::cout << "xButton: " << c.xButton << std::endl;
	std::cout << "yButton: " << c.yButton << std::endl;
	std::cout << "leftTrigger: " << c.leftTrigger << std::endl;
	std::cout << "rightTrigger: " << c.rightTrigger << std::endl;
	std::cout << "========" << std::endl;
}

// Turn the xbox's controller leds on (spinning) or off, depending on the parameter being 1 or 0 respectively.
static int rotateLeds(int shouldShow){
    unsigned char data[] = {1,3, 0x0a};
    
    if(shouldShow == 0) {
        data[2] = 0;
    }
    
    if ((error = libusb_interrupt_transfer(h, ENDPOINT2OUT, data, sizeof data, &transferred, 0)) != 0) {
        fprintf(stderr, "Transfer failed: %d\n", error);
        return (1);
    }
    return (0);
}

int main(int argc, char *argv[]) {
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
    if ((controller = (struct GameController *)mmap(0, sizeof(GameController), PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
        std::cout << "cannot mmap" << std::endl;
        return -1;
    }
    
    // Lock the shared memory.
    if (mlock(controller, sizeof(GameController)) != 0){
        std::cout << "cannot mlock" << std::endl;
        return -1;
    }
    
    // Shared memory is ready for use.
    std::cout << "Shared Memory successfully opened.\n" << std::endl;
    
    libusb_init(NULL);
    h = libusb_open_device_with_vid_pid(NULL, PRODUCTID, VENDORID);
    if (h == NULL) {
        fprintf(stderr, "Failed to open device\n");
        return (1);
    }
    
    mq_unlink("/commandQueue");
    
    struct mq_attr attr;  
	attr.mq_maxmsg = 1;  
	attr.mq_msgsize = 1024;  
    
    mqd_t m = mq_open("/commandQueue", O_CREAT | O_RDONLY, 0644, &attr);
    std::cout << "Opened MQ with mdq_t: " << m << std::endl;
    char message[8];
    
	int read = mq_receive(m, message, 1025, 0);
		
	if(read > 0){
		std::cout << "Received Message:  " << message << std::endl;
	} 
	
	std::cout << read << std::endl;
	std::cout << std::endl;

	mq_unlink("/commandQueue");
	exit(0);
	
	

		std::cout << read << " : NOT LISTENING ANYMORE" << std::endl;
    
    while(1) {
        u_int8_t inpData[BUFFERSIZE];
        
        if(libusb_interrupt_transfer(h, ENDPOINT2IN, inpData, BUFFERSIZE , &transferred, 0) == 0) {
            
            ignore = 1;
            
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
            
        } else {
            // An ignore is used because the xbox controller 'sends' an input update two times for every button press.
            // This way we 'filter' so we only act on it one time.
            ignore = 0;
        }
        
        printController(*controller);
   }
}
