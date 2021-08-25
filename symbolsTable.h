typedef struct label * ptr;

/* symbols table to be stored in */
typedef struct label{
	char name[LABEL_MAX];
	int address;
	int isExternal;
	int isEnternal;
    	int isCommandLine;
	ptr next;
}symbol;

/* turn on the enternal field of the label */
void setEnternal(ptr head, char label[]);

/* gets head for the table, and a label to look for, returns 1 if it is found, or 0 otherwise */
int searchLabel(ptr head, char label[]);

/* gets head of the table and all the feilds of a symbol to be added at the table
   the feilds are label name, counter(address), external (if it is external) enternal (if it is enternal) and commandline (if it is in commandline) */
void addLabelToSymbols(ptr *hptr, char label[], int counter, int external, int commandLine, int enternal);

/* return the address of givan label name  or -1 if the label is not found in the table */
int getLabelAddress(ptr head, char label[]);

/* update the corect adress of givan label and is enternal field */
void setLabelAddress(ptr head, char label[], int adress, int ent, int ext);

/* returns 1 if the label is external otherwise 0 */
int isLabelExternal(ptr head, char label[]);

/* return 1 if the label is enternal otherwise 0;*/
int isLabelEnternal(ptr head, char label[]);

/* return 1 if the label in the table in command line otherwise 0  */
int isLabelinCommand(ptr head, char label[]);

/* updates all the addresses of the table by adding the start position and the instruction memory length */
void updateLabelAddress(ptr head, int icLen, int startAdd);

/*return 1 if the label has been declerd 0 otherwise*/
int isUnDeclerd(ptr head);

/*return the name of the label */
char* getLabelName(ptr head);

/* frees the memory of the table */
void freeSymbols(ptr *hptr);
