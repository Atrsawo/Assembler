#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "constants.h"
#include "errorMasseges.h"
#include "supportingFun.h"

/* gets a string and returns the associated OPCODE number or -1 if it is not an OPCODE */
int isOpCode(char opcode[])
{
    if(strcmp(opcode, "mov") == 0)
        return MOV;
    if(strcmp(opcode, "cmp") == 0)
        return CMP;
    if(strcmp(opcode, "add") == 0)
        return ADD;
    if(strcmp(opcode, "sub") == 0)
        return SUB;
    if(strcmp(opcode, "not") == 0)
        return NOT;
    if(strcmp(opcode, "clr") == 0)
        return CLR;
    if(strcmp(opcode, "lea") == 0)
        return LEA;
    if(strcmp(opcode, "inc") == 0)
        return INC;
    if(strcmp(opcode, "dec") == 0)
        return DEC;
    if(strcmp(opcode, "jmp") == 0)
        return JMP;
    if(strcmp(opcode, "bne") == 0)
        return BNE;
    if(strcmp(opcode, "red") == 0)
        return RED;
    if(strcmp(opcode, "prn") == 0)
        return PRN;
    if(strcmp(opcode, "jsr") == 0)
        return JSR;
    if(strcmp(opcode, "rts") == 0)
        return RTS;
    if(strcmp(opcode, "stop") == 0)
        return STOP;

    return NOT_FOUND;
}

/*get a name of command and opcode number ad return the funct number wich associated to the command*/
int Funct(char code [])
{
  
    if(strcmp(code, "add") == 0)
        return ADDFUNCT;
    if(strcmp(code, "sub") == 0)
        return SUBFUNCT;
    if(strcmp(code, "not") == 0)
        return NOTFUNCT;
    if(strcmp(code, "clr") == 0)
        return CLRFUNCT;
    if(strcmp(code, "inc") == 0)
        return INCFUNCT;
    if(strcmp(code, "dec") == 0)
        return DECFUNCT;
    if(strcmp(code, "jmp") == 0)
        return JMPFUNCT;
    if(strcmp(code, "bne") == 0)
        return BNEFUNCT;
    if(strcmp(code, "jsr") == 0)
        return JSRFUNCT;

return 0;
}

/* gets a string and returns the associated register number or -1 if it is not a register */
int isRegister(char code[])
{
    if(strcmp(code, "r0") == 0)
        return 0;
    if(strcmp(code, "r1") == 0)
        return 1;
    if(strcmp(code, "r2") == 0)
        return 2;
    if(strcmp(code, "r3") == 0)
        return 3;
    if(strcmp(code, "r4") == 0)
        return 4;
    if(strcmp(code, "r5") == 0)
        return 5;
    if(strcmp(code, "r6") == 0)
        return 6;
    if(strcmp(code, "r7") == 0)
        return 7;

    return NOT_FOUND;
}

/* gets a string and returns 1 if it is a legal label otherwise 0 */
int isLabel(char labal[])
{
    	int i;
	if(strcmp(labal,"DC") == 0 || strcmp(labal,"IC") == 0) return 0;
    	if(isalpha(labal[0]) == 0) return 0;/* chek if the label start corectly */ 
        
    	if(labal[0] == ':') return 0; /* Checks if the label is empty */
        
    	for(i = 1; labal[i] ; i++)
        if(!(isalpha(labal[i]) || isdigit(labal[i]))) return 0;      

    	if(i >= LABEL_MAX) return 0; /*chek the length of the label */ 
       
    	if(isOpCode(labal) > -1) return 0; /* chek if the label doesn't any command */
       
    	if(isRegister(labal) > -1) return 0; /* chek if the label doesn't any command */
        
   	if(isGuide(labal)) return 0;  /* chek if the label doesn't any guide command */
        
    return 1;
}

/* gets a string and returns the associated guide number, -1 if it is a registered guide name in assembly
   or 0 if it is neither */
int isGuide(char guide[])
{
    if(strcmp(guide, "data") == 0)
        return -1;
    if(strcmp(guide, "string") == 0)
        return -1;
    if(strcmp(guide, "extern") == 0)
        return -1;
    if(strcmp(guide, "entry") == 0)
        return -1;

    if(strcmp(guide, ".data") == 0)
        return DATA;
    if(strcmp(guide, ".string") == 0)
        return STRING;
    if(strcmp(guide, ".extern") == 0)
        return EXTERN;
    if(strcmp(guide, ".entry") == 0)
        return ENTRY;

    return 0;
}

/* skips all spaces and returns the index which has no space */
int spaceSkiper(char arr[], int i)
{
    int on = 1;
    while(arr[i] && on)
        if(arr[i] == ' ' || arr[i] == '\t')
            i++;
        else
            on =0;

    return i;
}

/* gets source string with i being the index it should start from and copies every letter to the destintion array,
   until it counters space or tab or comma and returns the index which it reached */
int copySubString(char source[], char dest[], int i)
{
    int j = 0;
        for( ; source[i] && source[i] != ' ' && source[i] != '\t' && source[i] != ','; i++)
            dest[j++] = source[i];
        dest[j] = '\0';

    return i;
}

/* gets a string and index i to start from, it passes between the two supposed operands
   if it counters an error it prints to the stderr the error type and returns 0
   if it is successful it returns the updated index */
int betweenOperands(char sentence[], int i)
{
    i = spaceSkiper(sentence, i);
    if(sentence[i] != ',')
    {
        errorMissingComma();
        return ERROR;
    }
    i++;
    i = spaceSkiper(sentence, i);
    if(sentence[i] == '\0')
    {
        errorTwoOperand();
        return ERROR;
    }
    return i;
}

/* gets a string and returns 1 if it was a successful ending of an assembly statement (no additional characters at the end)
   if there is an error, it print to the stderr a message and returns 0 */
int isEnd(char arr[])
{
    int i = spaceSkiper(arr, 0);

    if(arr[i])
    {
        fprintf(stderr, "Invalid character/s at end of sentence.");
        return ERROR;
    }

    return 1;
}

/* get the register number and return the turn on the associated bit at the worrd that keep the register */
int onBit(int reg)
{
	int i, j=0, num=0, arr[12]={0};
	arr[reg]=1;
	for(i = 0 ; i <= 11 ; i++)
	{				
		num +=(int)(power(2,j++))*(arr[i]);	
	} 
	
return num;
}

/* get a number and power integer to be raised and return the result */
int power(int num, int pwr)
{
	if(pwr==0) return 1;
	if(pwr==1) return num;
	num *= power(num,--pwr);
return num;
} 

/*finds the number in the string and converts to int*/
int codeToInt(char code[],int i)
{
    	int j, num;
    	char str[13];
    	j=0;

    	str[j] = code[i];
    	for( ; code[i] && code[i] != ' ' && code[i] != ',' && code[i] != '\t'; i++)
    	{
        	str[j++] = code[i];
    	}
	str[j]='\0';
        num = atoi(str);
        return num;
}

/* Checks for every letter if it is a number until it reaches a space, tab, null or comma.
   returns the updated index or 0 if it is not an integer or -1 if it is overflow */
int isNumber(char nume[],int i)
{
    int val=0,j=i;
    char first =  nume[i];

    if(first != '+' && first != '-' && (!isdigit(first)))
        return ERROR;
    i++;
    for( ;  nume[i] &&  nume[i] != ' ' &&  nume[i] != ',' &&  nume[i] != '\t' ; i++)
    {
        if(!isdigit( nume[i]))
        return ERROR;
    }

        val = codeToInt(nume, j);
        if( val < MIN_NUM || val > MAX_NUM) /*if it is number that can not be represent at 12 bits*/
            return -1;

    return i;
}

/* gets a string and returns 1 if it is of type method that has immidiate operand, 2 if it overflow or 0 otherwise */
int methodImm(char code[])
{
    	int val;
    	if(code[0] != '#') return 0;
	val = isNumber(code, 1);
    	if(val)
    	{
        	val = codeToInt(code, 1);
        	if(val < MIN_NUM || val > MAX_NUM) return 2;
        	return 1;
    	}

    	return 0;
}


/* gets a string and returns 1 if it is of type method that has register operand otherwise 0 */
int methodLabel(char code[])
{	
    if(!isLabel(code)) return 0;
        
return 1;
}

/* gets a string and returns 1 if it is of type method that has register operand otherwise 0  */
int methodReg(char code[])
{	
        if(isRegister(code) == -1) return 0;
  
  return 1;
}

/* gets a string and returns 1 if it is of type method that has label wich start with percent("%") operand otherwise 0 */
int methodPerLabel(char code[])
{
    if(code[0]=='%') return isLabel(code + 1);
    
  return 0;
}


