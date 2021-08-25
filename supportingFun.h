/* legality and helper functions, help check for legal statements and moving the index */


/* gets a string and returns the associated opcode number or -1 if it is not an opcode  */
int isOpCode(char code[]);

/*gets a string and return the associated funct command number or 0 otherwise */
int Funct(char code [] );

/* gets a string and returns the associated register number or -1 if it is not a register */
int isRegister(char code[]);
 
/* torn on the associated register bit*/
int onBit(int reg);

/* get a num and the power to raise the num and return the the result */
int power(int, int);

/* gets a string and returns 1 if it is a legal variable(label), or 0 otherwise */
int isLabel(char code[]);

/* gets a string and returns the associated guide number, -1 if it is a registered guide name in assembly
   or 0 if it is neither */
int isGuide(char code[]);

/*finds the number in code[] and converts to int*/
int codeToInt(char code[],int i);

/* Checks for every letter if it is a number until it reaches a space, tab, null or comma.
   returns the updated index. or 0 if it is not an integer or 2 if it is overflow */
int isNumber(char code[],int i);

/* gets a string and returns 1 if it is of type method that has immidiate operand, 2 if it overflow otherwise 0  */
int methodImm(char code[]);

/* gets a string and returns 1 if it is of type method that has label operand otherwise 0 */
int methodLabel(char code[]);

/* gets a string and returns 1 if it is of type method that has register operand otherwise 0  */
int methodReg(char code[]);

/* gets a string and returns 1 if it is of type method that has label wich start with percent("%") operand otherwise 0  */
int methodPerLabel(char code[]);

/* skips all spaces and returns the index that no space charachter */
int spaceSkiper(char arr[], int i);

/* it gets a string and returns 1 if it was a successful ending of assembly statement (no additional characters at the end)
   if there is an error, it prints to the stderr a message and returns 0 */
int isEnd(char arr[]);

/* gets source string with initial index i and copies every letter to dest array
   until it counters space or tab or comma and returns the index which it reached */
int copySubString(char source[], char dest[], int i);

/* it gets a string and index i to start from, it passes between the two supposed operands
   if it counters an error it prints to the stderr the error type and returns 0
   if it is successful it returns the updated index */
int betweenOperands(char sent[], int i);
