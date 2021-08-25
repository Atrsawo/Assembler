#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "symbolsTable.h"

/* gets head for the table and a label to look for, returns 1 if it is found, 0 otherwise */
int searchLabel(ptr head, char label[])
{
    while(head)
    {
        if(strcmp(head->name, label) == 0)
            return 1;
        head = head->next;
    }

    return 0;
}
/* gets head of the table and all parameters of a symbol to be added at the end of the table
   parameters are label name, counter(address), external (if it is an external) enternal(if it is an enternal) and commandline (if it is in commandline) */
void addLabelToSymbols(ptr *head, char label[], int counter, int external, int commandLine, int enternal)
{
    ptr lab = *head, newLabel = (ptr)malloc(sizeof(symbol));
    if(!newLabel)
    {
        fprintf(stderr, "Can not Allocate memory.\n");
        exit(0);
    }

    	strcpy(newLabel->name, label);
    	newLabel->address = counter;
    	newLabel->isExternal = external;
	newLabel->isEnternal = enternal;	
    	newLabel->isCommandLine = commandLine;
    	newLabel->next = NULL;

    if(!*head)
    {
        *head = newLabel;
        return;
    }

    while(lab->next)
    {
        lab = lab->next;
    }
    lab->next = newLabel;
}

/* looks for a label name in the table, returns its address , or -1 if the label is not found in the table */
int getLabelAddress(ptr head, char label[])
{
    while(head)
    {
        if(strcmp(head->name, label) == 0)
        {
            return (head->address);
        }
        head = head->next;
    }
    return NOT_FOUND;
}

/* looks for a label name in the symole table and update his address and enternal feilds */
void setLabelAddress(ptr head, char label[], int adress, int ent, int ext)
{
	
 	while(head)
    	{
        	if(strcmp(head->name, label) == 0)
        	{
             		head->address = adress ;
			head->isEnternal = ent;
			head->isExternal = ext;
        	}
        	head = head->next;
    	}
}

/* returns 1 if the label in the table is external, otherwise 0 */
int isLabelExternal(ptr head, char label[])
{
    while(head)
    {
        if(strcmp(head->name, label) == 0)
        {
            return (head->isExternal);
        }
        head = head->next;
    }
    return 0;
}

/* returns 1 if the label in the table is enternal, otherwise 0 */ 
int isLabelEnternal(ptr head, char label[])
{

	while(head)
	    {
		if(strcmp(head->name, label) == 0)
		{
		    return (head->isEnternal);
		}
		head = head->next;
	    }
	    return 0;
}

/* return 1 if the label in the table is command line, otherwise 0 */
int isLabelinCommand(ptr head, char label[])
{
    while(head)
    {
        if(strcmp(head->name, label) == 0)
        {
            return (head->isCommandLine);
        }
        head = head->next;
    }
    return 0;
}

/* updates all the addresses of the table by adding the start position, and the instruction memory length */
void updateLabelAddress(ptr head, int icLen, int startAdd)
{
    while(head)
    {
        if(head->isExternal)
            head->address = 0;
	else head->address += startAdd;
        head = head->next;
    }
}

/*looks fot the label than do not declerd at the file and print the name of the label and return 1 if there is a label 0 otherwise */
int isUnDeclerd(ptr head)
{	
	int error = 0;
	while(head)
	{	
		if((head->address == -1) && (head->isExternal == 0) )
		{
			error = 1;
			fprintf(stderr,"%s ",head->name);
		}
		head = head->next ; 
	}
	if(error) return 1;
return 0;
}

/* frees the memory of the table */
void freeSymbols(ptr *head)
{
    ptr lab;

    while(*head)
    {
        lab = *head;
        *head = lab->next;
        free(lab);
    }
}
