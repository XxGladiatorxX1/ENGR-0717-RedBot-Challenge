
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
#define BLACK_MIN 900
#define BLACK_MAX 1100
#define RED_MIN 50
#define RED_MAX 200
#define BROWN_MIN 400
#define BROWN_MAX 600


//  Nominal speeds for the motors (max is 255)
#define SLOW_SPEED 10
#define FAST_SPEED 100
#define REV_SLOW -10
#define REV_FAST -100

void setup()
{
    //  Initialize baudrate
    Serial.begin(9600);

    //  Motor encoder calibration
    encoder.clearEnc(BOTH);
}

void loop()
{
    //  Provides IR readings through serial monitor
    Serial.print(leftIRSensor.read());
    Serial.print("\t");
    Serial.print(centerIRSensor.read());
    Serial.print("\t");
    Serial.print(rightIRSensor.read());
    delay(1000);
    Serial.println();

    //  Matthew: one question, how come the value being compared to is 0?
    if (leftIRSensor.read() == 0 && rightIRSensor.read() == 0 && centerIRSensor.read() == 0)
    {
        //  driving forward
        motors.drive(FAST_SPEED);
    }
    else if (leftIRSensor.read() > BLACK_MIN && rightIRSensor.read() < BLACK_MIN)
    {
        //  turn left
        motors.rightMotor(SLOW_SPEED);
        motors.leftMotor(REV_SLOW);
    }
    else if (leftIRSensor.read() < BLACK_MIN && rightIRSensor.read() > BLACK_MIN)
    {
        //  turn right
        motors.leftMotor(SLOW_SPEED);
        motors.rightMotor(REV_SLOW);
    }
    else if (leftIRSensor.read() > BLACK_MIN && centerIRSensor.read() > BLACK_MIN && rightIRSensor.read() > BLACK_MIN)
    {
        //  pause
        motors.brake();
        //  turn at the mound
        // Matthew: another question, is "i" supposed to be the time duration of the turn?
        for (int i = 0; i < 20000; i++)
        {
            motors.leftMotor(SLOW_SPEED);
            motors.rightMotor(REV_SLOW);
        }  

        //GRAB ALIEN HERE!!!!!!!!!!!!
        
        //  drive forward slightly
        motors.drive(FAST_SPEED, 1000);
        //  turn right even more (repeat earlier)
        for (int i = 0; i < 20000; i++) {
            motors.leftMotor(SLOW_SPEED);
            motors.rightMotor(REV_SLOW);
        } 
        //  drive to the end
        motors.drive(FAST_SPEED, 10000);
    }
}
