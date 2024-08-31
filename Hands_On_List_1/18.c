/*
============================================================================
Name : 18.c
Author : Tushar Dubey
Description : Write a program to perform Record locking.
                a. Implement write lock
                b. Implement read lock
            Create three records in a file. Whenever you access a particular record, first lock it then modify/accessto avoid race condition
Date: 31th Aug, 2024.
============================================================================
*/



#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define RECORD_SIZE 50 
#define NUM_RECORDS 3  

void lock_record(int fd, int record_num, short lock_type) 
{
    struct flock lock;

    lock.l_type = lock_type;  
    lock.l_whence = SEEK_SET;
    lock.l_start = record_num * RECORD_SIZE;  
    lock.l_len = RECORD_SIZE;               

    if (fcntl(fd, F_SETLKW, &lock) == -1) 
    {
        perror("fcntl");
        exit(0);
    }
}


void unlock_record(int fd, int record_num) 
{
    struct flock lock;

    lock.l_type = F_UNLCK; 
    lock.l_whence = SEEK_SET;
    lock.l_start = record_num * RECORD_SIZE;
    lock.l_len = RECORD_SIZE;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
        exit(0);
    }
}

int main() 
{
    int fd;
    char buffer[RECORD_SIZE];
    int record_num;

    fd = open("records.txt", O_RDWR | O_CREAT, 0644);
    if(fd == -1) 
    {
        perror("open");
        exit(0);
    }

    for(int i = 0; i < NUM_RECORDS; i++) 
    {
        snprintf(buffer, sizeof(buffer), "Record %d\n", i + 1);
        write(fd, buffer, strlen(buffer)); 
        lseek(fd, (i + 1) * RECORD_SIZE - 1, SEEK_SET);  
        write(fd, "\0", 1); 
    }
    printf("Enter the record number to access (0 to 2): ");
    scanf("%d", &record_num);

    if (record_num < 0 || record_num >= NUM_RECORDS) {
        printf("Invalid record number.\n");
        close(fd);
        exit(0);
    }

    lock_record(fd, record_num, F_RDLCK);
    printf("Record %d locked for reading.\n", record_num);

    lseek(fd, record_num * RECORD_SIZE, SEEK_SET);
    read(fd, buffer, RECORD_SIZE);
    printf("Read from record %d: %s\n", record_num, buffer);

    unlock_record(fd, record_num);
    printf("Record %d unlocked after reading.\n", record_num);

    lock_record(fd, record_num, F_WRLCK);
    printf("Record %d locked for writing.\n", record_num);

    printf("Enter new data for record %d: ", record_num);
    scanf(" %[^\n]", buffer);
    lseek(fd, record_num * RECORD_SIZE, SEEK_SET);
    write(fd, buffer, strlen(buffer));
    lseek(fd, (record_num + 1) * RECORD_SIZE - 1, SEEK_SET); 
    write(fd, "\0", 1);

    unlock_record(fd, record_num);
    printf("Record %d unlocked after writing.\n", record_num);

    close(fd);


    return 0;
}

//OUTPUT
/*Enter the record number to access (0 to 2): 1
Record 1 locked for reading.
Read from record 1: Record 2

Record 1 unlocked after reading.
Record 1 locked for writing.
Enter new data for record 1: 1212
Record 1 unlocked after writing.*/