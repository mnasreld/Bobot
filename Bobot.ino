#include <Servo.h>
#include "NewPing.h"
#define TRIGGER_PIN A4
#define ECHO_PIN A5
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int Rmotor[] = {9,7};
int Lmotor[] = {4,8};
int Rpwm = 5, Lpwm = 3;
int left = 12, right = 13;
//int trig = A4, echo = A5;
int red = 10, green = 11;
int s1 = A3;
int deg = 95, curdeg = 95, limitdeg = 1;
int start = 0, starting = 0, calibrate = 0;
long duration, distance = 300;
long Ldistance, Rdistance;
int obstacle = 20;
Servo servo;

void setup() {
  pinMode(Rmotor[0], OUTPUT);
  pinMode(Rmotor[1], OUTPUT);
  pinMode(Lmotor[0], OUTPUT);
  pinMode(Lmotor[1], OUTPUT);
  pinMode(Rpwm, OUTPUT);
  pinMode(Lpwm, OUTPUT);
  pinMode(left, INPUT);
  pinMode(right, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
//  pinMode(trig, OUTPUT);
//  pinMode(echo, INPUT);
  
  servo.attach(s1);
  Serial.begin(9600);
}

void motorSpeed() {
  analogWrite(Rpwm, 100);
  analogWrite(Lpwm, 100);
}

void halt(){
  Serial.println("Stop!");
  digitalWrite(Rmotor[0], LOW);
  digitalWrite(Rmotor[1], LOW);

  digitalWrite(Lmotor[0], LOW);
  digitalWrite(Lmotor[1], LOW);
}

void forward(){
  digitalWrite(Rmotor[0], HIGH);
  digitalWrite(Rmotor[1], LOW);

  digitalWrite(Lmotor[0], HIGH);
  digitalWrite(Lmotor[1], LOW);
  
  motorSpeed();
}

void reverse(){
  Serial.println("reverse");
  digitalWrite(Rmotor[0], LOW);
  digitalWrite(Rmotor[1], HIGH);

  digitalWrite(Lmotor[0], LOW);
  digitalWrite(Lmotor[1], HIGH);

  motorSpeed();
}

void turnLeft(){
  Serial.println("Turning Left");
  
  digitalWrite(Rmotor[0], LOW);
  digitalWrite(Rmotor[1], HIGH);

  digitalWrite(Lmotor[0], HIGH);
  digitalWrite(Lmotor[1], LOW);

  motorSpeed();
}

void turnRight(){
  Serial.println("Turning Right");
  digitalWrite(Rmotor[0], HIGH);
  digitalWrite(Rmotor[1], LOW);

  digitalWrite(Lmotor[0], LOW);
  digitalWrite(Lmotor[1], HIGH);

  motorSpeed();
}

void lookLeft(int degree){
  for(int x = degree; x <= 180; x++){
    x++;
    servo.write(x);

  }
  delay(1000);
  Ldistance = getDistance(5);
  delay(2000);
}

void lookRight(int degree){
  for(int y = degree; y > 20; y--){
    y--;
    servo.write(y);
  }
  delay(1000);
  Rdistance = getDistance(5);
  delay(2000);
}

int getDistance(int iteration){
  int condition = true;
  while(condition){
    duration = sonar.ping_median(iteration);
    distance = (duration / 2) * 0.0343;
    if(distance <= 2 || distance >= 400){  
     condition = true; 
    } else{
      condition = false;
    }
  }
  
  return distance;
}

void signal(int color){
  if(color == 1){
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);    
  }else{
    digitalWrite(green, LOW);
    digitalWrite(red, HIGH);
  }
}

void scan(int degree){  
  servo.write(degree);
  
  distance = getDistance(3);
  while(distance <= 2 || distance >= 400){
    distance = getDistance(3);
  }
}

void loop() {
  //------------------------------------------------------------------------------------------------
  if(digitalRead(left) == LOW || digitalRead(right) == LOW){
    start = 1;
    signal(1);
  }  
  
  if(start == 0){
    Serial.println("Waiting!");
    signal(0); //0 is red, 1 is green
  }
  else{
    if(starting == 0){
      delay(5000);
      starting = 1;  
    }
    
    signal(1);
    distance = 300;
    
    if(calibrate == 1){
      Serial.println("Calibrating!");
      delay(1000);
    }
    calibrate = 0;

    scan(curdeg);
    if(limitdeg == 1){
      curdeg += 2;
    }else{
      curdeg -= 2;
    }

    if(curdeg <= 50){
      limitdeg = 1;
    }
    if(curdeg >= 120){
      limitdeg = 0;
    }

    
    Serial.print("Distance: ");
    Serial.println(distance);
  
    //-------------------------obstacle------------------------------------
    if(distance < obstacle){
      signal(0);
      calibrate = 1;
      Serial.print("Obstacle detected!");
      reverse();
      delay(300);
      halt();
      
      lookLeft(deg);
      
      Serial.print("Left distance: ");
      Serial.println(Ldistance);
      delay(1000);
  
      lookRight(deg);
  
      Serial.print("Right distance: ");
      Serial.println(Rdistance);
      delay(1000);
  
      if(Rdistance <= obstacle && Ldistance <= obstacle){
          Serial.println("Turning Back!");
          turnLeft();
          delay(500);
          halt();
          servo.write(95); //upfront
          delay(1000);
      } else {
          if(Rdistance > Ldistance){
              Serial.println("Turning Right");
              turnRight();
              delay(250);
              halt();
              servo.write(95); //upfront
              delay(1000);
          } else{
              Serial.println("Turning Left");
              turnLeft();
              delay(250);
              halt();
              servo.write(95); //upfront
              delay(1000);
          }
      }
      delay(3000);
    }
    else{
      signal(1);
      Serial.println("forward");
      forward();
    }
  }

  
  

//-----------------------the code below is for testing------------------------------------------------
//  if(digitalRead(left) == LOW){
//    servo.write(180);
//    Serial.println("Looking Left");
//    lookLeft(deg);
//    Serial.print("Distance: ");
//    Serial.println(distance);
//    turnLeft();
//  }
//  
//  if(digitalRead(right) == LOW){
//    servo.write(0);
//    Serial.println("Looking Right");
//    lookRight(deg);
//    Serial.print("Distance: ");
//    Serial.println(distance);
//    turnRight();
//  }
//  
//  if(digitalRead(left) == HIGH && digitalRead(right) == HIGH){
//    servo.write(95);
//    halt();
//  }
//
//  if(digitalRead(left) == LOW && digitalRead(right) == LOW){
//    servo.write(150);
//    forward();
//  }
//  

}
