#ifndef USERINPUT_H
  #define USERINPUT_H
  #include <stdio.h>
  #include <string.h>
  #include <ctype.h>
  #include <stdbool.h>
  #include <stdlib.h>

  #include "canvas.h"

  typedef struct UserInput_struct {
    char* userString; //What the user enters into terminal
    char userKey; //The command
    char charArg; //Additional char argument (should be 'r' or 'c')
    char* fileName; //File name string for save/load
    int* intArgs; //Additional int arguments (for draw/add/erase/resize functions)
  } UserInput;

  enum LineType {S, H, V, D1, D2};

  bool isNumber(char number[]);
  bool isStarOrSame(Canvas* canvas, int x, int y, char c);
  bool isValidLine(UserInput* userInput, enum LineType* lineType);
  bool isVerticalLine(UserInput* userInput, enum LineType* lineType);
  bool isHorizontalLine(UserInput* userInput, enum LineType* lineType);
  bool isDiagonalLine(UserInput* userInput, enum LineType* lineType);
  UserInput* createBlankUserInput();
  UserInput* setUserInputKeyAndString(UserInput* userInput, const char* prompt);
  UserInput* setUserInputArgs(UserInput* userInput, bool isFirstArgChar, int numArgs);
  UserInput* getValidChar(const char* prompt, Canvas* canvas);
  bool isInputImproper(int numArgsRead, UserInput* userInput, Canvas* canvas);
  void orderIntArgs(UserInput* userInput, enum LineType lineType);
  void freeUserInput(UserInput* userInput);

#endif
