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

    encodedDrive();
    //encodedTankTurn(0);
    //driveYaw();
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

void driveYaw()
{
    accelerometer.read();

    // read in where redbot is in space (oooooooooooo)
    int xAccel = accelerometer.x;
    int yAccel = accelerometer.y;
    int zAccel = accelerometer.z;
    int motorPower, m, currentMillis;

    float XZ = accelerometer.angleXZ;  
    float YZ = accelerometer.angleYZ;  
    float XY = accelerometer.angleXY; 

    /* going to find angle values and buffers when 
    *  when there is the time to test
    *  using test values for now
    */

    if (XZ > 20)
    {
        while (XZ > 15) // chnage the angles?????
        {
            motorPower = map(XZ, 0, 90, 0, 255);
            motors.drive(motorPower); // setting the motors to new scaling
            accelerometer.read();       
            XZ = accelerometer.angleXZ;
        }
    }
    else
    {
        motors.drive(motorPower);
        while((currentMillis - m) < 2000) // number variable
        {
            motors.leftBrake();
            motors.rightBrake();
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

    /*
    // Provides accelerometer readings through serial monitor
    accelerometer.read();
    Serial.println("Accelerometer Values:");
    Serial.print(accelerometer.angleXZ);
    Serial.print("\t");
    Serial.print(accelerometer.angleXY);
    Serial.print("\t");
    Serial.print(accelerometer.angleYZ);
    Serial.println();
    */
}
