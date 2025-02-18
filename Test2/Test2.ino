//  IMPORTS
#include <RedBot.h>

//  INSTANCE VARIABLES
/* Sensor, encoder, and motor object initializations. Note that sensor constructors 
 * are used to instantiate no particular sensor
 */
RedBotEncoder encoder = RedBotEncoder(A2, 10);
RedBotMotors motors;

//  Variables to store motor speeds
int leftMotorSpeed;
int rightMotorSpeed;

//  CONSTANTS
//  The IR sensor values, measured 1/8" from the ground
#define BLACK_MIN 990
#define BLACK_MAX 1100
#define RED_MIN 50
#define RED_MAX 200
#define BROWN_MIN 400
#define BROWN_MAX 980


//  Nominal speeds for the motors (max is 255)
#define SLOW_SPEED 20
#define MID_SPEED 125
#define FAST_SPEED 245

void setup()
{
    //  Initialize baudrate
    Serial.begin(115200);

    //  Motor encoder calibration
    encoder.clearEnc(BOTH);
}

void loop()
{
    //  Provides IR readings through serial monitor
    readSensorData();

    //encodedDrive();
    encodedTankTurn(0);
}

int encodedOffset(int motorPower, int leftTicks, int rightTicks)
{
        //  variables for tracking the left and right encoder counts
        long prevLeftEncCount, prevRightEncCount;
        long leftDiff, rightDiff;  // difference between current encoder count and previous count
    
        //  variables for setting left and right motor power
        int leftPower = motorPower;
        int rightPower = motorPower;
    
        //  variable used to offset motor power on right vs left to keep straight.
        int offset = 5;  // offset amount to compensate Right vs. Left drive

        if (leftDiff > rightDiff) 
        {
            leftPower -= offset;
            rightPower += offset;
        }
        //  if right is faster than the left, speed up the left / slow down right
        else if (leftDiff < rightDiff) 
        {
            leftPower += offset;  
            rightPower -= offset;
        }
        delay(50);
}

void encodedDrive()
{
    int leftTicks = encoder.getTicks(LEFT);
    int rightTicks = encoder.getTicks(RIGHT);
    int offset = 10;

    if (leftTicks > rightTicks)
    {
        motors.leftBrake();
        motors.rightBrake();
        motors.leftMotor(FAST_SPEED - offset);
        motors.rightMotor(FAST_SPEED + offset);
    }
    else if (leftTicks < rightTicks)
    {
        motors.leftBrake();
        motors.rightBrake();
        motors.leftMotor(FAST_SPEED + offset);
        motors.rightMotor(FAST_SPEED - offset);
    }
}

//  direction: 0 is counterclockwise, 1 is clockwise
void encodedTankTurn(int direction)
{
    int leftTicks = encoder.getTicks(LEFT);
    int rightTicks = encoder.getTicks(RIGHT);
    int offset = 10;

    if (direction == 0)
    {
        if (abs(leftTicks) > rightTicks)
        {
            motors.leftBrake();
            motors.rightBrake();
            motors.leftMotor(-(FAST_SPEED - offset));
            motors.rightMotor(FAST_SPEED + offset);
        }
        else if (abs(leftTicks) < rightTicks)
        {
            motors.leftBrake();
            motors.rightBrake();
            motors.leftMotor(-(FAST_SPEED + offset));
            motors.rightMotor(FAST_SPEED - offset);
        }
    }
    else if (direction == 1)
    {
        if (leftTicks > abs(rightTicks))
        {
            motors.leftBrake();
            motors.rightBrake();
            motors.leftMotor(FAST_SPEED - offset);
            motors.rightMotor(-(FAST_SPEED + offset));
        }
        else if (leftTicks < abs(rightTicks))
        {
            motors.leftBrake();
            motors.rightBrake();
            motors.leftMotor(FAST_SPEED + offset);
            motors.rightMotor(-(FAST_SPEED - offset));
        }
    }
}

void readSensorData()
{
    //  Provides encoder readings through serial monitor
    Serial.println("Encoder Values:");
    Serial.print(encoder.getTicks(RIGHT));
    Serial.print("\t");
    Serial.print(encoder.getTicks(LEFT));
    Serial.print("\t");
    Serial.println();
}
