#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> 
#include <sys/stat.h> //for making directories
#include <dirent.h>   //for listing files in a directory
#include <errno.h>    //for error handling
#include <unistd.h>   // Include the header file for ftruncate
#include <sys/types.h>

#define MAX_LINE_SIZE 10000
struct userDetails
{
    char id[50], name[50], password[50];
};
char getRealName[50];
char getRealId[50];

void gimmickLoadingScreen()
{
    // making a gimmick loading screen for 3 seconds using for loops and sleep function
   
    printf("Loading in 2...\n");
    sleep(1);
    system("clear");
    printf("Loading in 1...\n");
    sleep(1);
    system("clear");
}
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
    printf("-------------------\n");
    printf("Register Page\n");
    printf("-------------------\n");

    char id[50], name[50], password[50];
    printf("Enter id: ");
    scanf(" %[^\n]s", id);
    printf("Enter name: ");
    scanf(" %[^\n]s", name);
    printf("Enter password: ");
    scanf(" %[^\n]s", password);

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
     printf("-------------------\n");
    printf("Login Page\n");
    printf("-------------------\n");
    char id[50], password[50];
    printf("Enter id: ");
    scanf(" %[^\n]s", id);
    printf("Enter password: ");
    scanf(" %[^\n]s", password);
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
        sscanf(line, "%s\t%s\t\%s", user.id, user.name, user.password);
        //
        if (strcmp(user.id, id) == 0 && strcmp(user.password, password) == 0)
        {

            strcpy(getRealName, user.name);
            strcpy(getRealId, user.id);
            return 1;
        }
    }
    return 0;
}
int addNote(char *id)
{

    // here we are going to add a note to the user's folder
    char path[100] = "Databases/";
    strcat(path, id);

    // the name of the note file will be asked from the user
    char noteName[50];
    printf("Enter the name of the note: ");
    scanf(" %[^\n]s", noteName);
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

    char note[1000];
    printf("Enter your notes below: \n\n");
       printf("Enter !q to save the note\n");
    printf("........................................\n");
    // take multi line input from the user
    while (fgets(note, 1000, stdin) != NULL)
    {

        if (strcmp(note, "!q\n") == 0)
        {
           
            break;
        }

        // Check if "!q" exists anywhere in the input line
        if (strstr(note, "!q") != NULL)
        {

            break;
        }
        if (ftell(fp) == 0 && note[0] == '\n')
        {
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
    // here we are going to view the notes of the user based on his id
    DIR *dir;
    struct dirent *ent;
    char path[100] = "Databases/";
    strcat(path, id);

    if ((dir = opendir(path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (strstr(ent->d_name, ".txt") != NULL)
            {
                printf("%s\n", ent->d_name);
            }
        }
        closedir(dir);
    }
    else
    {
        printf("Failed to open directory\n");
        return 0;
    }

    // asking the user if he wants to view any .txt file or not
    char noteName[50],displayNoteName[50];
     
    printf("Enter the name of the note you want to view: ");
    scanf(" %[^\n]s", noteName);
    strcpy(displayNoteName,noteName);
    strcat(path, "/");
    strcat(path, noteName);
    strcat(path, ".txt");
    
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("File not found\n");
        return 0;
    }
    system("clear");
    printf("%s\n\n", displayNoteName);
    printf("........................................\n");
    char line[MAX_LINE_SIZE];
    while (fgets(line, MAX_LINE_SIZE, fp) != NULL)
    {
        printf("%s", line);
    }
    printf("\n........................................\n");

    //returning 1 only if user enter back or Back or BACK other wise showing the note and then returning 0

    char back[10];
    printf("\n\nEnter back to go back: ");
    scanf(" %[^\n]s", back);
    if (strcmp(back, "back") == 0 || strcmp(back, "Back") == 0 || strcmp(back, "BACK") == 0)
    {
        fclose(fp);
        return 1;
    }

    return 0;

   
}
int DeleteNotes(char *id)
{
    // first of all listing all notes of the user.
    // we can take use of view notes function to list all the notes of the user
    DIR *dir;
    struct dirent *ent;
    char path[100] = "Databases/";
    strcat(path, id);

    if ((dir = opendir(path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (strstr(ent->d_name, ".txt") != NULL)
            {
                printf("%s\n", ent->d_name);
            }
        }
        closedir(dir);
    }
    else
    {
        printf("Failed to open directory\n");
        return 0;
    }

    // asking the user to the file that he wants to delete
    char noteName[50],displayNoteName[50];
    printf("Enter the name of the note you want to delete: ");
    scanf(" %[^\n]s", noteName);
    strcpy(displayNoteName,noteName);
    //checking the file that user entered exists or not
    strcat(path, "/");
    strcat(path, noteName);
    strcat(path, ".txt");
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("File not found\n");
        return 0;
    }

    
    // now deleting the  file that user entered
   
    remove(path);
    printf(" %s : Note deleted successfully\n",displayNoteName);

    return 1;
}
int editNotes(char *id)
{
    DIR *dir;
    struct dirent *ent;
    char path[100] = "Databases/";
    strcat(path, id);

    if ((dir = opendir(path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (strstr(ent->d_name, ".txt") != NULL)
            {
                printf("%s\n", ent->d_name);
            }
        }
        closedir(dir);
    }
    else
    {
        printf("Failed to open directory\n");
        return 0;
    }

    // asking the user if he wants to edit any .txt file or not
    char noteName[50],displayNoteName[50];
    printf("Enter the name of the note you want to edit: ");
    scanf(" %[^\n]s", noteName);
    strcpy(displayNoteName,noteName);
    strcat(path, "/");
    strcat(path, noteName);
    strcat(path, ".txt");
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("File not found\n");
        return 0;
    }
    system("clear");
    printf("%s\n\n", displayNoteName);
    printf("........................................\n");
    // he will be shown the content of the file and then he can edit the file
    char line[MAX_LINE_SIZE];
    while (fgets(line, MAX_LINE_SIZE, fp) != NULL)
    {
        printf("%s", line);
    }
    printf("\n........................................\n");
    fclose(fp);
    // now we will open the file in append mode and then he can edit the file
    fp = fopen(path, "a");
    if (fp == NULL)
    {
        printf("File not found\n");
        return 0;
    }
    char note[1000];
    printf("Enter the edit to the note: \n\n");
    printf("Enter !q to save the note\n");

    printf("........................................\n");
    // take multi line input from the user
    while (fgets(note, 1000, stdin) != NULL)
    {

        if (strcmp(note, "!q\n") == 0)
        { 
            
            break;
        }

        // Check if "!q" exists anywhere in the input line
        if (strstr(note, "!q") != NULL)
        {
            break;
        }
        if (ftell(fp) == 0 && note[0] == '\n')
        {
            continue; // Skip writing empty line at the beginning of the file
        }
        fprintf(fp, "%s", note);
        fflush(fp); // Flush the file buffer to ensure the data is written immediately
    }
    fclose(fp); // Close the file after writing
    return 1;
}

int postLogin()
{

    // this is the post login screen inside infinite while loop
    //  so that the user can do multiple tasks
    while (1)
    {
        system("clear");
        printf("\n Hi, %s\n", getRealName);
        printf("--------------------------------\n ");

        printf("1. Add Note\n");
        printf(" 2. View Notes\n");
        printf(" 3. Edit Notes\n");
        printf(" 4. Delete Notes\n");
        printf(" 5. Back to Login Screen\n");
        printf("--------------------------------\n ");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);
        system("clear");

        // based on the choice selecting functions to do that taskk.
        if (choice == 1)
        {
            printf("Redirecting to Add Note screen in 2..\n");
            sleep(1);
            system("clear");
            printf("Redirecting to Add Note screen in 1.\n");
            sleep(1);
            system("clear");

            // passing to add note function

            if (addNote(getRealId) == 1)
            {
                system("clear");
                
                printf("Note added successfully\n");
                sleep(1);
                system("clear");

                printf("Redirecting to main screen in 2..\n");
                sleep(1);
                system("clear");
                printf("Redirecting to main screen in 1.\n");
                sleep(1);
                system("clear");
                postLogin();
            }
            else
            {
                system("clear");
                printf("Note addition failed \n");
                sleep(1);

                printf("Redirecting to main screen in 2..\n");
                sleep(1);
                system("clear");
                printf("Redirecting to main screen in 1.\n");
                sleep(1);
                system("clear");
                postLogin();

            }
        }

        else if (choice == 2)
        {
            printf("Redirecting to View Note screen in 2..\n");
            sleep(1);
            system("clear");
            printf("Redirecting to View Note screen in 1.\n");
            sleep(1);
            system("clear");

            // passing to view note function

            if (viewNotes(getRealId) == 1)
            {
                system("clear");
               
                printf("Note viewed successfully\n");
                sleep(1);
                system("clear");

                printf("Redirecting to Post login screen in 2..\n");
                sleep(1);
                system("clear");
                printf("Redirecting to Post login screen in 1.\n");
                sleep(1);
                system("clear");
                postLogin();

            }
            else
            {
                system("clear");
                printf("\nNote view failed \n");
                printf("You entered the wrong note name or the note does not exist\n");
                sleep(1);
                system("clear");

                printf("Redirecting to Post login screen in 2..\n");
                sleep(1);
                system("clear");
                printf("Redirecting to Post login screen in 1.\n");
                sleep(1);
                system("clear");
                postLogin();

            }
        }
        else if (choice == 3)
        {
            printf("Redirecting to Edit Note screen in 2..\n");
            sleep(1);
            system("clear");
            printf("Redirecting to Edit Note screen in 1.\n");
            sleep(1);
            system("clear");

            // passing to edit note function

            if (editNotes(getRealId) == 1)
            {
                system("clear");
                
                printf("Note edited successfully\n");
                sleep(1);
                system("clear");

                printf("Redirecting to Post login screen in 2..\n");
                sleep(1);
                system("clear");
                printf("Redirecting to Post login screen in 1.\n");
                sleep(1);
                system("clear");
                postLogin();

            }
            else
            {
                system("clear");
                printf("Note edit failed \n");
                sleep(1);

                printf("Redirecting to Post login screen in 2..\n");
                sleep(1);
                system("clear");
                printf("Redirecting to Post login screen in 1.\n");
                sleep(1);
                system("clear");
                postLogin();

            }
        }
        else if (choice == 4)
        {
            printf("Redirecting to Delete Note screen in 2..\n");
            sleep(1);
            system("clear");
            printf("Redirecting to Delete Note screen in 1.\n");
            sleep(1);
            system("clear");

            // passing to delete note function

            if (DeleteNotes(getRealId) == 1)
            {
                system("clear");
               
                printf("Note deleted successfully\n");
                sleep(1);
                system("clear");

                printf("Redirecting to Post login screen in 2..\n");
                sleep(1);
                system("clear");
                printf("Redirecting to Post login screen in 1.\n");
                sleep(1);
                system("clear");
                postLogin();

            }
            else
            {
                system("clear");
                printf("Note delete failed \n");
                sleep(1);

                printf("Redirecting to post login screen in 2..\n");
                sleep(1);
                system("clear");
                printf("Redirecting to post login screen in 1.\n");
                sleep(1);
                system("clear");
                postLogin();

            }
        }
        else if (choice == 5)
        {
            system("clear");
            printf("Logging out in 2..\n");
            sleep(1);
           
            printf("Logging out in 1.\n");
            sleep(1);
            int main();
            main();
        }
        else
        {
            printf("Invalid choice\n");
        }
    }
    return 1;
}
int main()
{

  
   while (1)
    {
        // code for login screen

        printf("--------------------------------\n ");
        printf("Welcome to Notes Manager\n");
        printf("--------------------------------\n");

        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("--------------------------------\n ");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);
        system("clear");

        // if user enter 1 then he will be redirected to registration screen
        if (choice == 1)
        {
            printf("Redirecting to registration screen in 2..\n");
            sleep(1);
            system("clear");
            printf("Redirecting to registration screen in 1.\n");
            sleep(1);
            system("clear");

            // passing to register user function
            int registerUser();
            if (registerUser() == 1)
            {
                system("clear");
               
                printf("User registered successfully\n");
                sleep(1);
                system("clear");

                // after the user is successfully registered he will be redirected to login screen
                //  gimmicking the login screen for 3 seconds
                printf("Redirecting to login screen in 2..\n");
                sleep(1);
                system("clear");
                printf("Redirecting to login screen in 1.\n");
                sleep(1);
                system("clear");
            }
            else
            {
                system("clear");

                printf("User registration failed \n");
                sleep(1);
                system("clear");    
                // gimmicking the login screen for 3 seconds

                printf("Redirecting to login screen in 2..\n");
                sleep(1);
                system("clear");
                printf("Redirecting to login screen in 1.\n");
                sleep(1);
                system("clear");
            }
        }
        else if (choice == 2)
        {
            printf("Redirecting to login screen in 2..\n");
            sleep(1);
            system("clear");
            printf("Redirecting to login screen in 1.\n");
            sleep(1);
            system("clear");

            int signInUser();

            // passing to login user function
            if (signInUser() == 1)
            {
                system("clear");
               
                printf("User login successfully\n");
                sleep(1);
                system("clear");

                // after the user is successfully registered he will be redirected to login screen
                //  gimmicking the login screen for 3 seconds
                printf("Redirecting to post login screen in 2..\n");
                sleep(1);
                system("clear");
                printf("Redirecting to post login screen in 1.\n");
                sleep(1);
                system("clear");

                int postLogin();
                postLogin();
            }
            else
            {
                system("clear");
                printf("User login failed: Invalid Credentials \n");
                sleep(1);

                // gimmicking the login screen for 3 seconds

                
                system("clear");
                printf("Redirecting to login screen in 2..\n");
                sleep(1);
                system("clear");
                
                
                printf("Redirecting to login screen in 1.\n");
                sleep(1);
                system("clear");
            }
        }
        else if (choice == 3)
        {
            printf("Exiting in 2..\n");
            sleep(1);
            system("clear");
            printf("Exiting in 1.\n");
            sleep(1);
            system("clear");
            sleep(1);
            exit(0);
        }
        else
        {
            printf("Invalid choice\n");
        }
    }
    return 0;
}

