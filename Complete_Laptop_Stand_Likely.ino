#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <FastLED.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define encodeDT 8 //switch to change direction  
#define encodeCLK 6 
#define encodeSW 9
#define pwmPin 3
#define dataPin 4
#define numLeds 68 //1.70 Amps at white

long timeOfLastDebounce = 0;
int delayOfDebounce = 0.01;
int previousCLK;
int previousDATA;
int interval = 15;
int menu = 0; //starts false
int menuPos = 1;
int red = 60;
int green = 130;
int blue = 180;
long sleep = 0;

CRGB leds[numLeds];
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void setup(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {} // Address 0x3D for 128x64
  TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  analogWrite(pwmPin, computeInterval());
  display.clearDisplay();
  display.dim(1);
  display.setTextColor(WHITE);
  display.setFont(&FreeSansBold12pt7b);
  display.setCursor(12,33);
  display.print("Welcome");
  display.setCursor(4,54);
  display.print("Mr.Donker");
  display.display();
  delay(2000);
  display.clearDisplay();
  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
  }
  display.setFont(&FreeSansBold12pt7b);
  printInterval();
  previousCLK = digitalRead(encodeCLK);
  previousDATA = digitalRead(encodeDT);
  pinMode(pwmPin, OUTPUT);
  FastLED.addLeds<WS2812, dataPin, GRB>(leds, numLeds);
  FastLED.clear();
  for (int i = 0; i < numLeds; ++i){
    leds[i] = CRGB(red,green,blue);
  }
  FastLED.show();
}

void loop(){
  sleep++;
  if(sleep == 5000000){                                      
  //screen makes high pitched whiney noise, this timer is not elegant
    for(int16_t i=0; i<display.height()/2; i+=2) {
      display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
      display.display(); // Update screen with each newly-drawn rectangle
    }
    display.display();
    display.clearDisplay();
    display.display();
  }                                                           //should proably make it stop increasing after
  if ((millis() - timeOfLastDebounce) > delayOfDebounce){
    check_rotary(interval,5,0,100,menu);
    previousCLK = digitalRead(encodeCLK);
    previousDATA = digitalRead(encodeDT);
    timeOfLastDebounce = millis();
  }
  if (digitalRead(encodeSW) == LOW){
    while (digitalRead(encodeSW) == LOW){}
    menu = 1;
    printMenu(menu,0);
  }
  while(menu == 1){
    if ((millis() - timeOfLastDebounce) > delayOfDebounce){
      menuPos = check_rotary(menuPos,1,1,4,menu);
      previousCLK = digitalRead(encodeCLK);
      previousDATA = digitalRead(encodeDT);
      timeOfLastDebounce = millis();
    }
    if (digitalRead(encodeSW) == LOW){
      while (digitalRead(encodeSW) == LOW){}
      menu = 2;
      if(menuPos == 4){
        menu = 0;
        menuPos = 1;
        printInterval();
      }
      else {
        if(menuPos == 1){
          printMenu(menu,red);
        }
        else if(menuPos == 2){
          printMenu(menu,green);
        }
        else if(menuPos == 3){
          printMenu(menu,blue);
        }
      }   
    }
    while(menu == 2){
      if ((millis() - timeOfLastDebounce) > delayOfDebounce){
        if(menuPos == 1){
          red = check_rotary(red,15,0,255,menu);
        }
        else if(menuPos == 2){
          green = check_rotary(green,15,0,255,menu);
        }
        else{
          blue = check_rotary(blue,15,0,255,menu);
        } 
        previousCLK = digitalRead(encodeCLK);
        previousDATA = digitalRead(encodeDT);
        timeOfLastDebounce = millis();
      }
      if (digitalRead(encodeSW) == LOW){
        while (digitalRead(encodeSW) == LOW){}      
        menu = 1;
        FastLED.clear();
        for (int i = 0; i < numLeds; ++i){
          leds[i] = CRGB(red,green,blue);
        }
        FastLED.show();
        printMenu(menu,0); 
      }
    }
  } 
}

void printMenu(int menuChoice3,int colour){
  if(menuChoice3 == 0){
    printInterval();
    analogWrite(pwmPin, computeInterval());
  }
  else if(menuChoice3 == 1){
    if (menuPos == 1){
      display.clearDisplay();
      display.setCursor(39,12);
      display.print("Menu");
      display.setCursor(14,30);
      display.print("Red");
      display.setCursor(70,30);
      display.print("Green");
      display.setCursor(14,52);
      display.print("Blue");
      display.setCursor(72,52);
      display.print("Exit");
      display.drawRoundRect(11,16,40,17,2,SSD1306_WHITE);
      display.display();
    }
    else if (menuPos == 2){
      display.clearDisplay();
      display.setCursor(39,12);
      display.print("Menu");
      display.setCursor(14,30);
      display.print("Red");
      display.setCursor(70,30);
      display.print("Green");
      display.setCursor(14,52);
      display.print("Blue");
      display.setCursor(72,52);
      display.print("Exit");
      display.drawRoundRect(68,16,56,17,2,SSD1306_WHITE);
      display.display();
    }
    else if (menuPos == 3){
      display.clearDisplay();
      display.setCursor(39,12);
      display.print("Menu");
      display.setCursor(14,30);
      display.print("Red");
      display.setCursor(70,30);
      display.print("Green");
      display.setCursor(14,52);
      display.print("Blue");
      display.setCursor(72,52);
      display.print("Exit");
      display.drawRoundRect(11,38,46,17,2,SSD1306_WHITE);
      display.display();
    }
    else if (menuPos == 4){
      display.clearDisplay();
      display.setCursor(39,12);
      display.print("Menu");
      display.setCursor(14,30);
      display.print("Red");
      display.setCursor(70,30);
      display.print("Green");
      display.setCursor(14,52);
      display.print("Blue");
      display.setCursor(72,52);
      display.print("Exit");
      display.drawRoundRect(69,38,40,17,2,SSD1306_WHITE);
      display.display();
    } 
  }
  else if(menuChoice3 == 2){
    if (menuPos == 1){
      display.clearDisplay();
      display.setCursor(39,12);
      display.print("Menu");
      display.setCursor(17,34);
      display.print("Red");
      display.setCursor(75,34);
      display.println(colour);
      display.drawRoundRect(10,43,108,16,2,SSD1306_WHITE);
      display.fillRect(11,43,(int)(colour/2.4),15,SSD1306_WHITE);
      display.display();
    }
    else if (menuPos == 2){
      display.clearDisplay();
      display.setCursor(39,12);
      display.print("Menu");
      display.setCursor(8,34);
      display.print("Green");
      display.setCursor(82,34);
      display.println(colour);
      display.drawRoundRect(10,43,108,16,2,SSD1306_WHITE);
      display.fillRect(11,43,(int)(colour/2.4),15,SSD1306_WHITE);
      display.display();
    }
    else if (menuPos == 3){
      display.clearDisplay();
      display.setCursor(39,12);
      display.print("Menu");
      display.setCursor(16,34);
      display.print("Blue");
      display.setCursor(76,34);
      display.println(colour);
      display.drawRoundRect(10,43,108,16,2,SSD1306_WHITE);
      display.fillRect(11,43,(int)(colour/2.4),15,SSD1306_WHITE);
      display.display();
    }
  }
}

int check_rotary(int tempInterval,int chunk, int lowRange, int highRange,int menuChoice){
  if((previousCLK == LOW) && (previousDATA == HIGH)){
    if((digitalRead(encodeCLK) == HIGH) && (digitalRead(encodeDT) == LOW)){
      tempInterval = changeInterval(tempInterval,chunk, lowRange, highRange, menuChoice);
      printMenu(menuChoice,tempInterval);
      sleep = 0;
    }
    else if((digitalRead(encodeCLK) == HIGH) && (digitalRead(encodeDT) == HIGH)){
      tempInterval = changeInterval(tempInterval,-chunk, lowRange, highRange, menuChoice);
      printMenu(menuChoice,tempInterval);
      sleep = 0;
    }
  }
  
  else if((previousCLK == LOW) && (previousDATA == LOW)){
    if((digitalRead(encodeCLK) == HIGH) && (digitalRead(encodeDT) == LOW)){
      tempInterval = changeInterval(tempInterval,chunk, lowRange, highRange, menuChoice);
      printMenu(menuChoice,tempInterval);
      sleep = 0;
    }
    else if((digitalRead(encodeCLK) == HIGH) && (digitalRead(encodeDT) == HIGH)){
      tempInterval = changeInterval(tempInterval,-chunk, lowRange, highRange, menuChoice);
      printMenu(menuChoice,tempInterval);
      sleep = 0;
    }
  }
  return tempInterval;
}

int changeInterval(int tempInterval2, int chunk2,int lowRange2,int highRange2,int menuChoice2){
  tempInterval2+=chunk2;
      if(tempInterval2 > highRange2){
        tempInterval2 = highRange2;
      }
      else if (tempInterval2 < lowRange2){
        tempInterval2 = lowRange2;
      }
      if(menuChoice2 == 0){
        interval = tempInterval2;
      }
      else if(menuChoice2 == 1){
        menuPos = tempInterval2;   //Fix?
      }
  return tempInterval2;
 }
 
int computeInterval(){
  float temInterval;
  if (interval == 0){
    temInterval = 0;
  }
  else if(interval == 100){
    temInterval = 255;
  }
  else if(interval == 5){
    temInterval = 15;
  }
  else{
    temInterval = 1.8 * interval;
  }
  return (int)temInterval;
}

void printInterval(){
  display.clearDisplay();
  display.setFont(&FreeSansBold12pt7b);
  if(interval == 100){
    display.setCursor(1,45);         //x,y
    display.println(interval);
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(42,44);
    display.print("%");
  }
  else{
    display.setCursor(8,45);         //x,y
    display.println(interval);
    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(37,44);
    display.print("%");
  }
  display.setCursor(0,12);
  display.print("S");
  display.setCursor(13,11);
  display.print("p");
  display.setCursor(25,12);
  display.print("eed");
  display.setCursor(68,12);
  display.print("Colour");
  display.setCursor(71,28);
  display.print("R:");
  display.setCursor(71+22,28);
  display.println(red);
  display.setCursor(70,(28+15));
  display.print("G:");
  display.setCursor(71+22,28+15);
  display.println(green);
  display.setCursor(71,(28+15+15));
  display.print("B:");
  display.setCursor(71+22,28+15+15);
  display.println(blue);
  display.display();
}
