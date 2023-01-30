#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h> 
#include <sys/types.h>
#include <direct.h>
#include <windows.h>
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


#define MAX 10000
#define MAX_CONTENT 2048
#define MAX_LINE 2048

FILE *filepointer;
char command[MAX];
char command_extension[MAX];
char address[MAX];

void cat();
void createfile();
void insertstr();
void FindCommand();


int main()
{
    while(1)
    {
    // char buff[FILENAME_MAX];
    // GetCurrentDir( buff, FILENAME_MAX);
    // printf("%s\n", buff);
    // printf("------------------------------------\n");
    scanf("%s", command);
    if(!strcmp(command, "exit"))
        break;
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
    else if(!strcmp(command, "insertstr")) {
        insertstr();
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
        printf("Invalid Command!\nTry cat --file!\n");
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
        printf("Invalid Command!\nTry createfile --file!\n");
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
        // printf("dir = %s\n", directory);
        counter++;
        next_directory = strtok(NULL, "/");
        if(next_directory == NULL) {
            if(!access(directory, F_OK)) {
                printf("File already exist!\n");
                return;
            }
            filepointer = fopen(directory, "a");
            fclose(filepointer);
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

void insertstr() 
{
    FILE *temp;
    char *new_address;
    char str[MAX_LINE];
    char *new_str;
    char x;
    int row, column, current_row = 0, current_column = 0;

    scanf(" %[^ ]s", command_extension);
    // printf("command ext = i%si\n", command_extension);

    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nTry insertstr --file or --str or --pos!\n");
        return;
    }

    getchar();
    scanf("%c", &x);
    // printf("x = %c\n", x);

    if(x == '/') {
        scanf("%s", address);
        new_address = address;
        x = '\0';
    }else if(x == '"') {
        scanf("%[^\"]s", address);
        new_address = address + 1;
        // new_address[strlen(new_address) - 1] = '\0';
        x = '\0';
    }
    // printf("address = i%si\n", new_address);

    getchar();
    scanf(" %[^ ]s", command_extension);
    // printf("command ext = i%si\n", command_extension);

    if(strcmp(command_extension, "--str") != 0) {
        printf("Invalid Command!\nTry insertstr --file or --str or --pos!\n");
        return;
    }

    getchar();
    scanf("%c", &x);
    // printf("c = %c\n", x);
    if(x == '"') {
        scanf("%[^-]s", str);
        new_str = str;
        new_str[strlen(new_str) - 2] = '\0';
    }else {
        scanf("%s", str);
        new_str = str;
        for(int i = sizeof(str) - 2; i >=0; i--) {
            new_str[i + 1] = new_str[i];
        }
        new_str[0] = x;
    }
    int length = strlen(new_str);
    // printf("new str = i%si\n", new_str);

    scanf("%[^ ]s", command_extension);
    // printf("command ext = i%si\n", command_extension);

    if(strcmp(command_extension, "--pos") != 0) {
        printf("Invalid Command!\nTry insertstr --file or --str or --pos!\n");
        return;
    }

    scanf("%d:%d", &row, &column);

    filepointer = fopen(new_address, "r");
    if(filepointer == NULL) {
        printf("File Doesn't Exist!\n");
        return;
    }
    temp = fopen("temp", "w");


    while(current_row != row - 1) 
    { 
        x = fgetc(filepointer);
        if(x == '\n') {
            current_row++;
        }
        if(x == EOF) {
            x = '\n';
        }
        fputc(x, temp);
    }

    while(current_column != column)
    {
        x = fgetc(filepointer);
        if(x == EOF) {
            x = ' ';
        }
        fputc(x, temp);
        current_column++;
    }

    for(int i = 0; i < length; i++)
    {
        if(new_str[i] == '\\' && new_str[i + 1] == 'n') {
            fprintf(temp, "\n");
            i++;
        }else if(new_str[i] == '\\' && new_str[i + 1] == '\\' && new_str[i + 2] == 'n') { 
            fprintf(temp, "\\n");
            i += 2;
        }else {
            x = new_str[i];
            fputc(x, temp);
        }
    }

    while((x = fgetc(filepointer)) != EOF)
    {
        fputc(x, temp);
    }

    fclose(filepointer);
    fclose(temp);

    filepointer = fopen(new_address, "w");
    temp = fopen("temp", "r");

    x = fgetc(temp);
    while(x != EOF)
    {
        fputc(x, filepointer);
        x = fgetc(temp);
    }

    fclose(temp);
    fclose(filepointer);

    int result = remove("temp");
    if(!result) {
        printf("Operation done successfully!\n");
    }
}