/////////////////////////////////////////////////////////
// File Name       : gen_file.c                        //
// Date            : 2020/10/23 ~ 2020/11/05           //
// OS              : Ubuntu 16.04.3 LTS                //
// Student Name    : Seung Hoon Jeong                  //
// Student ID      : 2015707003                        //
// -----------------------------------------           //
// Title : OS_Assignment3-1                            //
// Description : random integer write on text file     //
/////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 64

int main(void)
{
	//// file open ////
	FILE *f_write = fopen("./temp.txt","wt");
	if(f_write != NULL){
		for(int i=0; i<MAX_PROCESSES*2; i++) // write int number on text
			fprintf(f_write, "%d\n", 2+rand()%20);
	}
	fclose(f_write);
	//// file close ////
	return 0;
}
