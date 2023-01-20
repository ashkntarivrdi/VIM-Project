#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h> 
#include <sys/types.h>

#define MAX 10000

FILE *filepointer;
char command[MAX];
char command_extension[MAX];
char* address[MAX];

void cat();
void createfile();
void FindCommand();


int main()
{
    scanf("%[^ ]s", command);
    FindCommand();

    return 0;    
}

void FindCommand()
{
    if(strcmp(command, "cat") == 0) 
        cat();
    else 
        printf("Invalid Command!\nSimply type <--help> for more information.");

}

void cat() 
{
    scanf(" %[^ ]s", command_extension);
    scanf(" /root/%[^\n]s", address);

    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nSimply type <--help> for more information.");
        return;
    }

    int str[MAX];
    filepointer = fopen(address , "r");
    if(filepointer != NULL) {
        while(fgets(str, MAX, filepointer)) {
        printf("%s", str);
        }
    } else {
        printf("No Such A File!");
    }
    fclose(filepointer);
}

void createfile()
{
    scanf(" %[^ ]s", command_extension);
    scanf(" /root/%[^\n]s", address);    
}
