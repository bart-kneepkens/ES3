#include <iostream>
#include "GameController.h"

struct GameController controller;

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

int main(){
	std::cout << "Herro!" << std::endl;
	
	controller.aButton = true;
	controller.bButton = true;
	
	printController(controller);
}
