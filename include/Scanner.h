#ifndef AC_SCANNER_H
#define AC_SCANNER_H

/*
	SCANNER - STAGE 1
	- TASK: Transform a stream of characters into a stream
			of words.
			Each word will be classified into a "part of speech."
*/

/*
	Pseudocode:
	
	char next_char;

	Token result;

	skipWhiteSpace();

	if (no more input) {
		result = new Token(EOF); return result;
	}

	switch(next_char)
	{
		case '(': result = new Token(LPAREN); getChar(); return result;
		case ')': result = new Token(RPAREN); getChar(); return result;
		// etc punctuation.. 
	
		case '!': // ! or !=
			getChar();
			if (next_char == '=') {
				result = new Token(NEQ); getChar(); return result;
			} else {
				result = new Token(NOT); return result;
			}
		// etc...
		
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': // integer constant
			String num = next_char;
			getChar();
			while (next_char is a digit) {
				num = num + next_char; 
				getChar();
			}
			result = new Token(INT, num);
			return result;
	

		case 'a': ... case 'z':
		case 'A': ... case 'Z': // id or keyword
			String s = next_char;
			getChar();
			while (next_char is a letter, digit, underscore) {
				s = s + next_char;
				getChar();
			}

			if (s is keyword) {
				result = new Token(KeywordID); 
			} else {
				result = new Token(ID, s);
			}
			return result;
	}
*/

#include "Core.h"
#include "SymbolTable.h"

///////////////////////////// SCANNER TYPES

#define AC_MAX_LEXEME_SIZE 100

typedef struct AC_Token
{
	AC_TokenFamily tok_family; // General	
	AC_TokenType tok_type;	    // Specific

	char lexeme[AC_MAX_LEXEME_SIZE];
	uint32_t ln_num;
	uint32_t char_num;

} AC_Token;

///////////////////////////// SCANNER FUNCTIONS

AC_Result 
AC_getToken
(
	char *ptr, 
	AC_Token *token
);

AC_Result 
AC_readFile
(
	const char *file_name, 
	char **char_buffer, 
	size_t *size
);

AC_Result 
AC_sourceToTokenStream
(
	const char *file_name
);

///////////////////////////// SCANNER HELPER FUNCTIONS

static char 
AC_lexCatChar
(
	char *lexeme, 
	char new_char, 
	char **next_ptr, 
	uint32_t *char_count
);

#endif
