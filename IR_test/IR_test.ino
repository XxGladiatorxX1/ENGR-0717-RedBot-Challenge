#include <RedBot.h>
// attatch sensors ot pins
RedBotSensor left = RedBotSensor(A3);   
RedBotSensor center = RedBotSensor(A6); 
RedBotSensor right = RedBotSensor(A7);  

// constants that are used in the code. LINETHRESHOLD is the level to detect 
// if the sensor is on the line or not. If the sensor value is greater than this
// the sensor is above a DARK line.
//
// SPEED sets the nominal speed

//change values later depending on value of red line
#define DARKLINE 810 // will be red line
#define LIGHTLINE 790
#define SPEED 40  

RedBotMotors motors;
int leftSpeed;   // variable used to store the leftMotor speed
int rightSpeed;  // variable used to store the rightMotor speed

void setup()
{
    Serial.begin(9600); // prints in serial monitor
    Serial.println("Welcome to Line Following");
    Serial.println("------------------------------------------");
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
   

    // while the line is under the left sensor, adjust relative speeds to turn to the right
    do
    {
        leftSpeed = SPEED;
        rightSpeed = 0;
    }
    while (left.read() < LIGHTLINE);

    //readjust to go straight



    // if all sensors are on black or up in the air, stop the motors.
    // otherwise, run motors given the control speeds above.
    if ((left.read() > DARKLINE || left.read() < LIGHTLINE) && (center.read() > DARKLINE || center.read() < LIGHTLINE) && (right.read() > DARKLINE || right.read() < LIGHTLINE))
    {
        motors.stop();
    }
    else
    {
        motors.leftMotor(leftSpeed);
        motors.rightMotor(rightSpeed);
    }
    delay(0);  // add a delay to decrease sensitivity.
}
