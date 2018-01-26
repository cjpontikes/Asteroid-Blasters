
extern bool gameStart;

extern Adafruit_ILI9341 tft;
extern TouchScreen ts;

extern int xposAst[numAsteroids];//youDied
extern int yposAst[numAsteroids];
extern int xposPow[numPowers];
extern int yposPow[numPowers];
extern int xpos[30];
extern int ypos[30];
extern int powerSize[numPowers];
extern bool poweredUp;
extern bool collision;
extern int playerScore;
extern int currentScore;
extern int radius[numAsteroids];
extern int level1cap;

//uint16_t getRandomU16();

extern int triangleX;
extern int triangleY;

//top vertex of triangle
extern int triVertex1X;
extern int triVertex1Y;

//right most vertex of triangle
extern int triVertex2X;
extern int triVertex2Y;

//bottom vertex of triangle
extern int triVertex3X;
extern int triVertex3Y;

extern int triPoint1X;
extern int triPoint1Y;

extern int triPoint2X;
extern int triPoint2Y;
