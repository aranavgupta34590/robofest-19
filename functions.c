#define WHEEL 18
#define AXLE 12.5
#define MINDETECTIONS1 20
#define MINDETECTIONS2 22
#define TENNIS 1
#define BOTTLE 2
#define FAVG 30
#define TABLEY 75
#define TABLEX 242.57
#define WHEELTOFRONT 13.97
#define LENGTHROB 20 //IDEK4SURE
#define OUT 0
#define IN 1
#define MAXVAL 65536

#define ALTS(A) ((A)==0 ? 3 : 0)

//TODO: code theta r polar -> cartesian

float GPSx = 0.0;
float GPSy = 0.0;

float theta = 0.0;

int initTime;


void lineFollow(int speed)
{
	while(1)
	{
		int offset = getColorReflected(S1);
		setMotorSpeed(motorA, speed + pow(offset,3));
		setMotorSpeed(motorD, speed - pow(offset,3));
	}
}

void updateGPS(float x,float y)
{
	GPSx = x;
	GPSy = y;
}

int detectionState()//TODO: replace with get bottle fxn that's an int
{
	if(getUSDistance(S1)<MINDETECTIONS1)
	{
		if(getUSDistance(S2)<MINDETECTIONS2)
		{
			return BOTTLE;
		}else
		{
			return TENNIS;
		}
	}else
	{
		return 0;
	}
}

void moveRob(float distance, int speed)
{
	speed*=-1;
	if(speed < 0)
	{
		speed *= -1;
		distance *= -1;
	}
	GPSy += (float)distance * sgn(speed) * sinDegrees(theta);
	GPSx += (float)distance * sgn(speed) * cosDegrees(theta);
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

void rotateRob(float angle, int speed)
{
	if(speed<0)
	{
		speed*=-1;
		angle*=-1;
	}
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
	theta+=angle;
}



void printTime()
{
	int milliTime = nSysTime - initTime;
	int milliseconds = milliTime % 1000;
	int seconds = (milliTime / 1000) % 60 ;
	int minutes = ((milliTime / (1000*60)) % 60);
	displayCenteredBigTextLine(0,"%02d:%02d:%02d", minutes, seconds, milliseconds);
}

void align(int time)
{
	int i = 0;
	while(1){
		setMotorSpeed(motorA,(FAVG-SensorValue[S1])/3);
		setMotorSpeed(motorD,(FAVG-SensorValue[S4])/3);
		if(++i>=time){
			break;
		}
	}
}

bool onTable(int sensor)
{
	if(SensorValue[sensor]<FAVG)
	{
		return OUT;
	}else
	{
		return IN;
	}
}

void edgeRob(float distance, int sensor, int speed)
{
	float coeff = .25;
	distance -= 3;
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
			}
			if(sensor){
				setMotorSpeed(motorA,-1*(speed-coeff*(FAVG-SensorValue(sensor))));
				setMotorSpeed(motorD,-1*(speed+coeff*(FAVG-SensorValue(sensor))));
				}else{
				setMotorSpeed(motorA,-1*(speed+coeff*(FAVG-SensorValue(sensor))));
				setMotorSpeed(motorD,-1*(speed-coeff*(FAVG-SensorValue(sensor))));
			}
			//sleep(30);
		}
		//sleep(30);
	}
	setMotorSpeed(motorA,0);
	setMotorSpeed(motorD,0);
}
