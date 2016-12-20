#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../GameController.h"
#include <sys/mman.h>
#include <fcntl.h> 
#include <unistd.h>

const char* SHM_NAME = "controller";

struct GameController* controller;
int shm_fd;

void printController(struct GameController c){
	std::cout << "========" << std::endl << "</br>";
	std::cout << "DPad: {" << c.dPad.up << ":" << c.dPad.down << ":" << c.dPad.left << ":" << c.dPad.right << "}" << std::endl << "</br>";
	std::cout << "leftStick: {" << c.leftStick.X << ":" << c.leftStick.Y << "}" << std::endl << "</br>";
	std::cout << "rightStick: {" << c.rightStick.X << ":" << c.rightStick.Y << "}" << std::endl << "</br>";
	std::cout << "startButton: " << c.startButton << std::endl << "</br>";
	std::cout << "backButton: " << c.backButton << std::endl << "</br>";
	std::cout << "leftStickPress: " << c.leftStickPress << std::endl << "</br>";
	std::cout << "rightStickPress: " << c.rightStickPress << std::endl << "</br>";
	std::cout << "leftBackButton: " << c.leftBackButton << std::endl << "</br>";
	std::cout << "rightBackButton: " << c.rightBackButton << std::endl << "</br>";
	std::cout << "xboxButton: " << c.xboxButton << std::endl << "</br>";
	std::cout << "aButton: " << c.aButton << std::endl << "</br>";
	std::cout << "bButton: " << c.bButton << std::endl << "</br>";
	std::cout << "xButton: " << c.xButton << std::endl << "</br>";
	std::cout << "yButton: " << c.yButton << std::endl << "</br>";
	std::cout << "leftTrigger: " << c.leftTrigger << std::endl << "</br>";
	std::cout << "rightTrigger: " << c.rightTrigger << std::endl<< "</br>"; 
	std::cout << "========" << std::endl << "</br>"; 
}

int main()
{
	std::cout << "Content-Type:text/html;charset=iso-8859-1" << 13 << 10 << std::endl << std::endl;
	std::cout << "<TITLE>Bart Kneepkens & Martin Donkersloot</TITLE>" << std::endl;
	std::cout << "<H3>Hi!</H3>" << std::endl;
	
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
    //std::cout << "Shared Memory successfully opened.\n" << std::endl;
    printController(*controller);
	
	return 0;
}
