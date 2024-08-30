/*
============================================================================
Name : 3.c
Author : Tushar Dubey
Description : Write a program to create a file and print the file descriptor value. Use creat ( ) system call
Date: 30th Aug, 2024.
============================================================================
*/
// 17. Write a program to simulate online ticket reservation. Implement write lock
// Write a program to open a file, store a ticket number and exit. Write a separate program, to
// open the file, implement write lock, read the ticket number, increment the number and print
// the new ticket number then close the file.
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
int main(){
    int ticket_Number = 3000;
    struct flock lock;
    int fd = open("ticket.txt",O_WRONLY|O_CREAT,0600);
    if(fd ==-1){
        perror("Error opening/creating the file!..\n");
        return 1;
    }
    if(write(fd,&ticket_Number,sizeof(ticket_Number)-1)==-1){
        perror("Error while writing the initial ticket number \n");
        return 1;
    
    }
    printf("Successfully Writtin the initial value of ticket as %d \n", ticket_Number);
    close(fd);
    printf("Now we will open the file and update the ticket value that we initially wrote\n ");
     fd = open("ticket.txt", O_RDWR);
     if(fd ==-1){
        perror("Error opening the file!\n");
        return 1;
     }
    lock.l_type = F_WRLCK;    // Write lock
    lock.l_whence = SEEK_SET; // From the beginning of the file
    lock.l_start = 0;         // Start at the beginning of the file
    lock.l_len = 0;           // Lock the whole file
    lock.l_pid = getpid();   
  if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error applying write lock");
        close(fd);
      return 1;
    }
    if (read(fd, &ticket_Number, sizeof(ticket_Number)) == -1) {
        perror("Error reading from file");
        close(fd);
        return 1;
    }
     ticket_Number++;
     if (write(fd, &ticket_Number, sizeof(ticket_Number)) == -1) {
        perror("Error writing to file");
        close(fd);
        return 1;
    }
        printf("New ticket number is %d\n", ticket_Number);

    
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error releasing write lock");
        close(fd);
      return 1;
    }

 
    close(fd);

    return 0;

}

//OUTPUT
/*Successfully Writtin the initial value of ticket as 3000 
Now we will open the file and update the ticket value that we initially wrote
 New ticket number is 3001*/