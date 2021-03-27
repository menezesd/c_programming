#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#define FULL 0777

typedef unsigned int bitfield;

/* side to move */
typedef enum {
    X, O, NONE = -1
} side;

/* evaluation scores */
enum { WIN = 1, LOSS = -1, DRAW = 0 };
typedef int score;


side next_player(side p);
char token(side p);
bool is_win(bitfield board);
score alpha_beta(bitfield me, bitfield opp, score achievable,
		 score cutoff, int *best);
void show_board(const bitfield players[2]);
side get_side(void);
int get_move(void);
int main(void);

/* The board is represented using two bitfields indicating the squares
   containing an X and an O.  The bits correspond to squares as shewn
   in the diagram below (0 indicates the least significant bit) */
/*  0 | 1 | 2
 * ---+---+----
 *  3 | 4 | 5
 * ---+---+----
 *  6 | 7 | 8 */

/* mask for square i */
#define SQUARE(i) (1 << i)



side next_player(side p)
{
    return (p + 1) % 2;
}

char token(side p)
{
    switch (p) {
    case X:
	return 'X';
    case O:
	return 'O';
    case NONE:
	return '?';
    }
}


/* Determine if there are three in a row.  Given an input n
   representing a bitmask for the squares occupied by a player, test
   if any of the patterns in wins is matched.  */
bool is_win(bitfield board)
{
    /* The array wins contains bitmasks corresponding to the possible
       ways to get three in a row */
    static const bitfield wins[] =
	{ 0007, 0070, 0700, 0111, 0222, 0444, 0421, 0124 };
    for (size_t i = 0; i < sizeof(wins) / sizeof(int); i++)
	if ((board & wins[i]) == wins[i])
	    return true;
    return false;
}

/* determine best move using recursive search */
/*  parameters:
 *      me:         bitfield indicating squares I occupy
 *      opp:        bitfield indicating squares opponent occupies
 *      achievable: score of best variation found so far
 *      cutoff:     if we can find a move better than this our opponent
 *                  will avoid this variation so we can stop searching
 *      best:       output parameter used to store index of square 
 *                  indicating the move with the best score
 *      return value: score of position, from perspective of player to move
*/
score
alpha_beta(bitfield me, bitfield opp, score achievable, score cutoff,
	   int *best)
{

    int bestmove = 0;

    if ((me | opp) == FULL)	/* all squares filled, draw */
	return DRAW;

    for (int i = 0; i < 9; i++) {
	score cur;
	bitfield tmp;
	if ((me | opp) & SQUARE(i))	// square filled
	    continue;
	tmp = me | SQUARE(i);	// make move
	// evaluate position after making move
	cur = is_win(tmp) ? WIN :
	    -alpha_beta(opp, tmp, -cutoff, -achievable, NULL);

	if (cur > achievable) {
	    achievable = cur;
	    bestmove = i;
	}
	if (achievable >= cutoff)
	    break;
    }
    if (best) *best = bestmove;
    return achievable;
}

/* Display the board */
void show_board(const bitfield players[2])
{

    /* 
     *     0 1 2
     *     -----
     * 0:  0 1 2
     * 1:  3 4 5
     * 2:  6 7 8
     *
     */

    for (int row = 0; row < 3; row++) {
	printf("\n+---+---+---+\n|");
	for (int col = 0; col < 3; col++) {
	    int square_idx = row * 3 + col;
	    char c = '0' + square_idx;
	    for (int j = 0; j < 2; j++)
		if (players[j] & SQUARE(square_idx))
		    c = token(j);
	    printf(" %c |", c);
	}
    }
    puts("\n+---+---+---+");
}

/* ask user which side the computer should play */
side get_side(void)
{
    side s;
    int c;

    printf("Enter AI player x/o or 2 for two player: ");
    while (isspace(c = getchar()))	/* skip whitespace */
	if (c == EOF)
	    exit(EXIT_FAILURE);

    if (c == 'X' || c == 'x')
	s = X;
    else if (c == 'O' || c == 'o' || c == '0')
	s = O;
    else
	s = NONE;

    while ((c = getchar()) != '\n')	/* ignore rest of line */
	if (c == EOF)
	    exit(EXIT_FAILURE);

    return s;
}

/* ask user where to move */
int get_move(void)
{
    char s[100];		// buffer for line input
    char *err;
    fflush(stdout);
    err = fgets(s, sizeof(s), stdin);
    if (err == NULL)
	exit(EXIT_FAILURE);
    return atoi(s);
}

int main(void)
{

    int ai;
    bool over = false;
    side cur_player = X;

    bitfield players[2] = { 0 };


    ai = get_side();

    while (!over) {
	int move;
	if (cur_player == ai) {	/* select AI move */
	    alpha_beta(players[cur_player],
		       players[next_player(cur_player)], -INT_MAX, INT_MAX,
		       &move);
	} else {
	    show_board(players);

	    printf("Player %c, make your move: ", token(cur_player));
	    move = get_move();

	    if (!(0 <= move && move < 9)) {
		puts("Invalid move.");
		continue;
	    }
	    if ((players[0] | players[1]) & SQUARE(move)) {
		puts("Square occupied; try again.");
		continue;
	    }
	}

	players[cur_player] |= SQUARE(move);

	if (is_win(players[cur_player])) {
	    show_board(players);
	    printf("%c wins\n", token(cur_player));
	    over = true;
	} else if ((players[0] | players[1]) == FULL) {
	    puts("Draw");
	    over = true;
	}
	cur_player = next_player(cur_player);
    }

    return EXIT_SUCCESS;
}

