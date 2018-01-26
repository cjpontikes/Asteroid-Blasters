#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <math.h>
#include <SPI.h>
#include <SD.h>
#include <TouchScreen.h>
#include "screens.h"
#include "globals.h"
#include "drawing.h"

#include "vars.h"

void titleScreen(){
 tft.fillScreen(ILI9341_BLACK);
 drawStars(1,30);
 tft.setTextSize(4);
 tft.setCursor(53,70);
 tft.setTextColor(ILI9341_BLUE);
 tft.println("Asteroid");
 tft.setCursor(53,100);
 tft.println("Blasters");
 tft.setCursor(150,150);
 tft.println("2");
}

void youDied(){
 tft.setCursor(53,70);
 tft.setTextSize(4);
 tft.setTextColor(ILI9341_RED);
 tft.print("YOU DIED");
 //delay(1000);
 tft.setCursor(95,120);
 tft.setTextSize(2);
 tft.setTextColor(ILI9341_WHITE);
 tft.print("Score: ");
 tft.print(playerScore);
 tft.setCursor(95,180);
 tft.fillRect(90,175,115,25,ILI9341_RED);
 tft.print("Continue?");
}

void gameOver(){
 tft.fillScreen(ILI9341_BLACK);
 drawStars(1,30);
 youDied();
 while (collision) {
	 TSPoint p = ts.getPoint();
	 //Serial.println("waiting");
		 if(p.z > 0 && p.x > 0 && p.y > 0){
			 Serial.println();
			 Serial.print("RETRYING LEVEL");
			 collision = false;
			 playerScore = 0;
			 tft.fillScreen(ILI9341_BLACK);
			 drawStars(1,20);
			 drawStars(2,3);
			 drawAsteroids();
			 drawPower();
			 drawTriangle(ILI9341_RED);
			 break;
		 }
 }
}

void levelComplete(int level){
		tft.fillScreen(ILI9341_BLACK);
		drawStars(1,30);
		tft.setTextSize(3);
		tft.setTextColor(ILI9341_BLUE);
		tft.fillRect(10,67,50,25,ILI9341_BLACK);
		tft.setCursor(13,70);
		tft.print("LEVEL ");
		tft.print(level);
		tft.print(" COMPLETE");
		delay(500);
		tft.fillRect(10,67,320,25,ILI9341_BLACK);
		delay(500);
		tft.fillRect(10,67,320,25,ILI9341_BLACK);
		tft.setCursor(13,70);
		tft.print("LEVEL ");
		tft.print(level);
		tft.print(" COMPLETE");
		delay(500);
		tft.fillRect(10,67,320,25,ILI9341_BLACK);
		delay(500);
		tft.fillRect(10,67,320,25,ILI9341_BLACK);
		tft.setCursor(13,70);
		tft.print("LEVEL ");
		tft.print(level);
		tft.print(" COMPLETE");
		delay(1000);
		tft.fillScreen(ILI9341_BLACK);
		delay(2000);
		tft.setCursor(10,70);
		tft.print("+100 Score");
		tft.setCursor(13,110);
		tft.setTextSize(2);
		tft.print("Next Level: Neptune");
		delay(2000);
		tft.fillScreen(ILI9341_BLACK);
		delay(1000);
		tft.fillScreen(ILI9341_BLACK);
		drawStars(1,20);
		drawStars(2,3);
		drawAsteroids();
		drawPower();
		drawTriangle(ILI9341_RED);
}
