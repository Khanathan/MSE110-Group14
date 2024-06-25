#pragma config(Sensor, S1, colorSensor, sensorEV3_Color)
#pragma config(Motor, motorA, rightMotor, tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor, motorD, leftMotor, tmotorEV3_Large, PIDControl, encoder)

const int speed = 25;

void forward() {
	setMotorSpeed(leftMotor, -speed);
	setMotorSpeed(rightMotor, -speed);
}

void stopMoving() {
	setMotorSpeed(leftMotor, 0);
	setMotorSpeed(rightMotor, 0);
}

task main()
{
	datalogFlush();
	datalogClose();

	if (!datalogOpen(420, 2, true)) {
			displayCenteredTextLine(4, "Datalog open failed");
			return;
	}
	forward();
	for (int i = 0; i < 200; i++) {
		datalogAddShort(i, getColorSaturation(colorSensor));
		sleep(25);
	}
	stopMoving();
	datalogClose();
}
