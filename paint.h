#ifndef PAINT_H
  #define PAINT_H
  #include <stdio.h>

  #include "canvas.h"
  #include "userInput.h"

  void print_help();
  void drawCanvas(UserInput* userInput, Canvas* canvas);
  void erase(UserInput* userInput, Canvas* canvas);
  void add(UserInput* userInput, Canvas* canvas);
  void delete(UserInput* userInput, Canvas* canvas);
  void resize(UserInput* userInput, Canvas* canvas);
  void saveCanvas(UserInput* userInput, Canvas* canvas);
  void loadCanvas(UserInput* userInput, Canvas* canvas);

#endif
