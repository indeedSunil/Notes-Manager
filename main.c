#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h> //for making directories
#include <dirent.h>   //for listing files in a directory
#include <errno.h>    //for error handling
#include <unistd.h>   // Include the header file for ftruncate

#define MAX_LINE_SIZE 100
struct userDetails
{
    char id[50], name[50], password[50];
};
char getRealName[50];
char getRealId[50];
int checker(char *id)
{
    FILE *fp = fopen("details.txt", "r");
    if (fp == NULL)
    {
        printf("File not found");
        return 0;
    }
    char line[MAX_LINE_SIZE];

    struct userDetails user;

    while (fgets(line, MAX_LINE_SIZE, fp) != NULL)
    {
        sscanf(line, "%s %s %s", user.id, user.name, user.password);
        if (strcmp(user.id, id) == 0)
        {
            return 1;
        }
    }
    return 0;
}
int registerUser()
{
    printf("Register Page\n");
    char id[50], name[50], password[50];
    printf("Enter id: ");
    scanf("%s", id);
    printf("Enter name: ");
    scanf("%s", name);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *fp = fopen("details.txt", "a");
    if (fp == NULL)
    {
        printf("File not found\n");
        return 0;
    }
    if (checker(id) == 1)
    {
        printf("User already exists\n");
        return 0;
        exit;
    }
    fprintf(fp, "%s\t%s\t%s\n", id, name, password);
    fclose(fp);
    // create a folder with the name of the user where all his notes are stored.
    // the folder is stored inside Databases folder
    char path[100] = "Databases/";
    strcat(path, id);
    mkdir(path, 0777);

    return 1;
}
int signInUser()
{
    printf("Sign In Page\n");
    char id[50], password[50];
    printf("Enter id: ");
    scanf("%s", id);
    printf("Enter password: ");
    scanf("%s", password);
    FILE *fp = fopen("details.txt", "r");
    if (fp == NULL)
    {
        printf("File not found\n");
        return 0;
    }
    char line[MAX_LINE_SIZE];
    struct userDetails user;
    while (fgets(line, MAX_LINE_SIZE, fp) != NULL)
    {
        sscanf(line, "%s\t%s\t%s", user.id, user.name, user.password);
        if (strcmp(user.id, id) == 0 && strcmp(user.password, password) == 0)
        {

            strcpy(getRealName, user.name);
            strcpy(getRealId, user.id);
            return 1;
        }
    }
    return 0;
}
int menu(char *name)
{
    system("clear");
    printf("\n Hi, %s\n", name);

    printf("1. Add Note\n");
    printf("2. View Notes\n");
    printf("3. Delete Note\n");
    printf("4. Exit\n");
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    system("clear");
    // based on the choice selecting functions to do that taskk.
    // Add function declaration for addNote

    switch (choice)
    {
    case 1:
    int addNote();
    addNote(getRealId);
        
        break;
    case 2:
        int viewNotes();
        if (viewNotes(getRealId) == 1)
        {
            printf("Note viewed successfully\n");
        }
        else
        {
            printf("Note view failed\n");
        }

        break;
    case 3:
        printf("Delete Note\n");
        break;
    case 4:
        printf("Exit\n");
        break;
    default:
        printf("Invalid choice\n");
        break;
    }

    return 1;
}
int addNote(char *id)
{

    // here we are going to add a note to the user's folder
    char path[100] = "Databases/";
    strcat(path, id);

    // the name of the note file will be asked from the user
    char noteName[50];
    printf("Enter the name of the note(without spaces): ");
    scanf("%s", noteName);
    strcat(path, "/");
    strcat(path, noteName);
    char extension[5] = ".txt";
    strcat(path, extension);
    FILE *fp = fopen(path, "w");
    if (fp == NULL)
    {
        printf("File not found\n");
        return 0;
    }
//putting the note name in the first line of text file in capital letters
char upperNoteName[50];
for (int i = 0; noteName[i] != '\0'; i++)
{
    upperNoteName[i] = toupper(noteName[i]);
}
// upperNoteName[i] = '\0'; // Add null terminator

fprintf(fp, "\t\t\t%s\n", upperNoteName);

    
    char note[1000];
    printf("Enter the note: \n");
    printf("You can enter multiple lines of notes\n");
    printf("........................................\n");
    // take multi line input from the user
    while (fgets(note, 1000, stdin) != NULL)
    {

        if (strcmp(note, "!q\n") == 0)
        {
            printf("Note added successfully\n");
            break;
        }

        // Check if "!q" exists anywhere in the input line
        if (strstr(note, "!q") != NULL)
        {
            printf("Note added successfully\n");
            break;
        }
        if (ftell(fp) == 0 && note[0] == '\n') {
            continue; // Skip writing empty line at the beginning of the file
        }
        fprintf(fp, "%s", note);
    }

    fprintf(fp, "%s", note);
    // so jati garepani tyo last ko !q hatena..aba teslai hatauna ko lagi we will try something
    //  Remove the last "!q" from the file
   
    fseek(fp, -3, SEEK_END);
    // The fseek function is used to change the position of the file pointer in a file. It takes three arguments: a file pointer, an offset, and a position from where the offset is added. In this case, it moves the file pointer 3 positions back from the end of the file (SEEK_END). If the file were "Hello", the file pointer would now be pointing at the second 'l'.

    ftruncate(fileno(fp), ftell(fp));
    //  The ftruncate function is used to reduce the size of a file to the specified length. It takes two arguments: a file descriptor and a length. The fileno function is used to get the file descriptor associated with the file pointer fp. The ftell function is used to get the current position of the file pointer fp. So, this line reduces the size of the file to the current position of the file pointer. In the "Hello" example, this would truncate the file to "Hell".
    fclose(fp);

    return 1;
}
int viewNotes(char *id)
{
   
    
}
int main()
    {
        printf("Welcome to Notes Manager\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);
        system("clear");
        switch (choice)
        {
        case 1:

            if (registerUser() == 1)
            {
                printf("User registered successfully\n");
            }
            else
            {
                printf("User registration failed \n");
            }
            break;
        case 2:
            printf("Login\n");

            if (signInUser() == 1)
            {
                printf("\nUser logged in successfully\n");
                menu(getRealName);
            }
            else
            {
                printf("User login failed\n");
                // Add a return statement here to exit the switch case else this wont run and i dont why...
                // so update on this : this return 0 deletes the menu function and the program exits.
                return 0;
            }

            system("clear");

            break;
        case 3:
            printf("Exit\n");
            break;
        default:
            printf("Invalid choice\n");
            break;
        }

        return 0;
    }