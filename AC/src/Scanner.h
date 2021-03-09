#ifndef AC_SCANNER_H
#define AC_SCANNER_H

//
//	SCANNER - STAGE 1
//	- TASK: Transform a stream of characters into a stream of words.
//			Each word will be classified into a "part of speech."
//

#include "Common.h"
#include "Core.h"

///////////////////////////// TOKEN TYPES

typedef enum AC_TokenType
{
	AC_ADD_SUBTRACT,
	AC_MULTIPLY_DIVIDE,   
	AC_NUMBER,     
	AC_SEMI_COLON,

	AC_TERMINAL

} AC_TokenType; 

AC_Result AC_tokenTypeToString(
	AC_TokenType tok_type,
	char *string
);


///////////////////////////// TOKEN STRUCT

#define AC_MAX_LEXEME_SIZE 100

typedef struct AC_Token
{
	uint32_t id;

	AC_TokenType tok_type;
	uint32_t ln_num;
	uint32_t char_num;

	char lexeme[AC_MAX_LEXEME_SIZE];

} AC_Token;

AC_Result AC_generateToken(
	AC_Token **tok_ptr,
	char *lexeme,
	uint32_t ln_num,
	uint32_t char_num,
	AC_TokenType tok_type
);

AC_Result AC_printToken(
	AC_Token *token
);


///////////////////////////// TOKEN STREAM STRUCT

typedef struct AC_TokenStreamNode
{
	AC_Token *data;
	struct AC_TokenStreamNode *next;
	struct AC_TokenStreamNode *prev;

} AC_TokenStreamNode;

typedef struct AC_TokenStream
{
	AC_TokenStreamNode *head;
	AC_TokenStreamNode *next;
	AC_TokenStreamNode *tail;

	uint32_t end_reached;
	uint32_t curr_line_num;
	uint32_t curr_char_num;

	AC_Token *curr_token;
	AC_Token *next_token;

} AC_TokenStream;

AC_Result AC_initTokenStream(
	AC_TokenStream **token_stream
);

AC_Result AC_destroyTokenStream(
	AC_TokenStream *token_stream
);

AC_Result AC_bumpTokenStream(
	AC_TokenStream *token_stream
);

AC_Result AC_nextTokenTokenStream(
	AC_TokenStream *token_stream,
	AC_Token **token_ptr
);

AC_Result AC_backTrackTokenStream(
	AC_TokenStream *token_stream
);

AC_Result AC_appendTokenStream(
	AC_TokenStream *token_stream,
	AC_Token *token
);

AC_Result AC_printTokenStream(
	AC_TokenStream *token_stream
);

AC_Result AC_resetTokenStream(
	AC_TokenStream *token_stream
);

AC_Result AC_sourceToTokenStream(
	const char *file_name,
	AC_TokenStream **token_stream
);


///////////////////////////// SCANNER HELPER FUNCTIONS

static AC_Result AC_getToken(
	char *ptr, 
	AC_Token **token_ptr
);

static AC_Result AC_readFile(
	const char *file_name, 
	char **char_buffer, 
	size_t *size
);

static AC_Result AC_lexCatChar(
	char *lexeme,
	char *new_char, 
	char **next_ptr, 
	uint32_t *char_count
);

static AC_Result AC_isNumeric(
	char check_char
);

#endif
