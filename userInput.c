#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "userInput.h"
#include "canvas.h"


/*
 * @desc: Checks if string is number
 * @number[] : String
 * @return TRUE if number, FALSE if not a number
 */
bool isNumber(char number[]) {
  for (int i = 0; i < (int)strlen(number); i++) {
    if (i == 0 && number[i] == '-') {
      i++;
    }
    if (!isdigit(number[i])) {
      return false;
    }
  }
  return true;
}
/*
 * @desc: Checks if a location in the canvas is a * or the same as a given char
 * @*canvas : Canvas with painting
 * @x : The row
 * @y : The column
 * @c : The character to compare it to
 * @return TRUE if the canvas location is a * or the same as c, otherwise FALSE
 */
bool isStarOrSame(Canvas* canvas, int x, int y, char c) {
  return canvas->canvas[x][y] == '*' || canvas->canvas[x][y] == c;
}
/*
 * @desc: Checks if the user drew a straight line (vertical, horizontal, diagonal, or (S)ingle cell)
 * @*userInput : Pointer to UserInput struct that contains the starting/ending coords of the line
 * @*lineType : A pointer to the type of line it is (vertical, horizontal, diagonal)
 * @return TRUE if valid line, FALSE if not
 */
bool isValidLine(UserInput* userInput, enum LineType* lineType) {
  if ((userInput->intArgs[0] == userInput->intArgs[2]) && (userInput->intArgs[1] == userInput->intArgs[3])) {
    *lineType = S;
    return true;
  }
  return isVerticalLine(userInput, lineType) || isHorizontalLine(userInput, lineType)
          || isDiagonalLine(userInput, lineType);
}
/*
 * @desc: Checks if the user drew a vertical line
 * @*userInput : Pointer to UserInput struct that contains the starting/ending coords of the line
 * @*lineType : A pointer to the type of line it is (this will be assigned)
 * @return TRUE if valid line, FALSE if not
 */
bool isVerticalLine(UserInput* userInput, enum LineType* lineType) {
  if (userInput->intArgs[1] == userInput->intArgs[3]) {
    *lineType = V;
    return true;
  }
  return false;
}
/*
 * @desc: Checks if the user drew a horizontal line
 * @*userInput : Pointer to UserInput struct that contains the starting/ending coords of the line
 * @*lineType : A pointer to the type of line it is (this will be assigned)
 * @return TRUE if valid line, FALSE if not
 */
bool isHorizontalLine(UserInput* userInput, enum LineType* lineType) {
  if (userInput->intArgs[0] == userInput->intArgs[2]) {
    *lineType = H;
    return true;
  }
  return false;
}
/*
 * @desc: Checks if the user drew a diagonal line of slope -1 or 1
 * @*userInput : Pointer to UserInput struct that contains the starting/ending coords of the line
 * @*lineType : A pointer to the type of line it is (this will be edited)
 * @return TRUE if valid line, FALSE if not
 */
bool isDiagonalLine(UserInput* userInput, enum LineType* lineType) {
  double slope = (userInput->intArgs[3] - userInput->intArgs[1]) / (userInput->intArgs[2] - userInput->intArgs[0]);
  if (slope == 1.0) {
    *lineType = D1;
    return true;
  }
  else if (slope == -1.0) {
    *lineType = D2;
    return true;
  }
  return false;
}
/*
 * @desc: Creates and initializes a new UserInput struct
 * @return pointer to *newUserInput, the new UserInput struct
 */
UserInput* createBlankUserInput() {
  UserInput* newUserInput = (UserInput*) calloc(1, sizeof(UserInput));
  newUserInput->userString = (char*) calloc(0, sizeof(char));
  return newUserInput;
}
/*
 * @desc: Assigns user command input to userKey and userString
 * @*userInput : UserInput struct to edit
 * @*prompt : A string prompt (should be "Enter your command: ")
 * @return *userInput, with input assigned
 */
UserInput* setUserInputKeyAndString(UserInput* userInput, const char* prompt) {
  int count = 0;
  userInput->userString = (char*) realloc(userInput->userString, count * sizeof(char));
  printf("%s", prompt);

  do {
    userInput->userString = (char*) realloc(userInput->userString, (count + 1) * sizeof(char));
    scanf("%c", &userInput->userString[count]);
    count++;
  } while (userInput->userString[count - 1] != '\n');
  sscanf(userInput->userString, " %c", &userInput->userKey);
  if (userInput->userKey == 'q' && userInput->userString[count - 1] != '\n') {
    userInput->userString = (char*) realloc(userInput->userString, (count + 1) * sizeof(char));
    userInput->userString[count] = '\n';
  }
  return userInput;
}
/*
 * @desc: Assigns any additional integer arguments to intArgs
 * @*userInput : UserInput struct to edit
 * @*prompt : A string prompt (should be "Enter your command: ")
 * @return *userInput, with input assigned
 */
UserInput* setUserInputArgs(UserInput* userInput, bool isSecondArgChar, int numExtraArgs) {
  if (isSecondArgChar) {
    userInput->intArgs = (int*) calloc(numExtraArgs - 1, sizeof(int));
  }
  else {
    userInput->intArgs = (int*) calloc(numExtraArgs, sizeof(int));
  }
  return userInput;
}
/*
 * @desc: Checks if input is valid
 * @*prompt : A string prompt (should be "Enter your command: ")
 * @*canvas : The paint canvas (so we can access the # of rows and columns)
 * @return *userInput, with valid user command
 */
UserInput* getValidChar(const char* prompt, Canvas* canvas) {
  UserInput* userInput = createBlankUserInput();
  char c = '-';
  bool isValidChar = false;

  userInput = setUserInputKeyAndString(userInput, prompt);
  int numArgsRead;

  char* e;
  int index;
  e = strchr(userInput->userString, userInput->userKey);
  index = (int)(e - userInput->userString);

  bool qCheck = false;
  int count = index + 1;

  if (userInput->userKey == 'q') {
    while (c != '\n') {
      c = userInput->userString[count];
      if (!isspace(c)) {
        break;
      }
      count++;
    }
    if (c == '\n') {
      return userInput;
    }
    else {
      qCheck = true;
    }
  }
  //Condition for valid command (ie, "hh" instead of "h")
  if (!isspace(userInput->userString[index + 1]) || qCheck) {
    printf("Unrecognized command. Type h for help.\n");
  }
  else {
    switch (userInput->userKey) {
      case 'q':
        c = userInput->userString[index + 1];
        if (c == '\n') { //Makes sure only single char was entered
          isValidChar = true;
        }
        break;

      case 'h':
        count = index + 1;
        while (c != '\n') {
          c = userInput->userString[count];
          if (!isspace(c)) {
            break;
          }
          count++;
        }
        if (c == '\n') { //Makes sure only single char was entered
          isValidChar = true;
        }
        break;

      case 'w':
        userInput = setUserInputArgs(userInput, false, 4);
        numArgsRead = sscanf(userInput->userString, " %c %d %d %d %d %c", &userInput->userKey,
                                &userInput->intArgs[0], &userInput->intArgs[1],
                                &userInput->intArgs[2], &userInput->intArgs[3], &c);
        if (isInputImproper(numArgsRead, userInput, canvas)) {
          printf("Improper draw command.\n");
        }
        else {
          isValidChar = true;
        }
        break;

      case 'r':
        userInput = setUserInputArgs(userInput, false, 2);
        numArgsRead = sscanf(userInput->userString, " %c %d %d %c", &userInput->userKey,
                              &userInput->intArgs[0], &userInput->intArgs[1], &c);
        if (isInputImproper(numArgsRead, userInput, canvas)) {
          printf("Improper resize command.\n");
        }
        else {
          isValidChar = true;
        }
        break;

      case 'a':
        userInput = setUserInputArgs(userInput, true, 2);
        numArgsRead = sscanf(userInput->userString, " %c %c %d %c", &userInput->userKey,
                              &userInput->charArg, &userInput->intArgs[0], &c);
        if (isInputImproper(numArgsRead, userInput, canvas)) {
          printf("Improper add command.\n");
        }
        else {
          isValidChar = true;
        }
        break;

      case 'd':
        userInput = setUserInputArgs(userInput, true, 2);
        numArgsRead = sscanf(userInput->userString, " %c %c %d %c", &userInput->userKey,
                              &userInput->charArg, &userInput->intArgs[0], &c);
        if (isInputImproper(numArgsRead, userInput, canvas)) {
          printf("Improper add command.\n");
        }
        else {
          isValidChar = true;
        }
        break;

      case 'e':
        userInput = setUserInputArgs(userInput, false, 2);
        numArgsRead = sscanf(userInput->userString, " %c %d %d %c", &userInput->userKey,
                              &userInput->intArgs[0], &userInput->intArgs[1], &c);
        if (isInputImproper(numArgsRead, userInput, canvas)) {
          printf("Improper erase command.\n");
        }
        else {
          isValidChar = true;
        }
        break;

      case 's':
        {
          //This checks if the number of arguments entered is valid (should be 2)
          char* s = (char*) calloc(200, sizeof(char));
          numArgsRead = sscanf(userInput->userString, " %c %s %c", &userInput->userKey, s, &c);
          if (numArgsRead != 2) {
            printf("Improper save command or file could not be created.\n");
            break;
          }

          // int count = 0;
          // char prevChar = '-';
          // userInput->fileName = (char*) calloc(0, sizeof(char));
          // //Find the starting index of the file name in userString
          // do {
          //   c = userInput->userString[count];
          //   if (!isspace(c) && isspace(prevChar)) {
          //     break;
          //   }
          //   prevChar = c;
          //   count++;
          // } while (c != '\n');
          //
          // //Once index is found, copy the file name into userInput->fileName
          // for (int l = 0, i = count; i < (int)strlen(userInput->userString) - 1; l++, i++) {
          //   userInput->fileName = (char*) realloc(userInput->fileName, (l + 1) * sizeof(char));
          //   userInput->fileName[l] = userInput->userString[i];
          // }
          userInput->fileName = (char*) realloc(userInput->fileName, strlen(s) * sizeof(char));
          strcpy(userInput->fileName, s);
          isValidChar = true;
        }
        break;

      case 'l':
      {
        //This checks if the number of arguments entered is valid (should be 2)
        char* s = (char*) calloc(200, sizeof(char));
        numArgsRead = sscanf(userInput->userString, " %c %s %c", &userInput->userKey, s, &c);
        if (numArgsRead != 2 && !isspace(c)) {
          printf("Improper load command or file could not be opened.\n");
          break;
        }
        // int count = 0;
        // char prevChar = '-';
        // userInput->fileName = (char*) calloc(0, sizeof(char));
        // //Find the starting index of the file name in userString
        // do {
        //   c = userInput->userString[count];
        //   if (!isspace(c) && isspace(prevChar)) {
        //     break;
        //   }
        //   prevChar = c;
        //   count++;
        // } while (c != '\n');
        // //Once index is found, copy the file name into userInput->fileName
        // for (int l = 0, i = count; i < (int)strlen(userInput->userString) - 1; l++, i++) {
        //   userInput->fileName = (char*) realloc(userInput->fileName, (l + 1) * sizeof(char));
        //   userInput->fileName[l] = userInput->userString[i];
        // }
        userInput->fileName = (char*) realloc(userInput->fileName, strlen(s) * sizeof(char));
        strcpy(userInput->fileName, s);
        isValidChar = true;
      }
        break;

      default:
        printf("Unrecognized command. Type h for help.\n");
      }
    }

  if (!isValidChar) {
    userInput->userKey = '-';
  }
  return userInput;
}
/*
 * @desc: Checks if input is invalid (specifically the arguments following the command)
 * @numArgsRead : The number of variables successfully assigned by sscanf
 * @*userInput : Pointer to UserInput struct
 * @*canvas : Pointer to canvas
 * @return TRUE if input is invalid, FALSE if it is valid
 */
bool isInputImproper(int numArgsRead, UserInput* userInput, Canvas* canvas) {
  switch(userInput->userString[0]){
    case 'w':
      // draw command should have 5 arguments total, with
      // start_row, start_col, end_row, end_col being within canvas dimensions
      return (numArgsRead != 5 || userInput->intArgs[0] < 0 || userInput->intArgs[0] > canvas->row - 1
        || userInput->intArgs[1] < 0 || userInput->intArgs[1] > canvas->col - 1
        || userInput->intArgs[2] < 0 || userInput->intArgs[2] > canvas->row - 1
        || userInput->intArgs[3] < 0 || userInput->intArgs[3] > canvas->col - 1);
    case 'r':
      // resize command should have 3 arguments, with smallest board size being 1x1
      return (numArgsRead != 3 || userInput->intArgs[0] < 1 || userInput->intArgs[1] < 1);
    case 'a':
      // add command should have 3 arguments, with second char being either 'r' or 'c'
      // and the third number within canvas dimensions
      return (numArgsRead != 3 || isdigit(userInput->charArg) ||
          (userInput->charArg != 'r' && userInput->charArg != 'c') || userInput->intArgs[0] < 0
          || (userInput->charArg == 'r' && userInput->intArgs[0] > canvas->row - 1)
          || (userInput->charArg == 'c' && userInput->intArgs[0] > canvas-> col - 1));
    case 'd':
      // delete command should have 3 arguments, with second char being either 'r' or 'c'
      // and the third number within canvas dimensions
      return (numArgsRead != 3 || isdigit(userInput->charArg) ||
          (userInput->charArg != 'r' && userInput->charArg != 'c') || userInput->intArgs[0] < 0
          || (userInput->charArg == 'r' && userInput->intArgs[0] > canvas-> row - 1)
          || (userInput->charArg == 'c' && userInput->intArgs[0] > canvas-> col - 1));
    case 'e':
      // erase command should have 3 arguments, with row and col being within canvas dimensions
      return (numArgsRead != 3 || userInput->intArgs[0] < 0 || userInput->intArgs[0] > canvas-> row - 1
          || userInput->intArgs[1] < 0 || userInput->intArgs[1] > canvas-> col - 1);
    default:
      return false;
  }
}

/*
 * @desc: Reorders coordinates entered by user if in wrong order
 * @*userInput : Pointer to UserInput struct
 * @lineType : Type of line (vertical, horizontal, right and left diagonal)
 */
void orderIntArgs(UserInput* userInput, enum LineType lineType) {
  //If diagonal or horizontal line is being drawn, order from left to right
  if (lineType == D1 || lineType == D2 || lineType == H) {
    if (userInput->intArgs[1] > userInput->intArgs[3]) {
      int temp = userInput->intArgs[1];
      userInput->intArgs[1] = userInput->intArgs[3];
      userInput->intArgs[3] = temp;

      temp = userInput->intArgs[0];
      userInput->intArgs[0] = userInput->intArgs[2];
      userInput->intArgs[2] = temp;
    }
  }
  //For vertical lines, order from bottom to top
  else {
    if (userInput->intArgs[0] > userInput->intArgs[2]) {
      int temp = userInput->intArgs[0];
      userInput->intArgs[0] = userInput->intArgs[2];
      userInput->intArgs[2] = temp;

      temp = userInput->intArgs[1];
      userInput->intArgs[1] = userInput->intArgs[3];
      userInput->intArgs[3] = temp;
    }
  }
}
/*
 * @desc: Free dynamically allocated memory used in userInput
 * @*userInput : Pointer to UserInput struct
 */
void freeUserInput(UserInput* userInput) {
  free(userInput->userString);
  userInput->userString = NULL;
  free(userInput->intArgs);
  userInput->intArgs = NULL;
  free(userInput->fileName);
  userInput->fileName = NULL;
}
