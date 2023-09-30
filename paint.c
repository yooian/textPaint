/*
 * Includes functions for canvas paint commands
 */
#include <stdio.h>

#include "paint.h"
#include "canvas.h"
#include "userInput.h"

/*
 * @desc: Displays the help information
 */
void print_help() {
  printf("Commands:\n");
  printf("Help: h\n");
  printf("Quit: q\n");
  printf("Draw line: w row_start col_start row_end col_end\n");
  printf("Resize: r num_rows num_cols\n");
  printf("Add row or column: a [r | c] pos\n");
  printf("Delete row or column: d [r | c] pos\n");
  printf("Erase: e row col\n");
  printf("Save: s file_name\n");
  printf("Load: l file_name\n");
}

/*
 * @desc: Draws a line from start_row, start_column to end_row, end_column
 * @*userInput: pointer to struct holding user's input
 * @*canvas: pointer to canvas struct
 */
void drawCanvas(UserInput* userInput, Canvas* canvas) {
  enum LineType lineType;
  //Input validation and determination of line type
  if (!isValidLine(userInput, &lineType)) {
    printf("Improper draw command.\n");
    return;
  }

  //Reorder coordinates if necessary
  orderIntArgs(userInput, lineType);

  //Draw appropriate line with respective line character, or '+' if intersection
  switch (lineType) {
    case S:
      if (isStarOrSame(canvas, (canvas->row - 1) - userInput->intArgs[0], userInput->intArgs[1], '-')) {
        canvas->canvas[(canvas->row - 1) - userInput->intArgs[0]][userInput->intArgs[1]] = '-';
      }
      else {
        canvas->canvas[(canvas->row - 1) - userInput->intArgs[0]][userInput->intArgs[1]] = '+';
      }
      break;

    case H:
      for (int i = userInput->intArgs[1]; i <= userInput->intArgs[3]; i++) {
        if (isStarOrSame(canvas, (canvas->row - 1) - userInput->intArgs[0], i, '-')) {
          canvas->canvas[(canvas->row - 1) - userInput->intArgs[0]][i] = '-';
        }
        else {
          canvas->canvas[(canvas->row - 1) - userInput->intArgs[0]][i] = '+';
        }
      }
      break;

    case V:
      for (int i = (canvas->row - 1) - userInput->intArgs[0]; i >= (canvas->row - 1) - userInput->intArgs[2]; i--) {
        if (isStarOrSame(canvas, i, userInput->intArgs[1], '|')) {
          canvas->canvas[i][userInput->intArgs[1]] = '|';
        }
        else {
          canvas->canvas[i][userInput->intArgs[1]] = '+';
        }
      }
      break;

    case D1:
      for (int i = (canvas->row - 1) - userInput->intArgs[0], j = userInput->intArgs[1];
            i >= (canvas->row - 1) - userInput->intArgs[2] && j <= userInput->intArgs[3]; i--, j++) {
            if (isStarOrSame(canvas, i, j, '/')) {
              canvas->canvas[i][j] = '/';
            }
            else {
              canvas->canvas[i][j] = '+';
            }
      }
      break;

    case D2:
      for (int i = (canvas->row - 1) - userInput->intArgs[0], j = userInput->intArgs[1];
            i <= (canvas->row - 1) - userInput->intArgs[2] && j <= userInput->intArgs[3]; i++, j++) {
            if (isStarOrSame(canvas, i, j, '\\')) {
              canvas->canvas[i][j] = '\\';
            }
            else {
              canvas->canvas[i][j] = '+';
            }
      }
      break;

    default:
      return;
  }
}

/*
 * @desc: Erases the character at specified position, reverting it back to a blank space represented by a '*'
 * @*userInput: pointer to struct holding user's input
 * @*canvas: pointer to canvas struct
 */
void erase(UserInput* userInput, Canvas* canvas) {
  canvas->canvas[(canvas->row - 1) - userInput->intArgs[0]][userInput->intArgs[1]] = '*';
}

/*
 * @desc: Adds either a new row or column (choice specified by user) at the specified position
 * @*userInput: pointer to struct holding user's input
 * @*canvas: pointer to canvas struct
 */
void add(UserInput* userInput, Canvas* canvas) {
  //If row is being added, reallocate memory and shift all rows down of
  // specified position, initialize new row with '*', update number of rows
  if (userInput->charArg == 'r') {
    canvas->row++;
    canvas->canvas = (char**) realloc(canvas->canvas, canvas->row * sizeof(char*));
    canvas->canvas[canvas->row - 1] = (char*) calloc(canvas->col, sizeof(char));

    for (int i = (canvas->row - 1); i > (canvas->row - 1) - userInput->intArgs[0]; i--) {
      for (int j = 0; j < canvas->col; j++) {
        canvas->canvas[i][j] = canvas->canvas[i - 1][j];
      }
    }
    for (int i = 0; i < canvas->col; i++) {
      canvas->canvas[(canvas->row - 1) - userInput->intArgs[0]][i] = '*';
    }
  }
  //If column is being added, reallocate memory and shift all columns right of
  // specificed position, initialize new column with '*', update number of columns
  else if (userInput->charArg == 'c') {
    canvas->col++;
    for (int i = 0; i < canvas->row; i++) {
      canvas->canvas[i] = (char*) realloc(canvas->canvas[i], canvas->col * sizeof(char));
      for (int j = (canvas->col - 1); j > userInput->intArgs[0]; j--) {
        canvas->canvas[i][j] = canvas->canvas[i][j - 1];
      }
      canvas->canvas[i][userInput->intArgs[0]] = '*';
    }
  }
}

/*
 * @desc: Deletes either a row or column (choice specified by user) at the specified position
 * @*userInput: pointer to struct holding user's input
 * @*canvas: pointer to canvas struct
 */
void delete(UserInput* userInput, Canvas* canvas) {
  //If row is being deleted, shift all rows below deleted row up, reallocate
  // memory, update number of rows
  if (userInput->charArg == 'r') {
    for (int i = (canvas->row - 1) - userInput->intArgs[0]; i < (canvas->row - 1); i++) {
      for (int j = 0; j < canvas->col; j++) {
        canvas->canvas[i][j] = canvas->canvas[i + 1][j];
      }
    }
    canvas->row--;
    canvas->canvas = (char**) realloc(canvas->canvas, canvas->row * sizeof(char*));
  }
  //If column is being deleted, shift all columns to the right of the deleted column to the left, reallocate
  // memory, update number of columns
  else if (userInput->charArg == 'c') {
    for (int i = 0; i < canvas->row; i++) {
      for (int j = userInput->intArgs[0]; j < (canvas->col - 1); j++) {
        canvas->canvas[i][j] = canvas->canvas[i][j + 1];
      }
      canvas->canvas[i] = (char*) realloc(canvas->canvas[i], (canvas->col - 1) * sizeof(char));
    }
    canvas->col--;
  }
}

/*
 * @desc: Resizes the canvas to be dimensions specified by user
 * @*userInput: pointer to struct holding user's input
 * @*canvas: pointer to canvas struct
 */
void resize(UserInput* userInput, Canvas* canvas) {
  //If shrinking column size, destroy all columns beyond new dimension size with realloc
  //Update number of columns
  if (userInput->intArgs[1] < canvas->col) {
    for (int i = 0; i < canvas->row; i++) {
      canvas->canvas[i] = (char*) realloc(canvas->canvas[i], userInput->intArgs[1] * sizeof(char));
    }
    canvas->col = userInput->intArgs[1];
  }
  //If increasing column size, reallocate memory to insert new column(s) and initialize them with '*'
  //Update number of columns
  else if (userInput->intArgs[1] > canvas->col) {
    for (int i = 0; i < canvas->row; i++) {
      canvas->canvas[i] = (char*) realloc(canvas->canvas[i], userInput->intArgs[1] * sizeof(char));
      for (int j = canvas->col; j < userInput->intArgs[1]; j++) {
        canvas->canvas[i][j] = '*';
      }
    }
    canvas->col = userInput->intArgs[1];
  }
  //If shrinking row size, shift all rows until specified point up, then reallocate memory
  //Update number of rows
  if (userInput->intArgs[0] < canvas->row) {
    for (int l = 0, i = (canvas->row) - userInput->intArgs[0]; i <= (canvas->row - 1); l++, i++) {
      for (int j = 0; j < canvas->col; j++) {
        canvas->canvas[l][j] = canvas->canvas[i][j];
      }
    }
    canvas->row = userInput->intArgs[0];
    canvas->canvas = (char**) realloc(canvas->canvas, canvas->row * sizeof(char*));
  }
  //If increasing row size, reallocate memory, shift all current rows down
  //Update number of rows
  if (userInput->intArgs[0] > canvas->row) {
    canvas->canvas = (char**) realloc(canvas->canvas, userInput->intArgs[0] * sizeof(char*));
    for (int i = userInput->intArgs[0] - 1; i > canvas->row - 1; i--) {
      canvas->canvas[i] = (char*) calloc(canvas->col, sizeof(char));
    }

    for (int l = (canvas->row - 1), i = (userInput->intArgs[0] - 1); l >= 0; i--, l--) {
      for (int j = 0; j < canvas->col; j++) {
        canvas->canvas[i][j] = canvas->canvas[l][j];
      }
    }
    for (int i = 0; i < userInput->intArgs[0] - canvas->row; i++) {
      for (int j = 0; j < canvas->col; j++) {
        canvas->canvas[i][j] = '*';
      }
    }
    canvas->row = userInput->intArgs[0];
  }
}
/*
 * @desc: Saves the current canvas to the specified file with the first value of
          the file being the row count, the second being the column count, and
          the rest being the canvas contents
 * @*userInput: pointer to struct holding user's input
 * @*canvas: pointer to canvas struct
 */
void saveCanvas(UserInput* userInput, Canvas* canvas) {
  //Open file
  FILE* file = NULL;

  file = fopen(userInput->fileName, "w");

  //Input validation
  if (file == NULL) {
    printf("Improper save command or file could not be created.\n");
    return;
  }

  //Saves first value of file as row size, second value as column size
  fprintf(file, "%d %d\n", canvas->row, canvas->col);

  //Saves canvas contents into file
  for (int i = 0; i < canvas->row; i++) {
    for (int j = 0; j < canvas->col; j++) {
      fprintf(file, "%c ", canvas->canvas[i][j]);
    }
    fprintf(file, "\n");
  }
  //Close file
  fclose(file);
  return;
}
/*
 * @desc: Loads a canvas from the specified file in the same format used in saveCanvas
 * @*userInput: Pointer to struct holding user's input
 * @*canvas: Pointer to canvas struct
 */
void loadCanvas(UserInput* userInput, Canvas* canvas) {
  //Open file
  FILE* file = NULL;

  file = fopen(userInput->fileName, "r");

  //Input validation
  if (file == NULL) {
    printf("Improper load command or file could not be created.\n");
    return;
  }
  freeCanvas(canvas);

  //Reads row and column into canvas struct
  fscanf(file, "%d %d\n", &canvas->row, &canvas->col);

  canvas->canvas = (char**) calloc(canvas->row, sizeof(char*));
  for (int i = 0; i < canvas->row; i++) {
    canvas->canvas[i] = (char*) calloc(canvas->col, sizeof(char));
  }

  //Reads canvas contents into canvas struct
  for (int i = 0; i < canvas->row; i++) {
    for (int j = 0; j < canvas->col; j++) {
      fscanf(file, "%c ", &canvas->canvas[i][j]);
    }
    fscanf(file, "\n");
  }

  //Close file
  fclose(file);
  return;
}
