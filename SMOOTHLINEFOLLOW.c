#define WHITE 10
#define BLACK 0
#define CENTER 5
task main()
{


	while(1)
	{

		int error = getColorReflected(S1)-CENTER;

		setMotorSpeed (motorA,50 + error);
		setMotorSpeed (motorB,50 - error);

	}
}
