#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "constants.h"
#include "errorMasseges.h"
#include "memoryStructs.h"
#include "symbolsTable.h"


/* 	Gets a string array and checks it for errors or if it is a correct assmebly command,
   	data and instructions pointer to be updated how big of a memory it needs, a pointer to the head of the table to build the symbols.updating the address of the words.
	returns 0 if it finds any error or 1 if it is a legal sentence */
int firstScan(char arr[], int * dc, int * ic, ptr *hptr);

/* 	gets a sentence to translate, a pointer to the head of an already built table, memory section to store the code,
   	pointer to the instructions and data counters to be updated while translating, pointer to the entry and externals files. counting the real data amount.
   	returns 1 if it translated successfully or 0 if it finds an error */
int secondScan(char sent[SENTENCE_MAX], ptr head, memorySection memo, int * icPtr, int * dcPtr, FILE* fent, FILE* fext);

/* 	Scans the input file by using two scanning stages, adds the ending needed, builds linked list and the memory section for the object file.
   	Updates the data counter,instructions and total number of words counter that is given to it to their  real size */
/* 	Gets a file pointer, file name, data counter, insturaction and words counter, pointer to the head of the table that needs to be built and a memory section than 	needs to be filled.
   	returns 1 if success or 0 if there are any errors and print them to the stderr */
int translateFile(FILE* fp, char fileName[FILE_NAME_MAX], int * dc, int * ic, int * numOfwords, ptr * hptr, memorySection memory)
{
    char sent[SENTENCE_MAX]; /* the assembly line to be scanned */
    char ch = 1;
    int line = 1, i = 0 ;
    int flagOfError = 0, errorMax;
    int DCF, ICF; /* to store the final dc and ic length */
    char assemblyFile[FILE_NAME_MAX], entryFile[FILE_NAME_MAX], externalFile[FILE_NAME_MAX];
    FILE *fent, *fext, *fentReal;

	strcpy(assemblyFile, fileName); 
	strcat(assemblyFile, ".as");    
	if(!(fp = fopen(assemblyFile , "r")))
    	{
        	fprintf(stderr, "Cannot open the file \"%s\"\n", assemblyFile);
        return 0;
    	}

    	fseek(fp, 0, SEEK_END);
    /* Checks if the file is empty */
    	if(ftell(fp) == 0)
    	{
        	fprintf(stderr, "The file \"%s\" is empty!\n", assemblyFile);
        	fclose(fp);
        return 0;
    	}

    	rewind(fp);
    /*  scan for errors at the file */
    	while(!feof(fp))
    	{
        /* Loops through sentence */
        	while(ch != '\n' && !feof(fp))
        	{
            		errorMax = 0;
            		ch = fgetc(fp);
            		if(i >= SENTENCE_MAX && sent[0] != ';')
            		{
                		fprintf(stderr, "Sentence length in file \"%s\" is too long. Line %d \n", assemblyFile, line);
                		while(ch != '\n' && !feof(fp))
                    		ch = fgetc(fp);
                		errorMax = 1;
            		}
            		if(ch != '\n' && !feof(fp))
                	sent[i++] = ch;
        	}
        	sent[i] = '\0';
        	if(!errorMax)
        	{	    
            		if(errorMax) flagOfError = 1;   
            		else if(!(firstScan(sent, dc, ic, hptr)))/* check if it is legal command */
                	{ 
                    		fprintf(stderr, "line %d in file \"%s\"!\n", line, assemblyFile);
                    		flagOfError = 1;
                	}
        	}     
        	line++;
        	i = 0;
        	if(ch == '\n')
        	{
            		ch = fgetc(fp);
            		sent[i++] = ch;
        	}
    	}/*end of first scanning of file*/

	if(isUnDeclerd(*hptr))/*chek if there is a label that do not declerd and print massege*/
	{	
		fprintf(stderr,"label/s do not declerd in file %s.", assemblyFile);
		flagOfError = 1;
	}
	if(flagOfError) return ERROR;/* if the is an error at thr first scan stop here */
	
    /* create entry and extern files to transfer what needed to be pasted in the translation part */
    	
	strcpy(externalFile, fileName);
    	strcat(externalFile, ".ext");
	if(!(fext = fopen(externalFile, "w+")))
    	{
        	fprintf(stderr, "Can not open the file \"%s\" \n", externalFile);
    	}
	strcpy(entryFile, fileName);
    	strcat(entryFile, ".ent");
	if(!(fent = fopen(entryFile, "w+")))
    	{
        	fprintf(stderr, "Can not open the entry file \"%s\" \n", entryFile);
    	}
	if(!(fentReal = fopen(entryFile, "w+")))
    	{
        	fprintf(stderr, "Can not open the entry file \"%s\" \n", entryFile);
    	}
   
    	ICF = *ic;
    	DCF = *dc;

    	if((ICF + DCF) >= MAX_MEMORY)
    	{
        	fprintf(stderr, "Not enough memory in file: \"%s\"\n", assemblyFile);
    	}
	
    	updateLabelAddress(*hptr, *ic, MEMORY_START);
	
    	*dc =0;
    	*ic = 0;
    	rewind(fp);/*start to build the memory section*/
    	ch = 1;
    	line = 1;
    	i = 0;
    	flagOfError = 0;
    /* Loops to build the memory and scan for errors */
    	while(!feof(fp))
    	{
        	/* Loops through the sentence */
        	while(ch != '\n' && !feof(fp))
        	{
            		ch = fgetc(fp);
            		if(ch != '\n' && !feof(fp))
                	sent[i++] = ch;
        	}
        	sent[i++] = '\0';
		
        	if(!(secondScan(sent, *hptr, memory, ic, dc, fent, fext)))
        	{
			fprintf(stderr, " Line %d in file \"%s\"!\n", line, assemblyFile);
            		flagOfError = 1;
        	}
        	line++;
        	if(ch == '\n')
        	{	i = 0;
            		ch = fgetc(fp);
            		sent[i++] = ch;
        	}
		
    	}
	
    /* closing all remaining opened files */
    	if(fent)
    	{
        	fseek(fent, 0, SEEK_END);     	
        	if(ftell(fent) == 0 || flagOfError)/* if the file is empty or an error has occuredthen delete it */
        	{	
            		fclose(fent);
            		remove(entryFile);
        	}
        	else
            		fclose(fent);
    	}

	if(fext)
	{
		fseek(fext, 0, SEEK_END);		
		if(ftell(fext) == 0 || flagOfError)/* if the file is empty or an error has occuredthen delete it */
		{
		    fclose(fext);		   
		    remove(externalFile);
		}
		else
			fclose(fext);
	}
    	fclose(fp);
    	if(flagOfError) return 0;
	*numOfwords = ICF - *dc;
    	*ic = ICF;
    	*dc = DCF;

    return 1;
}
