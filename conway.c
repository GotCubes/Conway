#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "conway.h"

int main(int argc, char **argv)
{
	assert(argc == 2);

	// Generate boards.
	int size, i;
	char **board = readInput(argv[1], &size);
	char **nextBoard = malloc(size * sizeof(char*));
	assert(nextBoard != NULL);
	for(i = 0; i < size; i++)
	{
		nextBoard[i] = malloc(size * sizeof(char));
		assert(nextBoard[i] != NULL);
	}

	// Preprocess board.
	printBoard(board, size);
	delay(1000);

	// Calculate generations.
	for(i = 0; i < 500; i++)
	{
		newGeneration(board, nextBoard, size);
		system("clear");
		printBoard(board, size);
		printf("Generation: %d\n", i + 1);
		delay(75);
	}

	freeBoard(board, size);
	freeBoard(nextBoard, size);
	return EXIT_SUCCESS;
}

void delay(int milliseconds)
{
	long pause;
	clock_t now, then;

	pause = milliseconds * CLOCKS_PER_SEC / 1000;
	now = then = clock();
	while((now - then) < pause)
		now = clock();
}

char** readInput(char *filename, int *size)
{
	FILE *fptr = fopen(filename, "r");
	assert(fptr != NULL);

	// Get board dimensions.
	char cell;
	do
	{
		cell = fgetc(fptr);
	}while(cell != '\n');

	*size = ftell(fptr) - 1;
	printf("Board size: %dx%d cells.\n", *size, *size);
	fseek(fptr, 0, SEEK_SET);

	// Malloc board.
	int i, j;
	char **board = malloc(*size * sizeof(char*));
	assert(board != NULL);
	for(i = 0; i < *size; i++)
	{
		board[i] = malloc(*size * sizeof(char));
		assert(board[i] != NULL);
	}

	// Read file into board.
	for(i = 0; i < *size; i++)
	{
		for(j = 0; j < *size; j++)
		{
			cell = fgetc(fptr);
			if(cell == '0')
				board[i][j] = ' ';
			else if(cell == '1')
				board[i][j] = '*';
			else
				return NULL;
		}
		fgetc(fptr); // Buffer cycle '\n'.
	}

	fclose(fptr);
	return board;
}

void printBoard(char **board, int size)
{
	int i,j;

	// Top border.
	printf("+");	
	for(i = 0; i < size; i++)
		printf("--");
	printf("-+\n");

	for(i = 0; i < size; i++)
	{
		printf("| ");
		for(j = 0; j < size; j++)
			printf("%c ", board[i][j]);		
		printf("|\n");
	}

	// Bottom border.
	printf("+");	
	for(i = 0; i < size; i++)
		printf("--");
	printf("-+\n");
}

void newGeneration(char **board, char **nextBoard, int size)
{
	int i,j;
	int liveNeighbors;
	// Determine next states for each cell.
	for(i = 0; i < size; i++)
	{
		for(j = 0; j < size; j++)
		{
			liveNeighbors = getNeighbors(board, i, j, size);
			if(board[i][j] == '*')
			{
				if(liveNeighbors < 2) // Underpopulation.
					nextBoard[i][j] = ' ';
				if(liveNeighbors == 2 || liveNeighbors == 3) // Satisfaction.
					nextBoard[i][j] = '*';
				if(liveNeighbors > 3) // Overpopulation.
					nextBoard[i][j] = ' ';
			}else if(board[i][j] == ' ')
			{
				if(liveNeighbors == 3)
					nextBoard[i][j] = '*'; // Reproduction.
				else
					nextBoard[i][j] = ' ';
			}
		}
	}

	// Copy data over to board.
	for(i = 0; i < size; i++)
	{
		for(j = 0; j < size; j++)
		{
			board[i][j] = nextBoard[i][j];
		}
	}
}

int getNeighbors(char **board, int i, int j, int size)
{
	int imod, jmod;
	int count = 0;

	for(imod = -1; imod <= 1; imod++)
	{
		for(jmod = -1; jmod <= 1; jmod++)
		{
			if(imod != 0 || jmod != 0) // Do not count parent cell.
			{
				if((i + imod >= 0) && (i + imod < size) && (j + jmod >= 0) && (j + jmod < size))
				{
					if(board[i + imod][j + jmod] == '*')
						count++;
				}
			}
		}
	}

	return count;
}

void freeBoard(char **board, int size)
{
	int i;
	for(i = 0; i < size; i++)
		free(board[i]);
	free(board);
}
