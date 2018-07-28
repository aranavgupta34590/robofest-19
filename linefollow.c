#define ON_TABLE 40
#define OFF_TABLE 10

void lineFollow(int speed);

task main()
{

}


void lineFollow(int speed)
{
	while(1)
	{
		int offset = getColorReflected(S1);
		setMotorSpeed(motorA, speed + pow(offset,3));
		setMotorSpeed(motorD, speed - pow(offset,3));
	}
}
