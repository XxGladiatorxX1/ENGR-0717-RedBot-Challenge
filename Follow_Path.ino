#include <RedBot.h>
RedBotSensor left = RedBotSensor(A3);   // initialize a left sensor object on A3
RedBotSensor center = RedBotSensor(A6); // initialize a center sensor object on A6
RedBotSensor right = RedBotSensor(A7);  // initialize a right sensor object on A7

// constants that are used in the code. LINETHRESHOLD is the level to detect 
// if the sensor is on the line or not. If the sensor value is greater than this
// the sensor is above a DARK line.
//
// SPEED sets the nominal speed

#define DARKLINE 810
#define LIGHTLINE 790
#define SPEED 40  // sets the nominal speed. Set to any number from 0 - 255.

RedBotMotors motors;
int leftSpeed;   // variable used to store the leftMotor speed
int rightSpeed;  // variable used to store the rightMotor speed

void setup()
{
    Serial.begin(9600);
    delay(2000);
    Serial.println("IR Sensor Readings: ");
    delay(500);
}

void loop()
{
    Serial.print(left.read());
    Serial.print("\t");  // tab character
    Serial.print(center.read());
    Serial.print("\t");  // tab character
    Serial.print(right.read());
    Serial.println();


    /*
    // if on the line drive left and right at the same speed (left is CCW / right is CW)
    if(center.read() > LIGHTLINE && center.read() < DARKLINE)
    {
        leftSpeed = SPEED;
        rightSpeed = SPEED;
    }

    // if the line is under the right sensor, adjust relative speeds to turn to the right
    else if(right.read() < LIGHTLINE)
    {
        rightSpeed = SPEED;
        leftSpeed = 0;
    }

    // if the line is under the left sensor, adjust relative speeds to turn to the left
    else if(left.read() < LIGHTLINE)
    {
        leftSpeed = SPEED;
        rightSpeed = 0;
    }

    // if all sensors are on black or up in the air, stop the motors.
    // otherwise, run motors given the control speeds above.
    if((left.read() > DARKLINE || left.read() < LIGHTLINE) && (center.read() > DARKLINE || center.read() < LIGHTLINE) && (right.read() > DARKLINE || right.read() < LIGHTLINE) )
    {
        motors.stop();
    }
    else
    {
        motors.leftMotor(leftSpeed);
        motors.rightMotor(rightSpeed);

    }
    delay(0);  // add a delay to decrease sensitivity.
    */
}