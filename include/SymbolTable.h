#ifndef AC_SYMBOL_TABLE_H 
#define AC_SYMBOL_TABLE_H

#include "Common.h"

///////////////////////////// TOKEN TYPES

typedef enum
{
	AC_LOOP,
	AC_COMPARISON,
	AC_IDENTIFIER,
	AC_DATA_TYPE,
	AC_LITERAL,
	AC_OPERATOR,
	AC_PUNCUATION

} AC_TokenFamily;

typedef enum
{
	/* LOOPS */

	AC_IF,
	AC_ELSE,
	AC_WHILE,
	AC_FOR,
	
	/* COMPARISION */

	AC_EQUAL,
	AC_NOT_EQUAL,

	AC_LESS_THAN,
	AC_LESS_THAN_EQUAL,

	AC_GREATER_THAN,
	AC_GREATER_THAN_EQUAL,

	AC_MODULO,	

	/* IDENTIFIER */
	
	AC_FUNCTION,
	AC_VARIABLE,
	AC_KEY_WORD,	

	/* DATA TYPE */

	AC_INT,
	AC_FLOAT,
	AC_DOUBLE,
	AC_CHARACTER,

	/* LITERAL */

	AC_STRING_LITERAL,

	/* OPERATOR */

	AC_ASSIGN,

	AC_ADD,
	AC_ADD_ASSIGN,

	AC_SUBTRACT,
	AC_SUBTRACT_ASSIGN,

	AC_MULTIPLY,
	AC_MULTIPLY_ASSIGN,

	AC_DIVIDE,
	AC_DIVIDE_ASSIGN,

	AC_INCREMENT,
	AC_DECREMENT,

	/* PUNCUATION */
	
	AC_L_PAREN,
	AC_R_PAREN,
	AC_S_COLON,
	AC_COLON,
	AC_L_SQUARE,
	AC_R_SQUARE,
	AC_L_BRACKET,
	AC_R_BRACKET,
	AC_PERIOD	

} AC_TokenType;

///////////////////////////// TOKEN FUNCTIONS

AC_Result 
AC_getTokenType
(
	const char *lexeme, 
	AC_TokenType *tok_type
);

AC_Result 
AC_getTokenFamily
(
	AC_TokenType tok_type, 
	AC_TokenFamily *tok_family
);

//
// Current Symbol Table Implementation:
//  - Array of linked lists
//  - Hash Function generates index in array
//



///////////////////////////// SYMBOL TABLE TYPES

#define AC_MAX_SYMBOL_TABLE_SIZE 100

typedef struct AC_SymbolTableItem
{
	char key[AC_MAX_SYMBOL_TABLE_SIZE];

	AC_TokenType   tok_type;
	AC_TokenFamily tok_family;

	struct AC_SymbolTableItem *next;
	
} AC_SymbolTableItem;


///////////////////////////// SYMBOL TABLE GLOBAL STATE

#define AC_MAX_HASH_ARRAY_SIZE 3

extern AC_SymbolTableItem *_ac_hash_array[AC_MAX_HASH_ARRAY_SIZE];

///////////////////////////// SYMBOL TABLE ITEMS FUNCTION

AC_Result
AC_printSymbolTableItem
(
	AC_SymbolTableItem *item
);

///////////////////////////// SYMBOL TABLE FUNCTIONS

AC_Result 
AC_hashStringSymbolTable
(
	char *key,
	uint32_t *idx
);

AC_Result
AC_insertSymbolTable
(
	char *key,
	AC_TokenType tok_type,
	AC_TokenFamily tok_family
);

AC_Result 
AC_initSymbolTable();

AC_Result
AC_printSymbolTable();

#endif
