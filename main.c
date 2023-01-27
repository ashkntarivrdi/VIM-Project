#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <direct.h>
#include <dirent.h> 
#include <sys/types.h>


#define MAX 10000

FILE *filepointer;
char command[MAX];
char command_extension[MAX];
char address[MAX];

void cat();
void createfile();
void FindCommand();


int main()
{
    while(1)
    {
    scanf("%s", command);
    FindCommand();
    }

    return 0;    
}

void FindCommand()
{
    if(!strcmp(command, "cat")) {
        cat();
        return;
    } 
    else if(!strcmp(command, "createfile")) {
        createfile();
        return;
    }
    else {
        printf("Invalid Command!\nSimply type <--help> for more information.\n");
        return;
    }

}

void cat() 
{
    char x;
    char *new_address;

    scanf(" %[^ ]s", command_extension);

    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nSimply type <--help> for more information.\n");
        return;
    }

    getchar();
    scanf("%c", &x);
    scanf(" %[^\n]s", address);

    if(x == '/') {
        new_address = address;
    }else if(x == '"') {
        new_address = address + 1;
        new_address[strlen(new_address) - 1] = '\0';
    }


    // int str[MAX];
    char str;
    filepointer = fopen(new_address , "r");
    if(filepointer != NULL) {
        str = fgetc(filepointer);
        while(str != EOF) {
            printf("%c", str);
            str = getc(filepointer);
            // printf("Fuck");
        }
        printf("\n");
    }else {
        printf("No Such A File!\n");
        return;
    }
    fclose(filepointer);
}

void createfile()
{
    char *directory;
    char *next_directory;
    char *new_address;
    char x;
    int counter = 0;
    int result;

    scanf(" %[^ ]s", command_extension);

    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nSimply type <--help> for more information.\n");
        return;
    }

    getchar();
    scanf("%c", &x);
    scanf(" %[^\n]s", address);

    if(x == '/') {
        new_address = address;
    }else if(x == '"') {
        new_address = address + 1;
        new_address[strlen(new_address) - 1] = '\0';
    }

    directory = strtok(new_address, "/");

    // printf("%s\n", directory);
    while(directory != NULL)
    {
        printf("dir = %s\n", directory);
        counter++;
        next_directory = strtok(NULL, "/");
        if(next_directory == NULL) {
            if(!access(directory, F_OK)) {
                printf("File already exist!\n");
                return;
            }
            filepointer = fopen(directory, "a");
            fclose(directory);
            printf("File created successfully!\n");
            break;
        }
        mkdir(directory); 
        chdir(directory);

        // mkdir(directory);
        strcpy(directory, next_directory);
    }
    // printf("count = %d\n", counter);
    
    while(counter != 1)
    {
        chdir("..");
        counter--;
    }
    return;
}
