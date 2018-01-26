#ifndef GLOBALS_H
#define GLOBALS_H

#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 5   // can be a digital pin
#define XP 4   // can be a digital pin

// joystick pins
#define JOY_VERT_ANALOG A1
#define JOY_HORIZ_ANALOG A0
#define JOY_SEL 2

// width/height of the display when rotated horizontally
#define TFT_WIDTH 320
#define TFT_HEIGHT 240

// layout of display for this assignment,
#define RATING_SIZE 48
#define DISPLAY_WIDTH TFT_WIDTH
#define DISPLAY_HEIGHT TFT_HEIGHT

// constants for the joystick
#define JOY_DEADZONE 64
#define JOY_CENTER 512
#define JOY_STEPS_PER_PIXEL 64

#define numAsteroids 8
#define numPowers 1

#endif
