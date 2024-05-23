#pragma config(Sensor, S1,     colorSensor,    sensorEV3_Color)
#pragma config(Sensor, S4,     ultrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          rightMotor,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          pinchMotor,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          leftMotor,     tmotorEV3_Large, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
const int forwardSpeed = 40;
const int defSpeed = 30;
const int lowRed = 10;
const int highRed = 14;
const int turnLimit = 220;
bool finish = false;
bool lastLeft = false;

void forward() {
	setMotorSpeed(leftMotor, -forwardSpeed);
	setMotorSpeed(rightMotor, -forwardSpeed);
}

void stopMoving() {
	setMotorSpeed(leftMotor, 0);
	setMotorSpeed(rightMotor, 0);
}

void rotateRight() {
	setMotorSpeed(leftMotor, -defSpeed);
	setMotorSpeed(rightMotor, defSpeed);	
}

void rotateLeft() {
	setMotorSpeed(leftMotor, defSpeed);
	setMotorSpeed(rightMotor, -defSpeed);	
}

void findLine() {
	int redVal;
	int greenVal;
	int blueVal;
	
	stopMoving();
	int motorEnc;
	
	//turned left last time
	if (lastLeft) {
		resetMotorEncoder(rightMotor);
		motorEnc = getMotorEncoder(rightMotor);
		//turn left
		rotateLeft();
		while (motorEnc < turnLimit) {
			getColorRGB(colorSensor, redVal, greenVal, blueVal);
			if (redVal <= lowRed) {
				stopMoving();
				lastLeft = true;
				return;	
			}
			motorEnc = getMotorEncoder(rightMotor);
			if (motorEnc < 0) {
				motorEnc *= -1;	
			}
		}
	
		resetMotorEncoder(leftMotor);
		motorEnc = getMotorEncoder(leftMotor);
		//turn right
		rotateRight();
		while (motorEnc < turnLimit * 2) {
			getColorRGB(colorSensor, redVal, greenVal, blueVal);
			if (redVal <= lowRed) {
				stopMoving();
				lastLeft = false;
				return;	
			}
			motorEnc = getMotorEncoder(leftMotor);
			if (motorEnc < 0) {
				motorEnc *= -1;	
			}
		}
	} else {
		resetMotorEncoder(leftMotor);
		motorEnc = getMotorEncoder(leftMotor);
		//turn right
		rotateRight();
		while (motorEnc < turnLimit) {
			getColorRGB(colorSensor, redVal, greenVal, blueVal);
			if (redVal <= lowRed) {
				lastLeft = false;
				stopMoving();
				return;	
			}
			motorEnc = getMotorEncoder(leftMotor);
			if (motorEnc < 0) {
				motorEnc *= -1;	
			}
		}
		resetMotorEncoder(rightMotor);
		motorEnc = getMotorEncoder(rightMotor);
		//turn left
		rotateLeft();
		while (motorEnc < turnLimit * 2) {
			getColorRGB(colorSensor, redVal, greenVal, blueVal);
			if (redVal <= lowRed) {
			lastLeft = true;	
			stopMoving();	
				return;	
			}
			motorEnc = getMotorEncoder(rightMotor);
			if (motorEnc < 0) {
				motorEnc *= -1;	
			}
		}
	}
	//if not found, stop
	stopMoving();
	finish = true;
}



task main()
{
	int redVal;
	int greenVal;
	int blueVal;
	
	float distance;
	
	while (!finish) {
		distance = getUSDistance(ultrasonicSensor);
		if (distance <= 10) {
			stopMoving();
			finish = true;
		}
		getColorRGB(colorSensor, redVal, greenVal, blueVal);
		if (redVal >= highRed) {
			findLine();	
		}
		forward();
		delay(40);
	}


}
