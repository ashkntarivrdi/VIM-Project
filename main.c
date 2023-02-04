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


#define MAX_CONTENT 2048
#define MAX_LINE 2048
#define MAX_ADD 1024

FILE *filepointer;
char command[MAX_ADD];
char command_extension[MAX_ADD];
char address[MAX_ADD];

void cat();
void createfile();
void insertstr();
void removestr();
void copystr();
void cutstr();
void pastestr();
void compare();
void grep();
void get_tree();
void undo();
void closingpair();
void FindCommand();


void tree(char *basepath, const int root, int depth, int counter);
void copyfile(char *filename1, char *filename2);
void hidefile(char *filename);
void showfile(char *filename);
char* namefile(const char *address);
const char* fileaddress(char *address);


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
    else if(!strcmp(command, "removestr")) {
        removestr();
        return;
    }
    else if(!strcmp(command, "copystr")) {
        copystr();
        return;
    }
    else if(!strcmp(command, "cutstr")) {
        cutstr();
        return;
    }
    else if(!strcmp(command, "pastestr")) {
        pastestr();
        return;
    }
    else if(!strcmp(command,"compare")) {
        compare();
        return;
    }
    else if(!strcmp(command, "grep")) {
        grep();
        return;
    }
    else if(!strcmp(command, "tree")) {
        get_tree();
        return;
    }
    else if(!strcmp(command, "undo")) {
        undo();
        return;
    }
    else if(!strcmp(command, "auto-indent")) {
        closingpair();
        return;
    }
    else {
        char trash[MAX_CONTENT];
        scanf("%[^\n]s", trash);
        printf("Invalid Command!\nSimply type <--help> for more information.\n");
        return;
    }

}

void copyfile(char *filename1, char *filename2)
{
    FILE *file1, *file2;
    //file1-->destination
    char x;
    file1 = fopen(filename1, "w");
    file2 = fopen(filename2, "r");

    x = fgetc(file2);
    while(x != EOF)
    {
        fputc(x, file1);
        x = fgetc(file2);
    }

    fclose(file1);
    fclose(file2);
}

void hidefile(char *filename)
{
    int attr = GetFileAttributes(filename);
    if((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
        SetFileAttributes(filename, attr | FILE_ATTRIBUTE_HIDDEN);
    }
}

void showfile(char *filename)
{
    int attr = GetFileAttributes(filename);
    if ((attr & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN) {
        SetFileAttributes(filename, attr & ~FILE_ATTRIBUTE_HIDDEN);
    }
}

const char *fileaddress( char *address)
{
    char *new_address = malloc(sizeof(char) * MAX_CONTENT);
    strcpy(new_address, address);
    int i = strlen(new_address) - 1;
    while(new_address[i] != '/')
    {
        new_address[i] = '\0';
        i--;
    }
    new_address[i] = '\0';
    return new_address;
}

char *namefile(const char *new_address)
{
        char *filename = malloc(sizeof(char) * MAX_CONTENT);   
        int count = 0;
        int k = strlen(new_address) - 1;
        while(new_address[k] != '/')
        {
            count++;
            k--;
        }

        for(int s = 0; s < MAX_CONTENT; s++) {
            filename[s] = '\0';
        }

        int l = count - 1;
        k = strlen(new_address) - 1;
        while(new_address[k] != '/')
        {
            filename[l] = new_address[k];
            k--;
            l--;
        }
        filename[strlen(filename)] = '1';
        filename[strlen(filename) + 1] = '\0';
    return filename;
}

void cat() 
{
    char x;
    char *new_address = malloc(sizeof(char) * MAX_ADD);

    scanf(" %[^ ]s", command_extension);

    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nTry cat --file!\n");
        free(new_address);
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
        }
        printf("\n");
    }else {
        printf("No Such A File!\n");
        free(new_address);
        return;
    }
    fclose(filepointer);
    free(new_address);
}

void createfile()
{
    char *directory = malloc(sizeof(char) * MAX_ADD);
    char *next_directory = malloc(sizeof(char) * MAX_ADD);
    char *new_address = malloc(sizeof(char) * MAX_ADD);
    char x;
    int counter = 0;
    int result;

    scanf(" %[^ ]s", command_extension);

    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nTry createfile --file!\n");
        free(directory);
        free(next_directory);
        free(new_address);
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
                free(directory);
                free(next_directory);
                free(new_address);
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
    free(directory);
    free(next_directory);
    free(new_address);
}

void insertstr() 
{
    FILE *temp;
    char *new_address = malloc(sizeof(char) * MAX_ADD);
    char str[MAX_LINE];
    char *new_str = malloc(sizeof(char) * MAX_CONTENT);
    char x;
    int row, column, current_row = 0, current_column = 0;

    scanf(" %[^ ]s", command_extension);
    // printf("command ext = i%si\n", command_extension);

    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nTry insertstr --file or --str or --pos!\n");
        free(new_address);
        free(new_str);
        return;
    }

    getchar();
    scanf("%c", &x);
    // printf("x = %c\n", x);

    if(x == '/') {
        scanf("%s", address);
        new_address = address;
    }else if(x == '"') {
        scanf("%[^\"]s", address);
        new_address = address + 1;
        // new_address[strlen(new_address) - 1] = '\0';
    }
    // printf("address = i%si\n", new_address);

    if(x == '"')
        getchar();
    getchar();
    scanf(" %[^ ]s", command_extension);
    // printf("command ext = i%si\n", command_extension);

    if(strcmp(command_extension, "--str") != 0) {
        printf("Invalid Command!\nTry insertstr --file or --str or --pos!\n");
        free(new_address);
        free(new_str);
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
        free(new_address);
        free(new_str);
        return;
    }

    scanf("%d:%d", &row, &column);
    
    //undo
    char new_path[MAX_CONTENT] = { 0 };
    strcpy(new_path, fileaddress(new_address));
    strcat(new_path, "/");
    strcat(new_path, namefile(new_address));
    // printf("new : i%si\n", new_path);
    showfile(new_path);
    copyfile(new_path, new_address);
    hidefile(new_path);

    filepointer = fopen(new_address, "r");
    if(filepointer == NULL) {
        printf("File Doesn't Exist!\n");
        free(new_address);
        free(new_str);
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
        printf("Text Inserted Successfully!\n");
    }
    free(new_address);
    free(new_str);
}

void removestr()
{
    FILE *temp;
    char *new_address = malloc(sizeof(char) * MAX_ADD);
    char x;
    int row, column, current_row = 1, current_column = 0, size;

    scanf(" %[^ ]s", command_extension);

    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nTry removestr --file!\n");
        free(new_address);
        return;
    }

    getchar();
    scanf("%c", &x);

    if(x == '/') {
        scanf("%s", address);
        new_address = address;
    }else if(x == '"') {
        scanf("%[^\"]s", address);
        new_address = address + 1;
    }
    
    if(x == '"')
        getchar();
    getchar();
    scanf("%[^ ]s", command_extension); 

    if(strcmp(command_extension, "--pos") != 0) {
        printf("Invalid Command!\nTry removestr --file or --pos!\n");
        free(new_address);    
        return;
    }

    scanf("%d:%d", &row, &column);

    getchar();
    scanf("%[^ ]s", command_extension);   

    if(strcmp(command_extension, "-size") != 0) {
        printf("Invalid Command!\nTry removestr --file or --pos or -size!\n");
        free(new_address);
        return;
    } 

    scanf("%d", &size);

    getchar();
    scanf("%[^\n]s", command_extension);

    //undo
    char new_path[MAX_CONTENT] = { 0 };
    strcpy(new_path, fileaddress(new_address));
    strcat(new_path, "/");
    strcat(new_path, namefile(new_address));
    // printf("new : i%si\n", new_path);
    showfile(new_path);
    copyfile(new_path, new_address);
    hidefile(new_path);
    
    filepointer = fopen(new_address, "r");
    if(filepointer == NULL) {
        printf("File Doesn't Exist!\n");
        free(new_address);
        return;
    }
    temp = fopen("temp", "w");
    
    if(!strcmp(command_extension, "-b")) {

        while(current_row != row) 
        {       
            x = fgetc(filepointer);
            if(x == '\n') {
                current_row++;
            }
            if(x == EOF) {
                printf("Out Of Range Position!\n");
                fclose(filepointer);
                fclose(temp);
                remove("temp");
                free(new_address);
                return;
            }            
        }

        while(current_column != column)
        {
            x = fgetc(filepointer);
            if(x == EOF || x == '\n') {
                printf("Out Of Range Position!\n");
                fclose(filepointer);
                fclose(temp);
                remove("temp");
                free(new_address);
                return;
            }
            current_column++;
        }

        for(int i = size; i > 0; i--)
        {
            if(current_column == 0 && current_row != 1) {
                current_row--;
                fclose(filepointer);
                filepointer = fopen(new_address, "r");
                
                row = current_row;
                current_row = 1;
                while(current_row != row) 
                {       
                    x = fgetc(filepointer);
                    if(x == '\n') {
                        current_row++;
                    }
                }
                while((x = fgetc(filepointer)) != '\n') {
                    current_column++;
                }
            } else {
                current_column--;
            }
            if(current_column < 0) {
                printf("Out Of Range Position!\n");
                fclose(filepointer);
                fclose(temp);
                remove("temp");
                free(new_address);
                return;
            }
        }

        row = current_row;
        column = current_column;
        current_column = 0;
        current_row = 1;

        fclose(filepointer);
        fclose(temp);

        filepointer = fopen(new_address, "r");
        temp = fopen("temp", "w");

        while(current_row != row) 
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

        while(current_column != column + size) 
        {
            x = fgetc(filepointer);
            current_column++;
        }

        while((x = fgetc(filepointer)) != EOF)
        {
            fputc(x, temp);
        }

        fclose(filepointer);
        fclose(temp);

    }else if(!strcmp(command_extension, "-f")) {
        current_row = 0;
        
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

        while(current_column != column + size) 
        {
            x = fgetc(filepointer);
            current_column++;
        }

        while((x = fgetc(filepointer)) != EOF)
        {
            fputc(x, temp);
        }

        fclose(filepointer);
        fclose(temp);

    }else {
        printf("Invalid Command!\nTry insertstr --file or --pos or -size or -b/-f\n");
        fclose(filepointer);
        fclose(temp);
        remove("temp");
        free(new_address);
        return;
    }

    filepointer = fopen(new_address, "w");
    temp = fopen("temp", "r");

    //copy temp into main file
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
        printf("Text Removed Successfully!\n");
    }

    free(new_address);
}

void copystr() 
{
    FILE *temp;
    char *new_address = malloc(sizeof(char) * MAX_ADD);
    char x;
    int row, column, current_row = 1, current_column = 0, size;
        
    scanf(" %[^ ]s", command_extension);

    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nTry copystr --file!\n");
        free(new_address);
        return;
    }
    // printf("comm ext : i%si\n", command_extension);

    getchar();
    scanf("%c", &x);
    // printf("x : %c\n", x);

    if(x == '/') {
        scanf("%s", address);
        new_address = address;
    }else if(x == '"') {
        scanf("%[^\"]s", address);
        new_address = address + 1;
    }
    // printf("new address : i%si\n", new_address);
    if(x == '"')
        getchar();
    getchar();
    scanf("%[^ ]s", command_extension); 
    // printf("comm ext : i%si\n", command_extension);

    if(strcmp(command_extension, "--pos") != 0) {
        printf("Invalid Command!\nTry copystr --file or --pos!\n");
        free(new_address);
        return;
    }

    scanf("%d:%d", &row, &column);

    getchar();
    scanf("%[^ ]s", command_extension);   
    // printf("comm ext : i%si\n", command_extension);

    if(strcmp(command_extension, "-size") != 0) {
        printf("Invalid Command!\nTry copystr --file or --pos or -size!\n");
        free(new_address);
        return;
    } 

    scanf("%d", &size);
    // printf("size : %d\n", size);

    getchar();
    scanf("%[^\n]s", command_extension);
    // printf("comm ext : i%si\n", command_extension);

    
    filepointer = fopen(new_address, "r");
    if(filepointer == NULL) {
        printf("File Doesn't Exist!\n");
        free(new_address);
        return;
    }
    temp = fopen("TempCopy", "w");

    if(!strcmp(command_extension, "-b")) {

        while(current_row != row)
        {
            x = fgetc(filepointer);
            if(x == '\n') {
                current_row++;
            }
            if(x == EOF) {
                printf("Out Of Range Position!\n");
                fclose(filepointer);
                fclose(temp);
                remove("temp");
                free(new_address);
                return;
            }            
        }

        while(current_column != column)
        {
            x = fgetc(filepointer);
            if(x == EOF || x == '\n') {
                printf("Out Of Range Position!\n");
                fclose(filepointer);
                fclose(temp);
                remove("temp");
                free(new_address);
                return;
            }
            current_column++;
        }

        for(int i = size; i > 0; i--)
        {
            if(current_column == 0 && current_row != 1) {
                current_row--;
                fclose(filepointer);
                filepointer = fopen(new_address, "r");
                
                row = current_row;
                current_row = 1;
                while(current_row != row) 
                {       
                    x = fgetc(filepointer);
                    if(x == '\n') {
                        current_row++;
                    }
                }
                while((x = fgetc(filepointer)) != '\n') {
                    current_column++;
                }
            } else {
                current_column--;
            }
            if(current_column < 0) {
                printf("Out Of Range Position!\n");
                fclose(filepointer);
                fclose(temp);
                remove("temp");
                free(new_address);
                return;
            }
        }

        row = current_row;
        column = current_column;
        current_column = 0;
        current_row = 1;

        fclose(filepointer);
        fclose(temp);

        filepointer = fopen(new_address, "r");
        temp = fopen("TempCopy", "w");

        while(current_row != row) 
        {    
            x = fgetc(filepointer);
            if(x == '\n') {
                current_row++;
            }
        }

        while(current_column != column)
        {
            x = fgetc(filepointer);
            current_column++;
        }

        while(current_column != column + size) 
        {
            x = fgetc(filepointer);
            fputc(x, temp);
            current_column++;
        }

        fclose(filepointer);
        fclose(temp);
        printf("Text Copied Successfully!\n");


    }else if(!strcmp(command_extension, "-f")) {
        current_row = 0;

        while(current_row != row - 1) 
        {    
            x = fgetc(filepointer);
            if(x == '\n') {
                current_row++;
            }
            if(x == EOF) {
                printf("Out Of Range Position!\n");
                fclose(filepointer);
                fclose(temp);
                remove("temp");
                free(new_address);
                return;
            }
        }

        while(current_column != column)
        {
            x = fgetc(filepointer);
            if(x == EOF) {
                printf("Out Of Range Position!\n");
                fclose(filepointer);
                fclose(temp);
                remove("temp");
                free(new_address);
                return;
            }
            current_column++;
        }

        while(current_column != column + size) 
        {
            x = fgetc(filepointer);
            fputc(x, temp);
            current_column++;
        }

    fclose(filepointer);
    fclose(temp);
    printf("Text Copied Successfully!\n");

    } else {
        printf("Invalid Command!\nTry insertstr --file or --pos or -size or -b/-f\n");
        fclose(filepointer);
        fclose(temp);
        remove("temp");
        free(new_address);
        return;
    }
    
    free(new_address);
}

void cutstr()
{
    FILE *TempCopy, *temp;
    char *new_address = malloc(sizeof(char) * MAX_ADD);
    char x;
    int row, column, current_row = 1, current_column = 0, size;
        
    scanf(" %[^ ]s", command_extension);

    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nTry cutstr --file!\n");
        free(new_address);
        return;
    }
    // printf("comm ext : i%si\n", command_extension);

    getchar();
    scanf("%c", &x);
    // printf("x : %c\n", x);

    if(x == '/') {
        scanf("%s", address);
        new_address = address;
    }else if(x == '"') {
        scanf("%[^\"]s", address);
        new_address = address + 1;
    }
    // printf("new address : i%si\n", new_address);
    if(x == '"')
        getchar();
    getchar();
    scanf("%[^ ]s", command_extension); 
    // printf("comm ext : i%si\n", command_extension);

    if(strcmp(command_extension, "--pos") != 0) {
        printf("Invalid Command!\nTry cutstr --file or --pos!\n");
        free(new_address);
        return;
    }

    scanf("%d:%d", &row, &column);

    getchar();
    scanf("%[^ ]s", command_extension);   
    // printf("comm ext : i%si\n", command_extension);

    if(strcmp(command_extension, "-size") != 0) {
        printf("Invalid Command!\nTry cutstr --file or --pos or -size!\n");
        free(new_address);
        return;
    } 

    scanf("%d", &size);
    // printf("size : %d\n", size);

    getchar();
    scanf("%[^\n]s", command_extension);
    // printf("comm ext : i%si\n", command_extension);

    //undo
    char new_path[MAX_CONTENT] = { 0 };
    strcpy(new_path, fileaddress(new_address));
    strcat(new_path, "/");
    strcat(new_path, namefile(new_address));
    // printf("new : i%si\n", new_path);
    showfile(new_path);
    copyfile(new_path, new_address);
    hidefile(new_path);
    
    filepointer = fopen(new_address, "r");
    if(filepointer == NULL) {
        printf("File Doesn't Exist!\n");
        free(new_address);
        return;
    }
    TempCopy = fopen("TempCopy", "w");
    temp = fopen("temp", "w");

    if(!strcmp(command_extension, "-b")) {

        while(current_row != row)
        {
            x = fgetc(filepointer);
            if(x == '\n') {
                current_row++;
            }
            if(x == EOF) {
                printf("Out Of Range Position!\n");
                fclose(filepointer);
                fclose(TempCopy);
                fclose(temp);
                remove("temp");
                remove("TempCopy");
                free(new_address);
                return;
            }            
        }

        while(current_column != column)
        {
            x = fgetc(filepointer);
            if(x == EOF || x == '\n') {
                printf("Out Of Range Position!\n");
                fclose(filepointer);
                fclose(TempCopy);
                fclose(temp);
                remove("temp");
                remove("TempCopy");
                free(new_address);
                return;
            }
            current_column++;
        }

        for(int i = size; i > 0; i--)
        {
            if(current_column == 0 && current_row != 1) {
                current_row--;
                fclose(filepointer);
                filepointer = fopen(new_address, "r");
                
                row = current_row;
                current_row = 1;
                while(current_row != row) 
                {       
                    x = fgetc(filepointer);
                    if(x == '\n') {
                        current_row++;
                    }
                }
                while((x = fgetc(filepointer)) != '\n') {
                    current_column++;
                }
            } else {
                current_column--;
            }
            if(current_column < 0) {
                printf("Out Of Range Position!\n");
                fclose(filepointer);
                fclose(TempCopy);
                fclose(temp);
                remove("temp");
                remove("TempCopy");
                free(new_address);
                return;
            }
        }

        row = current_row;
        column = current_column;
        current_column = 0;
        current_row = 1;

        fclose(filepointer);
        fclose(TempCopy);
        fclose(temp);

        filepointer = fopen(new_address, "r");
        TempCopy = fopen("TempCopy", "w");
        temp = fopen("temp", "w");

        while(current_row != row) 
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

        while(current_column != column + size) 
        {
            x = fgetc(filepointer);
            fputc(x, TempCopy);
            current_column++;
        }

        while((x = fgetc(filepointer)) != EOF)
        {
            fputc(x, temp);
        }

        fclose(filepointer);
        fclose(TempCopy);
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
        printf("Text Cut Successfully!\n");
    }

    }else if(!strcmp(command_extension, "-f")) {
        current_row = 0;

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

        while(current_column != column + size) 
        {
            x = fgetc(filepointer);
            fputc(x, TempCopy);
            current_column++;
        }

        while((x = fgetc(filepointer)) != EOF)
        {
            fputc(x, temp);
        }

        fclose(filepointer);
        fclose(TempCopy);
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
        printf("Text Cut Successfully!\n");
    }

    } else {
        printf("Invalid Command!\nTry cutstr --file or --pos or -size or -b/-f\n");
        fclose(filepointer);
        fclose(TempCopy);
        fclose(temp);
        remove("temp");
        remove("TempCopy");
        free(new_address);
        return;
    }
    free(new_address);
}

void pastestr()
{
    FILE *temp;
    FILE *TempCopy;
    char *new_address = malloc(sizeof(char) * MAX_ADD);
    char x;
    int row, column, current_row = 0, current_column = 0;

    scanf(" %[^ ]s", command_extension);

    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nTry pastestr --file!\n");
        free(new_address);
        return;
    }

    getchar();
    scanf("%c", &x);

    if(x == '/') {
        scanf("%s", address);
        new_address = address;
    }else if(x == '"') {
        scanf("%[^\"]s", address);
        new_address = address + 1;
    }
    if(x == '"')
        getchar();
    getchar();
    scanf("%[^ ]s", command_extension); 

    if(strcmp(command_extension, "--pos") != 0) {
        printf("Invalid Command!\nTry pastestr --file or --pos!\n");
        free(new_address);
        return;
    }

    scanf("%d:%d", &row, &column);

    //undo
    char new_path[MAX_CONTENT] = { 0 };
    strcpy(new_path, fileaddress(new_address));
    strcat(new_path, "/");
    strcat(new_path, namefile(new_address));
    // printf("new : i%si\n", new_path);
    showfile(new_path);
    copyfile(new_path, new_address);
    hidefile(new_path);
    
    filepointer = fopen(new_address, "r");
    if(filepointer == NULL) {
        printf("File Doesn't Exist!\n");
        free(new_address);
        return;
    }
    temp = fopen("temp", "w");
    TempCopy = fopen("TempCopy", "r");

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

    while((x = fgetc(TempCopy)) != EOF) 
    {
        fputc(x, temp);
    }

    while((x = fgetc(filepointer)) != EOF)
    {
        fputc(x, temp);
    }

    fclose(filepointer);
    fclose(temp);
    fclose(TempCopy);

    filepointer = fopen(new_address, "w");
    temp = fopen("temp", "r");

    //copy temp into main file
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
        printf("Text Pasted Successfully!\n");
    }
    free(new_address);
}
void compare()
{
    FILE *fptr1, *fptr2;
    char *address1 = malloc(sizeof(char) * MAX_ADD);
    char *address2 = malloc(sizeof(char) * MAX_ADD); 
    char *new_address1 = malloc(sizeof(char) * MAX_ADD); 
    char *new_address2 = malloc(sizeof(char) * MAX_ADD);
    char x;
    int row = 1;
    char str1[1000][1000];
    char str2[1000][1000];


    getchar();
    scanf("%c", &x);
    // printf("x : %c\n", x);
    if(x == '/') {
        scanf("%s", address1);
        new_address1 = address1;
    }else if(x == '"') {
        scanf("%[^\"]s", address1);
        new_address1 = address1 + 1;
    }
    // printf("new address : i%si\n", new_address1);
    
    if(x == '"')
        getchar();
    getchar();
    scanf("%c", &x);
    // printf("x : %c\n", x);

    if(x == '/') {
        scanf("%s", address2);
        new_address2 = address2;
    }else if(x == '"') {
        scanf("%[^\"]s", address2);
        new_address2 = address2 + 1;
    }

    if(x == '"')
        getchar();
    // printf("new address2 : i%si\n", new_address2);

    fptr1 = fopen(new_address1, "r");
    fptr2 = fopen(new_address2, "r");

    int i = 0;

    while(fgets(str1[i], MAX_CONTENT, fptr1))
    {
        i++;
    }
    for(int j = 0; j < i - 1; j++){
        str1[j][strlen(str1[j]) - 1] = '\0';
    }

    int z = 0;

    while(fgets(str2[z], MAX_CONTENT, fptr2))
    {
        z++;
    }
    for(int y = 0 ; y < z - 1; y++){
        str2[y][strlen(str2[y]) - 1] = '\0';
    }
    
    if(i > z) {
        printf("\"Fisrt File > Second File\"\n");
    }else if(i == z) {
        printf("\"Fisrt File = Second File\"\n");
    }else {
        printf("\"First File < Second File\"\n");
    }

    int size1 = i;
    int size2 = z;
    i = 0;
    z = 0;

    while(i < size1 && z < size2)
    {
        if(strcmp(str1[i], str2[z])) {
            printf("======== #%d ========\n", row);
            printf("%s\n", str1[i]);
            printf("%s\n", str2[z]);
        }
        i++;
        z++;
        row++;
    }

    if(size1 > size2) {
        printf(">>>>>>>> #%d - #%d >>>>>>>>\n", i+1, size1);
        while(i < size1) 
        {
            printf("%s\n", str1[i]);
            i++;
        }
    }
    else if(size2 > size1) {
        printf(">>>>>>>> #%d - #%d >>>>>>>>\n", z+1, size2);
        while(z < size2) 
        {
            printf("%s\n", str2[z]);
            z++;
        }
    }
    free(new_address1);
    free(new_address2);
    free(address1);
    free(address2);

}

void grep()
{
    char *new_address = malloc(sizeof(char) * MAX_ADD);
    // char *file_address = malloc(sizeof(char) * MAX_ADD);
    char str[MAX_CONTENT];
    char *new_str = malloc(sizeof(char) * MAX_CONTENT);
    char existing_str[1000][1000];
    char filename[MAX_CONTENT];
    char x;
    int length = 0;
    int IsFirtTime = 0;

    scanf(" %[^ ]s", command_extension);
    // printf("command ext = i%si\n", command_extension);

    if(!strcmp(command_extension, "--str")) {
        IsFirtTime = 0;
    }else if(!strcmp(command_extension, "-c")) {
        IsFirtTime = 1;
        // getchar();
        scanf(" %[^ ]s", command_extension);
        // printf("command ext = i%si\n", command_extension);    
    }else if(!strcmp(command_extension, "-l")) {
        IsFirtTime = 2;
        // getchar();
        scanf(" %[^ ]s", command_extension);
        // printf("command ext = i%si\n", command_extension);
    }

    if(strcmp(command_extension, "--str") != 0) {
        printf("Invalid Command!\nTry grep --str!\n");
        free(new_address);
        free(new_str);
        // free(file_address);
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
    // printf("new str : i%si\n", new_str);
    
    if(x == '/')
        getchar();
    scanf("%[^ ]s", command_extension);
    // printf("command ext = i%si\n", command_extension);

    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nTry grep --str or --file!\n");
        free(new_address);
        free(new_str);
        // free(file_address);
        return;
    }
    int counter = 0;
    while(1)
    {
        scanf("%c", &x);
        // printf("x : %c\n", x);
        if(x == '\n') {
            if(IsFirtTime == 1)
                printf("%d\n", counter);
            break;
            
        }
        scanf("%c", &x);
        // printf("x : %c\n", x);

        if(x == ' ') {
            scanf("%c", &x);
        }
        // printf("x : %c\n", x);

        if(x == '/') {
            scanf("%s", address);
            new_address = address;
            x = '\0';
        }else if(x == '"') {
            scanf("%[^\"]s", address);
            getchar();
            new_address = address + 1;
            x = '\0';
        }
        // printf("new address : i%si\n", new_address);

        filepointer = fopen(new_address, "r");
        if(filepointer == NULL) {
            printf("File Doesn't Exist!\n");
            return;
        }

        //filename
        int count = 0;
        int k = strlen(new_address) - 1;
        while(new_address[k] != '/')
        {
            count++;
            k--;
        }

        for(int s = 0; s < MAX_CONTENT; s++) {
            filename[s] = '\0';
        }

        int l = count - 1;
        k = strlen(new_address) - 1;
        while(new_address[k] != '/')
        {
            filename[l] = new_address[k];
            k--;
            l--;
        }
        // printf("file i%si\n", filename);

        //turn text into two dimensional array
        int i = 0;
        while(fgets(existing_str[i], MAX_CONTENT, filepointer))
        {
            i++;
        }
        for(int j = 0; j < i - 1; j++){
            existing_str[j][strlen(existing_str[j]) - 1] = '\0';
        }
        // printf("i%si\n", existing_str[3]);
        fclose(filepointer);

        length = strlen(new_str);
        int current_line = 0, current_char = 0, target_char = 0;
        char found_str[1000][1000];
        i = 0;

        filepointer = fopen(new_address, "r");
        x = fgetc(filepointer);
        while(x != EOF)
        {
            // printf("fuck\n");
            if(target_char == length) {
                strcpy(found_str[i], existing_str[current_line]); 
                i++;
                current_line++;
                target_char = 0;
                current_char = 0;
            }

            if(existing_str[current_line][current_char] == '\0') {
                current_line++;
                current_char = 0;
                target_char = 0;
            }

            if(existing_str[current_line][current_char] == new_str[target_char]) {
                current_char++;
                target_char++;
            } else {
                if(existing_str[current_line][current_char] == new_str[target_char - 1]) {
                    target_char = 0;
                } else {
                    current_char++;
                    target_char = 0;
                }
            }
            
            x = fgetc(filepointer);
        }

        if(IsFirtTime == 0) {
            for(int t = 0; t < i; t++) {
                printf("%s: %s\n", filename, found_str[t]);
            }
        }
        else if(IsFirtTime == 1) {
            counter += i;
            // printf("%d\n", counter);
        }
        else if(IsFirtTime == 2) {
            printf("%s\n", filename);
        }else {
            printf("No Match:(");
        }
    }

    free(new_address);
    free(new_str);
}

void get_tree()
{
    int depth;
    scanf("%d", &depth);
    if(depth < -1) {
        printf("Depth Must Be Greater Than -1!\n");
        return;
    }
    if(depth == 0) {
        printf("root\n");
        return;
    }

    depth++;
    int counter = depth;
    tree("root", 0, depth, counter);
}

void tree(char *basepath, const int root, int depth, int counter)
{
    int i;
    char path[1000];
    struct dirent *entity;
    DIR *directory = opendir(basepath);

    counter--;
    if(counter == 0) {
        return;
    }

    if (directory == NULL) {
        return;
    }

    while ((entity = readdir(directory)) != NULL)
    {
        if (strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)
        {
            for (i=0; i<root; i++) 
            {
                if (i%2 == 0 || i == 0)
                    printf("%c", 179);
                else
                    printf(" ");

            }

            printf("%c%c%s\n", 195, 196, entity->d_name);

            strcpy(path, basepath);
            strcat(path, "/");
            strcat(path, entity->d_name);
            tree(path, root + 2, depth, counter);
        }
    }

    closedir(directory);
}

void undo()
{
    FILE *undo, *temp;
    char x;
    char *new_address = malloc(sizeof(char) * MAX_ADD);
    char new_path[MAX_CONTENT] = { 0 };

    scanf(" %[^ ]s", command_extension);
    // printf("cmd ext i%si\n", command_extension);

    if(strcmp(command_extension, "--file") != 0) {
        // printf("Invalid Command!\nTry cat --file!\n");
        free(new_address);
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

    // printf("addres i%si\n", new_address);

    filepointer = fopen(new_address, "r");
    temp = fopen("temp", "w");
    
    x = fgetc(filepointer);
    while(x != EOF)
    {
        fputc(x, temp);
        x = fgetc(filepointer);
    }

    fclose(filepointer);
    fclose(temp);

    strcpy(new_path, fileaddress(new_address));
    strcat(new_path, "/");
    strcat(new_path, namefile(new_address));    

    filepointer = fopen(new_address, "w");
    undo = fopen(new_path, "r");

    x = fgetc(undo);
    while(x != EOF)
    {
        fputc(x, filepointer);
        x = fgetc(undo);
    }

    fclose(undo);
    fclose(filepointer);
    
    undo = fopen(new_path, "w");
    temp = fopen("temp", "r");

    showfile(new_path);
    copyfile(new_path, "temp");
    hidefile(new_path);

    fclose(undo);
    fclose(temp);
    
    int result = remove("temp");
    if(!result) {
        printf("Previous File Is Visible Now!\n");
    }
}


void closingpair()
{
    FILE *temp;
    char existing_str[1000][1000];
    char *new_address = malloc(sizeof(char) * MAX_ADD);
    char x;

    getchar();
    scanf("%c", &x);
    printf("x : i%ci\n", x);
    scanf(" %[^\n]s", address);

    if(x == '/') {
        new_address = address;
    }else if(x == '"') {
        new_address = address + 1;
        new_address[strlen(new_address) - 1] = '\0';
    }     

    printf("new add : i%si\n", new_address);
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX);
    printf("%s\n", buff);

    filepointer = fopen(new_address, "r");
    if(filepointer == NULL) {
        printf("File Doesn't Exist!\n");
        free(new_address);
        return;
    }
    temp = fopen("temp", "w");

    //turn text into two dimensional array
    int i = 0;
    while(fgets(existing_str[i], MAX_CONTENT, filepointer))
    {
        i++;
    }
    for(int j = 0; j < i - 1; j++){
        existing_str[j][strlen(existing_str[j]) - 1] = '\0';
    }
    // printf("i%si\n", existing_str[0]);
    
    // int j = 0;
    // i = 0;
    // x = fgetc(filepointer);
    // while(x != EOF)
    // {
    //     if(x == '{') {
    //         if(i == 0) {
    //             fputc(x, temp);
    //             fputc('\n', temp);
    //         } else {
    //             while()
    //         }

    //     }
    //     i++;

    // }

}