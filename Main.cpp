#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <math.h>
#include <SPI.h>
#include <SD.h>
#include <TouchScreen.h>
#include "globals.h"
#include "collisions.h"
#include "drawing.h"
#include "screens.h"

bool gameStart = false;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int xposAst[numAsteroids];//youDied
int yposAst[numAsteroids];
int xposPow[numPowers];
int yposPow[numPowers];
int xpos[30];
int ypos[30];
int powerSize[numPowers];
bool poweredUp = false;
bool collision = false;
int playerScore = 0;
int currentScore;
int radius[numAsteroids];
int gameSpeed = 6;
int level = 1;
bool haveWon = false;

//uint16_t getRandomU16();

int triangleX = DISPLAY_WIDTH/10;
int triangleY = DISPLAY_HEIGHT/2;

//top vertex of triangle
int triVertex1X = triangleX;
int triVertex1Y = triangleY - 10;

//right most vertex of triangle
int triVertex2X = triangleX + 20;
int triVertex2Y = triangleY;

//bottom vertex of triangle
int triVertex3X = triangleX;
int triVertex3Y = triangleY + 10;

int triPoint1X = triangleX + 8;
int triPoint1Y = triangleY + 6;

int triPoint2X = triangleX + 8;
int triPoint2Y = triangleY - 6;
//string array with all the level names
const char* planet[10] = {"Pluto","Neptune","Uranus", "Saturn", "Jupiter","Mars",
 									"Earth", "Venus", "Mercury","Escape"};

void game(){
 //while(collision==false){
	 redrawStars(1,25);
	 redrawStars(2,3);
	 redrawAsteroids();
	 redrawTriangle();
	 redrawPower();
	 collisionTest();
	 invincible();
	 //checks the player score
	 //if you reacha certain threshold, the player levels up
	 if (playerScore == level*100 && level <10) {

		 levelComplete(level);
		 level++;
		 Serial.print("level: ");
		 Serial.println(level);
		 gameSpeed +=2;
	 }
	 //if the player beats level 10, they win
	 if(playerScore == level*100 && level == 10){
		 haveWon = true;
		 gameOver();
	 }
 //}
}

void setup(){
 init();

 pinMode(JOY_SEL, INPUT_PULLUP);

 Serial.begin(9600);

 tft.begin();

 tft.setRotation(-1);
 tft.setTextWrap(false);
 tft.fillScreen(ILI9341_BLACK);
}

int main(){
 setup();

 titleScreen();
 //drawBeater();
 while (!gameStart) {
	 TSPoint p = ts.getPoint();
	 if(p.z > 0 && p.x > 0 && p.y > 0){
		 Serial.println();
		 Serial.print("Starting Game");
		 gameStart = true;
		 break;
	 }
 }
 tft.fillScreen(ILI9341_BLACK);
 drawStars(1,20);
 delay(2000);
 tft.setCursor(13,110);
 tft.setTextSize(2);
 tft.print("First Level: ");
 tft.print("Pluto");
 delay(2000);
 tft.fillScreen(ILI9341_BLACK);
 drawStars(1,20);
 drawStars(2,3);
 drawAsteroids();
 drawPower();
 drawTriangle(ILI9341_RED);
 while (true) {
	 if(collision == false) {
		 game();
		// tft.fillRect(triangleX, triangleY-10,10,20,ILI9341_WHITE);
		 //tft.fillRect(triangleX+10, triangleY-7, 10,15,ILI9341_WHITE);
	 }else
	 gameOver();
 }
 Serial.flush();
 return 0;
}
