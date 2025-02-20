#include <Arduino.h>
#line 1 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test_2\\Test_2.ino"
//  IMPORTS
#include <RedBot.h>

//  INSTANCE VARIABLES
/* Sensor, encoder, and motor object initializations. Note that sensor constructors 
 * are used to instantiate no particular sensor
 */
RedBotEncoder encoder = RedBotEncoder(A2, 10);
RedBotSensor leftIRSensor = RedBotSensor(A3);
RedBotSensor centerIRSensor = RedBotSensor(A6);
RedBotSensor rightIRSensor = RedBotSensor(A7);
RedBotAccel accelerometer;
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

#line 34 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test_2\\Test_2.ino"
void setup();
#line 43 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test_2\\Test_2.ino"
void loop();
#line 72 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test_2\\Test_2.ino"
void encodedDrive();
#line 101 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test_2\\Test_2.ino"
void encodedTankTurn(int direction);
#line 150 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test_2\\Test_2.ino"
void driveRamp();
#line 157 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test_2\\Test_2.ino"
void driveMound();
#line 167 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test_2\\Test_2.ino"
void readSensorData();
#line 34 "C:\\Users\\mdjor\\ENGR-0717-RedBot-Challenge\\Test_2\\Test_2.ino"
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
    accelerometer.read();
    readSensorData();

    //while (leftIRSensor.read() > BLACK_MIN && centerIRSensor.read() > BLACK_MIN && rightIRSensor.read() > BLACK_MIN)
    //{
    //    encodedDrive();
    //}
    //encodedDrive();
    //if (leftIRSensor.read() > BLACK_MIN && leftIRSensor.read() < BLACK_MAX)
    //{
    //    encodedTankTurn(0);
    //}
    //else if (rightIRSensor.read() > BLACK_MIN && rightIRSensor.read() < BLACK_MAX)
    //{
    //   encodedTankTurn(1);
    //}
    //encodedTankTurn(0);


    driveMound();
    if (accelerometer.angleXZ > 20)
    {
        driveRamp();
    }
}

void encodedDrive()
{
    if (-encoder.getTicks(LEFT) > 0 && abs(encoder.getTicks(RIGHT)) < 0 || -encoder.getTicks(LEFT) < 0 && abs(encoder.getTicks(RIGHT)) > 0)
    {
        encoder.clearEnc(BOTH);
    }

    int initializer = 1; // value to ensure starting encoder value is above 0
    int leftTicks = -encoder.getTicks(LEFT) + initializer;
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
    //  clear encoders if transitioning from straight drive to turning
    if (-encoder.getTicks(LEFT) > 0 && encoder.getTicks(RIGHT) > 0 || -encoder.getTicks(LEFT) < 0 && encoder.getTicks(RIGHT) < 0)
    {
        encoder.clearEnc(BOTH);
    }

    int initializer = 1; // value to ensure starting encoder value is above 0
    int leftTicks = -encoder.getTicks(LEFT) + initializer;
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

void driveRamp()
{
    motors.leftBrake();
    motors.rightBrake();
    encodedDrive();
}

void driveMound() 
{
    motors.leftBrake();
    motors.rightBrake();
    encodedTankTurn(1);
    delay(1000);
    motors.leftBrake();
    motors.rightBrake();
}

void readSensorData()
{
    //  Provides encoder readings through serial monitor
    /*
    Serial.println("Encoder Values:");
    Serial.print(encoder.getTicks(RIGHT));
    Serial.print("\t");
    Serial.print(-encoder.getTicks(LEFT));
    Serial.print("\t");
    Serial.println();
    */

    /*
    //  Provides IR readings through serial monitor
    Serial.println("IR Sensor Values:");
    Serial.print(leftIRSensor.read());
    Serial.print("\t");
    Serial.print(centerIRSensor.read());
    Serial.print("\t");
    Serial.print(rightIRSensor.read());
    Serial.println();
    */

    // Provides accelerometer readings through serial monitor
    accelerometer.read();
    Serial.println("Accelerometer Values:");
    Serial.print(accelerometer.angleXZ);
    Serial.print("\t");
    Serial.print(accelerometer.angleXY);
    Serial.print("\t");
    Serial.print(accelerometer.angleYZ);
    Serial.println();
}

