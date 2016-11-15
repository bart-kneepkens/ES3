#define PRODUCTID 0x045e
#define VENDORID 0x028e
#define ENDPOINT2IN 0x81
#define ENDPOINT2OUT 0x01
#define BUFFERSIZE 20
#define RUMBLETIME 200000

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <unistd.h>

libusb_device_handle *h;
int error, transferred, printStickOutputs, ignore;

int main(int argc, char *argv[]) {
    libusb_init(NULL);
    h = libusb_open_device_with_vid_pid(NULL, PRODUCTID, VENDORID);
    if (h == NULL) {
        fprintf(stderr, "Failed to open device\n");
        return (1);
    }
    
    while(1) {
        u_int8_t inpData[BUFFERSIZE];
        
        if(libusb_interrupt_transfer(h, ENDPOINT2IN, inpData, BUFFERSIZE , &transferred, 0) == 0 && !ignore) {
            
            ignore = 1;
            
            // D-Pad presses
            if (getBitAtIndex(inpData[2], 0) == 1) {
                printf("D-Pad up\n");
            }
            
            if (getBitAtIndex(inpData[2], 1) == 1) {
                printf("D-Pad down\n");
            }
            
            if (getBitAtIndex(inpData[2], 2) == 1) {
                printf("D-Pad left\n");
            }
            
            if (getBitAtIndex(inpData[2], 3) == 1) {
                printf("D-Pad right\n");
            }
            
            
            // Buttons and stick presses
            if (getBitAtIndex(inpData[2], 4) == 1) {
                printf("Start button\n");
            }
            
            if (getBitAtIndex(inpData[2], 5) == 1) {
                printf("Back button\n");
            }
            
            if (getBitAtIndex(inpData[2], 6) == 1) {
                printf("Left stick press\n");
            }
            
            if (getBitAtIndex(inpData[2], 7) == 1) {
                printf("Right stick press\n");
            }
            
            if (getBitAtIndex(inpData[3], 0) == 1) {
                printf("Button LB\n");
            }
            
            if (getBitAtIndex(inpData[3], 1) == 1) {
                printf("Button RB\n");
            }
            
            if (getBitAtIndex(inpData[3], 2) == 1) {
                printf("Xbox logo button\n");
                
                // Toggle whether or not the stick outputs should be printed.
                // If toggled ON, give feedback by a brief rumble and make the leds rotate ON.
                if(printStickOutputs) {
                    printStickOutputs = 0;
                    rotateLeds(0);
                }
                else{
                    printStickOutputs = 1;
                    rotateLeds(1);
                    rumble(1);
                    usleep(RUMBLETIME);
                    rumble(0);
                }
            }
            
            if (getBitAtIndex(inpData[3], 4) == 1) {
                printf("Button A\n");
            }
            
            if (getBitAtIndex(inpData[3], 5) == 1) {
                printf("Button B\n");
            }
            
            if (getBitAtIndex(inpData[3], 6) == 1) {
                printf("Button X\n");
            }
            
            if (getBitAtIndex(inpData[3], 7) == 1) {
                printf("Button Y\n");
            }
            
            
            // Trigger presses
            if(inpData[4] != 0){
                printf("Left Trigger: %i/255 \n", inpData[4]);
            }
            
            if(inpData[5] != 0){
                printf("Right Trigger: %i/255 \n", inpData[5]);
            }
            
            
            // Stick movements (can be toggled by pressing the Xbox Button)
            if(printStickOutputs){
                if(inpData[6] && inpData[7]){
                    printf("Left Stick X-axis: %i/65536\n", (inpData[6] << 8) | inpData[7]);
                }
                
                if(inpData[8] && inpData[9]){
                    printf("Left Stick Y-axis: %i/65536\n", (inpData[8] << 8) | inpData[9]);
                }
                
                if(inpData[10] && inpData[11]){
                    printf("Right Stick X-axis: %i/65536\n", (inpData[10] << 8) | inpData[11]);
                }
                
                if(inpData[12] && inpData[13]){
                    printf("Right Stick Y-axis: %i/65536\n", (inpData[12] << 8) | inpData[13]);
                }
            }
            
        } else {
            // An ignore is used because the xbox controller 'sends' an input update two times for every button press.
            // This way we 'filter' so we only act on it one time.
            ignore = 0;
        }
    }
}

//Gets the bit at the specified index in the specified byte. Its value will be either 0 or 1.
int getBitAtIndex(int byte, int index) {
    return byte >> index & 1;
}

// Turn the xbox's controller rumble on or off, depending on the parameter being 1 or 0 respectively.
int rumble(int shouldRumble){
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

// Turn the xbox's controller leds on (spinning) or off, depending on the parameter being 1 or 0 respectively.
int rotateLeds(int shouldShow){
    unsigned char data[] = {1,3, 0x0a  };
    
    if(shouldShow == 0) {
        data[2] = 0;
    }
    
    if ((error = libusb_interrupt_transfer(h, ENDPOINT2OUT, data, sizeof data, &transferred, 0)) != 0) {
        fprintf(stderr, "Transfer failed: %d\n", error);
        return (1);
    }
    return (0);
}
