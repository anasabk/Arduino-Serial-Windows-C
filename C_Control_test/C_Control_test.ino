#include <LiquidCrystal.h>

// set the pins for each module
const int led = 9;      //Set the pin of the LED
const int button = 8;   //Set the pin of the LED
const int tempsens = A0;//Set the pin of the LED         

// set the pins to connects the LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  pinMode(tempsens, INPUT);
  lcd.begin(16, 2);
  Serial.println('b');
}

void loop() {
  while ( Serial.available() == 0 );  // Wait for a serial input
  byte data_in = Serial.read();       //gets one byte from the serial buffer, which is the command number

  if (data_in == 'a') 
    Serial.println('b');//Send a respond to get commands
    
  else if (data_in >= 49 && data_in <= 54) { //Do not enter if the input is not an integer from 1 to 6
    data_in -= 48;    //Convert the input to an integer
    
    if(data_in == 1) {      //LED on command
      digitalWrite(led, HIGH);
      while ( Serial.available() == 0 );
    }
    
    else if(data_in == 2) { //LED off command
      digitalWrite(led, LOW);
      while ( Serial.available() == 0 );
    }

    //Flash the LED for half a second 3 times
    else if(data_in == 3) {
      for(int i=0; i<3; i++) {
          digitalWrite(led, HIGH);
          delay(500);
          digitalWrite(led, LOW);
          delay(500);
      }
      while ( Serial.available() == 0 );
    }

    //Calculate the square of the next integer input
    else if(data_in == 4) {
      while( Serial.available() == 0 ); //Wait for input
      
      int m = 0;
      while(Serial.available()) {   //Read every byte available in the buffer
        delay(5);
        m = m*10 + Serial.read() - 48;  //Update the total input
      }

      char result[8];
      sprintf(result, "%d", m*m);
      Serial.println(result);   //print the result to the serial port
      while ( Serial.available() == 0 );
    }

    /* Count the number of time a button is 
     * pressed and print it to the serial port 
     * on each update.
     */
    else if(data_in == 5) { 
      bool button_in1 = 0, button_in2 = 0;
      int i = 1;
      while(1) {
        if (Serial.read() == 'a') break;
        if (digitalRead(button) == HIGH) {
          Serial.println(i);
          i += 1;
          while (digitalRead(button) == HIGH);
        }
      }
    }

    /* Show the temperature recieved from the 
     * temperature sensor and show it in the
     * LCD.
     */
    else if(data_in == 6) {
      float temp;
      char tempo[1];
      lcd.setCursor(0, 0);
      lcd.print("Temperature:");
      do {
        tempo[0] = 0;
        lcd.setCursor(0, 1);
        temp = analogRead(tempsens);
        temp = (temp*500)/1023;
        lcd.print(temp);
        delay(200);
        if (Serial.available())
          tempo[0] = Serial.read();
      } while(tempo[0] != 'a');
      lcd.clear();
      while ( Serial.available() == 0 );
    }
  }
}
