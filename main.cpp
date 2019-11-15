#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include "MAX30100.h"
#include <LiquidCrystal_I2C.h>
#include <TIMER1_SERVO.h>
#define REPORTING_PERIOD_MS 1000
#define pulse 5
LiquidCrystal_I2C lcd(0x27, 20, 4);
MAX30100* pulseOxymeter;
volatile boolean tick_goc = false;
void massage();
void tick_sensor();
void tick_servo();
const int xung = 10;
int SpO2 = 0 ;
int mode = 1 ;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  // LCD
  LiquidCrystal_I2C lcd(0x27, 20, 4);        
  // LCD
  Serial.println("Pulse oxymeter test!");
  //pulseOxymeter = new MAX30100( DEFAULT_OPERATING_MODE, DEFAULT_SAMPLING_RATE, DEFAULT_LED_PULSE_WIDTH, DEFAULT_IR_LED_CURRENT, true, true );
  //pulseOxymeter->printRegisters();
  pulseOxymeter = new MAX30100(DEFAULT_OPERATING_MODE,DEFAULT_SAMPLING_RATE,DEFAULT_LED_PULSE_WIDTH,MAX30100_LED_CURRENT_27_1MA);
  port_attach(PB1);//PIN 9
  pinMode(xung,OUTPUT);
  //Interrupt
  pinMode(2,INPUT_PULLUP);
  attachInterrupt(0,tick_servo,RISING);
  //Interrupt
  //Initialize Servo 
  write(PB1, 0);
  //Initialize Servo
}

void loop() {
  //return;
  //You have to call update with frequency at least 37Hz. But the closer you call it to 100Hz the better, the filter will work.
  switch (mode) {
  case 1:
    tick_sensor();
    break;
  case 2:
    massage();
    break;
  default:
    tick_sensor();
    break;
  }  
}

void tick_servo(){
  if (tick_goc){
    tick_goc = false;
    write(PB1, 180);
    mode = 2;
  }
  else{
    tick_goc = true;
    write(PB1, 0);
    mode = 1;
  } 
}

void massage(){
    lcd.init(); 
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Massage...");
    
    digitalWrite(xung,1);
    delay(2);
    digitalWrite(xung,0);
    delay(5);
}

void tick_sensor(){
  pulseoxymeter_t result = pulseOxymeter->update();
  if( result.pulseDetected == true )
  {
    SpO2 = result.SaO2; 
    Serial.print( "SpO2: " );
    Serial.print( SpO2 );
    Serial.println( "%" );
    
    lcd.init(); 
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Detecting...");

    lcd.setCursor(0, 1);
    lcd.print("SpO2: ");
    lcd.print( SpO2 );
    lcd.println(" % ");
    
    delay(2000);
  }
}

