/* Structs to present each type of memory cell */

/* this is the memory for the command line */
typedef struct{         
    	unsigned int destOp:2;
	unsigned int sourceOp:2;	
	unsigned int funct:4;
	unsigned int opcode:4;
	unsigned int are:2;
}commandMemory;

/* this is the memory for the data to be stored */
typedef struct{
	unsigned int val:12;
	unsigned int are:2;
}dataMemory;

/* this is the memory for the register */
typedef struct{
    	unsigned int reg2:7;
	unsigned int are:2;
    	unsigned int reg1:7;	
}registerMemory;

/* this is the memory for the number or label address */
typedef struct{
	unsigned int number:12;
    	unsigned int are:2;
}numberMemory;

/* this is where all types of memories get stored in the final form to be printed out*/
typedef struct{
    unsigned int memory:14;
}memorySection[MAX_MEMORY];
