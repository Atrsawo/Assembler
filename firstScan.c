#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "constants.h"
#include "errorMasseges.h"
#include "symbolsTable.h"
#include "supportingFun.h"

/* 	gets a string array and checks it for errors or if it is a correct assmebly command, data and address pointer to be updated how big of a memory it needs, a 		pointer to the 	head of the table to build the symbols.
	returns 0 if it finds any error and prints it out to the stderr, or 1 if it is a legal sentence */
int firstScan(char arr[], int * dc, int * ic, ptr *hptr)
{
    int i = 0, j = 0, notLabel = 1, opCode, guide, cnt=0;
    char temp[LABEL_MAX],str[LABEL_MAX];
    char labelName[LABEL_MAX];

   /* skipping all the spaces and tabs*/ 
    	i = spaceSkiper(arr, 0);
	if(arr[i] == '\0') return 1;

    if(strlen(arr) == 1) /* if the sentence is empty line */
    {
        if(arr[i] == '\n')
            return 1;

        fprintf(stderr, "Eror: Unauthorized character.");
            return 0;
    }

    if(arr[i] == ';')	return 1; /* it's a note  */

    for( ; arr[i] && arr[i] != ' ' && arr[i] != ',' && arr[i] != '\t' && arr[i] != ':'; i++)
        temp[j++] = arr[i];
    temp[j] = '\0';

    if(arr[i] == ':')   /* if it is a label */
    {
	strcpy(labelName, temp);
        notLabel = 0;
        if(isLabel(temp)) /* check if it is a legal label */
    	{
        	
            	i = spaceSkiper(arr, ++i);
            	if(arr[i] == '\0') /* if it is only a label followed by an empty statement */
		{
		 	if(searchLabel(*hptr, labelName))
       		    	{
                        	errorLabelInUse();
                        return ERROR;
                    	}
                    	addLabelToSymbols(hptr, labelName, *dc, 0, 0,0);
                return 1;
	        }
		if(arr[i] == '.') /* if it is a guide statement */
              	{	
                	j = 0;
                	for( ; arr[i] && arr[i] != ' ' && arr[i] != '\t'; i++)
                    		temp[j++] = arr[i];
                	temp[j] = '\0';

                	guide = isGuide(temp);
                	if(!guide) /* if it is not any of the guide commands */
                	{
                    		errorCommandName();
                    	return ERROR;
                	}
                	switch(guide) /* switch to the correct type of command */
			{
		                case DATA: 
						cnt = 0;
		                    		i = spaceSkiper(arr, i);
		                    		if(arr[i] == ',')
		                    		{
		                        		errorIllegalComma();
		                        	return ERROR;
		                    		}
		                    		while(arr[i])
		                    		{
		                        		i = isNumber(arr, i); /* if it is a number and written correctly */
		                        		if(i == -1)
		                        		{
		                            			errorDataOverFlow();
		                            		return ERROR;
		                        		}

		                        		if(i)
		                        		{
		                            			cnt++; /* counting the numbers */
		                        		}
		                        		else
		                        		{
		                            			errorNumber();
		                            		return ERROR;
		                        		}
		                        		i = spaceSkiper(arr, i);
		                        		if(arr[i] != '\0')
		                        		{
		                            			if(arr[i] == ',')
		                            			{
		                                			i++;
		                                			i = spaceSkiper(arr, i);
		                                			if(arr[i] == '\0' || arr[i] == ',')
		                                			{
		                                    				errorIllegalComma();
		                                    			return ERROR;
		                                			}
		                            			}
		                            			else
		                            			{
		                                			errorMissingComma();
		                                		return ERROR;
		                            			}
							}
						}
		                    		if(cnt) /* if there is anything to store */
		                    		{
		                        		if(isLabelEnternal(*hptr, labelName))
		                        		{
		                            			setLabelAddress(*hptr, labelName, *ic, 1, 0);
		                            			
		                        		}
							else if(searchLabel(*hptr, labelName))
								setLabelAddress(*hptr, labelName, *ic, 0, 0);	                       			
							else addLabelToSymbols(hptr, labelName, *ic, 0, 0, 0);
		                        			*dc += cnt; 	/* updating the data counter */	
							if (cnt!=1) *ic+=cnt;
													
		                        	return 1;
		                    		}
		                    		else
		                    		{
		                        		errorNumber();
		                        	return ERROR;
		                    		}
		                    	break;
		                case STRING:
		                    		cnt = 0;
		                    		i = spaceSkiper(arr, i);
		                    		if(arr[i] == ',')
		                    		{
		                        		errorIllegalComma();
		                        	return ERROR;
		                    		}
		                    		if(arr[i] != '"')
		                    		{
		                        		errorString(); 
		                        	return ERROR;
		                    		}
		                    		i++;
		                    		for( ; arr[i] && arr[i] != '"'; i++) /* counting the characters */
		                        		cnt++;
		                    		if(arr[i] != '"')
		                    		{
		                        		errorString();
		                        	return ERROR;
		                    		}
		                    		i++;
		                    		i = spaceSkiper(arr, i);
		                    		if(arr[i])
		                    		{
		                        		errorOneOperand();
		                        	return ERROR;
		                    		}
		                    		if(searchLabel(*hptr, labelName) && getLabelAddress(*hptr, labelName) != -1 )
		                    		{
		                        		errorLabelInUse();
		                        	return ERROR;
		                    		}
						/*adding the label to symbol table */
		                    		if(getLabelAddress(*hptr, labelName) == -1)
		                        		{
		                            			setLabelAddress(*hptr, labelName, *ic, 0, 0);		                            			
		                        		}
						addLabelToSymbols(hptr, labelName, *ic, 0, 0, 0);	                       			
		                    		*dc += (cnt + 1); /* updating the data and instruacion counters */
						if (cnt!=1) *ic+=cnt+1;
		                    		return 1;
		                    
		                case ENTRY: 
						i = spaceSkiper(arr, i);
						j = 0;
                				for( ; arr[i] && arr[i] != ' ' && arr[i] != '\t'; i++)
                    					temp[j++] = arr[i];
                				temp[j] = '\0';
						if(!searchLabel(*hptr, temp))
							addLabelToSymbols(hptr, temp, -1, 0, 0, 1);
						return 1;
						
		                case EXTERN: 
						i = spaceSkiper(arr, i);
		                    		j = 0;
		                    		for( ; arr[i] && arr[i] != ' ' && arr[i] != '\t'; i++)
		                        		temp[j++] = arr[i];
		                    		temp[j] = '\0';

		                    		if(isLabel(temp)) /* checking if the label has allready defined and adding it to the symbol table if not */
		                    		{                         		                          
		                            		if(searchLabel(*hptr, temp) && getLabelAddress(*hptr,temp) != -1)
		                            		{
		                                		errorLabelInUse();
		                                	return ERROR;
		                            		}
		                           		setLabelAddress(*hptr, temp, *ic, 0, 1);                     
		                    		}
		                    		else
		                    		{
		                        		errorLabel(temp);
		                        	return ERROR;
		                    		}

		                    		i = spaceSkiper(arr, i);
		                    		if(arr[i])
		                    		{
		                        		errorOneOperand();
		                        	return ERROR;
		                    		}
		                 
		                   		return 1;                        
			}
		}
		else /* if the label is not before a guide statement */
		{
		        if(!(isalpha(arr[i]))) /* all the opcodes start with aleph bet*/
		        {
		            fprintf(stderr, "Unauthorized usage of the character %c after label", arr[i]);
		            return ERROR;
		        }
			strcpy(str,temp);
			j = 0;
		        for( ; arr[i] && arr[i] != ' ' && arr[i] != '\t' ; i++)
		        	temp[j++] = arr[i];
		        temp[j] = '\0';
			opCode = isOpCode(temp);
                	
			if(!searchLabel(*hptr, str))
                	{	
				addLabelToSymbols(hptr, str, *ic, 0, 1, 0); /* adding the label to the symbol table */	
			}
			else if(isLabelExternal(*hptr, str)) 
				fprintf(stderr, "Error: external label "" %s "" could not declerd at this file!\n", str);
			else if(isLabelEnternal(*hptr,str))
				{ 
					if(getLabelAddress(*hptr,str) == -1) setLabelAddress(*hptr, str, *ic, 1, 0);
					else 
					{
						errorLabelInUse();
		                             return ERROR;
					} 
				}
			else if(getLabelAddress(*hptr,str) != -1)/*if label allready defaind */
				{
					errorLabelInUse();
		                             return ERROR;
				}
			else setLabelAddress(*hptr, str, *ic, 0, 0); /* updating the address of the label */
			
			if(opCode == NOT_FOUND)/*print error massage if it is a illegal opcode */
                	{
                    		errorCommandName();
				return ERROR;             
                	}				
		}
	}
        else /* if it is an illegal label name */
        {
            errorLabel(temp);
            return ERROR;
        }
    }

   /* in case there is no label at the beginning */
    opCode = isOpCode(temp);
    guide = isGuide(temp);
    if(opCode == NOT_FOUND && guide == 0)/* if it's not an opcode, or any data word , then it's illegal sentence */
    {
        errorCommandName();
        return ERROR;
    }

    /* if there is no a label at the beginning then count the data */
    if(notLabel) 
	if(guide == DATA || guide == STRING)
        {
            cnt = 0;
            i = spaceSkiper(arr, i);
            if(arr[i] == ',')
            {
                errorIllegalComma();
                return ERROR;
            }
            if(guide == DATA)
            {
                while(arr[i])
                {
                	i = isNumber(arr, i);
                	if(i== NOT_FOUND)
                    	{  
                        	errorDataOverFlow();
                        return ERROR;
                    	}
                    	if(i)
                    	{
                       	 	cnt++; /*counting the data*/
                    	}
                    	else
                    	{
                        	errorNumber();
                        return ERROR;
                    	}
                    	i = spaceSkiper(arr, i);
                    	if(arr[i] != '\0')
                    	{
                        	if(arr[i] == ',')
                        	{
                            		i++;
                            		i = spaceSkiper(arr, i);
                            		if(arr[i] == '\0' || arr[i] == ',')
                            		{
                                		errorIllegalComma();
                                	return ERROR;
                            		}
                        	}
                        	else
                        	{
                            		errorMissingComma();
                            	return ERROR;
                        	}
                  	}
                }
                if(cnt) /* if there is data then we update the data counterand the adress */
                { 
                    	*dc += cnt;
			*ic+=cnt; 
                    return 1;
                }
                else
                {
                    errorNumber();
                    return ERROR;
                }
            }
            else /* in case of a string */
            { 
                if(arr[i] != '"')
                {
                    errorString();
                    return ERROR;
                }
                i++;
                for( ; arr[i] && arr[i] != '"'; i++)
                    cnt++; /* count the charachtres */
                if(arr[i] != '"')
                {
                    errorString();
                    return ERROR;
                }
                i++;
                i = spaceSkiper(arr, i);
                if(arr[i])
                {
                    errorOneOperand();
                    return ERROR;
                }

                *dc += cnt; /* update the data counter */
                return 1;
            }
        }
  /* if it is an any command the we the we count the associated numbers of words */
    if(opCode > -1)
    {
        
        i = spaceSkiper(arr, i);
        if(arr[i] == ',')
        {
            errorIllegalComma();
            return ERROR;
        }

	if(opCode == RTS || opCode == STOP) 
        {
            if(arr[i]) /*stop and rts command dont have any oprend */ 
            {
                errorZeroOperand();
                return ERROR;
            }
            else
            { 
               	*ic += 1;
              return 1;
            }
        }

        if(opCode == MOV || opCode == ADD || opCode == CMP || opCode == LEA) /* add and sub caommands has the same opcode number */
        {
            if(arr[i] == '\0')
            {
                errorTwoOperand();
                return ERROR;
            }
        }
        
        i = copySubString(arr, temp, i); 

        if(opCode == CMP)
        {
            if(methodImm(temp) == 2)
            {
                 errorImmOverFlow();
                 return ERROR;
            }
            /* checking for the right methods */
            if(methodImm(temp) || methodLabel(temp) || methodPerLabel(temp))
            {	/* in case there is a label that dosn't declered but it is an opernd print error at the end of first scan */
		if(methodLabel(temp) && !searchLabel(*hptr,temp) && !methodImm(temp) && !isLabelExternal(*hptr, temp))
			addLabelToSymbols(hptr, temp, -1, 0, 0, 0);
		else if (methodPerLabel(temp) && !searchLabel(*hptr,temp + 1) && !methodImm(temp) && !isLabelExternal(*hptr, temp))
			addLabelToSymbols(hptr, temp + 1, -1, 0, 0, 0);
                i = betweenOperands(arr, i);
                if(!i) return ERROR;               
                i = copySubString(arr, temp, i);

                 if(methodImm(temp)==2)
                 {
                     errorImmOverFlow();
                     return ERROR;
                 }

                if(methodImm(temp) || methodLabel(temp) || methodPerLabel(temp))
                {   	
		/* in case there is a label that dosn't declered but it is an opernd print error at the end of first scan */
			if(methodLabel(temp) && !searchLabel(*hptr,temp) && !methodImm(temp) && !isLabelExternal(*hptr, temp))
				addLabelToSymbols(hptr, temp, -1, 0, 0, 0);
			else if (methodPerLabel(temp) && !searchLabel(*hptr,temp + 1) && !methodImm(temp) && !isLabelExternal(*hptr, temp))
				addLabelToSymbols(hptr, temp + 1, -1, 0, 0, 0);           
                    	if(isEnd(arr +i))
                    	{          
                           	*ic += 3;
                        return 1;
                    	}
                    	else
                    	{
                        return ERROR;
                    	}

                	}
                	else
                	{
                    		fprintf(stderr, "%s: ", temp);
                    		errorOperand();
                    	return ERROR;
          		}

            	}
            	else
            	{
                	fprintf(stderr, "%s: ", temp);
                	errorOperand();
                return ERROR;
            	}
        }

	if(opCode == MOV || opCode == ADD )
        {
	   /* checking for the right methods */
            	if(methodImm(temp) == 2)
            	{
                 	errorImmOverFlow();
                 return ERROR;
            	}
            	if(methodImm(temp) || methodLabel(temp) || methodReg(temp))
            	{       
		/* in case there is a label that dosn't declered but it is an opernd print error at the end of first scan */
			if(methodLabel(temp) && !searchLabel(*hptr,temp) && !methodImm(temp) && !isLabelExternal(*hptr, temp))
				addLabelToSymbols(hptr, temp, -1, 0, 0, 0); /*fprintf(stderr,"%s %d\n",temp,getLabelAddress(*hptr, temp));     */
                	i = betweenOperands(arr, i);
                	if(!i) return ERROR;
                	i = copySubString(arr, temp, i);
                	if(methodLabel(temp) || methodReg(temp))
                	{      
		 /* in case there is a label that dosn't declered but it is an opernd print error at the end of first scan */
				if(methodLabel(temp) && !searchLabel(*hptr,temp) && !methodReg(temp) && !isLabelExternal(*hptr, temp))
					addLabelToSymbols(hptr, temp, -1, 0, 0, 0);                    
                    		if(isEnd(arr +i)) /* needs 3 memory cells */
                    		{                                                      
                 			*ic += 3;	
                        	return 1;
                    		}
                    		else return ERROR;                   		                     
                    	}            	
                	if(methodImm(temp) || methodPerLabel(temp))
                	{
                    		fprintf(stderr, "%s: ", temp);
                    		errorOperand();
                    	return ERROR;
                	}
                	else
                	{
                    		fprintf(stderr, "%s: ", temp);
                    		errorOperand();
                    	return ERROR;
                	}
       		}
        	else
        	{
         		fprintf(stderr, "%s: ", temp);
                	errorOperand();
                return ERROR;
        	}
	}

        if(opCode == LEA)
        {	/* first operand has to be oa label */
            	if(!methodLabel(temp))
            	{
                	fprintf(stderr, "The first operand in command lea need to be a label");
                	errorLabel(temp);
                return ERROR;
            	}
		if(methodLabel(temp) && !searchLabel(*hptr,temp) && !isLabelExternal(*hptr, temp))
			addLabelToSymbols(hptr, temp, -1, 0, 0, 0);
            
                i = betweenOperands(arr, i); /* checking between the two operands */
                if(!i) return ERROR;
                i = copySubString(arr, temp, i);/* analayze the second operand */
                if(!methodReg(temp) && !methodLabel(temp))
                {
                    	fprintf(stderr, "The second operand in command lea has to be a register or label.");
                return ERROR;
                }
		if(methodLabel(temp) && !methodReg(temp) && !searchLabel(*hptr,temp) && !isLabelExternal(*hptr, temp))
			addLabelToSymbols(hptr, temp, -1, 0, 0, 0);
                if(isEnd(arr +i))
                {
                    *ic += 3;
                  return 1;
                }
        return ERROR;     	
	}

        if(opCode == PRN)
        {	/* checking for the right methods */

            if(methodImm(temp) == 2)
            {
                 errorImmOverFlow();
                 return ERROR;
            }
            if(methodImm(temp) || methodLabel(temp) || methodReg(temp) || methodPerLabel(temp) )
            {
		if(methodLabel(temp) && !searchLabel(*hptr,temp) && !methodReg(temp) && !isLabelExternal(*hptr, temp))
			addLabelToSymbols(hptr, temp, -1, 0, 0, 0);
		else if(methodPerLabel(temp) && !searchLabel(*hptr,temp + 1) && !methodReg(temp) && !isLabelExternal(*hptr, temp))
			addLabelToSymbols(hptr, temp + 1, -1, 0, 0, 0);
                if(isEnd(arr+i))
                {
                    *ic += 2;
                    return 1;
                }

                return ERROR;
            }
            else
            {
                fprintf(stderr, "%s: ", temp);
                errorOperand();
                return ERROR;
            }
        }

        if(opCode == NOT || opCode == RED) /*clr, not, inc, dec and red needs only 2 memory cells */
        {	
            if(methodLabel(temp) || methodReg(temp)) /* checking for the right methods */
            {	 
		if(!methodReg(temp) && !isLabelExternal(*hptr, temp))
		{	
			addLabelToSymbols(hptr, temp, -1, 0, 0, 0);
		}
                if(isEnd(arr+i))/*chek the end of the statement */
                {
                    *ic += 2;
                    return 1;
                }
                return ERROR;
            }
            else
            {
                fprintf(stderr, "%s: ", temp);
                errorOperand();
                return ERROR;
            }
        }
	
        if(opCode == JMP)/* jmp, bne and jsr commands have the same opcode number and needs 2 memory cells */
        {
		/* checking for the right methods */
		if(temp[0] =='%') strcpy(temp,temp +1); 
		if(searchLabel(*hptr, temp))
		 {	 	
			if(methodLabel(temp))
            		{
                		if(isEnd(arr+i))
                		{
                        		*ic += 2;
                        		return 1;        
                		}
                	return ERROR;
            		}		             
		 }
		addLabelToSymbols(hptr, temp, -1, 0, 1, 0);
            	if(methodLabel(temp) || methodPerLabel(temp))
            	{
                	if(isEnd(arr+i))
                	{                  		
                        		*ic += 2;
                        	return 1;                  		
                	}
                return ERROR;
            	}
            	else
            	{
                	fprintf(stderr,"%s: ", temp);
                	errorOperand();
                return ERROR;
           	}
        }
	
    }
 /* dealing with the .entry .extern commands */
    if(guide)
    {
        i = spaceSkiper(arr, i);
        j = 0;
        for( ; arr[i] && arr[i] != ' ' && arr[i] != '\t'; i++)
            temp[j++] = arr[i];
        temp[j] = '\0';
        if(isLabel(temp))  /* if the operand is a legal name */
        {
            i = spaceSkiper(arr, i);
            if(arr[i] == ',')
            {
                errorIllegalComma();
                return ERROR;
            }
            if(arr[i])
            {
                errorOneOperand();
                return ERROR;
            }
		
            if(guide == ENTRY)
            {             
		/* if the label has allready defined turn on the enternal flag else adding it to the list */
		    	if(!searchLabel(*hptr, temp))
				addLabelToSymbols(hptr, temp, -1, 0, 0, 1);
			if(!isLabelEnternal(*hptr, temp))
			setLabelAddress(*hptr, temp, getLabelAddress(*hptr, temp),1,0);/*update the entry field of the lebal*/ 								                        	
             return 1;
           }
            else /*if the label is external */
            {
                /* checking and adding the label to the list */
                if(searchLabel(*hptr, temp)) 
                { 		
                    	setLabelAddress(*hptr, temp, 0, 0, 1);
                    return 1;
                }
                addLabelToSymbols(hptr, temp, *ic, 1, 0, 0); /*fprintf(stderr, "%s %d\n", temp,getLabelAddress(*hptr, temp)); */
                return 1;
            }
        }
        else /*illegal label, print error massage */ 
        {
            errorLabel(temp);
            return ERROR;
        }

    }

    return 1;
}
