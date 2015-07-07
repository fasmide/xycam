#include <AccelStepper.h>


AccelStepper stepper(AccelStepper::DRIVER, 2, 5); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

AccelStepper stepper2(AccelStepper::DRIVER, 3, 6);

unsigned long nextUpdate = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (stepper.distanceToGo() == 0)
    {
    
        stepper.moveTo(rand() % 200);
        stepper.setMaxSpeed((rand() % 200) + 1);
        stepper.setAcceleration((rand() % 200) + 1);
    }

    stepper.run();
    
    if (stepper2.distanceToGo() == 0)
    {
    
        stepper2.moveTo(rand() % 200);
        stepper2.setMaxSpeed((rand() % 200) + 1);
        stepper2.setAcceleration((rand() % 200) + 1);
    }

    stepper2.run();
    
    //this should be every second to so...
    //it will overflow at some point...
    if (millis() >= nextUpdate) {
        
        Serial.print("Value is:");
        Serial.print(stepper.currentPosition());
        Serial.print(" ");
        Serial.print(stepper2.currentPosition());
        Serial.print("\r\n");
        nextUpdate = millis() + 1000;
    }
}
