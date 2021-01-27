#include <LiquidCrystal.h>
const int led = 10;
const int button = 8;
const int tempsens = A0;
String readString;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float temp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  pinMode(tempsens, INPUT);
  lcd.begin(16, 2);
}

void loop() {
  
  while (Serial.available()) 
  {
    char data_in = Serial.read();  //gets one byte from serial buffer
    readString += data_in; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }
  
  if (readString.length() >0) {
    int button_in = 0;
    int count = 1;
    int n = readString.toInt();
    int i = 0;
    int j = 1;
    int m, tempm;
    char tempo[1];
    
    if(n == 1)
    {
      digitalWrite(led, HIGH);
    }
    
    if(n == 2)
    {
      digitalWrite(led, LOW);
    }

    if(n == 3)
    {
      for(i=0;i<3;i++)
      {
          digitalWrite(led, HIGH);
          delay(500);
          digitalWrite(led, LOW);
          delay(500);
      }
    }

    if(n == 4)
    {
      m = 0;
      while(m == 0)
        m = Serial.parseInt();
      delay(200);
      Serial.println(m*m, DEC);
      delay(20);
    }

    if(n==5)
    {
      delay(200);
      while(j<11)
      {
        button_in = digitalRead(button);
        if(button_in == HIGH)
        {
          Serial.println(j);
          delay(400);
          j = j + 1;
        }
      }
    }

    if(n==6)
    {
      lcd.setCursor(0, 0);
      lcd.print("Temperature:");
      do
      {
        lcd.setCursor(0, 1);
        tempo[0] = Serial.read();
        temp = analogRead(tempsens);
        temp=(temp*500)/1023;
        lcd.print(temp);
        delay(500);
      }while(tempo[0] != 'a');
      lcd.clear();
    }
    
    delay(1000);
    readString="";
  }
  
}
