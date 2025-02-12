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

void setup()
{
    // Initialize baudrate
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
}
