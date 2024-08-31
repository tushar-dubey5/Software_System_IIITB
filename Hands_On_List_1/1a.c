/*
============================================================================
Name : 1a.c
Author : Tushar Dubey
Description : Create the following types of a files using (i) shell command (ii) system call
 				a. soft link (symlink system call)
Date: 30th Aug, 2024.
============================================================================
*/


#include <stdio.h>
#include <unistd.h>

int main()
{
	const char *target_file = "file1";
	const char *symblink_name = "file1_soft_link";
	
	if(symlink(target_file, symblink_name) == -1)
	{
		perror("symlink");
		return 1;
	}
	printf("Success");
	return 0;
}
//Output
//Success