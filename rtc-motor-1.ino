#include <DallasTemperature.h>
#include <OneWire.h>
#include <virtuabotixRTC.h>
#include <LiquidCrystal.h>
#define ONE_WIRE_BUS 30

// Data wire is plugged into pin 4 on the Arduino 
// pin 52 led
// pin 39 button
// pin 26 buzzer
//Or CLK -> 6 , DAT -> 7, Reset -> 8

virtuabotixRTC myRTC(6, 7, 8); //If you change the wiring change the pins here also

int Pin0 = 12;//definition digital 12 pins as pin to control the IN1 (ULN24L01)
int Pin1 = 11;//definition digital 11 pins as pin to control the IN2 (ULN24L01)
int Pin2 = 10;//definition digital 10 pins as pin to control the IN3 (ULN24L01)
int Pin3 = 9;//definition digital 9 pins as pin to control the IN4 (ULN24L01)
int _step = 512; 
int _speed = 1; 
const int button1Pin = 39;
int led=52;
int buzzerPin=26; //definition digital 8 pins as pin to control the buzzer

const int rs = 24, en = 22, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

void setup() {
 Serial.begin(9600);
 sensors.begin(); 

 // set up the LCD's number of columns and rows:
 lcd.begin(16, 2);
 pinMode(Pin0, OUTPUT);//Set digital 8 port mode, the OUTPUT for the output
 pinMode(Pin1, OUTPUT);//Set digital 9 port mode, the OUTPUT for the output
 pinMode(Pin2, OUTPUT);//Set digital 10 port mode, the OUTPUT for the output
 pinMode(Pin3, OUTPUT);//Set digital 11 port mode, the OUTPUT for the output
 pinMode(button1Pin, INPUT);
 pinMode(5,OUTPUT);
 pinMode(buzzerPin,OUTPUT);
 pinMode(led,OUTPUT);
 
 // Set the current date, and time in the following format:
 // seconds, minutes, hours, day of the week, day of the month, month, year
 myRTC.setDS1302Time(50, 29, 20, 6, 27, 4, 2019); //Here you write your actual time/date as shown above 
 //but remember to "comment/remove" this function once you're done
 //The setup is done only one time and the module will continue counting it automatically

 float temp;
 // start serial port
 sensors.begin(); 
}

void loop() {
 // This allows for the update of variables for time or accessing the individual elements.
 myRTC.updateTime();
 
// Start printing elements as individuals
 Serial.print("Current Date / Time: ");
 Serial.print(myRTC.dayofmonth); //You can switch between day and month if you're using American system
 Serial.print("/");
 Serial.print(myRTC.month);
 Serial.print("/");
 Serial.print(myRTC.year);
 Serial.print(" ");
 Serial.print(myRTC.hours);
 Serial.print(":");
 Serial.print(myRTC.minutes);
 Serial.print(":");
 Serial.println(myRTC.seconds);
 
 if((myRTC.hours==9 && myRTC.minutes==00 && myRTC.seconds==00)||(myRTC.hours==21 && myRTC.minutes==00 && myRTC.seconds==00))
 {
  for(int i=0;i<3;i++)
    {
      Speed(15);//Stepper motor speed = 15 fast (note:speed from 1 to 15)
      Step(512);//Stepper motor forward 512 steps ---- 360 angle    
    }
  }
// Delay so the program doesn't print non-stop
  delay(1000);

  int button1State; 
  button1State = digitalRead(button1Pin);
  if (button1State == LOW)
  {
    for(int i=0;i<3;i++)
      {
        Speed(15);//Stepper motor speed = 15 fast (note:speed from 1 to 15)
        Step(512);//Stepper motor forward 512 steps ---- 360 angle    
      }
  }

 // call sensors.requestTemperatures() to issue a global temperature 
 // request to all devices on the bus 

 sensors.requestTemperatures(); // Send the command to get temperature readings 
 Serial.print("Temperature is: "); 
 float temp=sensors.getTempCByIndex(0);
 Serial.print(sensors.getTempCByIndex(0));// Why "byIndex"?  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire
   //float temp= sensors.getTempCByIndex(0);
   //lcd.print(temp);
  
  int m=myRTC.minutes;
  int s=myRTC.seconds;
  
 if(((m==30)&&(s<30))||((m==0)&&(s<30)))
  {
    if(temp<24)
    {
      digitalWrite(led,HIGH);
      digitalWrite(buzzerPin,LOW);
      lcd.print(temp);
      lcd.setCursor(0, 1);
      lcd.print("LOW TEMP!       ");    
    }
    else if(temp>25)
    {
      digitalWrite(buzzerPin,HIGH);
      digitalWrite(led,LOW);
      lcd.print(temp);
      lcd.setCursor(0, 1);
      lcd.print("HIGH TEMP!      ");    
    }
   else
   {
      digitalWrite(buzzerPin,LOW);
      digitalWrite(led,LOW);
      lcd.print(temp);
      lcd.setCursor(0, 1);
      lcd.print("TEMP NORMAL     ");    
    }
  }
  else
    {
      if(temp<25)
      {
        lcd.print(temp);
        lcd.setCursor(0, 1);
        lcd.print("LOW TEMP!       ");
        digitalWrite(buzzerPin,LOW);
        digitalWrite(led,HIGH);    
      }
      else if(temp>30)
      {
        lcd.print(temp);
        lcd.setCursor(0, 1);
        lcd.print("HIGH TEMP!      ");
        digitalWrite(buzzerPin,LOW);
        digitalWrite(led,LOW);
      }
    else
      {
        lcd.print(temp);
        lcd.setCursor(0, 1);
        lcd.print("NORMAL TEMP     ");
        digitalWrite(buzzerPin,LOW);
        digitalWrite(led,LOW);
      }
    }
}

void Speed(int stepperspeed)//set Stepper speed 
{
    _speed = 15 - stepperspeed;
    if( _speed<1)
    {
     _speed = 1;
    }
    if( _speed>15)
    {
     _speed = 15;
    }
}
void Step(int _step)//Stepper motor rotation
{
  if(_step>=0)
  {// Stepper motor forward
    for(int i=0;i<_step;i++)
    {   
      setStep(1, 0, 0, 1);
      delay(_speed); 
      setStep(1, 0, 0, 0);
      delay(_speed);
      setStep(1, 1, 0, 0);
      delay(_speed);
      setStep(0, 1, 0, 0);
      delay(_speed);
      setStep(0, 1, 1, 0);
      delay(_speed);
      setStep(0, 0, 1, 0);
      delay(_speed);
      setStep(0, 0, 1, 1);
      delay(_speed); 
      setStep(0, 0, 0, 1);
      delay(_speed);
    }
  }
  else
  {// Stepper motor backward
     for(int i=_step;i<0;i++)
     {  
      setStep(0, 0, 0, 1);
      delay(_speed);
      setStep(0, 0, 1, 1);
      delay(_speed);
      setStep(0, 0, 1, 0);
      delay(_speed);
      setStep(0, 1, 1, 0);
      delay(_speed);
      setStep(0, 1, 0, 0);
      delay(_speed);
      setStep(1, 1, 0, 0);
      delay(_speed);
      setStep(1, 0, 0, 0);
      delay(_speed);
      setStep(1, 0, 0, 1);
      delay(_speed);
    }
  }
}
void setStep(int a, int b, int c, int d)  
{  
    digitalWrite(Pin0, a);     
    digitalWrite(Pin1, b);     
    digitalWrite(Pin2, c);     
    digitalWrite(Pin3, d);     
}
