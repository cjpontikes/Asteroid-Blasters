#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <math.h>
#include <SPI.h>
#include <SD.h>
#include <TouchScreen.h>
#include "globals.h"
#include "screens.h"
#include "drawing.h"
#include "collisions.h"

#include "vars.h"

void drawTriangle(int colour){
  //draws a triangle of input color at specified points
  tft.fillTriangle(triVertex1X,triVertex1Y,triVertex2X,triVertex2Y,triVertex3X,triVertex3Y,colour);
  //draws yellow circles at the collision points
  tft.fillCircle(triVertex1X,triVertex1Y,1,ILI9341_YELLOW);
  tft.fillCircle(triVertex2X,triVertex2Y,1,ILI9341_YELLOW);
  tft.fillCircle(triVertex3X,triVertex3Y,1,ILI9341_YELLOW);
  tft.fillCircle(triPoint1X,triPoint1Y,1,ILI9341_YELLOW);
  tft.fillCircle(triPoint2X,triPoint2Y,1,ILI9341_YELLOW);

  //constrais the triangle to stay on the screen
  triVertex1Y = constrain(triVertex1Y,0,220);
  triVertex2Y = constrain(triVertex2Y,10,230);
  triVertex3Y = constrain(triVertex3Y,20,240);
  triPoint1Y = constrain(triPoint1Y,4,224);
  triPoint2Y = constrain(triPoint2Y,16,236);
  triangleY = constrain(triangleY,10,230);
}

void redrawTriangle(){
  int yVal = analogRead(JOY_VERT_ANALOG);
  int speed = 5;
  //function that will read the joystick value and either move th triangle up of down depending
  //on which way the joystick is pushed

  if(yVal>JOY_CENTER+JOY_DEADZONE){
    //increases each point on the triangle when the joystick is pushed down
    triangleY+=speed;
    triVertex1Y+=speed;
    triVertex2Y+=speed;
    triVertex3Y+=speed;
    triPoint1Y+=speed;
    triPoint2Y+=speed;
    //covers all the old positions in black
    tft.fillTriangle(triVertex1X,triVertex1Y-speed,triVertex2X,triVertex2Y-speed,triVertex3X,triVertex3Y-speed,ILI9341_BLACK);
    tft.fillCircle(triVertex1X,triVertex1Y-speed,1,ILI9341_BLACK);
    tft.fillCircle(triVertex2X,triVertex2Y-speed,1,ILI9341_BLACK);
    tft.fillCircle(triVertex3X,triVertex3Y-speed,1,ILI9341_BLACK);
    tft.fillCircle(triPoint1X,triPoint1Y-speed,1,ILI9341_BLACK);
    tft.fillCircle(triPoint2X,triPoint2Y-speed,1,ILI9341_BLACK);
    //then redraw the triagnel in either blue or red depending on if you are powered up or not
    if(poweredUp){
      drawTriangle(ILI9341_BLUE);
    }else{
      drawTriangle(ILI9341_RED);
    }

    //identical process as above, but everything is reversed
  }else if(yVal<JOY_CENTER-JOY_DEADZONE){
    triangleY-=speed;
    triVertex1Y-=speed;
    triVertex2Y-=speed;
    triVertex3Y-=speed;
    triPoint1Y-=speed;
    triPoint2Y-=speed;
    tft.fillTriangle(triVertex1X,triVertex1Y+speed,triVertex2X,triVertex2Y+speed,triVertex3X,triVertex3Y+speed,ILI9341_BLACK);
    tft.fillCircle(triVertex1X,triVertex1Y+speed,1,ILI9341_BLACK);
    tft.fillCircle(triVertex2X,triVertex2Y+speed,1,ILI9341_BLACK);
    tft.fillCircle(triVertex3X,triVertex3Y+speed,1,ILI9341_BLACK);
    tft.fillCircle(triPoint1X,triPoint1Y+speed,1,ILI9341_BLACK);
    tft.fillCircle(triPoint2X,triPoint2Y+speed,1,ILI9341_BLACK);
    if(poweredUp){
      drawTriangle(ILI9341_BLUE);
    }else{
      drawTriangle(ILI9341_RED);
    }

  }

}
//function dependent on two inputs
void drawStars(int radius, int numStars){
  for(int i = 0; i<numStars; i++){
    //randomly generates the x and y positions of stars and puts them in an array
    xpos[i] = getRandomU16()%DISPLAY_WIDTH + 1;
    ypos[i] = getRandomU16()%DISPLAY_HEIGHT + 1;
    //draws the stars in white
    tft.fillCircle(xpos[i],ypos[i],radius,ILI9341_WHITE);
  }
}

void redrawStars(int radius, int numStars){
  for(int i = 0; i<numStars; i++){
    // will move the stars at a rate dependent on the radius
    xpos[i]-=radius;
    //if the stars go off the screen, they will be regenerated back at the other end of the 'next' screen
    //with a new random x an dy positions
    if(xpos[i]<-5){
      xpos[i] = getRandomU16()%(DISPLAY_WIDTH+5) + DISPLAY_WIDTH;
      ypos[i] = getRandomU16()%DISPLAY_HEIGHT + 1;
    }
    //if a star goes over the ship, the triangle will be redrawn in blue or red depending on if
    //the player is powered up or not
    if(xpos[i]<triVertex2X && xpos[i]>triangleX-3 && ypos[i]<triVertex3Y && ypos[i]>triVertex1Y){
      tft.fillCircle(xpos[i]+radius+1, ypos[i],radius,ILI9341_BLACK);

      if(poweredUp){
        drawTriangle(ILI9341_BLUE);
      }else{
        drawTriangle(ILI9341_RED);
      }

    }else{
      tft.fillCircle(xpos[i]+radius+1,ypos[i],radius,ILI9341_BLACK);
      tft.fillCircle(xpos[i],ypos[i],radius,ILI9341_WHITE);
    }
  }
}

void drawAsteroids(){
  //randomly generates an x and y positions for each asteroid and stores them in an array
  for(int i = 0; i<numAsteroids; i++){
    radius[i] = getRandomU16()%7 + 3;
    xposAst[i] = getRandomU16()%(2*DISPLAY_WIDTH) + DISPLAY_WIDTH;
    yposAst[i] = getRandomU16()%DISPLAY_HEIGHT + 1;
    tft.fillCircle(xposAst[i],yposAst[i],radius[i],ILI9341_GREEN);
  }
}

void redrawStars(int radius, int numStars){
 for(int i = 0; i<numStars; i++){
	 xpos[i]-=radius;
	 if(xpos[i]<-5){
		 xpos[i] = getRandomU16()%(DISPLAY_WIDTH+5) + DISPLAY_WIDTH;
		 ypos[i] = getRandomU16()%DISPLAY_HEIGHT + 1;
	 }
	 if(xpos[i]<triVertex2X && xpos[i]>triangleX-3 && ypos[i]<triVertex3Y && ypos[i]>triVertex1Y){
		 tft.fillCircle(xpos[i]+radius+1, ypos[i],radius,ILI9341_BLACK);

     if(poweredUp){
       drawTriangle(ILI9341_BLUE);
     }else{
        drawTriangle(ILI9341_RED);
     }

	 }else{
		 tft.fillCircle(xpos[i]+radius+1,ypos[i],radius,ILI9341_BLACK);
		 tft.fillCircle(xpos[i],ypos[i],radius,ILI9341_WHITE);
	 }
 }
}

void drawAsteroids(){
 for(int i = 0; i<numAsteroids; i++){
	 radius[i] = getRandomU16()%7 + 3;
	 xposAst[i] = getRandomU16()%(2*DISPLAY_WIDTH) + DISPLAY_WIDTH;
	 yposAst[i] = getRandomU16()%DISPLAY_HEIGHT + 1;
	 tft.fillCircle(xposAst[i],yposAst[i],radius[i],ILI9341_GREEN);
 }
}

void redrawAsteroids(){
 for(int i = 0; i<numAsteroids; i++){
   //makes the asteroids move at a variable speed depending on the level of the game
	 xposAst[i] -= gameSpeed;
   //if the astreoid goes off the screen, the player score increases and the asteroid will be
   //redrawn on the 'next'  screen, similar to the stars
	 if((xposAst[i]+radius[i])<-25){

     playerScore++;

     Serial.print("playerScore: ");
     Serial.println(playerScore);

		 xposAst[i] = getRandomU16()%(DISPLAY_WIDTH+5+radius[i]) + DISPLAY_WIDTH;
		 yposAst[i] = getRandomU16()%DISPLAY_HEIGHT+1;
	 }
	 tft.fillCircle(xposAst[i]+gameSpeed,yposAst[i], radius[i], ILI9341_BLACK);
	  tft.fillCircle(xposAst[i],yposAst[i], radius[i], ILI9341_GREEN);

 }
}
//draws power similar to stars
void drawPower(){
 for (int i = 0; i < numPowers; i++) {
	 xposPow[i] = getRandomU16()%(2*DISPLAY_WIDTH) + DISPLAY_WIDTH;
	 yposPow[i] = getRandomU16()%DISPLAY_HEIGHT + 1;
	 tft.fillCircle(xposPow[i],yposPow[i],2,ILI9341_BLUE);
 }
}
//redraw power similar to the asteroids
void redrawPower(){
 for (int i = 0; i < numPowers; i++) {
	 xposPow[i]-=gameSpeed;
   if(poweredUp){
     //tft.fillCircle(xposPow[i], yposPow[i],2,ILI9341_WHITE);
     tft.fillCircle(xposPow[i]+gameSpeed, yposPow[i],2,ILI9341_BLACK);
     xposPow[i] = getRandomU16()%(2*DISPLAY_WIDTH) + (1*DISPLAY_WIDTH + 1);
		 yposPow[i] = getRandomU16()%DISPLAY_HEIGHT + 1;
   }
	 if(xposPow[i]<-10 && poweredUp == false){
     //will only regenerate the power if you are currently not powered up
		 xposPow[i] = getRandomU16()%(2*DISPLAY_WIDTH) + (1*DISPLAY_WIDTH + 1);
		 yposPow[i] = getRandomU16()%DISPLAY_HEIGHT + 1;
	 }
	 tft.fillCircle(xposPow[i]+gameSpeed,yposPow[i],2, ILI9341_BLACK);
	 tft.fillCircle(xposPow[i], yposPow[i],2,ILI9341_BLUE);
 }
}


  void drawBeater(){
    tft.fillScreen(ILI9341_WHITE);
    tft.fillRect(0,2,2,26,ILI9341_BLUE);
    tft.fillRect(2,4,2,22,ILI9341_BLUE);
    tft.fillRect(4,6,2,18,ILI9341_BLUE);
    tft.fillRect(6,8,2,2,ILI9341_BLUE);
    tft.fillRect(6,12,2,2,ILI9341_BLUE);
    tft.fillRect(6,16,2,2,ILI9341_BLUE);
    tft.fillRect(6,20,2,2,ILI9341_BLUE);

    tft.fillRect(6,4,2,4,ILI9341_BLACK);
    tft.fillRect(6,10,2,2,ILI9341_BLACK);
    tft.fillRect(6,14,2,2,ILI9341_BLACK);
    tft.fillRect(6,18,2,2,ILI9341_BLACK);
    tft.fillRect(6,22,2,4,ILI9341_BLACK);
    tft.fillRect(8,2,2,26,ILI9341_BLACK);

    tft.fillRect(10,2,6,2,ILI9341_BLACK);
    tft.fillRect(10,12,4,2,ILI9341_BLACK);
    tft.fillRect(10,16,4,2,ILI9341_BLACK);
    tft.fillRect(10,26,6,2,ILI9341_BLACK);

    tft.fillRect(12,10,4,2,ILI9341_BLACK);
    tft.fillRect(12,18,4,2,ILI9341_BLACK);
    tft.fillRect(14,8,4,2,ILI9341_BLACK);
    tft.fillRect(14,20,4,2,ILI9341_BLACK);
    tft.fillRect(14,0,10,2,ILI9341_BLACK);
    tft.fillRect(14,28,10,2,ILI9341_BLACK);

    tft.fillRect(16,6,6,2,ILI9341_BLACK);
    tft.fillRect(16,22,6,2,ILI9341_BLACK);
    tft.fillRect(20,4,2,4,ILI9341_BLACK);
    tft.fillRect(20,22,2,4,ILI9341_BLACK);
    tft.fillRect(22,2,22,2,ILI9341_BLACK);
    tft.fillRect(22,26,22,2,ILI9341_BLACK);

    tft.fillRect(24,4,4,8,ILI9341_BLACK);
    tft.fillRect(24,18,4,8,ILI9341_BLACK);
    tft.fillRect(26,12,2,6,ILI9341_BLACK);
    tft.fillRect(28,14,8,2,ILI9341_BLACK);
    tft.fillRect(28,6,4,2,ILI9341_BLACK);
    tft.fillRect(28,22,4,2,ILI9341_BLACK);

    tft.fillRect(30,8,10,2,ILI9341_BLACK);
    tft.fillRect(30,20,10,2,ILI9341_BLACK);
    tft.fillRect(34,4,2,2,ILI9341_BLACK);
    tft.fillRect(34,24,2,2,ILI9341_BLACK);
    tft.fillRect(36,6,2,18,ILI9341_BLACK);
    tft.fillRect(38,8,2,14,ILI9341_BLACK);
    tft.fillRect(40,12,2,6,ILI9341_BLACK);

    tft.fillRect(28,8,2,2,ILI9341_GREEN);
    tft.fillRect(28,16,8,4,ILI9341_GREEN);
    tft.fillRect(28,10,8,4,ILI9341_GREEN);
    tft.fillRect(28,20,2,2,ILI9341_GREEN);

    tft.fillRect(10,4,4,6,ILI9341_YELLOW);
    tft.fillRect(10,10,2,2,ILI9341_YELLOW);
    tft.fillRect(10,18,2,2,ILI9341_YELLOW);
    tft.fillRect(10,20,4,6,ILI9341_YELLOW);
    tft.fillRect(14,4,2,4,ILI9341_YELLOW);
    tft.fillRect(14,22,2,4,ILI9341_YELLOW);
    tft.fillRect(16,2,4,4,ILI9341_YELLOW);
    tft.fillRect(16,24,4,4,ILI9341_YELLOW);
    tft.fillRect(20,2,2,2,ILI9341_YELLOW);
    tft.fillRect(20,26,2,2,ILI9341_YELLOW);
    tft.fillRect(24,12,2,6,ILI9341_YELLOW);
    tft.fillRect(22,4,2,22,ILI9341_YELLOW);
    tft.fillRect(18,8,4,14,ILI9341_YELLOW);
    tft.fillRect(16,10,2,10,ILI9341_YELLOW);
    tft.fillRect(14,12,2,6,ILI9341_YELLOW);
    tft.fillRect(10,14,4,2,ILI9341_YELLOW);

    tft.fillRect(28,4,6,2,ILI9341_YELLOW);
    tft.fillRect(32,6,4,2,ILI9341_YELLOW);
    tft.fillRect(32,22,4,2,ILI9341_YELLOW);
    tft.fillRect(28,24,6,2,ILI9341_YELLOW);
  }
