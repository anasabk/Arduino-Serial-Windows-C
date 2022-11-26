#include <LiquidCrystal.h>
const int led = 9;
const int button = 8;
const int tempsens = A0;
byte data_in;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  pinMode(tempsens, INPUT);
  lcd.begin(16, 2);
}

void loop() {
  while ( Serial.available() == 0 );
  data_in = Serial.read();  //gets one byte from serial buffer

  if (data_in >= 49 && data_in <= 54) {
    data_in -= 48;
    
    if(data_in == 1)
      digitalWrite(led, HIGH);
    
    else if(data_in == 2)
      digitalWrite(led, LOW);
  
    else if(data_in == 3) {
      for(int i=0; i<3; i++) {
          digitalWrite(led, HIGH);
          delay(500);
          digitalWrite(led, LOW);
          delay(500);
      }
    }
  
    else if(data_in == 4) {
      while( Serial.available() == 0 );
      
      int m = 0;
      while(Serial.available()) {
        delay(5);
        m = m*10 + Serial.read() - 48;
      }

      char result[8];
      sprintf(result, "%d", m*m);
      Serial.println(result);
    }
  
    else if(data_in == 5) {
      bool button_in = 0;
      int i = 1;
      while(i < 11) {
        button_in = digitalRead(button);
        if(button_in == HIGH) {
          Serial.println(i);
          delay(300);
          i += 1;
        }
      }
    }
  
    else if(data_in == 6) {
      float temp;
      char tempo[1];
      lcd.setCursor(0, 0);
      lcd.print("Temperature:");
      do {
        lcd.setCursor(0, 1);
        temp = analogRead(tempsens);
        temp = (temp*500)/1023;
        lcd.print(temp);
        delay(200);
        tempo[0] = Serial.read();
      } while(tempo[0] != 'a');
      lcd.clear();
    }
  
    delay(1000);
  }
}
