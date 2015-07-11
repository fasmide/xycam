#include <AccelStepper.h>

//(type, step pin, direction pin)
AccelStepper stepperX(AccelStepper::DRIVER, 2, 5);
AccelStepper stepperY(AccelStepper::DRIVER, 3, 6);

unsigned long nextUpdate = 0;

int timer1_counter;

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(10);
    stepperX.setMaxSpeed(750);
    stepperX.setAcceleration(100);
    
    stepperY.setMaxSpeed(750);
    stepperY.setAcceleration(100);

    
    // initialize timer1 
    noInterrupts();           // disable all interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    
    // Set timer1_counter to the correct value for our interrupt interval
    timer1_counter = 64886;   // preload timer 65536-16MHz/256/100Hz
    //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
    //timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
    
    TCNT1 = timer1_counter;   // preload timer
    //TCCR1B |= (1 << CS11) | (1 << CS10);    // 64 prescaler 
    TCCR1B |= (1 << CS11);    // 8 prescaler
    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
    interrupts();             // enable all interrupts
}
ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
    TCNT1 = timer1_counter;   // preload timer
    stepperX.run();
    stepperY.run();
}
void loop()
{
    
    
    
    handleInput();
    update();

}


void update() {
  
    //this should be every second or so
    //it will overflow and stop working at some point :P
    if (millis() >= nextUpdate) {
      
        Serial.print(stepperX.currentPosition());
        Serial.print(" ");
        Serial.print(stepperY.currentPosition());
        Serial.print(" ");
        Serial.print(stepperX.targetPosition());
        Serial.print(" ");
        Serial.print(stepperY.targetPosition());
        Serial.print(" ");
        Serial.print(stepperX.distanceToGo());
        Serial.print(" ");
        Serial.print(stepperY.distanceToGo());
        Serial.print("\r\n");
        
        nextUpdate = millis() + 250;
    }
}
void handleInput() {
      if (Serial.available() > 0) {
          
          char command = Serial.read();
          
          if (command == 'j') { //j as in jog
              long jogX = Serial.parseInt();
              long jogY = Serial.parseInt();
              //we are jogging
              stepperX.move(jogX);
              stepperY.move(jogY);
          }
          
          if (command == 'g') { 
              long absoluteX = Serial.parseInt();
              long absoluteY = Serial.parseInt();
              stepperX.moveTo(absoluteX);
              stepperY.moveTo(absoluteY);
          }
        
      }
}
