#include <stdio.h>

int main ( int argc, char **argv)
{
	const int SIZE = 10;
	int A[SIZE];
	A[0] = 0; A[1] = 1;
	printf("0 1 ");
	for (int i = 2; i < SIZE; i++) {
		A[i] = A[i-1] + A[i-2];
		printf("%d ", A[i]);
	}
	putchar('\n');
	return 0;
}

