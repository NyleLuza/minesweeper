# include <stdio.h>
# include <string.h>
#include <malloc.h>

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




void display_cell(cell* c) {
		//printf("%4d", c->position);
        printf("%3s", "/");
}

cell** board;
int rows;
int cols;
int mines;

void command_new() {
	board = (cell **)malloc(sizeof(cell *) * rows); // allocates space for board

	for (int i = 0; i < rows; i++) {
		board[i] = (cell*) malloc(sizeof(cell) * cols);


	}
	for (int i = 0; i < rows;i++) {
		for (int j = 0; j < cols; j++) {
			board[i][j].position = i*cols +j;
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

int processcommand(char tokens[][MAXTOKENLENGTH], int tokencount) {
	if (strcmp(tokens[0], "new") == 0) {
		rows = atoi(tokens[1]);
		cols = atoi(tokens[2]);
		mines = atoi(tokens[3]);
		command_new();
	}
	else if (strcmp(tokens[0], "show") == 0)
		command_show();
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
	rungame();
	return 0;
}