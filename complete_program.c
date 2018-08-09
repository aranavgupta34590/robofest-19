/*
Allows the robot to accurately rotate a set number of degrees or move a set amount of distance at a set speed.
The parameters in the macros need to be changed for different use cases.

-Yujie
*/

#define WHEEL 18
#define AXLE 12.5
#define MINDETECTIONS1 20
#define MINDETECTIONS2 22
#define TENNIS 1
#define BOTTLE 2

#define ON_TABLE 40
#define AVG_TABLE 30
#define OFF_TABLE 10

#define WHITE 10
#define BLACK 0
#define CENTER 5

#define OUT false
#define IN true

//ALTS(sensor) will return motorD if motorA is passed and vice versa.
#define ALTS(A) ((A)==0 ? 3 : 0)

void moveRob(float distance, int speed);
void rotateRob(float angle, int speed);
bool onTable(int sensor);
void alignFront(int time);
void followEdge(float distance, int sensor, int speed);

//The two functions below are simple variations of followEdge without conditionals.
void lineFollow(int speed);
void altLineFollow(int speed);

task main()
{
	moveRob(10,-80);
	//sample moveRob with 10 cm forwards at 80% speed backwards.
	rotateRob(180,50);
	//sample rotateRob with 180 degree turn at 50 speed
	lineFollow(50);
	//lineFollow will not work with high speeds. TODO: Fix this.
	altLineFollow(50);
	//lineFollow will not work with high speeds. TODO: Fix this.
	if(onTable(S1))
	{
		//Do stuff
	}
	//onTable returns true if the sensor is on the table. Sample use case above.
	alignFront(1000);
	//sample alignFront for 1 second.
	followEdge(100,S1,50);
	//followEdge for 100 cm using S1 to guide and S4 to find the edge at 50 speed.
	//followEdge is not ideal. Improve if you can.
}

/*
moveRob takes distance in cm. Speed is from -100 to 100. Negative for either value is backwards.
*/
void moveRob(float distance, int speed)
{
	if(speed < 0)
	{
		speed *= -1;
		distance *= -1;
	}
	int encoder[2];
	encoder[0] = getMotorEncoder(motorA);
	encoder[1] = getMotorEncoder(motorD);
	setMotorSpeed(motorA,speed*sgn(distance));
	setMotorSpeed(motorD,speed*sgn(distance));
	distance = distance * (360 / WHEEL);
	encoder[0] += distance;
	encoder[1] += distance;
	int flag = sgn(encoder[0]-getMotorEncoder(motorA));
	while(flag+sgn(encoder[0]-getMotorEncoder(motorA)) && flag+sgn(encoder[1]-getMotorEncoder(motorD))){}
	setMotorSpeed(motorA,0);
	setMotorSpeed(motorD,0);
}

/*
rotateRob takes angle out of 360 with counterclockwise being positive and clockwise being negative. Speed is -100 to 100.
*/
void rotateRob(float angle, int speed)
{
	int encoder[2];
	encoder[0] = getMotorEncoder(motorA);
	encoder[1] = getMotorEncoder(motorD);
	setMotorSpeed(motorA,speed*sgn(angle));
	setMotorSpeed(motorD,speed*-sgn(angle));
	float distance = angle * (AXLE * PI * (360.0 / WHEEL)) / 360.0;
	encoder[0] += distance;
	encoder[1] -= distance;
	int flag[2];
	flag[0] = sgn(encoder[0]-getMotorEncoder(motorA));
	flag[1] = sgn(encoder[1]-getMotorEncoder(motorA));
	while(flag[0]+sgn(encoder[0]-getMotorEncoder(motorA)) && flag[1]+sgn(encoder[1]-getMotorEncoder(motorB))){}
	setMotorSpeed(motorA,0);
	setMotorSpeed(motorD,0);
}

/*
Modify this for your use case by changing the while(1) to while(desired conditional) TODO: make function generic
*/
void lineFollow(int speed)
{
	while(1)
	{
		int offset = getColorReflected(S1);
		setMotorSpeed(motorA, speed + pow(offset,3));
		setMotorSpeed(motorD, speed - pow(offset,3));
	}
}

/*
Modify this for your use case by changing the while(1) to while(desired conditional) TODO: make function generic
*/
void altLineFollow(int speed)
{
	while(1)
	{
		int error = getColorReflected(S1)-CENTER;
		setMotorSpeed (motorA,speed + error);
		setMotorSpeed (motorB,speed - error);
	}
}

/*
Returns whether the sensor is on or off the table.
*/
bool onTable(int sensor)
{
	if(SensorValue[sensor]<AVG_TABLE)
	{
		return OUT;
	}else
	{
		return IN;
	}
}

/*
Aligns the robot's front with the edge of the table if both sensors are correctly positioned. Time is in milliseconds.
TODO: change time to actual time.
*/
void alignFront(int time)
{
	int i = 0;
	while(1){
		//The /3 is slightly arbitrary. TODO: make passable?
		setMotorSpeed(motorA,(AVG_TABLE-SensorValue[S1])/3);
		setMotorSpeed(motorD,(AVG_TABLE-SensorValue[S4])/3);
		if(++i>=time){
			break;
		}
	}
}

/*
followEdge takes distance in cm; sensor as 0, 1, 2, or S1, S2, S3; speed from -100 to 100.
This is designed for a robot with two sensors in line, so it's not ideal.
*/
void followEdge(float distance, int sensor, int speed)
{
	float coeff = .25;
	//The severity at which the robot follows the edge. Its aggressiveness.
	distance -= 3;
	//Cuts robot's linefollow significantly as it nears the edge due to rounded corners on table.
	distance = distance * (360 / WHEEL);
	int initMotorEncoder = abs(getMotorEncoder(motorA));
	int deltaEncoder = 0;
	while(onTable(ALTS(sensor))){
		while(onTable(ALTS(sensor))){
			deltaEncoder = abs(getMotorEncoder(motorA))-initMotorEncoder;
			deltaEncoder = abs(deltaEncoder);
			if(deltaEncoder >= distance)
			{
				coeff = 0.0625;
				//TODO: Make this argument or make generic.
			}
			if(sensor){
				setMotorSpeed(motorA,-1*(speed-coeff*(AVG_TABLE-SensorValue(sensor))));
				setMotorSpeed(motorD,-1*(speed+coeff*(AVG_TABLE-SensorValue(sensor))));
				}else{
				setMotorSpeed(motorA,-1*(speed+coeff*(AVG_TABLE-SensorValue(sensor))));
				setMotorSpeed(motorD,-1*(speed-coeff*(AVG_TABLE-SensorValue(sensor))));
			}
			//sleep(30);
		}
		//sleep(30);
	}
	setMotorSpeed(motorA,0);
	setMotorSpeed(motorD,0);
}
