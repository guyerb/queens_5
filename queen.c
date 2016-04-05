#include <stdio.h>
#include <stdlib.h>

char board[8][8];

#define QUEEN 'Q'
#define NUMQUEENS 6

#define VALID_ROW(x) (x >= 0 && x <= 7)
#define VALID_COL(y) (y >= 0 && y <= 7)

int check_diag(int row, int col, int x, int y)
{
	int count = 0;

	row = row + x;
	col = col + y;

	if (VALID_ROW(row) && VALID_COL(col)) {
		if (board[row][col] == QUEEN)
			count++;
		count += check_diag(row, col, x, y);
	}

	return count;
}

int check_diags(int row, int col)
{
	int count = 0;

	count += check_diag(row, col, -1, -1);
	count += check_diag(row, col, -1, +1);
	count += check_diag(row, col, +1, +1);
	count += check_diag(row, col, +1, -1);

	return count;
}

int count_board(void)
{
	int i, j;
	int queens = 0;

	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			if (board[i][j] == QUEEN) {
//				printf("q[%d][%d]\n", i, j);
				queens++;
			}
		}
	}
	return queens;
}

void init_board(char ch)
{
	int i,j;

	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
			board[i][j] = ch;
}

int place_five(void)
{
	int i,j;
	int row, col;
	int count;
	
	init_board('.');

	for (i=0; i<NUMQUEENS; i++) {
		row = rand() % 8;
		col = rand() % 8;

		//	printf("q%d: %d,%d\n", i, row, col);

		board[row][col] = QUEEN;
	}

	/* if queens on top of each other then fail */
	if ((count = count_board()) != NUMQUEENS) {
//		printf("queens was %d: fail!\n", count);
		return 0;
	}

	/* if queens in same row or col then fail */
	for (i=0; i<8; i++) {

		col=0; row=0;
		for (j=0; j<8; j++) {
			if (board[i][j] == QUEEN)
				row++;
			if (board[j][i] == QUEEN)
				col++;
		}
		if (row > 1) {
//			printf("%d queens in row %d, fail!\n", row, i);
			return 0;
		}
		if (col > 1) {
//			printf("%d queens in col %d, fail!\n", col, i);
			return 0;
		}
	}

	/* if queens in same diagnol then fail */
	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			if (board[i][j] == QUEEN) {
				count = 1;
				count += check_diags(i, j);
				if (count > 1) {
//					printf("%d queens in diag of q[%d][%d], fail!\n", count, i,j);
					return 0;
				}
			}
		}
	}

	/* okay, now check that each square is covered by a row, col, or diag */
	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			if (board[i][j] != QUEEN) {
				int k;

				/* check row and col */
				for (k=0; k<8; k++) {
					if (board[i][k] == QUEEN) {
//						printf("row %d not covered, fail!\n", i);
						return 0;
					}
					if (board[k][j] == QUEEN) {
//						printf("col %d not covered, fail!\n", j);
				       		return 0;
					}
				}

				/* check that diags are covered */
				count = check_diags(i, j);
				if (count == 0) {
				  //printf("%d no queens in diag of q[%d][%d], fail!\n", count, i,j);
					//	return 0;
				}
			}
		}
	}

	return 1;
}

void draw_board(void)
{
	int i,j;
	
	printf("\t");
	for (j=0; j<8; j++)
		printf(" ___");
	printf("\n");

	for (i=0; i<8; i++) {
		printf("\t");
		for (j=0; j<8; j++)
			printf("| %c ", board[i][j]);
		printf("|\n");

		printf("\t");
		for (j=0; j<8; j++)
			printf(" ___");
		printf("\n");
	}
}


/* 
 * find positions for five queens such that all positions on board are
 * covered and no queen threatens another queen.
 *
 *  place a queen
 *      no queen already there.
 *      row is free of queen
 *      column is free of queen
 *      diagnols are free of queens
 *  five queens placed? done
 */

int main(int argc, char *argv[])
{
	int success = 0;
	int iterations = 0;

	sranddev();

	init_board(' ');
	draw_board();

	while (!success) {
		iterations++;
		success = place_five();
	}
	
	printf("\n\niterations = %d\n", iterations);
	draw_board();

	return 0;
}
