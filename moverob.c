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


void moveRob(float distance, int speed);
void rotateRob(float angle, int speed);


task main()
{
	moveRob(10,-80);
	//sample moveRob with 10 cm forwards at 80% speed backwards.
	rotateRob(180,50);
	//sample rotateRob with 180 degree turn at 50 speed
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
