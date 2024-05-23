#pragma config(Sensor, S1,     colorSensor,    sensorEV3_Color)
#pragma config(Sensor, S4,     ultrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          rightMotor,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          pinchMotor,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          leftMotor,     tmotorEV3_Large, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
const int defSpeed = 50;
const int lowRed = 7;
const int highRed = 15;
const int turnLimit = 95;

void forward() {
	setMotorSpeed(leftMotor, -defSpeed);
	setMotorSpeed(rightMotor, -defSpeed);
}

void stopMoving() {
	setMotorSpeed(leftMotor, 0);
	setMotorSpeed(rightMotor, 0);
}

void rotateRight() {
	setMotorSpeed(leftMotor, -defSpeed);
	setMotorSpeed(rightMotor, defSpeed);
}

void findLine() {
	int redVal;
	int greenVal;
	int blueVal;

	stopMoving();
	resetMotorEncoder(leftMotor);
	int motorEnc = getMotorEncoder(leftMotor);
	//turn right
	rotateRight();
	while (motorEnc < turnLimit) {
		getColorRGB(colorSensor, redVal, greenVal, blueVal);
		if (redVal <= lowRed) {
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
	while (motorEnc < turnLimit) {
		getColorRGB(colorSensor, redVal, greenVal, blueVal);
		if (redVal <= lowRed) {
			return;
		}
		motorEnc = getMotorEncoder(rightMotor);
		if (motorEnc < 0) {
			motorEnc *= -1;
		}
	}
	//if not found, stop
	stopMoving();
	finish = true;
}

bool finish = false;

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
		delay(20);
	}


}
