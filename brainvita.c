#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define SIZE 7
#define MIN 2
#define MAX 4
#define DOT 1
#define HOLE 2

char board[SIZE][SIZE] = {0};

int read_int(int *intp)
{
	char buf[16];
	char *retp;
	char *endptr;
	long input;

	retp = fgets(buf, sizeof(buf), stdin);
	if (retp == NULL)
		return -1;
	input = strtol(retp, &endptr, 10);

	if (*endptr != '\n')
		return -2;
	if (input > INT_MAX)
		return -2;
	if (input < INT_MIN)
		return -2;
	*intp = (int) input;
	return 0;
}

int check_limits(int a)
{
	if (a >= MIN && a <= MAX)
		return 1;
	return 0;
}

void init_board(void)
{
	int i;
	int j;

	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
			board[i][j] = ' ';

	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
			if (check_limits(i) || check_limits(j))
				board[i][j] = '.';

	board[SIZE/2][SIZE/2] = 'o';
}

void print_board(void)
{
	int i;
	int j;

	printf(" ");
	for (i = 0; i < SIZE; i++)
		printf(" %d", i);
	printf("\n");

	for (i = 0; i < SIZE; i++) {
		printf("%d", i);
		for (j = 0; j < SIZE; j++)
			printf(" %c", board[i][j]);
		printf("\n");
	}
}

int check_border(int *r_c)
{
	int i;
	int j;

	i = r_c[0];
	j = r_c[1];
	if (!(check_limits(i) || check_limits(j)))
		return -1;
	else
		return 1;
}

int check_dot(int *r_c)
{
	if (board[r_c[0]][r_c[1]] != '.')
		return -1;
	else
		return 1;
}

int check_hole(int *r_c)
{
	if (board[r_c[0]][r_c[1]] != 'o')
		return -1;
	else
		return 1;
}

int check(int *r_c, int op)
{
	int ret_val;

	ret_val = check_border(r_c);
	if (ret_val == -1) {
		printf("Outside board\n");
		return ret_val;
	}

	if (op == DOT) {
		ret_val = check_dot(r_c);
		if (ret_val == -1) {
			printf("FROM is not a DOT\n");
			return ret_val;
		}
	} else if (op == HOLE) {
		ret_val = check_hole(r_c);
		if (ret_val == -1) {
			printf("TO is not a HOLE\n");
			return ret_val;
		}
	}
	return 0;
}

int check_gap(int *from, int *to)
{
	int a;
	int b;

	if (!(from[0] == to[0] || from[1] == to[1])) {
		printf("Not is same line\n");
		return -1;
	}

	a = from[0] - to[0];
	b = from[1] - to[1];
	if (a < 0)
		a = -a;
	if (b < 0)
		b = -b;
	if ((a == 0 && b != 2) || (b == 0 && a != 2)) {
		printf("Gap error\n");
		return -1;
	}
	return 0;
}

void move(int *from, int *to)
{
	board[to[0]][to[1]] = board[from[0]][from[1]];
	board[from[0]][from[1]] = 'o';

	if (to[1] - from[1] == 0) {
		if (from[0] > to[0])
			board[from[0] - 1][from[1]] = 'o';
		else
			board[from[0] + 1][from[1]] = 'o';
	} else {
		if (from[1] > to[1])
			board[from[0]][from[1] - 1] = 'o';
		else
			board[from[0]][from[1] + 1] = 'o';
	}
}

int count_dots(void)
{
	int i;
	int j;
	int count = 0;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (board[i][j] == '.')
				count++;
		}
	}
	return count;
}

int *read_r_c(int *r_c)
{
	int i;

	for (i = 0; i < 2; i++)
		read_int(&r_c[i]);
}

int main(void)
{
	int count;
	int from_r_c[2];
	int to_r_c[2];

	init_board();
	while (1) {
		sleep(1);
		system("clear");
		count = count_dots();
		if (count == 1)
			break;
		print_board();
		printf("\tRemaining : %d\n", count);

		printf("enter FROM row & column(DOT)\n");
		read_r_c(from_r_c);

		if (check(from_r_c, DOT) == -1)
			continue;

to:		printf("enter TO row & column(HOLE)\n");
		read_r_c(to_r_c);

		if (check(to_r_c, HOLE) == -1)
			goto to;

		if (check_gap(from_r_c, to_r_c) == -1)
			continue;

		move(from_r_c, to_r_c);
	}
	print_board();
}
