#include <Arduino.h>
#line 1 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test1\\Test1.ino"
/* Matthew: Overall, everything looks great :)
 * 
 * There's one concept in C++ that we wouldn't have learned in 0711
 * that's kinda important here. In C++, there are these "classes" that
 * are basically just custom data types. You have your regular types like
 * your ints, your floats, your doubles, and your booleans, but classes 
 * are like combinations of these data types that you can create yourself 
 * and tailor to your liking.
 * 
 * The only reason this is important is because C++ is an object-oriented language.
 * In other words, the code is based entirely on variables that you declare with
 * classes (this kind of variable is called an "object"). I'll give you an example: you might 
 * declare an integer variable "int number = 20", where "number" is an instance of an int.
 * An object is like that, where you might declare "RedBotSensor IRSensor = RedBotSensor(A3)".
 * In this case, the object "IRSensor" is an instance of the RedBotSensor class.
 * 
 * So why does any of this matter? You might notice that when I went through your code,
 * I took all the "drive" functions and threw the word "motors" in front of all of them. This
 * is because every function that we're using in this code needs to be attached to some object.
 * If you don't mention the object, or variable, that you want C++ to run a function on, it won't
 * know which motor or sensor you want to operate. That's basically the takeaway -- when you call
 * a function, like "drive()" or "read()" or "leftMotor()", you NEED to call it on an object for
 * the function to make any sense to C++. If you take a look at the INSTANCE VARIABLES header, I
 * put all the objects there that you need for the line following algorithm.
 * 
 * You might also wonder what that "RedBotSensor(A3)" part of the declaration means in
 * "RedBotSensor IRSensor = RedBotSensor(A3)". That's formatting for another object-oriented concept
 * called constructors, which I'm also happy to talk about :)
 * 
 * *reader gains +999999999 kg of brain matter*
 */

 /* ohhhhh i see i see thank yopu so much!! I don't have experience with this kind of coding.
 * When I did robotics I didn't make it FTC or FRC so like it was just coding tih the premade lego language or 
 * whatever it was so thank youuuu thank you :))
 */

//  IMPORTS
#include <RedBot.h>

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
#define BLACK_MIN 990
#define BLACK_MAX 1100
#define RED_MIN 50
#define RED_MAX 200
#define BROWN_MIN 400
#define BROWN_MAX 980


//  Nominal speeds for the motors (max is 255)
#define SLOW_SPEED 20
#define MID_SPEED 125
#define FAST_SPEED 255
#define REV_MID -125
#define REV_SLOW -20
#define REV_FAST -255

#line 73 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test1\\Test1.ino"
void setup();
#line 82 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test1\\Test1.ino"
void loop();
#line 129 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test1\\Test1.ino"
void readSensorData();
#line 149 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test1\\Test1.ino"
void encodedDrive(int motorPower);
#line 73 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test1\\Test1.ino"
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
    while (true)
    {
    readSensorData();
    encodedDrive(MID_SPEED - 30);
    }

    if (leftIRSensor.read() > BLACK_MIN && centerIRSensor.read() > BLACK_MIN && rightIRSensor.read() > BLACK_MIN)
    {
        //  pause
        motors.leftBrake();
        motors.rightBrake();
        motors.leftMotor(MID_SPEED);
        motors.rightMotor(MID_SPEED);
        //GRAB ALIEN HERE!!!!!!!!!!!!

        //  program accelrometer here
    }
    else if (((centerIRSensor.read() > BROWN_MIN && centerIRSensor.read() < BROWN_MAX) && 
        (rightIRSensor.read() > BROWN_MIN && rightIRSensor.read() < BROWN_MIN) && 
        (leftIRSensor.read() > BROWN_MIN && leftIRSensor.read())) ||
        (centerIRSensor.read() > BLACK_MIN && 
        (rightIRSensor.read() > BROWN_MIN && rightIRSensor.read() < BROWN_MIN) && 
        (leftIRSensor.read() > BROWN_MIN && leftIRSensor.read())))
    {
        //  driving forward
        motors.leftBrake();
        motors.rightBrake();
        motors.leftMotor(MID_SPEED);
        motors.rightMotor(MID_SPEED);
    }
    else if (leftIRSensor.read() > BLACK_MIN && leftIRSensor.read() < BLACK_MAX)
    {
        //  turn left
        motors.rightMotor(MID_SPEED);
        motors.leftBrake();
    }
    else if (rightIRSensor.read() > BLACK_MIN && rightIRSensor.read() < BLACK_MAX)
    {
        //  turn right
        motors.leftMotor(MID_SPEED);
        motors.rightBrake();
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

    //  Provides IR readings through serial monitor
    Serial.println("IR Sensor Values:");
    Serial.print(leftIRSensor.read());
    Serial.print("\t");
    Serial.print(centerIRSensor.read());
    Serial.print("\t");
    Serial.print(rightIRSensor.read());
    Serial.println();
}

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

        //  store the encoder values for comparison.
        leftEncCount = encoder.getTicks(LEFT);
        rightEncCount = -encoder.getTicks(RIGHT);

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
