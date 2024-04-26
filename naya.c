#include <stdio.h>  
int main(){
    char buffer[100];
    printf("Enter your name: ");
    scanf("%[^\n]%*c", buffer);
    printf("Hello %s\n", buffer);
}