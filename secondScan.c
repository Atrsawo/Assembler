#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "constants.h"
#include "symbolsTable.h"
#include "memoryStructs.h"
#include "errorMasseges.h"
#include "supportingFun.h"


/* gets a legal sentence to translate, a pointer to the head of an already built table, memory section to store the code, pointer to the instructions and data counters to be updated while translating, pointer to the entry and externals files.
   	returns 1 if it translated successfully or 0 if it finds an error and prints it to the user */
int secondScan(char sent[SENTENCE_MAX], ptr head, memorySection memSection, int * icPtr, int * dcPtr, FILE* fent, FILE* fext)
{
    	int icL = *icPtr,dcL=0;
	int i = 0, j = 0;
	char code[LABEL_MAX], temp[LABEL_MAX];
    	int regi1, regi2;
    	int opCode, opSource, opDest;
    	int address, addressDest, addressSource;
    	int num=0, numDest, numSource;
    	int areSource, areDest;
    	unsigned int *p;
    	commandMemory comem;
    	dataMemory damem;
    	registerMemory remem;
    	numberMemory numem;
    	int guide;
    	

	/* skipping as long as there are spaces or tabs */
    i = spaceSkiper(sent, i);
    /* in case of a note or empty line */
    if(sent[i] == '\0' || sent[i] == ';')
        return 1;

    if(strlen(sent) == 1)/*Empty line*/
    	return 1;

    for( ; sent[i] && sent[i] != ':' && sent[i] != ' ' && sent[i] != '\t' && sent[i] != '.' && sent[i] != ';'; i++)
        code[j++] = sent[i];
    code[j] = '\0';
  
    if(sent[i] == ':')/* this is a label */
    {
        address = getLabelAddress(head, code);
	if(isLabelEnternal(head, code)) fprintf(fent, "%s %04d\n",code,address);            
	i = spaceSkiper(sent, ++i);
        if(sent[i] == '\0') return 1;/*in case there is nothing other than a label in the sentence */           
        if(sent[i] != '.')
        {
            j = 0;
            for( ; sent[i] && sent[i] != ' ' && sent[i] != '\t'; i++)
                code[j++] = sent[i];
            code[j] = '\0';
        }
    }

    if(sent[i] == '.') 
    {
        j = 0;
        for( ; sent[i] != ' ' && sent[i] != '\t'; i++)
            	temp[j++] = sent[i];
        temp[j] = '\0';
        guide = isGuide(temp);
       	if(guide == DATA)/* insert data into memory section */
        {	damem.are = A;
		while(sent[i] != '\0')
		{
			i = spaceSkiper(sent, i);
			if(sent[i] == ',')
		        {
		          	i++;
		          	i = spaceSkiper(sent, i);
			}
            		i = copySubString(sent, temp, i);  
			num = atoi(temp);
			if(num >= MIN_NUM && num <= MAX_NUM  )/* if it is legal number then add it to the memory*/
			{    
				dcL++;
				damem.val = num;
		             	p = (unsigned int *)(&damem);
		             	memSection[icL++].memory = *p;
			} 	
		}
            	*dcPtr += dcL; /*udate the data and instruaction counters */
		*icPtr = icL;
            return 1;
		             	
        }
        if(guide == STRING)
        {
            	i = spaceSkiper(sent, i);     
            	i++;
		damem.are = A;		
            	while(sent[i] != '"')
            	{	
			damem.val = sent[i++];
                	p = (unsigned int *)(&damem);
                	memSection[icL++].memory = *p;
			dcL++;
            	}       
            	damem.val = 0;/* adding 0 at the end of string data */
            	p = (unsigned int *)(&damem);
            	memSection[icL++].memory = *p;
            	*dcPtr += dcL;
		*icPtr = icL;
            return 1;
        }
        
        if(guide == EXTERN )return 1;
	if(guide == ENTRY)
	{
		if(getLabelAddress(head, temp) != -1)
			{
				fprintf(fent, "%s %04d\n",temp,getLabelAddress(head, temp) );
			}
		return 1;
	}

    return 0;
    }

	/* switching to the appropriate command and upating the correct type of memory cell with the needed data */	
    opCode = isOpCode(code);
    switch(opCode)
    {
        case MOV:
        case CMP:
        case ADD:/*add and sub have the same opcode number and the same amount of source and destinion method so it is the same case */
		comem.opcode = opCode;
	    	comem.funct = Funct(code);
		comem.are = A;
            	i = spaceSkiper(sent, i);
            	i = copySubString(sent, code, i);
           	 if(code[0] == '#')
            	{
                	opSource = IMMIDIATE;
                	numSource = atoi(code + 1);
			areSource = A;
            	}
            	else if(isRegister(code) > -1)
            	{
                	opSource = REGISTER;
			regi1 = onBit(isRegister(code));
			areSource = A;
            	}
            	else /*it is a label */
            	{
                	addressSource = getLabelAddress(head, code);
			areSource = R;
                	if(addressSource == NOT_FOUND)
               	 	{
                    		errorLabelDeclaration();
                    		fprintf(stderr, " \"%s\"", code);
                    		areSource = R;
				numSource = 0;
                	}
			opSource = LABEL;
                	if(isLabelExternal(head, code)) /* writing to the external file */
                	{
                    		areSource = E;
				numSource = 0;                    
                    		if(fext)
                    		{
                        		fprintf(fext, "%s %04d\n",code ,icL + MEMORY_START +1);
                    		}
                	}
                	else
                		numSource = addressSource; 
				
            	}
            	i = spaceSkiper(sent, i);
            	i++; /* skipping the "," between the operands */
            	i = spaceSkiper(sent, i);
            	i = copySubString(sent, code, i); /* getting the next word */
            	if((opCode == CMP) && code[0] == '#')
            	{
                	numDest = atoi(code + 1);
                	opDest = IMMIDIATE;
			areDest = A;
            	}        
            else if(isRegister(code) > -1)
            {              
                opDest = REGISTER;
		regi2 = onBit(isRegister(code));
		areDest = A;
            }
            else /*it is a label */
            {
                addressDest = getLabelAddress(head, code);
		areDest = R;
                if(addressDest == NOT_FOUND)
                {
                    errorLabelDeclaration();
                    fprintf(stderr, " \"%s\"", code);
	                areDest = R;
			numDest = 0;
                }
		opDest =  LABEL;
                if(isLabelExternal(head, code)) /* writing to the external file */
                {
                    	areDest = E;
			numDest = 0;                 
                    if(fext)
                    {
                        fprintf(fext, "%s %04d\n",code ,icL + MEMORY_START + 2);
                    }
                }
                else
               		numDest = addressDest;		
            }
	    /* update all the memory section with the correct code */          
            comem.sourceOp = opSource;
            comem.destOp = opDest;          
            p = (unsigned int *)(&comem); /* put the instruction code */
            memSection[icL++].memory = *p;
            if(opSource == IMMIDIATE || opSource == LABEL)
            { 
                numem.are = areSource;
                numem.number = numSource;
                p = (unsigned int *)(&numem);
                memSection[icL++].memory = *p;
            }
            if(opSource == REGISTER)
            {                           
		remem.reg1 =0;
		remem.reg2 =regi1; 
		remem.are = A;
                p = (unsigned int *)(&remem);
                memSection[icL++].memory = *p;
            }
            if(opDest == IMMIDIATE || opDest == LABEL)
            {
                numem.are = areDest;
                numem.number = numDest;
                p = (unsigned int *)(&numem);
                memSection[icL++].memory = *p;
            }   
            if(opDest == REGISTER)
            {
                remem.are = areDest;
		remem.reg1 = 0 ;
                remem.reg2 = regi2;
                p = (unsigned int *)(&remem);
                memSection[icL++].memory = *p;
            }
            *icPtr = icL; /* updating the counter */
            return 1;

        case PRN:           
	  	comem.opcode = opCode;
	    	comem.funct = Funct(code);
		comem.sourceOp = 0;
		comem.are = A;
	    	i = spaceSkiper(sent, i);
            	i = copySubString(sent, code, i);/* the destinion operand is an immidiate number */
            	if(code[0] == '#')
            	{                      
                	comem.destOp = IMMIDIATE;
                	p = (unsigned int *)(&comem);
                	memSection[icL++].memory = *p;
                	numem.are = A;
                	numem.number = atoi(code + 1);
                	p = (unsigned int *)(&numem);
                	memSection[icL++].memory = *p;
                	*icPtr = icL;
                return 1;
            	}
 	    	if(isLabel(code)) /*the destinion operand is a label */
		{
			addressDest = getLabelAddress(head, code);
		        if(addressDest == NOT_FOUND)
		        {
		            errorLabelDeclaration();
		            fprintf(stderr, " \"%s\"", code);
		            return 0;
		        }
			comem.destOp = LABEL;
			p = (unsigned int *)(&comem);
                	memSection[icL++].memory = *p;  
		        if(isLabelExternal(head, code))
		        {
		            /* writing to the external file */
		            	if(fext)
		            	{
		                	fprintf(fext, "%s %04d\n",code ,icL + MEMORY_START + 2);
		            	}            		
                		numem.number = 0;
				numem.are = E;
                		p = (unsigned int *)(&numem);
                		memSection[icL++].memory = *p;
                		*icPtr = icL;
				return 1;
		        }
		        else		        		       
				numem.number = addressDest;
				numem.are = R;
                		p = (unsigned int *)(&numem);
                		memSection[icL++].memory = *p;
                		*icPtr = icL;
				return 1;
		}
	    	else if(isRegister(code) > -1) /*the destinion operand is a register */
      		{       	
               		comem.destOp = REGISTER;
			p = (unsigned int *)(&comem);
                	memSection[icL++].memory = *p;			
                	remem.reg1 = onBit(isRegister(code));
                	remem.reg2 = 0;
			remem.are = A;
                	p = (unsigned int *)(&remem);
                	memSection[icL++].memory = *p;
			return 1;
            	}	    	
		break;
        case NOT:   /*not,inc,dec,clr and red cammands do not have a source operand*/
        case RED:
		comem.opcode = opCode;
            	comem.funct = Funct(code);
		comem.sourceOp = 0;
		comem.are = A;
		i = spaceSkiper(sent, i);		              
                i = copySubString(sent, code, i);                        
            /* updating the correct code */
            	if((regi2 = isRegister(code)) > -1)/*The destinion operand is a register */
           	{              
                	comem.destOp = REGISTER;               
                	p = (unsigned int *)(&comem);
               	 	memSection[icL++].memory = *p;
                	remem.are = A;
                	remem.reg1 = 0;
                	remem.reg2 = onBit(regi2);
                	p = (unsigned int *)(&remem);
                	memSection[icL++].memory = *p;
                	*icPtr = icL;
                return 1;
            	}
             
         	else if(isLabel(code)) /* The destinion operand is a label */
           	 {               		
                	address = getLabelAddress(head, code);
                	if(address == NOT_FOUND)
               	 	{
                 	  	errorLabelDeclaration();
                  	  	fprintf(stderr, " \"%s\"", code);
                  	  return 0;
               		}
			comem.destOp = LABEL;               
                	p = (unsigned int *)(&comem);
               	 	memSection[icL++].memory = *p;
                	if(isLabelExternal(head, code))
               		 {
                   	 /* writing to the external file */
                  	  	if(fext)
                    		{
                       		 	fprintf(fext, "%s %04d\n",code ,icL + MEMORY_START );
                    		}
				numem.number = 0;
				numem.are = E;
                        	p = (unsigned int *)(&numem);
                		memSection[icL++].memory = *p;
               			*icPtr = icL;
               		return 1;
                	}
                	else				              	        
                    		numem.number = address;
				numem.are = R;
                                p = (unsigned int *)(&numem);
                		memSection[icL++].memory = *p;
               			*icPtr = icL;
               			return 1;
           	}
        break;
        case LEA:
		comem.opcode = opCode;	
	    	comem.funct = Funct(code);
		comem.are = A;
	   /* The source operand can be only a label */
            	i = spaceSkiper(sent, i);
            	i = copySubString(sent, code, i);
            	addressSource = getLabelAddress(head, code);
            	if(addressSource == NOT_FOUND)
            	{ 
                    errorLabelDeclaration();
                    fprintf(stderr, "%s", code);
                    return 0;
            	}
		comem.sourceOp = LABEL;
            	if(isLabelExternal(head, code))
            	{            	
                /* writing to the external file */
                	if(fext)
                	{
                  	  fprintf(fext, "%s %04d\n",code ,icL + MEMORY_START +1);
                	}				
			numem.number = 0;
			numem.are = E;       	            
           	 }
           	else
		{                				
			numem.number = 0;
			numem.are = R;			
		}
	     	i = spaceSkiper(sent, ++i);
            	i = copySubString(sent, code, i);
	/* The seconed operand can be a register or label */	
            	if((regi1 = isRegister(code)) > -1)
           	 {
                	comem.destOp = REGISTER;
                	p = (unsigned int *)(&comem);
                	memSection[icL++].memory = *p;
			p = (unsigned int *)(&numem);
                	memSection[icL++].memory = *p;
			remem.reg1 = 0;			
			remem.reg2 = onBit(regi1);
			remem.are = A;
			p = (unsigned int *)(&remem);
                	memSection[icL++].memory = *p;
			*icPtr = icL;
                	return 1;
		}     
            	else /* it is a label */
            	{
                	addressDest = getLabelAddress(head, code);
                	if(addressDest == NOT_FOUND)
                	{
                    		errorLabelDeclaration();
                    		fprintf(stderr, "%s", code);
                    	return 0;
                	}
			comem.destOp = LABEL;
			p = (unsigned int *)(&comem);
			memSection[icL++].memory = *p;
			p = (unsigned int *)(&numem);
                	memSection[icL++].memory = *p;
                	if(isLabelExternal(head, code))
                	{
                    		if(fext)
                    		{
                        		fprintf(fext, "%s %04d\n",code ,icL + MEMORY_START + 2);/* writing to the external file */
                    		}							
				numem.are=E;
				numem.number = 0;
				p = (unsigned int *)(&numem);
                		memSection[icL++].memory = *p;
               			*icPtr = icL;
               		return 1;
                	}
                	else               	
				numem.number = addressDest;
                		numem.are = R;              	
                		p = (unsigned int *)(&numem);
                		memSection[icL++].memory = *p;               	
                		*icPtr = icL;
               		return 1;
            	}
        		break;
        
        case JSR:
	/* jmp, bne and jsr commands have the same opcode number and do not have a source operand */ 
		comem.opcode = opCode;
            	comem.funct = Funct(code);
		comem.sourceOp = 0;
		comem.are = A;
	    	i = spaceSkiper(sent, i);
		if(sent[i] == '%')  
		{ 
			i++;
			comem.destOp = LABEL_WITH_PERCENT;
			numem.number = icL + MEMORY_START + 1;/* distance to the lebal address */ 
		}
		else comem.destOp = LABEL;
            	i = copySubString(sent, code, i);
           	if(isLabel(code)) /* The destinion operand is a label  */
            	{
               		addressDest = getLabelAddress(head, code);
                	if(addressDest == NOT_FOUND)
                	{
                   		errorLabelDeclaration();
                    		fprintf(stderr, " \"%s\"", code);
                    		return 0;
                	}
			p = (unsigned int *)(&comem);
              		memSection[icL++].memory = *p;
                	if(isLabelExternal(head, code))
                	{
                    /* writing to the external file */
                    		if(fext)
                    		{
                        		fprintf(fext, "%s %04d\n",code ,icL + MEMORY_START + 2);
                    		}
				numem.number = 0;
                		numem.are = E;              	
                		p = (unsigned int *)(&numem);
                		memSection[icL++].memory = *p;               	
                		*icPtr = icL;
               			return 1;
                	}
			numem.are = A;
                	if(comem.destOp == LABEL_WITH_PERCENT )
				{	
					numem.number = addressDest - numem.number;
					p = (unsigned int *)(&numem);
                			memSection[icL++].memory = *p;               	
                			*icPtr = icL; 
               			return 1;  			
				}
			else
				numem.number = addressDest;              	
                		p = (unsigned int *)(&numem);
                		memSection[icL++].memory = *p;               	
                		*icPtr = icL;
               			return 1;        
		}     
	  break;
        case RTS:
        case STOP:
		comem.opcode = opCode; 
		comem.funct = Funct(code);			    	
            	comem.destOp = 0;
            	comem.sourceOp = 0; 
		comem.are = A;        	
            	p = (unsigned int *)(&comem);
            	memSection[icL++].memory = *p;
            	*icPtr = icL;
            	return 1;
            
        default: return 0;            
    }
    return 1;
}
