#include <AccelStepper.h>
#include <TimerOne.h>

//(type, step pin, direction pin)
AccelStepper stepperX(AccelStepper::DRIVER, 5, 4);
AccelStepper stepperY(AccelStepper::DRIVER, 8, 7);

unsigned long nextUpdate = 0;


boolean enabled = false;
boolean homeing = false;

void setup()
{
  Serial.begin(57600);
  Serial.setTimeout(25);
  stepperX.setMaxSpeed(3000);
  stepperX.setAcceleration(4000);


  stepperY.setMaxSpeed(1000);
  stepperY.setAcceleration(4000);


  Timer1.initialize(86);              // Microseconds
  Timer1.attachInterrupt( timerIsr );  // Attach stepper update function
  pinMode(2, INPUT_PULLUP);            // Enable pullup resistors for the homing switch
  attachInterrupt(0, homeXIsr, FALLING);   // Attach homing function
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);


}
void timerIsr()
{
  stepperY.run();
  stepperX.run();
}
void homeXIsr() {
  stepperX.setCurrentPosition(0);
  stepperX.move(0);
  homeing = false;
}
void loop()
{


  handleInput();
  
  update();

  if (homeing) {
    enable();
    if (!digitalRead(2)) { // Allready at home position
      stepperX.move(1000); // Move away from home
      }
    if (digitalRead(2)) {  // If not homed
      stepperX.move(-1000);// Move towards home
    }
  }
}

void enable() {
  enabled = true;
  digitalWrite(6, HIGH);
  digitalWrite(9, HIGH);
}
void disable() {
  enabled = false;
  digitalWrite(6, LOW);
  digitalWrite(9, LOW);

}
void update() {

  //this should be every second or so
  //it will overflow and stop working at some point :P
  if (millis() >= nextUpdate) {

    Serial.print(stepperX.currentPosition());
    Serial.print(" ");
    Serial.print(stepperY.currentPosition());
//    Serial.print(" ");
//    Serial.print(stepperX.targetPosition());
//    Serial.print(" ");
//    Serial.print(stepperY.targetPosition());
//    Serial.print(" ");
//    Serial.print(stepperX.distanceToGo());
//    Serial.print(" ");
//    Serial.print(stepperY.distanceToGo());
    Serial.print(" ");
    Serial.print((homeing ? 'y' : 'n'));
    Serial.print(" ");
    Serial.print((enabled ? 'y' : 'n'));
    Serial.print(" ");
    Serial.print(millis()-nextUpdate);
    Serial.print("\r");
    nextUpdate = millis() + 1000;
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
    if (command == 'E') {
      enable();
    }
    if (command == 'e') {
      disable();
    }
  }
}
