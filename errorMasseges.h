/* error messages */

/* prints out a message for illegal comma */
void errorIllegalComma();

/* prints out a message for missing comma */
void errorMissingComma();

/* prints out a message for unrecognized command name */
void errorCommandName();

/* prints out a message for illegal number */
void errorNumber();

/* prints out a message for illegal string */
void errorString();

/* prints out a message for illegal label (variable) name */
void errorLabel(char []);

/* prints out a message for a label already in use */
void errorLabelInUse();

/* prints out a message for a label that hasn't been declared yet or if it is defined as an external */
void errorLabelDeclareEntry();

/* prints out a message for a label that hasn't been declared yet, or if it is defined as an internal */
void errorLabelDeclareExtern();

/* prints out a message for a label that hasn't been declared yet */
void errorLabelDeclaration();

/* prints out a message for illegal operand */
void errorOperand();

/* prints out a message for operands that comes after OPCODEs that don't accept any operand */
void errorZeroOperand();

/* prints out a message for operands that comes after OPCODEs that accept more one operand */
void errorOneOperand();

/* prints out a message for operands that comes after OPCODEs that accept two operands */
void errorTwoOperand();

/*prints out a message for data overflow*/
void errorDataOverFlow();

/*prints out a message for Immediate overflow*/
void  errorImmOverFlow();
