#include "Core.h"
#include "Common.h"
#include "Scanner.h"

///////////////////////////// SCANNER FUNCTIONS

//
// Split string into tokens 
// 
// Returns zero if '\0' found
// Returns one if there are still more characters to parse
//
AC_Result AC_getToken(char *ptr, AC_Token *token)
{
	static uint32_t ln_num   =  0;
	static uint32_t char_num = -1;
	static char *next_ptr;

	char lexeme[AC_MAX_LEXEME_SIZE] = {};
	char curr_char;
	char peek_char;

	// Set static pointer
	if (ptr != NULL) { 
		next_ptr = ptr;
	}
	
	// Skip white space
	curr_char = (*next_ptr);
	while (curr_char == ' ' || curr_char == '\n' || curr_char == '\t') {
		next_ptr++;
		curr_char = (*next_ptr);
	}

	next_ptr++;
	peek_char = (*next_ptr);

	// Return early if end of file found	
	if (curr_char == '\0') 
		return AC_EOF;
	
	// Begin parsing current lexeme 
	token->lexeme[0]    = curr_char;
	uint32_t char_count = 1;
	switch (curr_char)
	{
		//
		// SINGLE CHARACTER LEXEMES 
		//
		case '(':
		case ')':
		case ';':
		case ':':
		case '[':
		case ']':
		case '{':
		case '}':
		case ',':
			token->lexeme[1] = '\0';
			break;
		
		//
		// LEXEMES THAT STAND ALONE, OR CAN BE PAIRED WITH '=' 
		//
		case '~': // '~'
		case '^': // '^'
		case '=': // '=' and '=='
		case '!': // '!' and '!='
		case '*': // '*' and '*='
		case '/': // '/' and '/='
		case '%': // '%' and '%='
			if (peek_char == '=') {
				token->lexeme[1] = peek_char;
				token->lexeme[2] = '\0';
				next_ptr++;
			}
			else 
				token->lexeme[1] = '\0';
			break;
		
		//
		// LEXEMES THAT STAND ALONE, CAN BE DUPLICATED '++', 
		// OR CAN BE PAIRED WITH '=' 
		//
		case '<': // '<' and '<<' and '<='
		case '>': // '>' and '>>' and '>='
		case '+': // '+' and '++' and '+='
			if (peek_char == '=' || curr_char == peek_char) {
				token->lexeme[1] = peek_char;
				token->lexeme[2] = '\0';
				next_ptr++;
			}
			else 
				token->lexeme[1] = '\0';
			break;

		//
		// LEXEMES THAT CAN STAND ALONE OR CAN BE DUPLICATED 
		//
		case '&':
		case '|':
			if (curr_char == peek_char) {
				token->lexeme[1] = peek_char;
				token->lexeme[2] = '\0';

				next_ptr++;
			}
			else {
				token->lexeme[1] = '\0';
			}	
			break;

		//
		// Identifiers: keywords, variables:
		// ([_]|[A...Z]|[a...z])([_]|[A...Z]|[a...z]|[0...9])* 
		//
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':	
		case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':	
		case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':	
		case 's': case 't': case 'u': case 'v': case 'w': case 'x':	
		case 'y': case 'z': 	
		
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':	
		case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':	
		case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':	
		case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':	
		case 'Y': case 'Z':

		case '_':
			while ((peek_char >= 'a' && peek_char <= 'z') ||
				   (peek_char >= 'A' && peek_char <= 'Z') ||
				   (peek_char >= '0' && peek_char <= '9') ||
				   (peek_char == '_')) 
				peek_char = AC_lexCatChar(token->lexeme, peek_char, &next_ptr, &char_count); 
			
			token->lexeme[char_count] = '\0';
			break;
		
		//
		// Handle '.' by itself and '.' at the beginning of a float:
		// (['.'])([0...9])*
		//
		case '.': 			
			while (peek_char >= '0' && peek_char <= '9') 
				peek_char = AC_lexCatChar(token->lexeme, peek_char, &next_ptr, &char_count); 
			
			token->lexeme[char_count] = '\0';
			break;	

		//
		// Handle integers and floats	
		//
		case '0': case '1': case '2': case '3': case '4': 
		case '5': case '6': case '7': case '8': case '9':
			while (peek_char >= '0' && peek_char <= '9') 
				peek_char = AC_lexCatChar(token->lexeme, peek_char, &next_ptr, &char_count); 
			
			if (peek_char == '.') {
				peek_char = AC_lexCatChar(token->lexeme, peek_char, &next_ptr, &char_count); 

				while (peek_char >= '0' && peek_char <= '9') 
					peek_char = AC_lexCatChar(token->lexeme, peek_char, &next_ptr, &char_count); 
			}
			token->lexeme[char_count] = '\0';
			break;
		
		//
		// Handle: '-', '--', '-=', negative ints, negative floats
		//
		case '-': ;
			char *next_next_ptr = next_ptr;
			next_next_ptr++;
			
			//
			// '-=' and '--'
			//
			if (peek_char == '=' || curr_char == peek_char) {
				token->lexeme[1] = peek_char;
				char_count++;
				next_ptr++;
			}
			
			//
			// (-)(.)([0...9])([0...9])* 
			//
			else if (peek_char == '.' && 
					((*next_next_ptr) >= '0' &&
					 (*next_next_ptr) <= '9') ) {
				peek_char = AC_lexCatChar(token->lexeme, peek_char, &next_ptr, &char_count); 

				while (peek_char >= '0' && peek_char <= '9') 
					peek_char = AC_lexCatChar(token->lexeme, peek_char, &next_ptr, &char_count); 
			}
			
			//
			// ('-')([0...9])([0...9])* 
			// ('-')([0...9])('.')([0...9])*
			//
			else if (peek_char >= '0' && peek_char <= '9') {
				while (peek_char >= '0' && peek_char <= '9') 
					peek_char = AC_lexCatChar(token->lexeme, peek_char, &next_ptr, &char_count); 
				
				if (peek_char == '.') {
					peek_char = AC_lexCatChar(token->lexeme, peek_char, &next_ptr, &char_count); 

					while (peek_char >= '0' && peek_char <= '9') 
						peek_char = AC_lexCatChar(token->lexeme, peek_char, &next_ptr, &char_count); 
					
				}
			}
			token->lexeme[char_count] = '\0';
			break;
	}

	// Fill out the rest of the struct	
	token->ln_num   = ln_num;
	token->char_num = char_num;
	AC_getTokenType(lexeme, &token->tok_type);

	return AC_SUCCESS;
}

//
// Dynamically allocates a buffer, reads contents of file into a buffer
//
// Returns AC_Return_Type Enum
//
AC_Result AC_readFile(const char *file_name, char **char_buffer, size_t *size)
{
	FILE* fp;	

	fp = fopen(file_name, "r");

	if (fp == NULL) {
		printf("ERROR: File not found!\n");
		return AC_IO_ERROR;
	}
	
	// Get file size
	fseek(fp, 0L, SEEK_END);
	(*size) = ftell(fp);

	if ((*size) == -1) {
		printf("ERROR: Seek error!\n");
		return AC_IO_ERROR;
	}

	fseek(fp, 0L, SEEK_SET);

	// Read entire file into buffer and null terminate just to be safe
	(*char_buffer) = (char *)malloc(sizeof(char) * ((*size) + 1));

	fread(*char_buffer, sizeof(char), *size, fp);
	
	if (ferror(fp) != 0) {
		printf("ERROR: Read error!\n");
		return AC_IO_ERROR;
	}

	(*char_buffer)[*size] = '\0';
	(*size)++;

	fclose(fp);

	return AC_SUCCESS;
}


AC_Result AC_sourceToTokenStream(const char *file_name)
{
	char *buffer;
	size_t buff_size;
	AC_Result ret;

	// Read File into buffer
	ret = AC_readFile(file_name, &buffer, &buff_size);
	if (ret != AC_SUCCESS) {
		printf("Error reading file!\n");
		return ret;
	}

	
	AC_Token token;
	//uint32_t tokens_available = AC_strLexeme(buffer, &token);
	AC_Result result = AC_getToken(buffer, &token);

	while (result == AC_SUCCESS) {
//		printf("%s ", token.lexeme);
		result = AC_getToken(NULL, &token);
	}
	printf("\n");
	free(buffer);

	return AC_SUCCESS;
}

///////////////////////////// SCANNER HELPER FUNCTIONS

//
// AC_getToken Helper function
//
static char AC_lexCatChar(char *lexeme, char new_char, char **next_ptr, uint32_t *char_count) {
	lexeme[*char_count] = new_char;;

	(*next_ptr)++;
	char next_char = (**next_ptr);
	(*char_count)++;
	return next_char;
}
