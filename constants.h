#define ERROR 0
#define NOT_FOUND -1

#define MOV 0
#define CMP 1
#define ADD 2
#define SUB 2
#define LEA 4
#define CLR 5
#define NOT 5
#define INC 5
#define DEC 5
#define JMP 9
#define BNE 9
#define JSR 9
#define RED 12
#define PRN 13
#define RTS 14
#define STOP 15

#define MOVFUNCT 0
#define CMPFUNCT 0
#define ADDFUNCT 10
#define SUBFUNCT 11
#define LEAFUNCT 0
#define CLRFUNCT 10
#define NOTFUNCT 11
#define INCFUNCT 12
#define DECFUNCT 13
#define JMPFUNCT 10
#define BNEFUNCT 11
#define JSRFUNCT 12
#define REDFUNCT 0
#define PRNFUNCT 0
#define RTSFUNCT 0
#define STOPFUNCT 0

#define MEMORY_START 100
#define MAX_MEMORY 4096
#define LABEL_MAX 31
#define SENTENCE_MAX 80
#define FILE_NAME_MAX 100
#define MAX_NUM 2047
#define MIN_NUM -2048

#define DATA 1
#define STRING 2
#define ENTRY 3
#define EXTERN 4

#define A 0
#define R 1
#define E 2

#define IMMIDIATE 0     
#define LABEL 1     
#define REGISTER 3 /*METHOD_TWO*/   
#define LABEL_WITH_PERCENT 2 

#define MAX_ARRAY 3
#define ITERATIONS 3 

