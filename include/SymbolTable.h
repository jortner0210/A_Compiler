#ifndef AC_SYMBOL_TABLE_H 
#define AC_SYMBOL_TABLE_H

#include "Common.h"

///////////////////////////// TOKEN TYPES

typedef enum
{
	AC_CONDITIONAL,
	AC_LOOP,
	AC_COMPARISON,
	AC_IDENTIFIER,
	AC_DATA_TYPE,
	AC_LITERAL,
	AC_OPERATOR,
	AC_BIT_WISE_OPERATOR,
	AC_PUNCUATION

} AC_TokenFamily;

// '//' next to enums need to be added to AC_getTokenInfo

typedef enum
{
	/* CONDITIONALS */

	AC_IF,
	AC_ELSE,

	/* LOOPS */

	AC_WHILE,
	AC_FOR,
	
	/* COMPARISION */

	AC_EQUAL,
	AC_NOT_EQUAL,

	AC_LESS_THAN,
	AC_LESS_THAN_EQUAL,

	AC_GREATER_THAN,
	AC_GREATER_THAN_EQUAL,

	/* IDENTIFIER */
	
	AC_USER_DEFINED_WORD, //
	AC_RESERVED_WORD, //	

	/* DATA TYPE */

	AC_INT_8, 
	AC_INT_16,
	AC_INT_32,

	AC_UINT_8, 
	AC_UINT_16,
	AC_UINT_32,

	AC_FLOAT_32,
	AC_FLOAT_64,

	AC_CHARACTER,

	/* LITERAL */

	AC_STRING_LITERAL,

	/* OPERATOR */

	AC_NOT,

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
	
	AC_MODULO,	
	AC_MODULO_ASSIGN,	

	AC_AND,
	AC_OR,

	/* PUNCUATION */
	
	AC_L_PAREN,
	AC_R_PAREN,
	AC_S_COLON,
	AC_COLON,
	AC_L_SQUARE,
	AC_R_SQUARE,
	AC_L_BRACKET,
	AC_R_BRACKET,
	AC_PERIOD,
	AC_COMMA,

	/* BIT WISE OPERATORS */

	AC_BIT_AND,
	AC_BIT_OR,
	AC_BIT_XOR,
	AC_BIT_FLIP,
	AC_BIT_LEFT_SHIFT,
	AC_BIT_RIGHT_SHIFT

} AC_TokenType;


typedef struct AC_TokenInfo 
{
	AC_TokenFamily tok_family;
	AC_TokenType   tok_type;
	uint8_t 	   is_keyword;
} AC_TokenInfo;

///////////////////////////// TOKEN FUNCTIONS

// TO DO: IMPLEMENT GET TOKEN TYPE AND GET TOKEN FAMILY

static AC_Result
AC_getTokenType
(
	const char *lexeme,
	AC_TokenType *tok_type
);

static AC_Result
AC_getTokenFamily
(
	AC_TokenType tok_type,
	AC_TokenFamily *tok_family,
	uint8_t *is_keyword
);

AC_Result 
AC_getTokenInfo
(
	const char *lexeme, 
	AC_TokenInfo *tok_info
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

	AC_TokenInfo info;

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
AC_initSymbolTable();

AC_Result 
AC_destroySymbolTable();

AC_Result
AC_printSymbolTable();

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

#endif
