  #include <FirebaseESP8266.h>
#include  <ESP8266WiFi.h>
#include <Wire.h>      
#include <RtcDS3231.h> 
 #include<SoftwareSerial.h>
#define FIREBASE_HOST "https://first-app-41fa0-default-rtdb.firebaseio.com/"
#define WIFI_SSID "IIITS_Student" // Change the name of your WIFI
#define WIFI_PASSWORD "iiit5@2k18" // Change the password of your WIFI
#define FIREBASE_Authorization_key "ahGw879heMMpTVqeB3EQUPAB5OWlD8vT5lXRuSDz"
SoftwareSerial myserial (D3,D4);
RtcDS3231<TwoWire> rtcObject(Wire);
FirebaseData firebaseData;
FirebaseJson json;
int Device_1 = D5;
int Device_2 = D6;
int a=D0;
int a1=D7;
int a2=D8;
#define PIN A0
#define pi 0.89
float CostPerUnit=10;
float bill=0;
int motor=3;
float resolution  = 3.3 / 1024;                     // Input Voltage Range is 1V to 3.3V
       int y=0;   
     // ESP8266 ADC resolution is 10-bit. 2^10 = 1024
uint32_t period = 1000000 / 60;                     // One period of a 60Hz periodic waveform 
uint32_t t_start = 0;

// setup
float zero_ADC_Value = 0;   

// loop
float aDC = 0, Vrms = 0, Current = 0, Q = 0.000,ptotal=0,kw=0,energy=0;
 
float sensitivity = 0.185;      
void setup() {
Serial.begin(115200);
 myserial.begin(9600);
 pinMode(PIN, INPUT);   
  WiFi.begin(WIFI_SSID , WIFI_PASSWORD);//   WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
   Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    
  }
  wifi_set_sleep_type(NONE_SLEEP_T);
  pinMode(Device_1, OUTPUT);
pinMode(Device_2, OUTPUT);
pinMode(motor, OUTPUT);
pinMode(a, INPUT);
pinMode(a1, INPUT);
pinMode(a2, INPUT);
 rtcObject.Begin();     //Starts I2C
  
  RtcDateTime currentTime = RtcDateTime(16, 05, 18, 21, 20, 0); //define date and time object
  rtcObject.SetDateTime(currentTime);
 t_start = micros();
  uint32_t ADC_SUM = 0, n = 0;
  while(micros() - t_start < period) {
    ADC_SUM += analogRead(PIN);
    n++;
  }
  zero_ADC_Value = ADC_SUM / n;
  Firebase.begin(FIREBASE_HOST,FIREBASE_Authorization_key);
}
void loop() {
  int o1,o2,f1,f2;
  if(digitalRead(a)==1){
      
      digitalWrite(Device_2,LOW);
    }
    if(digitalRead(a1)==1 ){
     
      digitalWrite(Device_1,LOW);
    }
    if(digitalRead(a2)==1 ){
      
      digitalWrite(motor,HIGH);
    }
   RtcDateTime currentTime = rtcObject.GetDateTime();    //get the time from the RTC
  
  char str[20];   //declare a string as an array of chars
  
  
  sprintf(str, "%d/%d/%d %d:%d:%d",     //%d allows to print an integer to the string
          currentTime.Year(),   //get year method
          currentTime.Month(),  //get month method
          currentTime.Day(),    //get day method
          currentTime.Hour(),   //get hour method
          currentTime.Minute(), //get minute method
          currentTime.Second()  //get second method
         );
  
  Serial.println(str);

  String msg=myserial.readStringUntil('\r');
 int k,l;
 t_start = micros();                             
  uint32_t ADC_Dif = 0, ADC_SUM = 0, m = 0;        
  while(micros() - t_start < period) {            // Defining one period of the waveform. US frequency(f) is 60Hz. Period = 1/f = 0.016 seg = 16,666 microsec
    ADC_Dif = zero_ADC_Value - analogRead(PIN);   // To start from 0V we need to subtracting our initial value when no current passes through the current sensor, (i.e. 750 or 2.5V).
    ADC_SUM += ADC_Dif * ADC_Dif;                 // SUM of the square
    m++;                                          // counter to be used for avg.
  }
  aDC = sqrt(ADC_SUM / m);                        // The root-mean-square ADC value. 
  Vrms = aDC * resolution ;                       // The root-mean-square analog voltage value.   
  Current = (Vrms  / sensitivity) - Q;        // The root-mean-square analog current value. Note: Q
  //------------------------------//
  
  Serial.print("analogRead = ");
  Serial.println(analogRead(PIN));
  
  Serial.print("Vrms = ");                        
  Serial.print(Vrms, 6);
  Serial.println(" V");        

  Serial.print("Irms = ");                       
  Serial.print(Current, 6);
  Serial.println(" A");
  kw=Current*Vrms*pi;
  ptotal=ptotal+kw;
  float p=ptotal*1000;
  Serial.print("TotalPower = ");
  Serial.print(p,6);
  
  Serial.println("mW");
  energy=(ptotal*millis())/((1000)*3600);
  Serial.print("Energy = ");
  Serial.print(energy,6);
  Serial.println("KWH");

  bill=energy*CostPerUnit;
  Serial.print(bill,6);
  Serial.print("\n");
 
  
  
  
 String s1,s2;
 sscanf(msg.c_str(),"%s %d %s %d",s1,&k,s2,&l);
 
 
  
    if (Firebase.get(firebaseData,"b1") && y==0) {
    if (firebaseData.dataType() == "string") {
      String De1 = firebaseData.stringData();
      Serial.print(De1);
          if (De1=="1"){
              digitalWrite(Device_1,LOW); //Device1 is ON
          }
          else if (De1=="0"){
              digitalWrite(Device_1,HIGH);//Device1 if OFF
            }}}
             if (Firebase.get(firebaseData,"b2")) {
    if (firebaseData.dataType() == "string") {
      String De2 = firebaseData.stringData();
      Serial.print(De2);
          if (De2=="1"){
              digitalWrite(Device_2,LOW); //Device1 is ON
          }
          else if (De2=="0"){
              digitalWrite(Device_2,HIGH);//Device1 if OFF
            }}}
            if (Firebase.get(firebaseData,"b3")) {
    if (firebaseData.dataType() == "string") {
      String De3 = firebaseData.stringData();
      Serial.print(De3);
          if (De3=="1"){
//              digitalWrite(motor,HIGH);
              if(k>35){
                 analogWrite(motor,255);}
                 else{
                  analogWrite(motor,190);
                  }
                   
   }
   
             //Device1 is ON
          
          else if (De3=="0"){
              digitalWrite(motor,LOW);//Device1 if OFF
            }}}
           if (Firebase.get(firebaseData,"On")) {
    if (firebaseData.dataType() == "string") {
       String o3 = firebaseData.stringData();
       o1=o3.toInt();
      Serial.println(o1);
        }}
        if (Firebase.get(firebaseData,"Ons")) {
    if (firebaseData.dataType() == "string") {
       String o4 = firebaseData.stringData();
       o2=o4.toInt();
      Serial.println(o2);
        }}
        if (Firebase.get(firebaseData,"Of")) {
    if (firebaseData.dataType() == "string") {
       String f3 = firebaseData.stringData();
       f1=f3.toInt();
      Serial.println(f1);
        }}
        if (Firebase.get(firebaseData,"Ofs")) {
    if (firebaseData.dataType() == "string") {
      String f4 = firebaseData.stringData();
      f2=f4.toInt();
      Serial.println(f2);
        }}
         if(currentTime.Hour()==o1 &&  currentTime.Minute()==o2){
          digitalWrite(Device_1,LOW);
          y=1;
    Serial.println("Bulb glows");
  }
   if(currentTime.Hour()==f1 &&  currentTime.Minute()==f2){
     digitalWrite(Device_1,HIGH);
     y=0;
    Serial.println("Bulb stops");
  }

        
  if(l<180 && p<2000){
      Firebase.setFloat(firebaseData, "TEMPERATURE", k);
       Firebase.setFloat(firebaseData, "Smoke", l);
          Firebase.setFloat(firebaseData, "Power",p);
     Firebase.setFloat(firebaseData, "Energy",energy);
      Firebase.setFloat(firebaseData, "Bill",bill);
       Firebase.setFloat(firebaseData, "Hour", currentTime.Hour());
       Firebase.setFloat(firebaseData, "Minute",currentTime.Minute());
       Firebase.setFloat(firebaseData, "Second", currentTime.Second());
        String yes="smoke_do_not_detected";
        String p1="Power_consumption_is_well_maintained";
      Firebase.setString(firebaseData, "smoke",yes);
      Firebase.setString(firebaseData, "not",p1);
}else{
        
    Firebase.setFloat(firebaseData, "TEMPERATURE", k);

     Firebase.setFloat(firebaseData, "Power",p);
     Firebase.setFloat(firebaseData, "Energy",energy);
      Firebase.setFloat(firebaseData, "Bill",bill);
       Firebase.setFloat(firebaseData, "Hour", currentTime.Hour());
       Firebase.setFloat(firebaseData, "Minute",currentTime.Minute());
       Firebase.setFloat(firebaseData, "Second", currentTime.Second());
        String no="smoke_detected";
        String p2="Power_consumption_is_not_well_maintained";
      Firebase.setString(firebaseData, "smoke",no);
       Firebase.setString(firebaseData, "not",p2);
    }
   delay(1000); 
}
