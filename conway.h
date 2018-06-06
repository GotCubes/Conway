#ifndef __conway_h_
#define __conway_h_

char** readInput(char *filename, int *size);
void delay(int milliseconds);
void printBoard(char **board, int size);
void newGeneration(char **board, char **nextBoard, int size);
int getNeighbors(char **board, int i, int j, int size);
void freeBoard(char **board, int size);
#endif
