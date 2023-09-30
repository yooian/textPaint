#ifndef CANVAS_H
  #define CANVAS_H
  #include <stdio.h>
  #include <stdlib.h>


  typedef struct Canvas_struct {
    char** canvas;
    int row;
    int col;
  } Canvas;

  Canvas* createCanvas(int row, int col);
  Canvas* fillCanvas(Canvas* canvas);
  void printCanvas(Canvas* canvas);
  void freeCanvas(Canvas* canvas);

#endif
