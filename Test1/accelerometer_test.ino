
#include <RedBot.h>

//add setup mototr stuff
int motorPower;
int currentMillis;
int m;

void setup(void) {

    Serial.begin(9600);
    Serial.println("Accelerometer Readings:");
    Serial.println();
    Serial.println("(X, Y, Z) -- [X-Z, Y-Z, X-Y]");
    Serial.println("============================");

}

void loop(void) {

    accelerometer.read();
    
    //read in where redbot is in space (oooooooooooo)
    int xAccel = accelerometer.x;
    int yAccel = accelerometer.y;
    int zAccel = accelerometer.z;

    float XZ = accelerometer.angleXZ;  
    float YZ = accelerometer.angleYZ;  
    float XY = accelerometer.angleXY; 

    /*going to find angle values and buffers when 
    *when there is the time to test
    *using test values for now
    */

    if (XZ > 20) {

        while (XZ > 15) { //chnage the angles?????
            motorPower = map(XZ, 0, 90, 0, 255);
            motors.drive(motorPower); //setting the motors to new scaling
            accelerometer.read();       
            XZ = accelerometer.angleXZ;
        }
    }
    else {

        motors.drive(motorPower);
        m=millis();
        currentMillis=millis();
        while((currentMillis()-m) <2000) { //number variable
            motors.leftBrake();
            motors.rightBrake();
        }
    }
}