/*
 * Includes functions for canvas creation, display, and destruction
 */

#include <stdio.h>
#include <stdlib.h>

#include "canvas.h"

/*
 * @desc: Creates a canvas struct with specified dimensions
 * @row: number of rows
 * @col: number of columns
 * @return a pointer to the created canvas
 */
Canvas* createCanvas(int row, int col) {
  Canvas* newCanvas = (Canvas*) calloc(1, sizeof(Canvas));
  newCanvas->row = row;
  newCanvas->col = col;
  newCanvas->canvas = (char**) calloc(row, sizeof(char*));
  for (int i = 0; i < row; i++) {
    newCanvas->canvas[i] = (char*) calloc(col, sizeof(char));
  }
  //Initialize canvas with '*'
  newCanvas = fillCanvas(newCanvas);
  return newCanvas;
}
/*
 * @desc: Initializes canvas with blank spaces represented by '*'
 * @*canvas: pointer to canvas struct
 * @return pointer to canvas structs
 */
Canvas* fillCanvas(Canvas* canvas) {
  for (int i = 0; i < canvas->row; i++) {
    for (int j = 0; j < canvas->col; j++) {
      canvas->canvas[i][j] = '*';
    }
  }
  return canvas;
}
/*
 * @desc: Displays canvas with numbered rows and cols
 * @*canvas: pointer to canvas struct
 */
void printCanvas(Canvas* canvas) {
  for (int i = 0; i < canvas->row; i++) {
    printf("%d ", canvas->row - 1 - i);
    for (int j = 0; j < canvas->col; j++) {
      printf("%c ", canvas->canvas[i][j]);
    }
    printf("\n");
  }
  printf("  ");
  for (int i = 0; i < canvas->col; i++) {
    printf("%d ", i);
  }
  printf("\n");
}
/*
 * @desc: Frees dyanmically allocated memory used to store the canvas
 * @*canvas: pointer to canvas struct
 */
void freeCanvas(Canvas* canvas) {
  for (int i = 0; i < canvas->row; i++) {
		free(canvas->canvas[i]);
	}
	free(canvas->canvas);
	canvas->canvas = NULL;
}
