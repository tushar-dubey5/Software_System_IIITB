/*
============================================================================
Name : 8.c
Author : Tushar Dubey
Description : Write a program to open a file in read only mode, read line by line and display each line as it is read.
                Close the file when end of file is reached.
Date: 30th Aug, 2024.
============================================================================
*/


#include <stdio.h>
#include<unistd.h>

int main() {
    FILE *file = fopen("example3.txt", "r");  
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) { 
        putchar(ch);  

        if (ch == '\n') {
           sleep(1);
        }
    }

    fclose(file); 
    return 0;
}
