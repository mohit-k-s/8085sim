#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <Simulator.h>

//file type opened
enum
{
	binary=0,
	text
};


int main(int argc, char const *argv[])
{
	int mode;
	//init
	if(argc==1)	//no file specified
	{
		fprintf(stderr,"No file specified\n");
		printf("type -h for help\n");
		return -1;
	}
	else if(argc==2)	//for help and info
	{
		if(strcmp(argv[1],"-h")==0)
		{
			printf("\n");
			printf("8085sim.exe [options] [filename]\n");
			printf("[options]\n");
			printf("\t\t-b: For loading binary files\n");
			printf("\t\t-t: For loading text based assembly files\n");
			printf("[filename]\n");
			printf("\t\tfilename: enter filename to be executed\n");
		}
		return 0;
	}
	else if(argc==3)	//main -b filename
	{	
		if(strcmp(argv[1],"-b")==0)
		{
			printf("binary mode\n");
			mode=binary;
		}
		else if(strcmp(argv[1],"-t")==0)
		{
			printf("text mode\n");
			mode=text;
		}
	}
	else
	{
		fprintf(stderr,"Too many inputs\n");
		printf("type -h for help\n");
		return -1;
	}

	//all arguments are set initialise the simulator
	Simulator simulator;
	uint16_t start;
	cleanmemory(&simulator);
	//read file
	if(mode==0)	//binary
	{
		FILE *fp=fopen(argv[2],"rb");	//read binary
		if(fp!=NULL)					//file opened
		{
			//read binary mode
		}
		else
		{
			fprintf(stderr,"Couldn't open file: ");
			fprintf(stderr,argv[2]);
			fprintf(stderr,"\n");
			return -1;
		}
		fclose(fp);
	}
	else if(mode==1)	//text
	{
		//pass
		//change the text file in assembled binary file
		FILE *fp=fopen(argv[2],"r");	//read binary
		if(fp!=NULL)					//file opened
		{
			//read text mode
		}
		else
		{
			fprintf(stderr,"Couldn't open file: ");
			fprintf(stderr,argv[2]);
			fprintf(stderr,"\n");
			return -1;
		}
		fclose(fp);
	}
	//run program
	init(&simulator,start);

	run(&simulator);

	//dump memory after running program to a bin file
	dump(&simulator,"output.bin");
	//cleanup
	cleanmemory(&simulator);
	return 0;
}