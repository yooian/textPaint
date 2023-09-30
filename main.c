/*
 * 12/2/22
 * Angelina Loh and Ian Yoo
 *
 * This program will implement a text based version of paint.
 * This program will:
 * 1. Show the user the current canvas
 * 2. Allow the user to enter a command
 * 3. Execute the command
 * 4. Repeat until the user enters a quit command
 */

#include <stdio.h>
#include <stdlib.h>

#include "userInput.h"
#include "paint.h"
#include "canvas.h"

int main(int argc, char *argv[]) {
  Canvas* canvas;
  UserInput* userInput;
  bool tenByTen = false;
  // Validate command line args
  if (argc > 1 && argc != 3) {
    printf("Wrong number of command line arguments entered.\n");
    printf("Usage: ./paint.out [num_rows num_cols]\n");
    printf("Making default board of 10 X 10.\n");
    tenByTen = true;
  }
  else if (argc == 3) {
    if (atoi(argv[1]) > 0 && atoi(argv[2]) > 0) {
    }
    else {
      if (!isNumber(argv[1])) {
        printf("The number of rows is not an integer.\n");
        tenByTen = true;
      }
      else if (atoi(argv[1]) < 1) {
        printf("The number of rows is less than 1.\n");
        tenByTen = true;
        printf("Making default board of 10 X 10.\n");
      }
      if (!isNumber(argv[2])) {
        printf("The number of columns is not an integer.\n");
        tenByTen = true;
      }
      else if (atoi(argv[2]) < 1) {
        printf("The number of columns is less than 1.\n");
        tenByTen = true;
        printf("Making default board of 10 X 10.\n");
      }
    }
  }
  else {
    tenByTen = true;
  }
  if (tenByTen) {
    canvas = createCanvas(10, 10);
  }
  else {
    canvas = createCanvas(atoi(argv[1]), atoi(argv[2]));
  }


  do {
    if (tenByTen) {
      for (int i = 0; i < canvas->row; i++) {
        printf("%d  ", canvas->row - 1 - i);
        for (int j = 0; j < canvas->col; j++) {
          printf("%c  ", canvas->canvas[i][j]);
        }
        printf("\n");
      }
      printf("   ");
      for (int i = 0; i < canvas->col; i++) {
        printf("%d  ", i);
      }
      printf("\n");
      tenByTen = false;
    }
    else {
      printCanvas(canvas);
    }
    userInput = getValidChar("Enter your command: ", canvas);

     //Executes appropriate paint command while program is running
    if (userInput->userKey == 'h') {
      print_help();
    }
    else if (userInput->userKey == 'w') {
      drawCanvas(userInput, canvas);
    }
    else if (userInput->userKey == 'e') {
      erase(userInput, canvas);
    }
    else if (userInput->userKey == 'a') {
      add(userInput, canvas);
    }
    else if (userInput->userKey == 'd') {
      delete(userInput, canvas);
    }
    else if (userInput->userKey == 'r') {
      resize(userInput, canvas);
    }
    else if (userInput->userKey == 's') {
      saveCanvas(userInput, canvas);
    }
    else if (userInput->userKey == 'l') {
      loadCanvas(userInput, canvas);
    }
    freeUserInput(userInput);
  } while (userInput->userKey != 'q');

  //Frees any dynamically allocated memory in use
  freeCanvas(canvas);

  return 0;
}
