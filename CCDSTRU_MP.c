
#include <stdio.h>
#include <stdbool.h>

#define BOARD_SIZE 3

typedef struct 
{
    bool good;
    bool go;
    bool start;
    bool over;
    bool found;
    int val;
    bool red[BOARD_SIZE + 1][BOARD_SIZE + 1];
    bool blue[BOARD_SIZE + 1][BOARD_SIZE + 1];
    bool seen[BOARD_SIZE + 1][BOARD_SIZE + 1];
    bool touched[BOARD_SIZE + 1][BOARD_SIZE + 1];
} Game_State;

bool in_bounds(int row, int col);
void init_game(Game_State *game);
int count_board(bool board[BOARD_SIZE + 1][BOARD_SIZE + 1]);
int count_free_cells(Game_State *game);
void update_over(Game_State *game);
void remove_position(Game_State *game, int row, int col);
void replace_position(Game_State *game, int row, int col);
void expand_position(Game_State *game, int row, int col);
void update_position(Game_State *game, int row, int col);
void next_player_move(Game_State *game, int row, int col);
void print_board(Game_State *game);
void print_result(Game_State *game);

bool in_bounds(int row, int col) 
{
    return (row >= 1 && row <= BOARD_SIZE && col >= 1 && col <= BOARD_SIZE);
}

void init_game(Game_State *game) 
{
    int row;
    int col;

    game->good = false;
    game->go = true;
    game->start = true;
    game->over = false;
    game->found = false;
    game->val = 0;

    for (row = 0; row <= BOARD_SIZE; row++) 
    {
        for (col = 0; col <= BOARD_SIZE; col++) 
        {
            game->red[row][col] = false;
            game->blue[row][col] = false;
            game->seen[row][col] = false;
            game->touched[row][col] = false;
        }
    }
}

int count_board(bool board[BOARD_SIZE + 1][BOARD_SIZE + 1]) 
{
    int row;
    int col;
    int count = 0;

    for (row = 1; row <= BOARD_SIZE; row++)
     {
        for (col = 1; col <= BOARD_SIZE; col++) 
        {
            if (board[row][col]) 
            {
                count++;
            }
        }
    }

    return count;
}

int count_free_cells(Game_State *game) 
{
    int redCount = count_board(game->red);
    int blueCount = count_board(game->blue);
    int occupied = redCount + blueCount;
    int freeCells = (BOARD_SIZE * BOARD_SIZE) - occupied;

    return freeCells;
}

void update_over(Game_State *game) 
{
    int redCount = count_board(game->red);
    int blueCount = count_board(game->blue);
    int freeCells = count_free_cells(game);
    bool one_side_left = (!game->start) &&
                         ((redCount > 0 && blueCount == 0) ||
                          (redCount == 0 && blueCount > 0));

    game->over = (freeCells == 3) || (game->val >= 20) || one_side_left;
}

void remove_position(Game_State *game, int row, int col) 
{
    if (in_bounds(row, col)) {
        if (game->go) {
            game->red[row][col] = false;
        } else {
            game->blue[row][col] = false;
        }

        game->seen[row][col] = false;
        game->touched[row][col] = false;
    }
}

void replace_position(Game_State *game, int row, int col) 
{
    if (in_bounds(row, col)) 
    {
        game->found = false;

        if (game->go) {
            if (game->blue[row][col]) 
            {
                game->blue[row][col] = false;
                game->found = true;
            }

            if (game->red[row][col]) 
            {
                game->found = true;
            }

            if (!game->red[row][col]) 
            {
                game->red[row][col] = true;
            }
        } else {
            if (game->red[row][col]) 
            {
                game->red[row][col] = false;
                game->found = true;
            }

            if (game->blue[row][col]) 
            {
                game->found = true;
            }

            if (!game->blue[row][col]) 
            {
                game->blue[row][col] = true;
            }
        }

        if (game->found && !game->seen[row][col]) 
        {
            game->seen[row][col] = true;
            game->found = false;
        }

        if (game->found && game->seen[row][col] && !game->touched[row][col]) 
        {
            game->touched[row][col] = true;
            expand_position(game, row, col);
        }
    }
}

void expand_position(Game_State *game, int row, int col) 
{
    int up = row - 1;
    int down = row + 1;
    int left = col - 1;
    int right = col + 1;

    remove_position(game, row, col);

    if (game->go) 
    {
        replace_position(game, up, col);
        } 
            else 
        {
        replace_position(game, down, col);
    }

    replace_position(game, row, left);
    replace_position(game, row, right);
}

void update_position(Game_State *game, int row, int col) 
{
    game->good = false;

    if (in_bounds(row, col)) {
        if (!game->seen[row][col]) {
            game->seen[row][col] = true;
            game->good = !game->good;
        } else if (game->seen[row][col] && !game->touched[row][col]) {
            game->touched[row][col] = true;
            expand_position(game, row, col);
        }
    }
}

void next_player_move(Game_State *game, int row, int col) {
    if (!game->over && in_bounds(row, col)) {
        if (game->start && game->go) {
            game->red[row][col] = true;
            game->seen[row][col] = true;
            game->good = true;
        } else if (game->start && !game->go) {
            game->blue[row][col] = true;
            game->seen[row][col] = true;
            game->good = true;
        } else if (!game->start &&
                   ((game->go && game->red[row][col]) ||
                    (!game->go && game->blue[row][col]))) {
            update_position(game, row, col);
            game->good = true;
        }

        if (game->start) {
            if (count_board(game->red) == 1 && count_board(game->blue) == 1) {
                game->start = false;
            }
        }

        if (game->good) {
            game->good = !game->good;
            game->go = !game->go;
            game->val++;
        }

        update_over(game);
    }
}

void print_board(Game_State *game) {
    int row;
    int col;
    int redCount = count_board(game->red);
    int blueCount = count_board(game->blue);

    printf("\n");
    printf("Turn: %s | Start: %s | Moves: %d | Free cells: %d\n",
           game->go ? "Red" : "Blue",
           game->start ? "true" : "false",
           game->val,
           count_free_cells(game));
    printf("Red pieces: %d | Blue pieces: %d\n", redCount, blueCount);
    printf("\n");
    
    for (row = 1; row <= BOARD_SIZE; row++) {
        if (row > 1) {
            printf("---|---|---\n");
        }
        
        for (col = 1; col <= BOARD_SIZE; col++) {
            char cell = ' ';
            
            if (game->red[row][col] && game->blue[row][col]) {
                cell = 'X';
            } else if (game->red[row][col]) {
                cell = 'R';
            } else if (game->blue[row][col]) {
                cell = 'B';
            } else {
                cell = ' ';
            }
            printf(" %c ", cell);
            if (col < BOARD_SIZE) {
                printf("|");
            }
        }
        printf("\n");
    }
    
    printf("\n");
}

void print_result(Game_State *game) {
    int redCount = count_board(game->red);
    int blueCount = count_board(game->blue);

    if (game->over) {
        if (redCount > blueCount) {
            printf("Game over: R WINS!\n");
        } else if (redCount < blueCount) {
            printf("Game over: B WINS!\n");
        } else {
            printf("Game over: DRAW!\n");
        }
    } else {
        printf("Game stopped before reaching an over state.\n");
    }
}

int main(void) {
    Game_State game;
    int row;
    int col;
    int status;

    init_game(&game);
	
	printf("\n=====================================================================================\n");
	printf("                       WELCOME TO CCDSTRU Project Simulator\n");
	printf("                              Board size: 3 x 3\n");
	printf("                          Enter moves as: row col\n");
    printf("   The program ends when the PDF-defined over condition is reached or input ends.\n\n\n");
    printf("================================= GAME RULES ========================================\n\n");
	printf("STEP 1: FIRST MOVES:\n");
	printf("       - Red places 1 piece anywhere\n");
	printf("       - Blue places 1 piece anywhere\n\n");
	printf("STEP 2: MAIN GAME:\n");
	printf("       - On your turn, select a cell containing YOUR piece\n\n");
	printf("STEP 3: SELECTING YOUR PIECE:\n");
	printf("       -  First time you pick it: The piece stays, nothing happens\n");
	printf("       -  Second time you pick the same piece it EXPLODES and spreads to nearby cells\n\n");
	printf("STEP 4: WHEN A PIECE EXPLODES:\n");
	printf("       - Red spreads: UP, LEFT, RIGHT\n");
	printf("       - Blue spreads: DOWN, LEFT, RIGHT\n\n");
	printf("STEP 5: PLACING NEW PIECES:\n");
	printf("       - Empty cell: Place your piece and your piece will appear\n");
	printf("       - Opponent's cell: Capture and place your piece\n");
	printf("       - Your cell: Nothing happens your not allowed to stack\n\n");
	printf("STEP 6: GAME ENDS WHEN:\n");
	printf("       - 3 or less empty cells left on the board\n");
	printf("       - 20 total moves have been made\n");
	printf("       - One player has no pieces left\n\n");
	printf("STEP 7: HOW TO WIN:\n");
	printf("       - More pieces than opponent = YOU WIN!\n");
	printf("       - Same number of pieces = DRAW\n\n");
    printf("======================================================================================\n");

    status = 2;
    while (!game.over && status == 2) {
        print_board(&game);
        printf("Move for %s: ", game.go ? "Red" : "Blue");
        status = scanf("%d %d", &row, &col);

        if (status == 2) {
            if (in_bounds(row, col)) {
                next_player_move(&game, row, col);
            } else {
                printf("Invalid position. Use values from 1 to 3.\n");
            }
        }
    }

    update_over(&game);
    print_board(&game);
    print_result(&game);

    return 0;
}
