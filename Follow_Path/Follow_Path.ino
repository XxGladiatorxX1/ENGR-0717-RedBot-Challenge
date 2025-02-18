//  IMPORTS
#include <RedBot.h>
#include <math.h>

//  INSTANCE VARIABLES
/* Sensor, encoder, and motor object initializations. Note that sensor constructors 
 * are used to instantiate no particular sensor
 */
RedBotSensor leftIRSensor = RedBotSensor(A3);
RedBotSensor centerIRSensor = RedBotSensor(A6);
RedBotSensor rightIRSensor = RedBotSensor(A7);
RedBotEncoder encoder = RedBotEncoder(A2, 10);
RedBotMotors motors;

//  Variables to store motor speeds
int leftMotorSpeed;
int rightMotorSpeed;

//  CONSTANTS
//  The IR sensor values, measured 1/8" from the ground
#define BLACK_MIN 900
#define BLACK_MAX 1100
#define RED_MIN 50
#define RED_MAX 200
#define BROWN_MIN 400
#define BROWN_MAX 600

//  ---------------------------------NEED TO FIX COUNTS_PER_REV FOR NEW GEAR RATIO--------------------------------------
#define COUNTS_PER_REV 192  // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev
#define WHEEL_DIAM 8.597  // diam = 218.364mm / 25.4 mm/in
#define WHEEL_CIRC PI * WHEEL_DIAM  // Redbot wheel circumference = pi * D

//  Nominal speeds for the motors (max is 255)
#define SLOW_SPEED 10
#define FAST_SPEED 100

void setup()
{
    // Initialize baudrate
    Serial.begin(9600);

    //  Motor encoder calibration
    encoder.clearEnc(BOTH);
}

void loop()
{
    readSensorData();

    // NEED TO TEST whether this function pauses the loop
    // If it does, I'll need to do away with its while loop
    encodedDrive(FAST_SPEED);
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

    //  Provides IR readings through serial monitor
    Serial.println("IR Sensor Values:");
    Serial.print(leftIRSensor.read());
    Serial.print("\t");
    Serial.print(centerIRSensor.read());
    Serial.print("\t");
    Serial.print(rightIRSensor.read());
    Serial.println();
    delay(1000);
}

/*  Function that uses encoders to drive the robot in a straight, accurate path.
 *  motorPower is the speed (0 - 255) that the motors can be set to
 */
void encodedDrive(int motorPower)
{
    //  variables for tracking the left and right encoder counts
    long leftEncCount = 0;
    long rightEncCount = 0;
    long prevLeftEncCount, prevRightEncCount;
    long leftDiff, rightDiff;  // difference between current encoder count and previous count

    //  variables for setting left and right motor power
    int leftPower = motorPower;
    int rightPower = motorPower;

    //  variable used to offset motor power on right vs left to keep straight.
    int offset = 5;  // offset amount to compensate Right vs. Left drive

    encoder.clearEnc(BOTH);  // clear the encoder count
    delay(100);  // short delay before starting the motors.
    motors.drive(motorPower);  // start motors 

    while (true)
    {
        //  store the encoder values for comparison.
        leftEncCount = encoder.getTicks(LEFT);
        rightEncCount = encoder.getTicks(RIGHT);

        motors.leftDrive(leftPower);
        motors.rightDrive(rightPower);

        //  calculate the rotation "speed" as a difference in the count from previous cycle.
        leftDiff = (leftEncCount - prevLeftEncCount);
        rightDiff = (rightEncCount - prevRightEncCount);

        //  store the current count as the "previous" count for the next cycle.
        prevLeftEncCount = leftEncCount;
        prevRightEncCount = rightEncCount;

        //  if left is faster than the right, slow down the left / speed up right
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
        delay(50); //  short delay to give motors a chance to respond.
    }
}

/*  Function that uses encoders to accurately turn the robot in place.
 *  motorPower is the speed (0 - 255) that the motors can be set to.
 *  direction is 0 for counterclockwise and 1 for clockwise turning
 */
void encodedTankTurn(int motorPower, int direction)
{
    //  variables for tracking the left and right encoder counts
    long leftEncCount = 0;
    long rightEncCount = 0;
    long prevLeftEncCount, prevRightEncCount;
    long leftDiff, rightDiff;  // difference between current encoder count and previous count

    //  variables for setting left and right motor power
    int leftPower = motorPower;
    int rightPower = motorPower;

    //  depending on the direction of the turn, negate one pair of motors
    if (direction == 0)
        leftPower *= -1;
    else if (direction == 1)
        rightPower *= -1;

    //  variable used to offset motor power on right vs left to keep straight.
    int offset = 5;  // offset amount to compensate Right vs. Left drive

    encoder.clearEnc(BOTH);  //  clear the encoder count
    delay(100);  //  short delay before starting the motors.
    motors.drive(motorPower);  //  start motors

    while (true)
    {
        //  store the encoder values for comparison.
        leftEncCount = abs(encoder.getTicks(LEFT));
        rightEncCount = abs(encoder.getTicks(RIGHT));

        motors.leftDrive(leftPower);
        motors.rightDrive(rightPower);

        //  calculate the rotation "speed" as a difference in the count from previous cycle.
        leftDiff = (leftEncCount - prevLeftEncCount);
        rightDiff = (rightEncCount - prevRightEncCount);

        //  store the current count as the "previous" count for the next cycle.
        prevLeftEncCount = leftEncCount;
        prevRightEncCount = rightEncCount;

        //  if left is faster than the right, slow down the left / speed up right
        if (leftDiff > rightDiff) 
        {
            //  adds or subtracts power offset depending on direction of turn
            if (leftPower > 0)
            {
                leftPower = leftPower - offset;
                rightPower = rightPower + offset;
            }
            else
            {
                leftPower = leftPower + offset;
                rightPower = rightPower - offset;
            }
        }
        //  if right is faster than the left, speed up the left / slow down right
        else if (leftDiff < rightDiff) 
        {
            //  adds or subtracts power offset depending on direction of turn
            if (rightPower > 0)
            {
                leftPower = leftPower + offset;  
                rightPower = rightPower - offset;
            }
            else
            {
                leftPower = leftPower - offset;
                rightPower = rightPower + offset;
            }
        }
        delay(50); //  short delay to give motors a chance to respond.
    }
}

/*  Function that uses encoders to accurately turn the robot via powering
 *  a singular set of motors on one side of the robot.
 *  motorPower is the speed (0 - 255) that the motors can be set to.
 *  direction is 0 for powering right motors and 1 for powering left motors
 */
void encodedSkidTurn(int motorPower, int direction)
{
    // TO BE IMPLEMENTED
    // I'll probably end up using a motor power-to-wheel rotation function to acheive this
}
