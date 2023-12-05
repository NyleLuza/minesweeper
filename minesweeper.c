# include <stdio.h>
# include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>

//#pragma warning(disable : 4996)

#define MAXTOKENCOUNT 20
#define MAXTOKENLENGTH 20
#define MAXLINELENGTH 400


struct cell {
	int position;
	int adjcount;
	int mined;
	int covered;
	int flagged;
};

typedef struct cell cell;






cell** board;
int rows;
int cols;
int mines;

void display_cell(cell* c) {
    //printf("%4d", c->position);
    //printf("%3s", "/");

    if (c->flagged==1) printf("%2c", 'P');
    else if(c->covered==1) printf("%2s", "/");
    else if(c->mined == 1) printf("%2s", "*");
    else if (c->adjcount > 0) printf("%2i", c->adjcount);
    else printf("%2s",".");
}

void init_cell(cell *c, int p) {
    c->position = p; // each cell knows its pos in board
    c->mined = 0; // mined initially false
    c->adjcount = 0; // adjcount initially 0
    c->covered = 0; // covered initially false
    c->flagged = 0;
}
int get_random(int range){
    return ((int)floor((float)range*rand()/RAND_MAX))%range;
}

void coverall(){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            board[i][j].covered = 1;
        }
    }
}

void command_new() {
	board = (cell **)malloc(sizeof(cell *) * rows); // allocates space for board

	for (int i = 0; i < rows; i++) {
		board[i] = (cell*) malloc(sizeof(cell) * cols);
	}
	for (int i = 0; i < rows;i++) {
		for (int j = 0; j < cols; j++) {
            int pos = i*cols+j;
			init_cell(&board[i][j], pos);
		}
	}
    for(int p=0;p<mines-1;p++){
        int r = get_random(rows);
        int c = get_random(cols);
        while(board[r][c].mined==1){
            r = get_random(rows);
            c = get_random(cols);
        }
        board[r][c].mined = 1;
    }

    int neighborhoodcount = 8;
    int rowneighbors[] = {-1,-1,+0,+1,+1,+1,+0,-1};
    int colneighbors[] = {+0,+1,+1,+1,+0,-1,-1,-1};

    for(int r=0;r<rows-1;r++){
        for(int c=0;c<cols-1;c++){
            int minecount=0;
            for(int d=0;d<neighborhoodcount;d++){
                int r2=r+rowneighbors[d];
                int c2=c+colneighbors[d];
                if (0 <= r2 && r2 < rows && 0 <= c2 && c2 < cols){
                    if(board[r2][c2].mined == 1){
                        minecount++;
                    }
                }
            }
            board[r][c].adjcount=minecount;
        }
    }

}

void command_show() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			display_cell(&board[i][j]);
		}
		printf("\n");
	}
}

void command_flag(int r, int c){
    if(board[r][c].flagged==0){
        board[r][c].flagged=1;
    }
    else{
        printf("cell already flagged\n");
    }

}

void command_unflag(int r, int c){
    if(board[r][c].flagged==1){
        board[r][c].flagged = 0;
    }
    else{
        printf("cell already unflagged\n");
    }
}


void uncover_recursive(int r, int c){
    board[r][c].covered = 0;
    if(board[r][c].adjcount==0){
        int neighborhoodcount = 8;
        int rowneighbors[] = {-1,-1,+0,+1,+1,+1,+0,-1};
        int colneighbors[] = {+0,+1,+1,+1,+0,-1,-1,-1};
                for(int d=0;d<7;d++){
                    int r2=r+rowneighbors[d];
                    int c2=c+colneighbors[d];
                        if(0 <= r2 && r2 < rows && 0 <= c2 && c2 < cols) {
                            if (board[r2][c2].covered==1) {
                                if(board[r2][c2].mined == 1){
                                    board[r2][c2].covered=1;
                                }
                                else {
                                    uncover_recursive(r2, c2);
                                }
                            }
                        }
                }



    }
}



int processcommand(char tokens[][MAXTOKENLENGTH], int tokencount) {
	if (strcmp(tokens[0], "new") == 0) {
		rows = atoi(tokens[1]);
		cols = atoi(tokens[2]);
		mines = atoi(tokens[3]);
		command_new();
	}
	else if (strcmp(tokens[0], "show") == 0)
		command_show();

    else if (strcmp(tokens[0], "coverall") == 0)
        coverall();

    else if(strcmp(tokens[0], "flag")== 0) {
        int first = atoi(tokens[1]);
        int second = atoi(tokens[2]);
        command_flag(first, second);
    }
    else if(strcmp(tokens[0], "unflag")== 0) {
        int first = atoi(tokens[1]);
        int second = atoi(tokens[2]);
        command_unflag(first, second);
    }

    else if (strcmp(tokens[0], "uncover") == 0) {
        int first = atoi(tokens[1]);
        int second = atoi(tokens[2]);
        uncover_recursive(first, second);
    }
	else if (strcmp(tokens[0], "quit") == 0)
		return 0;

}

void getline(char line[], int maxlinelen) {
	fgets(line, maxlinelen, stdin);
	int linelen = strlen(line);
	if (line[linelen - 1] == '\n')
		line[linelen - 1] = '\0';
}
void gettokens(char line[], char tokens[][MAXTOKENLENGTH], int* count) {
	char linecopy[MAXLINELENGTH];
	char* p;
	int tokencount = 0;
	strcpy(linecopy, line);
	p = strtok(linecopy, " ");

	while (p != NULL) {
		strcpy(tokens[tokencount], p);
		p = strtok(NULL, " ");
		tokencount++;
	}
	*count = tokencount;
}

void rungame() {

	char line[MAXLINELENGTH];
	char tokens[MAXTOKENCOUNT][MAXTOKENLENGTH];
	
	while (1) {
		int tokencount;
		int result;
		printf(">>");
		getline(line, MAXLINELENGTH);
		gettokens(line, tokens, &tokencount);
		result = processcommand(tokens, tokencount);

		if (result == 0)
			break;
	}
}

void main() {
    srand(time(0));
	rungame();
	return 0;
}