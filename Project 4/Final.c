#pragma config(Sensor, S1,     ultrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          rightMotor,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          leftMotor,     tmotorEV3_Large, PIDControl, encoder)


typedef struct
{
	int NorthWall;
	int EastWall;
	int SouthWall;
	int WestWall;
} Cell;

Cell Grid[4][6];

// Start Facing North
int RobotDirection = 0; // 0=North, 1=East, 2=South, 3=West

// Start in the 0,0 Cell
int StartPosRow = 0; // Starting position
int StartPosCol = 0;

int CurrentPosRow = StartPosRow; // Starting position
int CurrentPosCol = StartPosCol;

int TargetPosRow = 3;
int TargetPosCol = 0;

int turnRightArr[] = {1, 2, 3, 0};
int turnLeftArr[] = {3, 0, 1, 2};


const int colMove[] = {0, 1, 0, -1};
const int rowMove[] = {1, 0, -1, 0};


int turns[24];
int cnt=0;

int Solver();
void endRun();
void GridInit();
void forward();
void turnLeft();
void turnRight();

task main()
{
		GridInit();
		setSoundVolume(2);
        playImmediateTone(300, 200);
        wait1Msec(2000);
		while ((CurrentPosRow != TargetPosRow) || (CurrentPosCol != TargetPosCol))
        {
				Solver();
		}
		playImmediateTone(300, 200);
		wait1Msec(2000);
		while (true)
		{
			endRun();
		}
}

void GridInit()
{
		for (int x = 0; x < 24; x++){
			turns[x] =-1 ;
		}
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            Grid[i][j].NorthWall = 0;
            Grid[i][j].EastWall = 0;
            Grid[i][j].WestWall = 0;
            Grid[i][j].SouthWall = 0;
        }
    }
}

int Solver()
{

        float distance;

		turnRight();
		distance = getUSDistance(ultrasonicSensor);
        wait1Msec(500);
		if (distance > 15)
		{
			forward();
		}
		else
		{
            for (int x = 0; x < 3; x++){
                turnLeft();
                wait1Msec(500);
                distance = getUSDistance(ultrasonicSensor);
                if (distance > 15)
                {
                    forward();
                    break;
                }
            }
		}
	return 0;
}

void forward()
{
    if (cnt != 0){
        if (abs(turns[cnt-1] - RobotDirection) != 2 ){
            turns[cnt] = RobotDirection;
            cnt++;
        }
        else{
            turns[cnt-1] = -1;
            cnt--;
        }

    }
    else{
        turns[0] = RobotDirection;
        cnt++;
    }
    CurrentPosCol += colMove[RobotDirection];
	CurrentPosRow += rowMove[RobotDirection];
	setMotorSpeed(leftMotor, 20);
	setMotorSpeed(rightMotor, 20);
    wait1Msec(2600);
    setMotorSpeed(leftMotor, 0);
    setMotorSpeed(rightMotor, 0);
}

void turnRight()
{
    RobotDirection = turnRightArr[RobotDirection];
    setMotorSpeed(leftMotor, 35);
    setMotorSpeed(rightMotor, -25);
   wait1Msec(1200);
    setMotorSpeed(leftMotor, 0);
    setMotorSpeed(rightMotor, 0);

}

void turnLeft()
{
    RobotDirection = turnLeftArr[RobotDirection];

   setMotorSpeed(leftMotor, -25);
    setMotorSpeed(rightMotor, 35);

    wait1Msec(1200);
    setMotorSpeed(leftMotor, 0);
    setMotorSpeed(rightMotor, 0);
}

void endRun()
{
    turnRight();
    turnRight();
    forward();
    for(int x = 23; x < -1; x++){
        if (turns[x] != -1){
            if ((RobotDirection-turns[x]) == 2){
                forward();
            }
            else if ((RobotDirection-turns[x]) == 1){
                turnLeft();
            }
            else{
                turnRight();
            }
        }
    }
}
