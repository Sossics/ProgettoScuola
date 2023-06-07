#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#define EMPTY_P 0
#define WHITE_P 1
#define BLACK_P 2

int count_pieces(int turn, int board[8][8]) {

	int p = 0, i = 0, j = 0;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board[i][j] % 3 == turn) {
				p++;
			}
		}
	}

	return p;

}

int is_valid_move(int player, int x1, int y1, int x2, int y2, int board[8][8]) {

	if (((board[x1][y1]) % 3) != player) {
		return 0;
	}

	if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 || x1>7 || y1>7 || x2>7 || y2>7) {
		return 0;
	}

	if (x1 == x2 || y1 == y2) {
		return 0;
	}

	if (board[x2][y2] != 0) {
		return 0;
	}

	if (((x2 * 8) + y2 + (x2 % 2)) % 2 == 0) {
		return 0;
	}

	if ((board[x1][y1]) == 1 && x2 < x1) {
		return 0;
	}

	if ((board[x1][y1]) == 2 && x2 > x1) {
		return 0;
	}

	if ((x1 - x2) == -2 || (x1 - x2) == 2) {
		if (player == WHITE_P) {
			int middle_x = (x1 + x2) / 2;
			int middle_y = (y1 + y2) / 2;
			if (board[middle_x][middle_y] == 5 && board[x1][y1] == 1) {
				return 0;
			}
			if (board[middle_x][middle_y] % 3 == BLACK_P) {
				return 1;
			}
		}
		else if (player == BLACK_P) {
			int middle_x = (x1 + x2) / 2;
			int middle_y = (y1 + y2) / 2;
			if (board[middle_x][middle_y] == 4 && board[x1][y1] == 2) {
				return 0;
			}
			if (board[middle_x][middle_y] % 3 == WHITE_P) {
				return 1;
			}
		}

		return 0;
	}
	else if((x1-x2)>2 || (x1 - x2) < -2){
		return 0;
	}

	return 1;
}

void make_move(int player, int x1, int y1, int x2, int y2, int board[8][8]) {

	if (x2 == 7 && player == 1) {
		board[x2][y2] = 4;
	}
	else if (x2 == 0 && player == 2) {
		board[x2][y2] = 5;
	}
	else if (board[x1][y1] == 5 && player == 2) {
		board[x2][y2] = 5;
	}
	else if (board[x1][y1] == 4 && player == 1) {
		board[x2][y2] = 4;
	}
	else {
		board[x2][y2] = player;
	}

	if ((x1 - x2) == -2 || (x1 - x2) == 2) {
		int middle_x = (x1 + x2) / 2;
		int middle_y = (y1 + y2) / 2;
		board[middle_x][middle_y] = EMPTY_P;
	}

	board[x1][y1] = EMPTY_P;

}

int aggiorna_b(int board[8][8], int turn) {

	srand(time(NULL));

	int x1, y1, x2, y2, i = 0, j = 0, ok = 0, ancora = 0, blocco = 0;

	ok = 0, blocco = 0;

	while (ok == 0 && blocco < 1000) {

		blocco++;

		for (i = 0; i < 8 && ok == 0; i++) {
			for (j = 0; j < 8 && ok == 0; j++) {

				if (board[i][j] == 5) {

					x1 = i;
					y1 = j;

					if ((is_valid_move(BLACK_P, x1, y1, x1 + 1, y1 + 1, board)) == 1 && board[(x1 + x1 + 1) / 2][(y1 + y1 + 1) / 2] == 4) {
						y2 = (y1 + 1);
						x2 = x1 + 1;
						ok = 1;
					}
					else if ((is_valid_move(BLACK_P, x1, y1, x1 + 1, y1 + 1, board)) == 1 && board[(x1 + x1 + 1) / 2][(y1 + y1 - 1) / 2] == 4) {
						y2 = (y1 + 1);
						x2 = x1 + 1;
						ok = 1;
					}
					else if ((is_valid_move(BLACK_P, x1, y1, x1 - 1, y1 + 1, board)) == 1 && board[(x1 + x1 - 1) / 2][(y1 + y1 + 1) / 2] == 4) {
						y2 = (y1 + 1);
						x2 = x1 + 1;
						ok = 1;
					}
					else if ((is_valid_move(BLACK_P, x1, y1, x1 - 1, y1 + 1, board)) == 1 && board[(x1 + x1 - 1) / 2][(y1 + y1 - 1) / 2] == 4) {
						y2 = (y1 + 1);
						x2 = x1 + 1;
						ok = 1;
					}
					else if ((is_valid_move(BLACK_P, x1, y1, x1 + 2, y1 + 2, board)) == 1 && (rand() % 2) != 1) {
						y2 = (y1 + 2);
						x2 = x1 + 2;
						ok = 1;
					}
					else if ((is_valid_move(BLACK_P, x1, y1, x1 + 2, y1 - 2, board)) == 1 && (rand() % 2) != 1) {
						y2 = (y1 - 2);
						x2 = x1 + 2;
						ok = 1;
					}
					else if ((is_valid_move(BLACK_P, x1, y1, x1 - 2, y1 + 2, board)) == 1 && (rand() % 2) != 1) {
						y2 = (y1 + 2);
						x2 = x1 - 2;
						ok = 1;
					}
					else if ((is_valid_move(BLACK_P, x1, y1, x1 - 2, y1 - 2, board)) == 1 && (rand() % 2) != 1) {
						y2 = (y1 - 2);
						x2 = x1 - 2;
						ok = 1;
					}
					else {

					}

				}
				else if ((board[i][j]) == 2) {

					x1 = i;
					y1 = j;

					if ((is_valid_move(BLACK_P, x1, y1, x1 - 2, y1 + 2, board)) == 1 && (rand() % 2 + 1) != 2) {
						y2 = (y1 + 2);
						x2 = x1 - 2;
						ok = 1;
					}
					else if ((is_valid_move(BLACK_P, x1, y1, x1 - 2, y1 - 2, board)) == 1 && (rand() % 2 + 1) != 2) {
						y2 = (y1 - 2);
						x2 = x1 - 2;
						ok = 1;
					}
					else {

					}
				}
			}
		}

		for (i = 0; i < 8 && ok == 0; i++) {
			for (j = 0; j < 8 && ok == 0; j++) {
				if (board[i][j] == 5) {

					x1 = i;
					y1 = j;

					if ((is_valid_move(BLACK_P, x1, y1, x1 + 1, y1 + 1, board)) == 1 && (rand() % 2) == 1) {
						y2 = (y1 + 1);
						x2 = x1 + 1;
						ok = 1;
					}
					else if ((is_valid_move(BLACK_P, x1, y1, x1 + 1, y1 - 1, board)) == 1 && (rand() % 2) == 1) {
						y2 = (y1 - 1);
						x2 = x1 + 1;
						ok = 1;
					}
					else if ((is_valid_move(BLACK_P, x1, y1, x1 - 1, y1 + 1, board)) == 1 && (rand() % 2) == 1) {
						y2 = (y1 + 1);
						x2 = x1 - 1;
						ok = 1;
					}
					else if ((is_valid_move(BLACK_P, x1, y1, x1 - 1, y1 - 1, board)) == 1 && (rand() % 2) == 1) {
						y2 = (y1 - 1);
						x2 = x1 - 1;
						ok = 1;
					}
					else {

					}

				}
				else if ((board[i][j]) == 2) {

					x1 = i;
					y1 = j;

					if ((is_valid_move(BLACK_P, x1, y1, x1 - 1, y1 + 1, board)) == 1 && (rand() % 2) != 1) {
						y2 = (y1 + 1);
						x2 = x1 - 1;
						ok = 1;
					}
					else if ((is_valid_move(BLACK_P, x1, y1, x1 - 1, y1 - 1, board)) == 1 && (rand() % 2) != 1) {
						y2 = (y1 - 1);
						x2 = x1 - 1;
						ok = 1;
					}
					else {

					}
				}
			}
		}

	}

	if (blocco >= 1000) {
		x1 = 0; y1 = 0;
		make_move(turn, x1, y1, x1, y1, board);
		return 1;
	}
	else {
		make_move(turn, x1, y1, x2, y2, board); //fai il movimento

		if ((y1 - y2) == 2 || (y1 - y2) == -2) {

			for (ancora = 0; ancora == 0;) {
				ancora = 1;
				if (board[x2][y2] == 5) {
					if ((is_valid_move(turn, x2, y2, x2 + 2, y2 - 2, board)) == 1) {
						make_move(turn, x2, y2, x2 + 2, y2 - 2, board);
						ancora = 0;
					}
					else if ((is_valid_move(turn, x2, y2, x2 + 2, y2 + 2, board)) == 1) {
						make_move(turn, x2, y2, x2 + 2, y2 + 2, board);
						ancora = 0;
					}
				}
				else {
					if ((is_valid_move(turn, x2, y2, x2 - 2, y2 - 2, board)) == 1) {
						make_move(turn, x2, y2, x2 - 2, y2 - 2, board);
						ancora = 0;
					}
					else if ((is_valid_move(turn, x2, y2, x2 - 2, y2 + 2, board)) == 1) {
						make_move(turn, x2, y2, x2 - 2, y2 + 2, board);
						ancora = 0;
					}
				}
			}
		}

		return 0;

	}

}


int aggiorna_p(int board[8][8], int x1, int y1, int x2, int y2, int turn) {

	int ancora = 0;

	if ((is_valid_move(turn, x1, y1, x2, y2, board)) == 1) { //se il movimento è valido

		make_move(turn, x1, y1, x2, y2, board); //fai il movimento

		if ((y1 - y2) == 2 || (y1 - y2) == -2) {

			for (ancora = 0; ancora == 0;) {

				ancora = 1;

				if (turn == WHITE_P) {

					if (board[x2][y2] == 4) {

						if ((is_valid_move(turn, x2, y2, x2 - 2, y2 - 2, board)) == 1) {

							make_move(turn, x2, y2, x2 - 2, y2 - 2, board);
							ancora = 0;

						}
						else if ((is_valid_move(turn, x2, y2, x2 - 2, y2 + 2, board)) == 1) {

							make_move(turn, x2, y2, x2 - 2, y2 + 2, board);
							ancora = 0;

						}

					}
					else if (board[x2][y2] % 3 == 1) {
						if ((is_valid_move(turn, x2, y2, x2 + 2, y2 - 2, board)) == 1) {

							make_move(turn, x2, y2, x2 + 2, y2 - 2, board);
							ancora = 0;

						}
						else if ((is_valid_move(turn, x2, y2, x2 + 2, y2 + 2, board)) == 1) {

							make_move(turn, x2, y2, x2 + 2, y2 + 2, board);
							ancora = 0;

						}

					}

				}
				if (turn == BLACK_P) {
					if (board[x2][y2] == 5) {

						if ((is_valid_move(turn, x2, y2, x2 + 2, y2 - 2, board)) == 1) {

							make_move(turn, x2, y2, x2 + 2, y2 - 2, board);
							ancora = 0;

						}
						else if ((is_valid_move(turn, x2, y2, x2 + 2, y2 + 2, board)) == 1) {

							make_move(turn, x2, y2, x2 + 2, y2 + 2, board);
							ancora = 0;

						}

					}
					else if (board[x2][y2] % 3 == 2) {

						if ((is_valid_move(turn, x2, y2, x2 - 2, y2 - 2, board)) == 1) {

							make_move(turn, x2, y2, x2 - 2, y2 - 2, board);
							ancora = 0;

						}
						else if ((is_valid_move(turn, x2, y2, x2 - 2, y2 + 2, board)) == 1) {
							
							make_move(turn, x2, y2, x2 - 2, y2 + 2, board);
							ancora = 0;

						}
			
					}
			
				}
			
			}

		}

		return 0;

	}

	else {

		return 1;
	
	}

}
