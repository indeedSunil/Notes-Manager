// Write a program to read a string in main() using gets(). Pass it to a function that finds the
// longest word of the string, counts the number of vowels and consonants in the word and
// displays the counts and the world form main(). 
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void longest_word(char *str, char *longest, int *vowels, int *consonants) {
    int i = 0, j = 0, max = 0, count_vowels = 0, count_consonants = 0;
    while (str[i] != '\0') {
        if (isalpha(str[i])) {
            j = i;
            while (isalpha(str[j])) {
                j++;
            }
            if (j - i > max) {
                max = j - i;
                strncpy(longest, str + i, max);
            }
            i = j;
        } else {
            i++;
        }
    }
    i = 0;
    while (longest[i] != '\0') {
        if (strchr("aeiouAEIOU", longest[i])) {
            count_vowels++;
        } else {
            count_consonants++;
        }
        i++;
    }
    *vowels = count_vowels;
    *consonants = count_consonants;
}

int main() {
    char str[100], longest[100];
    int vowels, consonants;
    printf("Enter a string: ");
    gets(str);
    longest_word(str, longest, &vowels, &consonants);
    printf("Longest word: %s\n", longest);
    printf("Vowels: %d\n", vowels);
    printf("Consonants: %d\n", consonants);
    return 0;
}
