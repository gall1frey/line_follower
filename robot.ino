#include <PID_v1.h>
#include <NewPing.h>
#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define MOTOR_R_DIR 2
#define MOTOR_R_PWM 3
#define MOTOR_L_DIR 4
#define MOTOR_L_PWM 5

#define TRIG 12
#define ECHO 11
#define MAX_DIST 200
#define THRESH 5

#define kP 2
#define kI 5
#define kD 1

NewPing sonar(TRIG,ECHO,MAX_DIST);
unsigned int pingSpeed = 50;
unsigned long pingTimer;
int dist = 0;

double val = 0;
int mapval = 0;
char line_col = 'B';
String in_string;

int dutyCycle = 60;
double diff = 0;
double Setpoint = 0;
PID myPID(&val, &diff, &Setpoint,kP,kI,kD, DIRECT);

int SPEED = 60;  //0 to 100, mapped to 0 to 255

char mode = 'L';

Adafruit_MCP23017 mcp;

void echoCheck() {
  if(sonar.check_timer()){
    dist = sonar.ping_result / US_ROUNDTRIP_CM;
  }
}

void straight(){
  digitalWrite(MOTOR_L_PWM,LOW);
  digitalWrite(MOTOR_R_PWM,LOW);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_R_DIR,LOW);
  delay(10);
  digitalWrite(MOTOR_L_DIR,HIGH);
  digitalWrite(MOTOR_R_DIR,HIGH);
  if (mode == 'B'){
    //Set duty cycle to speed, then go straight
    analogWrite(MOTOR_L_PWM,255-map(SPEED,0,100,0,255));
    analogWrite(MOTOR_R_PWM,255-map(SPEED,0,100,0,255));
  } else {
    //analog write the updated duty cycles
    analogWrite(MOTOR_L_PWM,255-map(max(min((dutyCycle+diff),100),0),0,100,0,255));
    analogWrite(MOTOR_R_PWM,255-map(max(min((dutyCycle-diff),100),0),0,100,0,255));
  }
}

void right(){
  //code to go right
  digitalWrite(MOTOR_L_PWM,LOW);
  digitalWrite(MOTOR_R_PWM,LOW);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_R_DIR,LOW);
  delay(10);
  digitalWrite(MOTOR_L_DIR,HIGH);
  analogWrite(MOTOR_L_PWM,255-map(SPEED,0,100,0,255));
}

void left(){
  //code to go left
  digitalWrite(MOTOR_L_PWM,LOW);
  digitalWrite(MOTOR_R_PWM,LOW);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_R_DIR,LOW);
  delay(10);
  digitalWrite(MOTOR_R_DIR,HIGH);
  analogWrite(MOTOR_R_PWM,255-map(SPEED,0,100,0,255));
}

void fright(){
  //code to go front and right
  digitalWrite(MOTOR_L_PWM,LOW);
  digitalWrite(MOTOR_R_PWM,LOW);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_R_DIR,LOW);
  delay(10);
  digitalWrite(MOTOR_L_DIR,HIGH);
  digitalWrite(MOTOR_R_DIR,HIGH);
  analogWrite(MOTOR_L_PWM,255-map(SPEED,0,100,0,255));
  analogWrite(MOTOR_R_PWM,255-map((SPEED/3),0,100,0,255));
}

void fleft(){
  //code to go front and left
  digitalWrite(MOTOR_L_PWM,LOW);
  digitalWrite(MOTOR_R_PWM,LOW);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_R_DIR,LOW);
  delay(10);
  digitalWrite(MOTOR_L_DIR,HIGH);
  digitalWrite(MOTOR_R_DIR,HIGH);
  analogWrite(MOTOR_R_PWM,255-map(SPEED,0,100,0,255));
  analogWrite(MOTOR_L_PWM,255-map((SPEED/3),0,100,0,255));
}

void bright(){
  //code to go back and right
  digitalWrite(MOTOR_L_PWM,LOW);
  digitalWrite(MOTOR_R_PWM,LOW);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_R_DIR,LOW);
  delay(10);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_R_DIR,LOW);
  analogWrite(MOTOR_L_PWM,map(SPEED,0,100,0,255));
  analogWrite(MOTOR_R_PWM,map((SPEED-(SPEED/3)),0,100,0,255));
}

void bleft(){
  //code to go back and left
  digitalWrite(MOTOR_L_PWM,LOW);
  digitalWrite(MOTOR_R_PWM,LOW);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_R_DIR,LOW);
  delay(10);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_R_DIR,LOW);
  analogWrite(MOTOR_L_PWM,map((SPEED-(SPEED/3)),0,100,0,255));
  analogWrite(MOTOR_R_PWM,map(SPEED,0,100,0,255));
}

void back(){
  //code to go back
  digitalWrite(MOTOR_L_PWM,LOW);
  digitalWrite(MOTOR_R_PWM,LOW);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_R_DIR,LOW);
  delay(10);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_R_DIR,LOW);
  analogWrite(MOTOR_L_PWM,map(SPEED,0,100,0,255));
  analogWrite(MOTOR_R_PWM,map(SPEED,0,100,0,255));
}

void Stop(){
  //Code to stop
  digitalWrite(MOTOR_L_PWM,LOW);
  digitalWrite(MOTOR_R_PWM,LOW);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_R_DIR,LOW);
}

int readOne(int pin){
  if(line_col == 'B')
    return mcp.digitalRead(pin);
   return !mcp.digitalRead(pin);
}

int readAll(){
  int val = 0;
  for(int pin=0; pin < 8; pin++){
    if(line_col == 'B')
      val += mcp.digitalRead(pin) << pin;
    else
      val += !(mcp.digitalRead(pin)) << pin;
  }
  return val;
}

void mcp_init(){
  for(int pin = 0; pin < 8; pin++)
    mcp.pinMode(pin, INPUT);
}

int mapper(int val){
  int nval = 0;
  for(int i = 0; i<8; i++){
    nval -= ((val) & (1 << (i+4)))>>4;
  }
  for(int i = 3; i>=0; i--){
    if(val & (1 << i)){
      nval += (1 << (3-i));
    }
  }
  return nval;
}

void setup() {  
  mcp.begin();
  mcp_init();
  Serial.begin(9600);
  Serial.setTimeout(200);
  pingTimer = millis();
  pinMode(MOTOR_R_DIR,OUTPUT);
  pinMode(MOTOR_R_PWM,OUTPUT);
  pinMode(MOTOR_L_DIR,OUTPUT);
  pinMode(MOTOR_L_PWM,OUTPUT);
  digitalWrite(MOTOR_R_DIR,LOW);
  digitalWrite(MOTOR_R_PWM,LOW);
  digitalWrite(MOTOR_L_DIR,LOW);
  digitalWrite(MOTOR_L_PWM,LOW);
  Serial.println("Hello there!");
  Serial.println("Simple commands:");
  Serial.println("MODE=LINE => Line follower");
  Serial.println("MODE=BT => Bluetooth Control");
  delay(3000);
  myPID.SetMode(AUTOMATIC);
}

void loop() {
  if(millis() >= pingTimer){
    pingTimer += pingSpeed;
    sonar.ping_timer(echoCheck);
  }
  if(dist < THRESH){
    Stop();
  } else {
    if (Serial.available() > 0) {
      in_string = Serial.readString();
    }
    //Setting Mode
    if((in_string == "MODE>LINE\n" || in_string == "MODE>LINE") && mode != 'L'){
      mode = 'L';
      Serial.println("MODE CHANGED TO LINE FOLLOWER.");
    }
    else if((in_string == "MODE>BT\n" || in_string == "MODE>BT") && mode != 'B'){
      mode = 'B';
      Serial.println("MODE CHANGED TO BLUETOOTH.");
    }
    else if((in_string == "LINE>W\n" || in_string == "LINE>W") && mode == 'L'){
      line_col = 'W';
      Serial.println("LINE COLOR SET TO WHITE.");
    }
    else if((in_string == "LINE>B\n" || in_string == "LINE>B") && mode == 'L'){
      line_col = 'B';
      Serial.println("LINE COLOR SET TO BLACK.");
    }
    //If bluetooth control
    if(mode == 'B' && (in_string == "F" || in_string == "F\n")) {
      straight();
    } else if(mode == 'B' && (in_string == "R" || in_string == "R\n")) {
      right();
    } else if(mode == 'B' && (in_string == "L" || in_string == "L\n")) {
      left();
    } else if(mode == 'B' && (in_string == "B" || in_string == "B\n")) {
      back();
    } else if(mode == 'B' && (in_string == "FR" || in_string == "FR\n")) {
      fright();
    } else if(mode == 'B' && (in_string == "FL" || in_string == "FL\n")) {
      fleft();
    } else if(mode == 'B' && (in_string == "BR" || in_string == "BR\n")) {
      bright();
    } else if(mode == 'B' && (in_string == "BL" || in_string == "BL\n")) {
      bleft();
    } else if(mode == 'B' && (in_string == "S" || in_string == "S\n")) {
      Stop();
    } else if(mode == 'B' && (in_string == "UP" || in_string == "UP\n")) {
      SPEED += 10;
      in_string = "";
    } else if(mode == 'B' && (in_string == "DOWN" || in_string == "DOWN\n")) {
      SPEED -= 10;
      in_string = "";
    }
    //Line follower mode
    else if(mode == 'L'){
      val = readAll();
      myPID.Compute();
      straight();
      /*if(val == 255)
        right();
      mapval = mapper(val);
      if(mapval < 0)
        fleft();
      else if(mapval > 0)
        fright();
      else{
        straight();
        delay(50);
      }*/
    }
    if(millis() % 100 == 0){
      Serial.print("STATUS: MODE: ");
      Serial.println(mode);
      if(mode == 'B'){
        Serial.print(" SPEED: ");
        Serial.print(SPEED);
        Serial.print(" SONAR:");
        Serial.print(dist);
        Serial.println("cm");
      } else if(mode == 'L'){
        Serial.print(" dutyCycleL: ");
        Serial.print(max(min((dutyCycle+diff),100),0));
        Serial.print(" dutyCycleR: ");
        Serial.print(max(min((dutyCycle-diff),100),0));
        Serial.print(" LINE FOLLOWER: ");
        Serial.print(val);
        Serial.print(" DIFF: ");
        Serial.print(diff);
        Serial.print(" SONAR:");
        Serial.print(dist);
        Serial.println("cm");
      }
    }
  }
  delay(30);
}
/*val = readAll();
  mapval = mapper(val);
  //val = (val*(i-1) + readAll()) / i;
  Serial.print(mapval);
  Serial.print("\t");
  if(mapval < 0){
    Serial.println("GO LEFT!");
  } else if(mapval > 0){
    Serial.println("GO RIGHT!");
  } else {
    Serial.println("KEEP STRAIGHT!");
  }*/
