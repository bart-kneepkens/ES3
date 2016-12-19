#include "DPad.h"
#include "Stick.h"

struct GameController {
	struct DPad dPad;
	struct Stick leftStick;
	struct Stick rightStick;
	bool startButton;
	bool backButton;
	bool leftStickPress;
	bool rightStickPress;
	bool leftBackButton;
	bool rightBackButton;
	bool xboxButton;
	bool aButton;
	bool bButton;
	bool xButton;
	bool yButton;
	int leftTrigger;
	int rightTrigger;
};
