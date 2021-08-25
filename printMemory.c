#include <stdio.h>
#include "constants.h"
#include "memoryStructs.h"
#include "supportingFun.h"

char hexa(int );

char are(int );

/* gets a memory section, the length of the instructions counter ic,
   the length of the data counter dc, and a file pointer, and copies the memory to the file
   in hexa */

void printMemory(memorySection memory, int ic, int dc,int amountOfWords, FILE * fob)
{
	int i, j, b1, b2, b3,b4, num;
	char temp[3] = {0};
    	unsigned int *p;
	unsigned mask=1;
	int btc = ITERATIONS ; /* btc = bits to convert to intiger nunmber*/
	fprintf(fob, "\n");
	fprintf(fob, "    %d  %d\n", amountOfWords , dc); /* print the words and data acount */
    	for(i = 0; i <= ic && i < MAX_MEMORY; i++)
    	{	j = 0;
		mask = 1;
		btc = ITERATIONS;		
        	p = (unsigned int *)(&memory[i]);
        	fprintf(fob, "%04d ", i + MEMORY_START);
		while(btc)
		{
		    	if(mask&(*p))
		        	b1=1;
		    	else
		        	b1=0;
		    	mask = mask << 1;
		    	if(mask&(*p))
		        	b2=1;
		    	else
		        	b2=0;
		    	mask = mask << 1;
		    	if(mask&(*p))
		        	b3=1;
		    	else
		        	b3=0;
		    	mask = mask << 1;
			if(mask&(*p))
		        	b4=1;
		    	else
		        	b4=0;
		    	mask = mask << 1;
		    temp[j++] = (char)(power(2,0)*b1 + power(2,1)*b2 + power(2,2)*b3 + power(2,3)*b4);		
		    	btc--;
		}
		for(j = 2;j >= 0;j--) 
		{
			if(temp[j] >= 0 && temp[j] <= 9)  fprintf(fob, "%d",temp[j]);
			else fprintf(fob, "%c", hexa((int)temp[j]));
		}
			
		if(mask&(*p))
		        b1=1;
		else
		        b1=0;
	    	mask = mask <<1;
		if(mask&(*p))
		        b2=1;
		    else
		        b2=0;
		    
		b1 = (power(2,0)*b1);
		b2 = (power(2,1)*b2);
		num = b1+b2;
		fprintf(fob, " %c\n",are(num));	
		
	}	
		
}

char hexa(int num)
{	 
	char val=(char)num;
	if(num == 10 ) return 'A';
	if(num == 11 ) return 'B';
	if(num == 12 ) return 'C';
	if(num == 13 ) return 'D';
	if(num == 14 ) return 'E';
	if(num == 15 ) return 'F';
return val;
}


char are(int num)
{
	 
	if(num == 0) return 'A';
	if(num == 1) return 'R';
	if(num == 2) return 'E';
return 'F';

}











