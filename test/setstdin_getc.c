#include<stdio.h>

int main()
{
	int c;
	char separator = '|';
	while (1)
	{
		c = fgetc(stdin);
		if (c == EOF)
			break;
		putchar(c);
		putchar(separator);

		c = getc(stdin);
		if (c == EOF)
			break;
		putchar(c);
		putchar(separator);

		c = getchar();
		if (c == EOF)
			break;
		putchar(c);
		putchar(separator);
	}
}