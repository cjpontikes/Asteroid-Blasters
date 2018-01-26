#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <math.h>
#include <SPI.h>
#include <SD.h>
#include <TouchScreen.h>
#include "globals.h"
#include "screens.h"
#include "drawing.h"

#include "vars.h"

const int randomPin = A7;
//generates a random unsigned 16 bit integer
uint16_t getRandomU16() {
  uint16_t num = 0;
  for (int i = 0; i < 16; i++) {
    // shift the number over and add a random bit
    num = (num<<1) | (analogRead(randomPin)&1);
  }
  return num;
}
//calculates the distance between two points
int distance(int32_t x1, int32_t x2, int32_t y1, int32_t y2){
  int32_t x = abs(x1-x2);
  int32_t y = abs(y1-y2);
  int32_t xsquared = abs(x*x);
  int32_t ysquared = abs(y*y);

  int32_t distsquared = xsquared+ysquared;
  int32_t dist = sqrt(distsquared);

  return dist;
}

void invincible(){
  //for loop to scan through each power up
  for(int i = 0; i<numPowers; i++){
    //checks if the power up has passed a certain point
    if(xposAst[i]<triVertex2X+10){
      //checks if the power up is 'inside' the ship, if so, the player has powered up
      //current score will take the players score when the power up occurs, as the power up
      //lasts for a finite time
      if(xposPow[i]<triangleX+25 && xposPow[i]>triangleX && yposPow[i]<triangleY+12 && yposPow[i]>triangleY-12){
        poweredUp = true;
        currentScore = playerScore;
      }
      else if (xposPow[i]<triangleX+10 && xposPow[i]>triangleX && yposPow[i]<triangleY+15 && yposPow[i]>triangleY-15){
        poweredUp = true;

        currentScore = playerScore;
      }
      //once powered up, the ship will be blue
      if(poweredUp){

        drawTriangle(ILI9341_BLUE);


      }
    }
  }
  //once thplayer has accumulated 10 points while invincible, hte power up goes off
  if(playerScore == (currentScore + 10) && poweredUp){

    poweredUp = false;

  }
}

void collisionTest(){
  //creates empty arrays of distances between points on the ship and the asteroids
  int32_t dist1[numAsteroids];
  int32_t dist2[numAsteroids];
  int32_t dist3[numAsteroids];
  int32_t dist4[numAsteroids];
  int32_t dist5[numAsteroids];
  for(int i = 0; i<numAsteroids; i++){
    //checks to see if the asteroids have passed a certain point
    if(xposAst[i]<triVertex2X+10 && poweredUp == false){
      //scans the distance between 5 points on the ship and teh center of each asteroids
      dist1[i] = distance(xposAst[i], triVertex1X, yposAst[i], triVertex1Y);
      dist2[i] = distance(xposAst[i], triVertex2X, yposAst[i], triVertex2Y);
      dist3[i] = distance(xposAst[i], triVertex3X, yposAst[i], triVertex3Y);
      dist4[i] = distance(xposAst[i], triPoint1X, yposAst[i], triPoint1Y);
      dist5[i] = distance(xposAst[i], triPoint2X, yposAst[i], triPoint2Y);

      //series of if statements to see if the distance between a point on the triangle
      //and the center of an asteroid is equal to the radius of that asteroid
      //if so, there is a collision
      if((dist1[i] < radius[i])){
        collision = true;

      }else if((dist2[i] < radius[i])){
        collision = true;

      }else if((dist3[i] < radius[i])){
        collision = true;

      }else if(dist4[i] < radius[i]){
        collision = true;

      }else if(dist5[i] < radius[i]){
        collision = true;

      }
      // redraw the triangle white when a collision occures
      if(collision){
        drawTriangle(ILI9341_WHITE);

      }
    }
  }
}
