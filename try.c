
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <ctype.h>

void func1(void)
{
printf("Hey, you entered foo! \n");
}

void func2(void)
{
printf("Hey, you entered bar! \n");
}


void ctrl_g(void)
{
printf("Hey, you pressed CTRL g! \n");
}


int main(void)
{
char word[80];
char ch;

do {
puts("Enter some text :");
scanf("%s", word);

if ( !strcmp(word, "foo") ) {
func1();
}
else if (!strcmp(word, "\001") ) {
ctrl_g();
}

printf("Try again? (y/n) : ");
scanf(" %c%*c", &ch);
}

while( toupper(ch) != 'N' );
return 0;
}
