#include <AccelStepper.h>

//(type, step pin, direction pin)
AccelStepper stepperX(AccelStepper::DRIVER, 5, 4);
AccelStepper stepperY(AccelStepper::DRIVER, 8, 7);

unsigned long nextUpdate = 0;

int timer1_counter;

boolean homeing = false;

void setup()
{
  Serial.begin(57600);
  Serial.setTimeout(25);
  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(300);


  stepperY.setMaxSpeed(750);
  stepperY.setAcceleration(100);


  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  timer1_counter = 64886;   // preload timer 65536-16MHz/8/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  //timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz

  TCNT1 = timer1_counter;   // preload timer
  //TCCR1B |= (1 << CS11) | (1 << CS10);    // 64 prescaler 
  TCCR1B |= (1 << CS11);    // 8 prescaler
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  pinMode(2, INPUT_PULLUP);
  digitalWrite(6, HIGH);
  digitalWrite(9, HIGH);
}
ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = timer1_counter;   // preload timer
  stepperX.run();
  stepperY.run();
}
void loop()
{


  if (!homeing) {
    handleInput();
  }
  update();

  if (homeing) {
    if (digitalRead(2)) {
      
      stepperX.move(-10);
    
    } else {
      stepperX.stop();
      stepperX.setCurrentPosition(0);
      homeing = false;
      
    }
  }

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
    Serial.print(" ");
    Serial.print((homeing ? 'y' : 'n'));
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

    if (command == 's') {
      char motor = Serial.read();
      long speed = Serial.parseInt();

      if (motor == 'Y') {

        stepperY.setMaxSpeed(speed);
      }          
      if (motor == 'X') {

        stepperX.setMaxSpeed(speed);
      }

    }

    if (command == 'a') {
      char motor = Serial.read();
      long acceleration = Serial.parseInt();

      if (motor == 'Y') {

        stepperY.setAcceleration(acceleration);
      }
      if (motor == 'X') {

        stepperX.setAcceleration(acceleration);
      }
    }

    if (command == 'h') {
      homeing = true;
    }

  }
}

