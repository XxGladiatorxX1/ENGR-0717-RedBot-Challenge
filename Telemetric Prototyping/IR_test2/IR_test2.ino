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
    if (centerIRSensor.read() >leftIRSensor.read() && centerIRSensor.read() > rightIRSensor.read())
    {
        //  driving forward
        motors.drive(FAST_SPEED);
    }
    else if (leftIRSensor.read() > rightIRSensor.read() && leftIRSensor.read() > centerIRSensor.read())
    {
        //  turn left
        motors.rightMotor(SLOW_SPEED);
        motors.leftMotor(REV_SLOW);
    }
    else if (rightIRSensor.read() > leftIRSensor.read() && rightIRSensor.read() > centerIRSensor.read())
    {
        //  turn right
        motors.leftMotor(SLOW_SPEED);
        motors.rightMotor(REV_SLOW);
    }
    else if (leftIRSensor.read() > BLACK_MIN && centerIRSensor.read() > BLACK_MIN && rightIRSensor.read() > BLACK_MIN)
    {
        //  pause
        motors.drive(FAST_SPEED, 1000);
        //GRAB ALIEN HERE!!!!!!!!!!!!

        //  program accelrometer here
    }
}
