#include <stdio.h>
#include <stdbool.h>

#define GRID_SIZE 3

typedef struct 
{
    bool valid;
    bool turnFlag;
    bool isInitial;
    bool isFinished;
    bool triggerFlag;
    int moveCounter;
    bool boardA[GRID_SIZE + 1][GRID_SIZE + 1];
    bool boardB[GRID_SIZE + 1][GRID_SIZE + 1];
    bool marked[GRID_SIZE + 1][GRID_SIZE + 1];
    bool expanded[GRID_SIZE + 1][GRID_SIZE + 1];
} GameInfo;

bool is_inside(int r, int c);
void setup(GameInfo *state);
int count_grid(bool grid[GRID_SIZE + 1][GRID_SIZE + 1]);
int count_empty_cells(GameInfo *state);
void update_finished(GameInfo *state);
void clear_pos(GameInfo *state, int r, int c);
void apply_pos(GameInfo *state, int r, int c);
void spread_pos(GameInfo *state, int r, int c);
void update_pos(GameInfo *state, int r, int c);
void next_move(GameInfo *state, int r, int c);
void print_grid(GameInfo *state);
void print_outcome(GameInfo *state);

bool is_inside(int r, int c) 
{
    return (r >= 1 && r <= GRID_SIZE && c >= 1 && c <= GRID_SIZE);
}

void setup(GameInfo *state) 
{
    int r;
    int c;

    state->valid = false;
    state->turnFlag = true;
    state->isInitial = true;
    state->isFinished = false;
    state->triggerFlag = false;
    state->moveCounter = 0;

    for (r = 0; r <= GRID_SIZE; r++) 
    {
        for (c = 0; c <= GRID_SIZE; c++) 
        {
            state->boardA[r][c] = false;
            state->boardB[r][c] = false;
            state->marked[r][c] = false;
            state->expanded[r][c] = false;
        }
    }
}

int count_grid(bool grid[GRID_SIZE + 1][GRID_SIZE + 1]) 
{
    int r;
    int c;
    int total = 0;

    for (r = 1; r <= GRID_SIZE; r++)
     {
        for (c = 1; c <= GRID_SIZE; c++) 
        {
            if (grid[r][c]) 
            {
                total++;
            }
        }
    }

    return total;
}

int count_empty_cells(GameInfo *state) 
{
    int countA = count_grid(state->boardA);
    int countB = count_grid(state->boardB);
    int occupied = countA + countB;
    int empty = (GRID_SIZE * GRID_SIZE) - occupied;

    return empty;
}

void update_finished(GameInfo *state) 
{
    int countA = count_grid(state->boardA);
    int countB = count_grid(state->boardB);
    int empty = count_empty_cells(state);
    bool oneSide = (!state->isInitial) &&
                   ((countA > 0 && countB == 0) ||
                    (countA == 0 && countB > 0));

    state->isFinished = (empty == 3) || (state->moveCounter >= 20) || oneSide;
}

void clear_pos(GameInfo *state, int r, int c) 
{
    if (is_inside(r, c)) {
        if (state->turnFlag) {
            state->boardA[r][c] = false;
        } else {
            state->boardB[r][c] = false;
        }

        state->marked[r][c] = false;
        state->expanded[r][c] = false;
    }
}

void apply_pos(GameInfo *state, int r, int c) 
{
    if (is_inside(r, c)) 
    {
        state->triggerFlag = false;

        if (state->turnFlag) {
            if (state->boardB[r][c]) 
            {
                state->boardB[r][c] = false;
                state->triggerFlag = true;
            }

            if (state->boardA[r][c]) 
            {
                state->triggerFlag = true;
            }

            if (!state->boardA[r][c]) 
            {
                state->boardA[r][c] = true;
            }
        } else {
            if (state->boardA[r][c]) 
            {
                state->boardA[r][c] = false;
                state->triggerFlag = true;
            }

            if (state->boardB[r][c]) 
            {
                state->triggerFlag = true;
            }

            if (!state->boardB[r][c]) 
            {
                state->boardB[r][c] = true;
            }
        }

        if (state->triggerFlag && !state->marked[r][c]) 
        {
            state->marked[r][c] = true;
            state->triggerFlag = false;
        }

        if (state->triggerFlag && state->marked[r][c] && !state->expanded[r][c]) 
        {
            state->expanded[r][c] = true;
            spread_pos(state, r, c);
        }
    }
}

void spread_pos(GameInfo *state, int r, int c) 
{
    int up = r - 1;
    int down = r + 1;
    int left = c - 1;
    int right = c + 1;

    clear_pos(state, r, c);

    if (state->turnFlag) 
    {
        apply_pos(state, up, c);
        } 
            else 
        {
        apply_pos(state, down, c);
    }

    apply_pos(state, r, left);
    apply_pos(state, r, right);
}

void update_pos(GameInfo *state, int r, int c) 
{
    state->valid = false;

    if (is_inside(r, c)) {
        if (!state->marked[r][c]) {
            state->marked[r][c] = true;
            state->valid = !state->valid;
        } else if (state->marked[r][c] && !state->expanded[r][c]) {
            state->expanded[r][c] = true;
            spread_pos(state, r, c);
        }
    }
}

void next_move(GameInfo *state, int r, int c) {
    if (!state->isFinished && is_inside(r, c)) {
        if (state->isInitial && state->turnFlag) {
            state->boardA[r][c] = true;
            state->marked[r][c] = true;
            state->valid = true;
        } else if (state->isInitial && !state->turnFlag) {
            state->boardB[r][c] = true;
            state->marked[r][c] = true;
            state->valid = true;
        } else if (!state->isInitial &&
                   ((state->turnFlag && state->boardA[r][c]) ||
                    (!state->turnFlag && state->boardB[r][c]))) {
            update_pos(state, r, c);
            state->valid = true;
        }

        if (state->isInitial) {
            if (count_grid(state->boardA) == 1 && count_grid(state->boardB) == 1) {
                state->isInitial = false;
            }
        }

        if (state->valid) {
            state->valid = !state->valid;
            state->turnFlag = !state->turnFlag;
            state->moveCounter++;
        }

        update_finished(state);
    }
}

void print_grid(GameInfo *state) {
    int r;
    int c;
    int countA = count_grid(state->boardA);
    int countB = count_grid(state->boardB);

    printf("\n");
    printf("Turn: %s | Start: %s | Moves: %d | Free cells: %d\n",
           state->turnFlag ? "Red" : "Blue",
           state->isInitial ? "true" : "false",
           state->moveCounter,
           count_empty_cells(state));
    printf("Red pieces: %d | Blue pieces: %d\n", countA, countB);
    printf("\n");
    
    for (r = 1; r <= GRID_SIZE; r++) {
        if (r > 1) {
            printf("---|---|---\n");
        }
        
        for (c = 1; c <= GRID_SIZE; c++) {
            char cell = ' ';
            
            if (state->boardA[r][c] && state->boardB[r][c]) {
                cell = 'X';
            } else if (state->boardA[r][c]) {
                cell = 'R';
            } else if (state->boardB[r][c]) {
                cell = 'B';
            } else {
                cell = ' ';
            }
            printf(" %c ", cell);
            if (c < GRID_SIZE) {
                printf("|");
            }
        }
        printf("\n");
    }
    
    printf("\n");
}

void print_outcome(GameInfo *state) {
    int countA = count_grid(state->boardA);
    int countB = count_grid(state->boardB);

    if (state->isFinished) {
        if (countA > countB) {
            printf("Game over: R WINS!\n");
        } else if (countA < countB) {
            printf("Game over: B WINS!\n");
        } else {
            printf("Game over: DRAW!\n");
        }
    } else {
        printf("Game stopped before reaching an over state.\n");
    }
}

int main(void) {
    GameInfo state;
    int r;
    int c;
    int status;

    setup(&state);
	
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
    while (!state.isFinished && status == 2) {
        print_grid(&state);
        printf("Move for %s: ", state.turnFlag ? "Red" : "Blue");
        status = scanf("%d %d", &r, &c);

        if (status == 2) {
            if (is_inside(r, c)) {
                next_move(&state, r, c);
            } else {
                printf("Invalid position. Use values from 1 to 3.\n");
            }
        }
    }

    update_finished(&state);
    print_grid(&state);
    print_outcome(&state);

    return 0;
}
