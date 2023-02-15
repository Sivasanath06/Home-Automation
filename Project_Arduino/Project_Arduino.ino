
#include<SoftwareSerial.h>
SoftwareSerial mySerial(11,12);

int led2=2;
int temp;
int tempPin = 0;
int pir=13;
int val = 0;
int state=LOW;

int k=0;
int buzzer=9;
int smokeA0 = A3;
int sensorThres = 180;


void setup() {
  Serial.begin(9600);
//pinMode(4,INPUT);
mySerial.begin(9600);
pinMode(led2,OUTPUT);

}

void loop() {
   
  
   temp = analogRead(tempPin);
     temp = temp * 0.4;
     Serial.print("TEMPERATURE = ");
   Serial.println(temp); 
  
  val = digitalRead(pir);  
  if (val == HIGH) {           
    digitalWrite(led2, LOW);   
                  
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      
      state = HIGH;       
    }
  } 
  else {
      digitalWrite(led2, HIGH); 
                  
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       
    }
  }

  int analogSensor = analogRead(smokeA0);
Serial.print("Pin A0: ");
  Serial.println(analogSensor);
//   Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
    
    tone(buzzer, 1000, 200);
    Serial.print("Smoke is detected");
  }
  else
  {
    noTone(buzzer);
  }

  char buf[20];
  sprintf(buf,"the %d and %d",temp,analogSensor);
  mySerial.print(buf);
    delay(1000);
}
