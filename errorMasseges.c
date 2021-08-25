#include <stdio.h>

/* all types of error messages */

void errorIllegalComma()
{
    fprintf(stderr, "Eror: Illegal command. ");
}

void errorMissingComma()
{
    fprintf(stderr, "Eror: Missing integer number after , character. ");
}

void errorCommandName()
{
    fprintf(stderr, "Eror: Illegal command. ");
}

void errorNumber()
{
    fprintf(stderr, "Eror: Expected integer number. ");
}

void errorString()
{
    fprintf(stderr,"Eror: Expected quotations for the string. ");
}

void errorLabel(char labelName[]  )
{
    fprintf(stderr, "Eror: Invalid label name - " "%s" " or label is too long. ",labelName);
}

void errorLabelInUse()
{
    fprintf(stderr, "Eror: Label has already defined. ");
}

void errorLabelDeclareEntry()
{
    fprintf(stderr, "Eror: Label has not been declared or has been declared external.");
}

void errorLabelDeclareExtern()
{
    fprintf(stderr, "Eror: Label has not been declared or has been declared internal.");
}

void errorLabelDeclaration()
{
    fprintf(stderr, "Eror: Undeclared label. ");
}

void errorOperand()
{
    fprintf(stderr, "Eror: Illegal operand. ");
}

void errorZeroOperand()
{
    fprintf(stderr, "Eror: This command doesn't allow any Operands. ");
}

void errorOneOperand()
{
    fprintf(stderr, "Eror: AT this command needed only one Operand. ");
}

void errorTwoOperand()
{
    fprintf(stderr, "Eror: AT this command needed two Operands. ");
}

void errorDataOverFlow()
{
    fprintf(stderr, "Eror: Data overflow, value can not be repesent in 12 bits. ");
}

void errorImmOverFlow()
{
    fprintf(stderr, "Eror: Immediate overflow, value can not be repesent in 12 bits. ");
}

