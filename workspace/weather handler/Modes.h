#ifndef MODES_H
#define MODES_H

struct modes{
  byte MOVE;
  byte UP;
  byte DOWN;
  byte INIT;
  byte ANI;
  byte LIGHT;
  byte LEVEL;
};

modes MODE={1,2,3,4,5,6,7};

struct speeds{
  byte FULL;
  byte HALF;
  byte QUARTER;
  byte EIGTHTH;
};

speeds SPEED={1,2,3,4};

struct initShortcut{
  byte TOP;
  byte BOTTOM;
  byte CALIBRATE;
};

initShortcut INIT={1,2,3};

struct animationTypes{
	byte SINGLE;
	byte OVERALL;
}

animationTypes ANIMATIONTYPES={1,2}

struct animations{
  byte BLINK;
  byte BEACON;
  byte STAIRCASE;
  byte PULSESLOW;
  
  byte ON;
  byte OFF;
  byte UP;
  byte DOWN;
  byte COM;
  byte BOUNCE;
  byte MOVE;
  byte GLOW;

};

animations ANIMATION={1,2};

struct sides{
  char A;
  char B;
  char C;
  char D;
};

sides SIDE={1,2,3,4};

struct lightOperations{
  char ADD;
  char REMOVE;
  char CLEARSIDE;
};

lightOperations LIGHTOPERATIONS={1,2,3};


#endif