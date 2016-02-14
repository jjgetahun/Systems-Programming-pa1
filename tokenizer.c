/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */
 
 enum states {				
	undetermined,			//The begining of a token
	malformed,				
	int_float,				//final state (Integer) if no malformed char or '.' or 'e' is found. 
	oct_hex_float_zero,
	mightBeOct,				//Is the final state if no malformed character is found.
	mightBeHex, 			//Is the final state if no malformed character is found.
	mightBeHexFirst,
	mightBeFloatAfterDot,	//Is the final state if no malformed character is found, state can change if an 'e' is found
	mightBeFloat,
	mightBeFloat_onlyints_neg_pos, 
	mightBeFloat_onlyints,   //Is the final state if no malformed character is found after a negative or positive is found
	mightBeFloat_FirstInt,
	isAlphaNumeric, //Once a letter is found, it will be a word no matter what.
	done,
	mightBeLArrow,
	isLessThanEqual,
	mightBeLShift,
	isLShiftEqual,
	mightBeRArrow,
	isGreaterThanEqual,
	mightBeRShift,
	isRShiftEqual,
     mightBePlus,
     isIncrement,
     isPlusEqual,
     mightBeMinus,
     isDecrement,
     isMinusEqual,
     isStructPtr,
     isStructMember,
     mightBeMult,
     isMultEqual,
     mightBeDiv,
     isDivEqual,
     mightBeEqual,
     isEqualEqual,
     mightBeNegate,
     isNotEqual,
     mightBeBitAnd,
     isLogicalAnd,
     isBitAndEqual,
     mightBeBitXOR,
     isBitXOREqual,
     mightBeBitOr,
     isLogicalOr,
     isBitOrEqual,
     mightBeMod,
     isModEqual,
     is1sComp,
     isComma,
     isLeftBrace,
     isRightBrace,
     isLeftParenthesis,
     isRightParenthesis,
     isConditionalOperator,
     isSingleComment,
     isMultiComment,
     mightBeEOC,
     isEOC,
     isSingleQuote,
     isEOSQ,
     isDoubleQuote,
     isEODQ,
} curr_State, temp_State;

char err;
char* pc;
int onlySpaces = 0;

struct TokenizerT_ {
	int i;   			//Index number used later for creating tokens
	int stringSize;		//String size for getnexttoken function	
	char *myString;		//input string
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) {
	char copy[strlen(ts)]; 					//copy of string ts
	strcpy(copy,ts);  
	
	TokenizerT *ptr = (TokenizerT*) malloc (sizeof(struct TokenizerT_));   //Allocation of size for TokenizerT_
	ptr -> i = 0;		
	ptr -> stringSize = strlen(copy);	
	ptr -> myString = strdup(copy);
	
	pc = (ptr -> myString);
	
	return ptr;           /*returning the pointer*/
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
    free(tk);
}


/*beginning of all states and function calls for states. Each function checks for the
 * current state and current char and makes a decision based on that. 
 * It may change states or figure out that the current token is in a finished
 * state. Each function is called upon based on the previous state the FSM
 * was in. */

/*set the global char to the malform char of token*/
int malChar(char ptr) {
	err = ptr;
	return 0;
}

int chkAfterLeft(char x) {
	if (x == '=') {
		curr_State = isLessThanEqual;
		temp_State = curr_State;
	}
	else if (x == '<') {
		curr_State = mightBeLShift;
	}
	else {
		temp_State = curr_State;
		curr_State = done;
	}
	return 0;	
}

int chkLShift(char x) {
	if (x == '=') {
		curr_State = isLShiftEqual;
		temp_State = curr_State;
	}
	else {
		temp_State = curr_State;
		curr_State = done;
	}
	return 0;
}

int chkAfterRight(char x) {
	if (x == '=') {
		curr_State = isGreaterThanEqual;
		temp_State = curr_State;
	}
	else if (x == '>') {
		curr_State = mightBeRShift;
	}
	else {
		temp_State = curr_State;
		curr_State = done;
	}
	return 0;	
}

int chkRShift(char x) {
	if (x == '=') {
		curr_State = isRShiftEqual;
		temp_State = curr_State;
	}
	else {
		temp_State = curr_State;
		curr_State = done;
	}
	return 0;
}

int chkAfterPlus(char x) {
    if (x == '+') {
        curr_State = isIncrement;
        temp_State = curr_State;
    }
    else if (x == '=') {
        curr_State = isPlusEqual;
        temp_State = curr_State;
    }
    else {
        temp_State = curr_State;
        curr_State = done;
    }
    return 0;
}

int chkAfterMinus(char x) {
    if (x == '-') {
        curr_State = isDecrement;
        temp_State = curr_State;
    }
    else if (x == '=') {
        curr_State = isMinusEqual;
        temp_State = curr_State;
    }
    else if (x == '>') {
        curr_State = isStructPtr;
        temp_State = curr_State;
    }
    else {
        temp_State = curr_State;
        curr_State = done;
    }
    return 0;
}

int chkAfterMult(char x) {
    if (x == '=') {
        curr_State = isMultEqual;
        temp_State = curr_State;
    }
    else {
        temp_State = curr_State;
        curr_State = done;
    }
    return 0;
}

int chkAfterDiv(char x) {
    if (x == '=') {
        curr_State = isDivEqual;
        temp_State = curr_State;
    }
    else if (x == '/') {
        curr_State = isSingleComment;
        temp_State = curr_State;
    }
    else if (x == '*') {
        curr_State = isMultiComment;
        temp_State = curr_State;
    }
    else {
        temp_State = curr_State;
        curr_State = done;
    }
    return 0;
}

int chkAfterEqual(char x) {
    if (x == '=') {
        curr_State = isEqualEqual;
        temp_State = curr_State;
    }
    else {
        temp_State = curr_State;
        curr_State = done;
    }
    return 0;
}

int chkAfterNegate(char x) {
    if (x == '=') {
        curr_State = isNotEqual;
        temp_State = curr_State;
    }
    else {
        temp_State = curr_State;
        curr_State = done;
    }
    return 0;
}

int chkAfterBitAnd(char x) {
    if (x == '&') {
        curr_State = isLogicalAnd;
        temp_State = curr_State;
    }
    else if (x == '=') {
        curr_State = isBitAndEqual;
        temp_State = curr_State;
    }
    else {
        temp_State = curr_State;
        curr_State = done;
    }
    return 0;
}

int chkAfterBitXOR(char x) {
    if (x == '=') {
        curr_State = isBitXOREqual;
        temp_State = curr_State;
    }
    else {
        temp_State = curr_State;
        curr_State = done;
    }
    return 0;
}

int chkAfterBitOr(char x) {
    if (x == '|') {
        curr_State = isLogicalOr;
        temp_State = curr_State;
    }
    else if (x == '=') {
        curr_State = isBitOrEqual;
        temp_State = curr_State;
    }
    else {
        temp_State = curr_State;
        curr_State = done;
    }
    return 0;
}

int chkAfterMod(char x) {
    if (x == '=') {
        curr_State = isModEqual;
        temp_State = curr_State;
    }
    else {
        temp_State = curr_State;
        curr_State = done;
    }
    return 0;
}

int chkAfterSingleComment(char x) {
    if (x == 10 || x == '\0') {
        temp_State = curr_State;
        curr_State = done;
    }
    else {

    }
    return 0;
}

int chkAfterMultiComment(char x) {
    if (x == '*') {
        curr_State = mightBeEOC;
        temp_State = curr_State;
    }
    else if (x == '\0') {
        temp_State = curr_State;
        curr_State = done;
    }
    else {

    }
    return 0;
}

int chkAfterSecondStar(char x) {
    if (x == '/') {
        curr_State = isEOC;
        temp_State = curr_State;
    }
    else if (x == '\0') {
        temp_State = curr_State;
        curr_State = done;
    }
    else {
        curr_State = isMultiComment;
        temp_State = curr_State;
    }
    return 0;
}

int chkAfterSingleQuote(char x) {
    if (x == '\'') {
        curr_State = isEOSQ;
        temp_State = curr_State;
    }
    else if (x == '\0') {
        temp_State = curr_State;
        curr_State = done;
    }
    else {

    }
    return 0;
}

int chkAfterDoubleQuote(char x) {
    if (x == '"') {
        curr_State = isEODQ;
        temp_State = curr_State;
    }
    else if (x == '\0') {
        temp_State = curr_State;
        curr_State = done;
    }
    else {

    }
    return 0;
}

/* this will check whether or not the first character of token is a valid char of
 * (1-9), (this will be int or float), '0' (which will be float, hex, oct or zero state) or a letter (word)
 * or one of the many C operators or a malformed character. */
int isZeroLetterOrNot(char x) {
	if (x == '0') {
		curr_State = oct_hex_float_zero;
	}
	else if (x > '0' && x <= '9') {
		curr_State = int_float;
	}
	else if (x == '.') {
		curr_State = isStructMember;
	}	
	else if ((x >= 'a' && x <= 'z')||(x >= 'A' && x <= 'Z')) {
		curr_State = isAlphaNumeric;
	}
	else if (x == '<') {
		curr_State = mightBeLArrow;
	}
	else if (x == '>') {
		curr_State = mightBeRArrow;
	}
    else if (x == '+') {
        curr_State = mightBePlus;
    }
    else if (x == '-') {
        curr_State = mightBeMinus;
    }
    else if (x == '*') {
        curr_State = mightBeMult;
    }
    else if (x == '/') {
        curr_State = mightBeDiv;
    }
    else if (x == '=') {
        curr_State = mightBeEqual;
    }
    else if (x == '!') {
        curr_State = mightBeNegate;
    }
    else if (x == '&') {
        curr_State = mightBeBitAnd;
    }
    else if (x == '^') {
        curr_State = mightBeBitXOR;
    }
    else if (x == '|') {
        curr_State = mightBeBitOr;
    }
    else if (x == '%') {
        curr_State = mightBeMod;
    }
    else if (x == '~') {
        curr_State = is1sComp;
    }
    else if (x == ',') {
        curr_State = isComma;
    }
    else if (x == '[') {
        curr_State = isLeftBrace;
    }
    else if (x == ']') {
        curr_State = isRightBrace;
    }
    else if (x == '(') {
        curr_State = isLeftParenthesis;
    }
    else if (x == ')') {
        curr_State = isRightParenthesis;
    }
    else if (x == '?' || x == ':') {
        curr_State = isConditionalOperator;
    }
    else if (x == '\'') {
        curr_State = isSingleQuote;
    }
    else if (x == '"') {
        curr_State = isDoubleQuote;
    }
	else {
		curr_State = malformed;
		temp_State = malformed;
		malChar(x);
	}
	
	return 0;
}

/* if isZeroLetterOrNot determines that it could be a hex, float, oct or zero, 
 * this will determine if the next char is a 'x' or 'X' for hex, (0-7) for oct
 * or '.' for float and malform for all other characters. 'e' will not make it a 
 * float as the previous state was determined from to be a '0', "0e" should not be valid. */
int isOctHexOrFloat(char x) {
	if (x >= '0' && x <= '7') {
		curr_State = mightBeOct;
	}
	else if (x == 'x' || x == 'X') { 
		curr_State = mightBeHexFirst;
	}
	else if (x == '.') {
		curr_State =  mightBeFloatAfterDot;
	}
	else {
		temp_State = curr_State;
		curr_State = done;		
	}
	
	return 0;
}

/*This will change the state from mightBeOct to malform
 * if anything besides (0-7) is found*/
int isOctal(char x) {				
	if (x >= '0' && x <= '7') {
	}
	else {
		temp_State = curr_State;
		curr_State = done;		
	}
	return 0;
}

/*This will change the state from mightBeHex to malform
 * if anything besides (0-9), (a-f) or (A-F) is found is found*/
int isHex(char x) {
	if ((x >= '0' && x <= '9') || (x >= 'a' && x <= 'f') || (x >= 'A' && x <= 'F')) {
		}
	else {
		temp_State = curr_State;
		curr_State = done;		
	}
	return 0;
}

/*This will change the state from mightBeHexFirst to mightBeHex if the char
 * immediately after "0x" is valid hex number, else malformed. 
 * if anything besides (0-9), (a-f) or (A-F) is found is found*/
int isHexFirst(char x) {
	if ((x >= '0' && x <= '9') || (x >= 'a' && x <= 'f') || (x >= 'A' && x <= 'F')) {
		curr_State = mightBeHex;
		}
	else {
		curr_State = malformed;	
		temp_State = malformed;
		malChar(x);	
	}
	return 0;
}

/*This is for a float that consists of an 'e' and '+' or '-', the only thing left that 
 * should be found is integers, if not, malform state*/
int isDigit(char x) {
	if (x >= '0' && x <= '9') {
	}
	else {
		temp_State = curr_State;
		curr_State = done;
	}
	return 0;
}

/*This is for a float that consists of an 'e' and '+' or '-', the next char must be a number from 1 - 9
 * if not, malform state*/
int isDigitFloatFirst(char x) {
	if (x >= '1' && x <= '9') {
		curr_State = mightBeFloat_onlyints;
	}
	else {
		curr_State = malformed;
		temp_State = malformed;
		malChar(x);
	}
	return 0;
}

/*This will determine if a token is an integer or float, the 
 * possible values for integer are (0-9), '.', 'e', 'E' will change the 
 * state appropriately */
int isDigitDotOrE (char x) {
	if (x == '.') {
		curr_State = mightBeFloatAfterDot;
	}
	else if (x == 'e' || x == 'E') {
		curr_State = mightBeFloat_onlyints_neg_pos;
	}
	else if (x >= '0' && x <= '9') {
	}
	else {
		temp_State = curr_State;
		curr_State = done;
	}
	return 0;
}

/* This is called after a token is determined to be a float 
 * because a '.' has been found, only digits should be found after a '.' */
 int isDigitNotE(char x) {
	if (x >= '0' && x <= '9') {
		curr_State = mightBeFloat;
	}
	else {
		curr_State = malformed;
		temp_State = malformed;
		malChar(x);
	}
	return 0;
}
 
 /* This is called after a token is determined to be a float 
 * because a '.' and a number has been found and not an e afterwards, only digits and 'e' can be found after the first '.' */
int isDigitOrE(char x) {
	if (x >= '0' && x <= '9') {
		curr_State = mightBeFloat;
	}
	else if (x == 'e' || x == 'E'){
		curr_State = mightBeFloat_onlyints_neg_pos;
	}
	else {
		temp_State = curr_State;
		curr_State = done;
	}
	return 0;
}

/* This is called after a possible float token has an 'e'
 * the only possible char after this e is digits or a negative or positive sign */
int isDigitOrSign(char x) {
	if (x >= '1' && x <= '9') {
		curr_State	=  mightBeFloat_onlyints;
	}
	else if (x == '-' || x == '+') {
		curr_State = mightBeFloat_FirstInt;
	}
	else {
		curr_State = malformed;
		temp_State = malformed;
		malChar(x);
	}
	return 0;
}

/*This function will check if the next character after the
 * first character in a word token is valid, if it is not
 * state becomes undetermined again*/
int isAlphaNumericOrNot(char x) {
	if (isalnum(x)) {
	}
	else {
		temp_State = curr_State;
		curr_State = done;
	}
	return 0;
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {
	if (*pc == '\0') {
        TKDestroy(tk);
		exit (0);
	}
	
	char* token = (char*) malloc (((tk -> stringSize)+2)*sizeof(char));
	char* tempToken = (char*) malloc(sizeof(char*));	
	tempToken = token;
	
	while (isspace(*pc)) {
		pc++;
	}
		
	if (*pc == '\0' && onlySpaces == 0) {
        printf("Only whitespace entered\n");
        TKDestroy (tk);
		exit (0);
	}
    else if (*pc == '\0') {
        TKDestroy (tk);
        exit(0);
    }
	
	onlySpaces++;			//only if a nonspace is found;
	
	curr_State = undetermined;
	
	while (*pc != '\0') {
		if ((curr_State != done)&&(curr_State != undetermined)) {
            *token = *pc;
			pc++;
			token++;
		}
		switch(curr_State) {
			case (malformed): {     /*temporary*/
				*token = '\0';
				return tempToken;
			}
			case (undetermined): {
				isZeroLetterOrNot(*pc);
				break;
			}
			case (mightBeRArrow): {
				chkAfterRight(*pc);
				break;
			}
			case (mightBeRShift): {
				chkRShift(*pc);
				break;
			}
			case (isGreaterThanEqual): {
				*token = '\0';
				return tempToken;
			}
			case (isRShiftEqual): {
				*token = '\0';
				return tempToken;
			}
			case (mightBeLArrow): {
				chkAfterLeft(*pc);
				break;
			}
			case (mightBeLShift): {
				chkLShift(*pc);
				break;
			}
			case (isLessThanEqual): {
				*token = '\0';
				return tempToken;
			}
			case (isLShiftEqual): {
				*token = '\0';
				return tempToken;
			}
            case (mightBePlus): {
                chkAfterPlus(*pc);
                break;
            }
            case (isIncrement): {
                *token = '\0';
                return tempToken;
            }
            case (isPlusEqual): {
                *token = '\0';
                return tempToken;
            }
            case (mightBeMinus): {
                chkAfterMinus(*pc);
                break;
            }
            case (isDecrement): {
                *token = '\0';
                return tempToken;
            }
            case (isMinusEqual): {
                *token = '\0';
                return tempToken;
            }
            case (isStructPtr): {
                *token = '\0';
                return tempToken;
            }
            case (mightBeMult): {
                chkAfterMult(*pc);
                break;
            }
            case (isMultEqual): {
                *token = '\0';
                return tempToken;
            }
            case (mightBeDiv): {
                chkAfterDiv(*pc);
                break;
            }
            case (isDivEqual): {
                *token = '\0';
                return tempToken;
            }
            case (mightBeEqual): {
                chkAfterEqual(*pc);
                break;
            }
            case (isEqualEqual): {
                *token = '\0';
                return tempToken;
            }
            case (mightBeNegate): {
                chkAfterNegate(*pc);
                break;
            }
            case (isNotEqual): {
                *token = '\0';
                return tempToken;
            }
            case (mightBeBitAnd): {
                chkAfterBitAnd(*pc);
                break;
            }
            case (isLogicalAnd): {
                *token = '\0';
                return tempToken;
            }
            case (isBitAndEqual): {
                *token = '\0';
                return tempToken;
            }
            case (mightBeBitXOR): {
                chkAfterBitXOR(*pc);
                break;
            }
            case (isBitXOREqual): {
                *token = '\0';
                return tempToken;
            }
            case (mightBeBitOr): {
                chkAfterBitOr(*pc);
                break;
            }
            case (isLogicalOr): {
                *token = '\0';
                return tempToken;
            }
            case (isBitOrEqual): {
                *token = '\0';
                return tempToken;
            }
            case (mightBeMod): {
                chkAfterMod(*pc);
                break;
            }
            case (isModEqual): {
                *token = '\0';
                return tempToken;
            }
            case (is1sComp): {
                *token = '\0';
                temp_State = curr_State;
                curr_State = done;
                return tempToken;
            }
            case (isComma): {
                *token = '\0';
                temp_State = curr_State;
                curr_State = done;
                return tempToken;
            }
            case (isLeftBrace): {
                *token = '\0';
                temp_State = curr_State;
                curr_State = done;
                return tempToken;
            }
            case (isRightBrace): {
                *token = '\0';
                temp_State = curr_State;
                curr_State = done;
                return tempToken;
            }
            case (isStructMember): {
                *token = '\0';
                temp_State = curr_State;
                curr_State = done;
                return tempToken;
            }
            case (isLeftParenthesis): {
                *token = '\0';
                temp_State = curr_State;
                curr_State = done;
                return tempToken;
            }
            case (isRightParenthesis): {
                *token = '\0';
                temp_State = curr_State;
                curr_State = done;
                return tempToken;
            }
            case (isConditionalOperator): {
                *token = '\0';
                temp_State = curr_State;
                curr_State = done;
                return tempToken;
            }
            case (isSingleComment): {
                chkAfterSingleComment(*pc);
                break;
            }
            case (isMultiComment): {
                chkAfterMultiComment(*pc);
                break;
             }
            case (mightBeEOC): {
                chkAfterSecondStar(*pc);
                break;
            }
            case (isEOC): {
                *token = '\0';
                temp_State = curr_State;
                curr_State = done;
                return tempToken;
            }
            case (isSingleQuote): {
                chkAfterSingleQuote(*pc);
                break;
             }
            case (isEOSQ): {
                *token = '\0';
                temp_State = curr_State;
                curr_State = done;
                return tempToken;
            }
            case (isDoubleQuote): {
                chkAfterDoubleQuote(*pc);
                break;
             }
            case (isEODQ): {
                *token = '\0';
                temp_State = curr_State;
                curr_State = done;
                return tempToken;
            }
			case (oct_hex_float_zero): {
				isOctHexOrFloat(*pc);
				break;
			}
			case (mightBeOct): {
				isOctal(*pc);
				break;
			}
			case (mightBeHex): {
				isHex(*pc);
				break;
			}
			case (mightBeHexFirst): {
				isHexFirst(*pc);
				break;
			}
			case (mightBeFloatAfterDot): {
				isDigitNotE(*pc);
				break;
			}
			case (mightBeFloat): {
				isDigitOrE(*pc);
				break;
			}
			case (mightBeFloat_onlyints_neg_pos): {
				isDigitOrSign(*pc);
				break;
			}
			case (mightBeFloat_onlyints): {
				isDigit(*pc);
				break;
			}
			case (mightBeFloat_FirstInt): {
				isDigitFloatFirst(*pc);
				break;
			}
			case (int_float): {
				isDigitDotOrE(*pc);
				break;
			}
			case (isAlphaNumeric): {
				isAlphaNumericOrNot(*pc);
				break;
			}
			case (done): {
				*token = '\0';
				return tempToken;
			}			
			default:
			break;
		}
	}
	*token = '\0';
	return tempToken;
}

/*This function will check any word token to see if 
 * it a C keyword*/
int isJavaKeyword(char *toke) {
    if (strcasecmp(toke, "if") == 0) {
        return 1;
    }
    else if (strcasecmp(toke, "auto") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "break") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "case") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "char") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "const") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "continue") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "default") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "do") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "double") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "else") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "extern") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "float") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "for") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "goto") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "int") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "long") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "register") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "return") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "short") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "signed") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "sizeof") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "static") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "struct") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "switch") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "typedef") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "union") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "unsigned") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "void") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "volatile") == 0) {
        return 1;        
    }
    else if (strcasecmp(toke, "while") == 0) {
        return 1;        
    }
    return 0;
}

/* Returns the type of token for print to terminal*/
char* TKGetState(char* token) {
	switch (temp_State) {
		case(malformed): {
			return "ERROR";
			break;
		}
		case(mightBeFloatAfterDot): {
			return "ERROR";
			break;
		}
		case (oct_hex_float_zero): {
			return "ZERO";
			break;
		}
		case (mightBeOct): {
			return "OCTAL";
			break;
		}
		case (mightBeHex): {
			return "HEXADECIMAL";
			break;
		}
		case (mightBeFloat): {
			return "FLOAT";
			break;
		}
		case (mightBeFloat_onlyints_neg_pos): {
			return "FLOAT";
			break;
		}
		case (mightBeFloat_onlyints): {
			return "FLOAT";
			break;
		}
		case (int_float): {
			return "INTEGER";
			break;
		}
		case (isAlphaNumeric): {
			if (isJavaKeyword(token) == 1){
                return "C KEYWORD";
                break;
            }
            else {
                return "WORD";
			    break;
            }
		}
		case (mightBeRArrow): {
			return "GREATER THAN";
			break;
		}
		case (isGreaterThanEqual): {
			return "GREATER OR EQUAL";
			break;
		}
		case (mightBeRShift): {
			return "BITWISE RIGHT SHIFT";
			break;
		}
		case (isRShiftEqual): {
			return "BITWISE RIGHT SHIFT ASSIGNMENT";
			break;
		}
		case (mightBeLArrow): {
			return "LESS THAN";
			break;
		}
		case (isLessThanEqual): {
			return "LESS OR EQUAL";
			break;
		}
		case (mightBeLShift): {
			return "BITWISE LEFT SHIFT";
			break;
		}
		case (isLShiftEqual): {
			return "BITWISE LEFT SHIFT ASSIGNMENT";
			break;
		}
        case (mightBePlus): {
            return "PLUS";
            break;
        }
        case (isIncrement): {
            return "INCREMENT";
            break;
        }
        case (isPlusEqual): {
            return "PLUS EQUAL";
            break;
        }
        case (mightBeMinus): {
            return "MINUS/SUBTRACT";
            break;
        }
        case (isDecrement): {
            return "DECREMENT";
            break;
        }
        case (isMinusEqual): {
            return "MINUS EQUAL";
            break;
        }
        case (isStructPtr): {
            return "STRUCTURE POINTER";
            break;
        }
        case (mightBeMult): {
            return "MULTIPLY/INDIRECT";
            break;
        }
        case (isMultEqual): {
            return "MULTIPLY EQUAL";
            break;
        }
        case (mightBeDiv): {
            return "DIVIDE";
            break;
        }
        case (isDivEqual): {
            return "DIVIDE EQUAL";
            break;
        }
        case (mightBeEqual): {
            return "EQUAL";
            break;
        }
        case (isEqualEqual): {
            return "EQUAL EQUAL";
            break;
        }
        case (mightBeNegate): {
            return "NEGATE";
            break;
        }
        case (isNotEqual): {
            return "NOT EQUAL";
            break;
        }
        case (mightBeBitAnd): {
            return "BITWISE AND/ADDRESS";
            break;
        }
        case (isLogicalAnd): {
            return "LOGICAL AND";
            break;
        }
        case (isBitAndEqual): {
            return "BITWISE AND EQUAL";
            break;
        }
        case (mightBeBitXOR): {
            return "BITWISE EXCLUSIVE OR";
            break;
        }
        case (isBitXOREqual): {
            return "BITWISE EXCLUSIVE OR EQUAL";
            break;
        }
        case (mightBeBitOr): {
            return "BITWISE OR";
            break;
        }
        case (isLogicalOr): {
            return "LOGICAL OR";
            break;
        }
        case (isBitOrEqual): {
            return "BITWISE OR EQUAL";
            break;
        }
        case (mightBeMod): {
            return "MODULO";
            break;
        }
        case (isModEqual): {
            return "MODULO EQUAL";
            break;
        }
        case (is1sComp): {
            return "ONE'S COMPLEMENT";
            break;
        }
        case (isComma): {
            return "COMMA OPERATOR";
            break;
        }
        case (isLeftBrace): {
            return "LEFT BRACE";
            break;
        }
        case (isRightBrace): {
            return "RIGHT BRACE";
            break;
        }
        case (isLeftParenthesis): {
            return "LEFT PARENTHESIS";
            break;
        }
        case (isRightParenthesis): {
            return "RIGHT PARENTHESIS";
            break;
        }
        case (isConditionalOperator): {
            return "CONDITIONAL OPERATOR";
            break;
        }
        case (isStructMember): {
            return "STRUCT MEMBER";
            break;
        }
        case (isSingleComment): {
            return "SINGLE COMMENT";
            break;
         }
        case (isMultiComment): {
            return "MULTI COMMENT";
            break;
         }
        case (isEOC) : {        
            return "MULTI COMMENT";
            break;
        }
        case (isSingleQuote): {
            return "SINGLE QUOTE";
            break;
         }
        case (isEOSQ) : {        
            return "SINGLE QUOTE";
            break;
        }
        case (isDoubleQuote): {
            return "DOUBLE QUOTE";
            break;
         }
        case (isEODQ) : {        
            return "DOUBLE QUOTE";
            break;
        }
		default:
		break;
	}
	return 0;
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
	if (argc == 1 || argv[1][0] == '\0') {									//in case no arguments are given
		fprintf(stdout,"No arguments given!\n");
		return 0;
	}
		
	TokenizerT *tokenizer = TKCreate (argv[1]);        //creation of tokenizerT	
	curr_State = undetermined;	
	char* token;
	
	while (*tokenizer->myString != '\0') {
		token = TKGetNextToken(tokenizer);
		if (temp_State != isMultiComment && temp_State != isSingleComment && temp_State !=isEOC) { 
			char* status = TKGetState(token);
		    	fprintf(stdout,"%s: %s ",status,token);
		    	if (temp_State == malformed) {
		    	fprintf(stdout,"Error caused by: [0x%x]",err);
		    	}
            		printf("\n");
        	}
	}
	TKDestroy(tokenizer);
	return 0;
}
