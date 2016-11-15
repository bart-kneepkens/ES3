#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <unistd.h>

libusb_device_handle *h;
int error, transferred;

int
main(int argc, char *argv[])
{
 
 unsigned char data[] = { 0, 8, 0, 0xcc, 0xcc, 0, 0, 0  };  //000800bbll000000
 

 libusb_init(NULL);
 h = libusb_open_device_with_vid_pid(NULL, 0x045e, 0x028e);
 if (h == NULL) {
  fprintf(stderr, "Failed to open device\n");
  return (1);
 }
 
 //rumble(1);
 //sleep(5);
 //rumble(0);

 //rotateLeds(1);
 //sleep(5);
 //rotateLeds(0);

while(1){
  u_int8_t inpData[20];

  if(libusb_interrupt_transfer(h, 0x81, inpData, 20 , &transferred, 0) == 0) {
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
  }
}
}

/** Gets the bit at the specified index in the specified byte. Its value
  will be either 0 or 1. */
int getBitAtIndex(int byte, int index) {
  return byte >> index & 1;
}

int rumble(int shouldRumble){
 unsigned char data[] = { 0, 8, 0, 0xcc, 0xcc, 0, 0, 0  };  //000800bbll000000

 if(shouldRumble == 0){
  data[3] = 0;
  data[4] = 0;
 } 
 
 if ((error = libusb_interrupt_transfer(h, 0x01, data, sizeof data, &transferred, 0)) != 0) {
  fprintf(stderr, "Transfer failed: %d\n", error);
  return (1);
 }
 return (0);
}

int rotateLeds(int shouldShow){
 unsigned char data[] = {1,3, 0x0a  };

 if(shouldShow == 0) {
  data[2] = 0;
 }

 if ((error = libusb_interrupt_transfer(h, 0x01, data, sizeof data, &transferred, 0)) != 0) {
  fprintf(stderr, "Transfer failed: %d\n", error);
  return (1);
 }
 return (0);
}
