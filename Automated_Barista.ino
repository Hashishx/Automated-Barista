/******************************************************************************
 *
 * File Name: Automated_Barista.ino
 *
 * Description: Application File for the Automated Barista Project
 *
 * Author: Mahmoud Hashish
 ******************************************************************************/

/* Includes */
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <LiquidCrystal.h>

/* Initializing LCD*/
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/* Button pin definitions */
const int button1Pin = 6;
const int button2Pin = 7;

/* Global Variables */
int pressCount = 0;
int sugar_Delay_Time = 0;
bool continueProgram = false;
bool system = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
bool lastBtn1State = HIGH;
bool lastBtn2State = HIGH;

/* Create servo driver object */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/* Pre-Processor Macros */
#define SERVOMIN  10 
#define SERVOMAX  600 
#define base_Servo_Channel  0
#define shoulder_Servo_Channel  1
#define arm_Servo_Channel  2
#define wrist_Servo_Channel  3
#define gripper_Servo_Channel  4
#define sugar_Servo_Channel   5
#define coffee_Servo_Channel   6
#define mixer_Holder_Servo_Channel   7
#define water_Pump_Relay_Pin  8
#define milk_Pump_Relay_Pin  9
#define mixer_Relay_Pin  10

/* Function to Map angle (0–180) to PWM pulse length */
uint16_t angleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

/* Function to control the servo speed using pulses that is sent from Arduino to servo driver */
void moveServoSmoothly(int channel, int startAngle, int targetAngle, int stepDelay) {
  if (startAngle < targetAngle) {
    for (int angle = startAngle; angle <= targetAngle; angle++) {
      pwm.setPWM(channel, 0, angleToPulse(angle));
      delay(stepDelay); 
    }
  } else {
    for (int angle = startAngle; angle >= targetAngle; angle--) {
      pwm.setPWM(channel, 0, angleToPulse(angle));
      delay(stepDelay);
    }
  }
}
/* Function to clear LCD rows */
void clearRow(int row) {
  lcd.setCursor(0, row);
  for (int i = 0; i < 20; i++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, row); // Optionally reset cursor to start of row
}
/* Function to read button states */
void readButtonState (void) {
  /* Read current states */
  bool btn1State = digitalRead(button1Pin);
  bool btn2State = digitalRead(button2Pin);
  /* Enter Button */
  if (btn2State == LOW && lastBtn2State == HIGH) {
    delay(debounceDelay);
    if (digitalRead(button2Pin) == LOW) {
      continueProgram = true;
    }
  }
  /* Select Button */
  if (btn1State == LOW && lastBtn1State == HIGH && pressCount < 4) {
    delay(debounceDelay);
    if (digitalRead(button1Pin) == LOW) {
      if (pressCount == 3) {
        pressCount = 0;
        sugar_Delay_Time = pressCount * 300;
        lcd.setCursor(13, 3);
        lcd.print(pressCount);
        lcd.print("   ");
      }
      else {
        pressCount++;
        sugar_Delay_Time = pressCount * 300;
        lcd.setCursor(13, 3);
        lcd.print(pressCount);
        lcd.print("   ");
      }
    }
  }
  /* Update previous states */
  lastBtn1State = btn1State;
  lastBtn2State = btn2State;
}
/* Function to enter the desired Coffee Scoop */
void coffeeScoop(void) {
  if (continueProgram) {
    continueProgram = false;
    lcd.setCursor(0, 3);
    lcd.print("Coffee Count: 0");
    lcd.setCursor(0, 1);
    lcd.print("Enter the desired ");
    lcd.setCursor(0, 2);
    lcd.print("amount of Coffee");
    readButtonState();
    if (continueProgram) {
      system = true;
    }
  }
}
/* Function to move Robot to Sugar Container*/
void moveToSugar (void) {
    clearRow(1);
    clearRow(3);
    clearRow(2);
    lcd.setCursor(0, 2);
    lcd.print("Moving to Sugar");
    int gripper_Rest_Angle = 160;
    int gripper_Holding_Angle = 130;
    int base_Rest_Current_Angle = 161;
    int base_Sugar_Target_Angle = 130;
    moveServoSmoothly(shoulder_Servo_Channel, 120, 125, 20); 
    delay(1000);
    moveServoSmoothly(gripper_Servo_Channel, gripper_Rest_Angle, gripper_Holding_Angle, 20); 
    delay(1000);
    moveServoSmoothly(base_Servo_Channel, base_Rest_Current_Angle, base_Sugar_Target_Angle, 20); 
    delay(500);
    moveServoSmoothly(wrist_Servo_Channel, 29, 27, 20); 
}

/* Function to put the desired sugar scoop */
void putSugarScoop (void) {
    clearRow(2);
    lcd.setCursor(0, 2);
    lcd.print("putting Sugar scoop");
    delay(2000);
    moveServoSmoothly(sugar_Servo_Channel, 75, 85, 20);
    delay(sugar_Delay_Time);
    moveServoSmoothly(sugar_Servo_Channel, 85, 75, 20);
    delay(1000);
}
/* Function to move Robot to Coffee Container */
void moveToCoffee (void) {
    clearRow(2);
    lcd.setCursor(0, 2);
    lcd.print("Moving to Coffee");
    int shoulder_sugar_Current_Angle = 120;
    int shoulder_Coffee_Target_Angle = 132;
    int arm_Sugar_Current_Angle = 160;
    int arm_Coffee_Target_Angle = 178;  
    int base_Sugar_Current_Angle = 130;
    int base_Coffee_Target_Angle = 105;
    moveServoSmoothly(arm_Servo_Channel, arm_Sugar_Current_Angle, arm_Coffee_Target_Angle, 20); 
    delay(500);
    moveServoSmoothly(base_Servo_Channel, base_Sugar_Current_Angle, base_Coffee_Target_Angle, 20); 
}
/* Function to put the desired Coffee Scoop */
void putCoffeeScoop (void) {
    clearRow(2);
    lcd.setCursor(0, 2);
    lcd.print("putting Coffee scoop");
    moveServoSmoothly(coffee_Servo_Channel, 110, 85, 20);
    delay(1500); 
    moveServoSmoothly(coffee_Servo_Channel, 85, 110, 20);
    delay(1000);
    moveServoSmoothly(coffee_Servo_Channel, 110, 85, 20);
    delay(1500);
    moveServoSmoothly(coffee_Servo_Channel, 85, 110, 20);
    delay(1000);
    moveServoSmoothly(coffee_Servo_Channel, 110, 85, 20);
    delay(1500);
    moveServoSmoothly(coffee_Servo_Channel, 85, 110, 20);
    delay(1000);  
}
/* Function to move robot to pick up station*/
void moveToPickUp (void) {
    clearRow(2);
    lcd.setCursor(0, 2);
    lcd.print("Moving to Pick Up");
    int base_Coffee_Current_Angle = 110;
    int base_Pick_Target_Angle = 72;
    int shoulder_Coffee_Current_Angle = 130;
    int shoulder_Pick_Target_Angle = 114;
    int arm_Coffee_Current_Angle = 180;
    int arm_Pick_Target_Angle = 132;   
    int wrist_Default_Angle = 33;
    int wrist_Pick_Angle = 45;
    int gripper_Realese_Angle = 170;
    moveServoSmoothly(base_Servo_Channel, base_Coffee_Current_Angle, base_Pick_Target_Angle, 20); 
    delay(1000);
    moveServoSmoothly(wrist_Servo_Channel, wrist_Default_Angle, wrist_Pick_Angle, 15); 
    delay(1000);
    moveServoSmoothly(arm_Servo_Channel, arm_Coffee_Current_Angle, arm_Pick_Target_Angle, 15); 
    moveServoSmoothly(shoulder_Servo_Channel, shoulder_Coffee_Current_Angle, shoulder_Pick_Target_Angle, 15); 
    moveServoSmoothly(gripper_Servo_Channel, gripper_Holding_Angle, gripper_Realese_Angle, 20); 
    delay(2000);
}
/* Function to move robot to rest position */
void moveToRest (void) {
    clearRow(2);
    lcd.setCursor(0, 2);
    lcd.print("Going back to Rest");  
    moveServoSmoothly(arm_Servo_Channel, 140, 160, 20); 
    delay(1000);
    moveServoSmoothly(shoulder_Servo_Channel, 114, 125, 20); 
    moveServoSmoothly(wrist_Servo_Channel, wrist_Pick_Angle, wrist_Default_Angle, 15); 
    delay(500);
    moveServoSmoothly(base_Servo_Channel, 70, 130, 15); 
    moveServoSmoothly(base_Servo_Channel, 130, 160, 15); 
    delay(1000);
}
/* Function to dispense water */
void dispenseWater (void) {
    clearRow(2);
    lcd.setCursor(0, 2);
    lcd.print("Dispensing Water");
    digitalWrite(water_Pump_Relay_Pin, HIGH);
    delay(7000);
    digitalWrite(water_Pump_Relay_Pin, LOW);
    delay(1000);
}
/* Function to dispence milk */
void dispenceMilk (void) {
    clearRow(2);
    lcd.setCursor(0, 2);
    lcd.print("Dispensing Milk");
    digitalWrite(milk_Pump_Relay_Pin, HIGH);
    delay(7000);
    digitalWrite(milk_Pump_Relay_Pin, LOW);
    delay(1000);
}
/* Function to mix the Coffee */
void mixing (void) {
    clearRow(2);
    lcd.setCursor(0, 2);
    lcd.print("Mixing");
    moveServoSmoothly(mixer_Holder_Servo_Channel, 10, 150, 10); 
    delay(1000);
    digitalWrite(mixer_Relay_Pin, HIGH);
    delay(7000);
    digitalWrite(mixer_Relay_Pin, LOW); 
    moveServoSmoothly(mixer_Holder_Servo_Channel, 150, 10, 10); 
    clearRow(2);
    lcd.setCursor(0, 2);
    lcd.print("Enjoy your Coffee :)");
}

/* Setup Code, will be done only once */
void setup() {
  /* Configure buttons with internal pull-ups */
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  /* Water Pump Pin Setup */
  pinMode(water_Pump_Relay_Pin, OUTPUT);
  digitalWrite(water_Pump_Relay_Pin, LOW); 
  /* Milk Pump Pin Setup */
  pinMode(milk_Pump_Relay_Pin, OUTPUT);
  digitalWrite(milk_Pump_Relay_Pin, LOW); 
  /* Mixer Pin Setup */
  pinMode(mixer_Relay_Pin, OUTPUT);
  digitalWrite(mixer_Relay_Pin, LOW); 

  /* LCD Setup */
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("AUTOMATED BARISTA");
  lcd.setCursor(0, 3);
  lcd.print("Sugar Count: 0");
  lcd.setCursor(0, 1);
  lcd.print("Enter the desired ");
  lcd.setCursor(0, 2);
  lcd.print("amount of Sugar");

  /* Initializing the servo driver */
  pwm.begin();
  pwm.setPWMFreq(50);  
  delay(500);

  /* Adjusting the robot arm to start position */ 
  int base_Current_Angle = 160;
  int base_Target_Angle = 160;  
  moveServoSmoothly(base_Servo_Channel, base_Current_Angle, base_Target_Angle, 20); 
  delay(500);  
  int shoulder_Current_Angle = 130;
  int shoulder_Target_Angle = 120;  
  moveServoSmoothly(shoulder_Servo_Channel, shoulder_Current_Angle, shoulder_Target_Angle, 20); 
  delay(500);  
  int arm_Current_Angle = 180;
  int arm_Target_Angle = 160;  
  moveServoSmoothly(arm_Servo_Channel, arm_Current_Angle, arm_Target_Angle, 20); 
  delay(1000);  
  int wrist_Current_Angle = 29;
  int wrist_Target_Angle = 28;  
  moveServoSmoothly(wrist_Servo_Channel, wrist_Current_Angle, wrist_Target_Angle, 20); 
  delay(500);  
  int gripper_Current_Angle = 160;
  int gripper_Target_Angle = 160;  
  moveServoSmoothly(gripper_Servo_Channel, gripper_Current_Angle, gripper_Target_Angle, 15); 
}

/*Loop Code, will be repeated infinitly*/
void loop() {
  readButtonState();
  coffeeScoop();

/* Program is locked to continue until Enter button is pressed*/
  if (system) {
    /* Moving to sugar */
    moveToSugar();
    /* Putting Sugar scoop */
    putSugarScoop();
    /* Moving to coffee */
    moveToCoffee();
    /* Putting Coffee scoop */
    putCoffeeScoop();
    /* Moving to pick up */
    moveToPickUp();
    /* Returning to rest */
    moveToRest();
    /* Dispensing Water */
    dispenseWater(); 
    /* Dispensing Milk */
    dispenceMilk();
    /* Mixing */
    mixing();
    while(1);
  }
}
