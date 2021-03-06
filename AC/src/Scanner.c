#include "Core.h"
#include "Common.h"
#include "Scanner.h"
#include "SymbolTable.h"

///////////////////////////// TOKEN TYPE FUNCTIONS

AC_Result AC_tokenTypeToString(
	AC_TokenType tok_type,
	char *string
)
{
	switch (tok_type)
	{
		case AC_ADD_SUBTRACT:	 strcpy(string, "Add/Subtract"); 	break;
		case AC_MULTIPLY_DIVIDE: strcpy(string, "Multiply/Divide"); break;
		case AC_NUMBER:		     strcpy(string, "Number"); 	  		break;
		case AC_SEMI_COLON:	     strcpy(string, "Semi Colon"); 		break;
		case AC_TERMINAL:	     strcpy(string, "Terminal Char");	break;
		default:	     	     strcpy(string, "Not Found");
	}
	return AC_SUCCESS;
}

///////////////////////////// TOKEN STRUCT FUNCTIONS

//
// Uses malloc to generate a new AC_Token struct, filling it 
// with the passed data and using AC_getTokenInfo to get
// information based on the lexeme. 
//
AC_Result AC_generateToken(
	AC_Token **tok_ptr,
	char *lexeme,
	uint32_t ln_num,
	uint32_t char_num,
	AC_TokenType tok_type
)
{
	static uint32_t tok_id = 0;

	AC_Token *new_token_ptr = (AC_Token *)malloc(sizeof(AC_Token));
	new_token_ptr->id 		= tok_id;
	new_token_ptr->ln_num 	= ln_num;
	new_token_ptr->char_num = char_num;
	new_token_ptr->tok_type = tok_type;

	//AC_getTokenInfo(lexeme, &new_token_ptr->info);
	strcpy(new_token_ptr->lexeme, lexeme);

	(*tok_ptr) = new_token_ptr;

	tok_id++;

	return AC_SUCCESS;
}

//
// Print the contents of a token to the screen.
//
AC_Result AC_printToken(
	AC_Token *token
)
{
	char tok_type_str[100];
	AC_tokenTypeToString(token->tok_type, tok_type_str);

	printf("Token(type={%s:%d}, ", tok_type_str, token->tok_type);
	printf("line#=%d, char#=%d, ", token->ln_num, token->char_num);
	printf("lexeme={%s})\n", token->lexeme);

	//printf("Token id: %d\n", token->id);
	//printf("Type    : %d, %s\n", token->tok_type, tok_type_str);
	//printf("Lexeme  : %s\n", token->lexeme);
	//printf("Line #%d, Char #%d\n", token->ln_num, token->char_num);
	//printf("/////////////////////////////\n");
}

///////////////////////////// TOKEN STREAM FUNCTIONS

//
// Allocate memory for the Token Stream struct and initialize its values to NULL.
//
// About AC_TokenStream:
// 	- Its a linked list
//  - The nodes in the list are AC_TokenStreamNode structs which contain:
// 		1) A pointer to an AC_Token struct 
// 		2) A next pointer to an AC_TokenStreamNode
//	- The struct AC_TokenStream contains a head and tail pointer
//
AC_Result AC_initTokenStream(
	AC_TokenStream **token_stream
)
{
	(*token_stream) = (AC_TokenStream *)malloc(sizeof(AC_TokenStream));
	(*token_stream)->head = NULL;
	(*token_stream)->tail = NULL;
	(*token_stream)->next = (*token_stream)->head;
	(*token_stream)->end_reached   = 1;
	(*token_stream)->curr_line_num = 0;
	(*token_stream)->curr_char_num = 0;
}

//
// Deallocate the resources of an AC_TokenStream
//
// NOTE: Deallocating the stream deallocates all of 
//		 the tokens that it was given.
//		 Tokens should not outlive the Token Stream.
//
AC_Result AC_destroyTokenStream(
	AC_TokenStream *token_stream
)
{
	if (token_stream != NULL && token_stream->head != NULL) {
		// The following is basic Linked List Traversal
		AC_TokenStreamNode *del_node;
		AC_TokenStreamNode *next_node;
		AC_Token *del_token;

		del_node = token_stream->head;

		while (del_node != NULL) {
			next_node = del_node->next;
			free(del_node->data);
			free(del_node);

			del_node = next_node;
		}
		free(token_stream);
	}
	AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "TOKEN STREAM DESTROYED!")
	return AC_SUCCESS;
}

AC_Result AC_bumpTokenStream(
	AC_TokenStream *token_stream
)
{
	if (token_stream->next != NULL) {
		token_stream->next = token_stream->next->next;

		if (token_stream->next == NULL) {
			token_stream->curr_token = NULL;
			token_stream->next_token = NULL;
		}
		else {
			AC_DEBUG_TRACE_FMT(AC_FINE, "Next Token: %s", token_stream->curr_token->lexeme)
			
			token_stream->curr_token = token_stream->next->data;
			if (token_stream->next->next == NULL) 
				token_stream->next_token = NULL;
			else 
				token_stream->next_token = token_stream->next->next->data;
			
			token_stream->curr_line_num = token_stream->curr_token->ln_num;
			token_stream->curr_char_num = token_stream->curr_token->char_num;
		}

		return AC_SUCCESS;
	}
	else {
		token_stream->next_token = NULL;
		token_stream->end_reached = 1;
		AC_DEBUG_TRACE_ARG(AC_FINE, "END OF TOKEN STREAM!")
		return AC_END_OF_TOKEN_STREAM;
	}
}

//
// Get a pointer to the next token in a stream.
// The 'next' pointer is held in the state of the token_stream.
//
AC_Result AC_nextTokenTokenStream(
	AC_TokenStream *token_stream,
	AC_Token **token_ptr
)
{
	if (token_stream->next != NULL) {
		(*token_ptr) = token_stream->next->data;
		token_stream->next = token_stream->next->next;
		token_stream->curr_line_num = (*token_ptr)->ln_num;
		token_stream->curr_char_num = (*token_ptr)->char_num;
		AC_DEBUG_TRACE_FMT(AC_FINE, "Next Token: %s", (*token_ptr)->lexeme)
		return AC_SUCCESS;
	}
	else {
		(*token_ptr) = NULL;
		token_stream->end_reached = 1;
		AC_DEBUG_TRACE_ARG(AC_FINE, "END OF TOKEN STREAM!")
		return AC_END_OF_TOKEN_STREAM;
	}
}

AC_Result AC_backTrackTokenStream(
	AC_TokenStream *token_stream
)
{
	AC_DEBUG_TRACE_ARG(AC_FINE, "Token Stream Backtracking")
	if (token_stream->next == NULL) {
		token_stream->next = token_stream->tail;
		token_stream->end_reached = 1;
	}
	else {
		token_stream->next = token_stream->next->prev;	
	}
}

//
// Add a new node to the token stream with data
// set to the passed AC_Token
//
AC_Result AC_appendTokenStream(
	AC_TokenStream *token_stream,
	AC_Token *token
)
{
	AC_TokenStreamNode *new_node = (AC_TokenStreamNode *)malloc(sizeof(AC_TokenStreamNode));
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->data = token;
	
	token_stream->end_reached = 0;

	// If the stream is empty set head and tail.
	// Otherwise append to tail and update tail.
	if (token_stream->head == NULL) {
		token_stream->head = new_node;
		token_stream->tail = new_node;
		token_stream->next = token_stream->head;
	}
	else {
		new_node->prev = token_stream->tail;
		token_stream->tail->next = new_node;
		token_stream->tail = new_node;
	}
	return AC_SUCCESS;
}

//
// Display the tokens in a Token Stream to the terminal.
//
AC_Result AC_printTokenStream(
	AC_TokenStream *token_stream
)
{
	printf("/----- AC Token Stream -----/\n\n");
	AC_TokenStreamNode *print_node = token_stream->head;
	while (print_node != NULL) {
		AC_printToken(print_node->data);
		print_node = print_node->next;
	}
}

//
// Sets the token_stream next member to the token_stream head memeber
//
AC_Result AC_resetTokenStream(
	AC_TokenStream *token_stream
)
{
	token_stream->next = token_stream->head;
	token_stream->end_reached = 0;
	
	if (token_stream->head != NULL) {
		token_stream->curr_token = token_stream->head->data;
		if (token_stream->head->next != NULL)
			token_stream->next_token = token_stream->head->next->data;
		else
			token_stream->next_token = NULL;
	}
	else {
		token_stream->curr_token = NULL;
		token_stream->next_token = NULL;
	}
}

//
// Converts a raw source file into a stream of identified tokens
//
// Stream is implemented as a linked list based queue 
//
// To deallocate resources: call AC_destroyTokenStream
//
AC_Result AC_sourceToTokenStream(
	const char *file_name,
	AC_TokenStream **token_stream
)
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

	AC_initTokenStream(token_stream);

	// Get next token from source and append to Token Stream
	// until there are no more tokens left.
	AC_Token *token;
	AC_Result result = AC_getToken(buffer, &token);

	while (result == AC_SUCCESS) {
		AC_appendTokenStream((*token_stream), token);
		result = AC_getToken(NULL, &token);
	}
	free(buffer);

	// Add terminal token to the end of the stream
	AC_generateToken(&token, "-$", 0, 0, AC_TERMINAL);
	AC_appendTokenStream((*token_stream), token);

	AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "TOKEN STREAM GENERATED!")

	return AC_SUCCESS;
}

///////////////////////////// SCANNER HELPER FUNCTIONS

//
// Split string into tokens 
// 
// Returns zero if '\0' found
// Returns one if there are still more characters to parse
//
static AC_Result AC_getToken(
	char *ptr, 
	AC_Token **token_ptr
) 
{
	static uint32_t ln_num   = 1;
	static uint32_t char_num = 0;
	static char *next_ptr;

	char lexeme[AC_MAX_LEXEME_SIZE];
	char curr_char;
	char peek_char;
	AC_TokenType tok_type;

	// Set static pointer
	if (ptr != NULL) { 
		next_ptr = ptr;
	}
	
	// Skip white space
	curr_char = (*next_ptr);
	char_num++;
	while (curr_char == ' ' || curr_char == '\n' || curr_char == '\t') {
		if (curr_char == '\n') {
			char_num = 0;
			ln_num++;
		}
		next_ptr++;
		curr_char = (*next_ptr);
		char_num++;
	}

	uint32_t char_start_num = char_num;
	next_ptr++;
	peek_char = (*next_ptr);

	// Return early if end of file found	
	if (curr_char == '\0') 
		return AC_EOF;
	
	// Begin parsing current lexeme 
	lexeme[0]    = curr_char;
	uint32_t char_count = 1;
	switch (curr_char)
	{

		//
		// SEMI COLON
		//
		case ';':
			lexeme[1] = '\0';
			tok_type = AC_SEMI_COLON;
			break;

		//
		// OPERATOR
		//
		case '+':
		case '-':
			lexeme[1] = '\0';
			tok_type = AC_ADD_SUBTRACT;
			break;

		case '*':
		case '/':
			lexeme[1] = '\0';
			tok_type = AC_MULTIPLY_DIVIDE;
			break;

		//
		// CONSTANT: integers and floats
		//
		case '0': case '1': case '2': case '3': case '4': 
		case '5': case '6': case '7': case '8': case '9':
			while (AC_isNumeric(peek_char) == AC_SUCCESS) {
				AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
				char_num++;
			}
			if (peek_char == '.') {
				AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
				char_num++;
				while (AC_isNumeric(peek_char) == AC_SUCCESS){
					AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
			 		char_num++; 
				}
			}
			lexeme[char_count] = '\0';
			tok_type = AC_NUMBER;
			break;

		default:
			AC_SCANNER_CHAR_ERROR(curr_char, ln_num, char_num)
			break;

		//
		// KEYWORD/IDENTIFIER
		//
		//case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':	
		//case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':	
		//case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':	
		//case 's': case 't': case 'u': case 'v': case 'w': case 'x':	
		//case 'y': case 'z': 	
		//case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':	
		//case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':	
		//case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':	
		//case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':	
		//case 'Y': case 'Z':
		//case '_':
		//	while ((peek_char >= 'a' && peek_char <= 'z') ||
		//		   (peek_char >= 'A' && peek_char <= 'Z') ||
		//		   (peek_char >= '0' && peek_char <= '9') ||
		//		   (peek_char == '_')) {
		//		AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
		//		char_num++;
		//	}
		//	lexeme[char_count] = '\0';
//
		//	AC_SymbolTableItem *st_lookup;
		//	AC_Result kw_res = AC_getItemSymbolTable(lexeme, &st_lookup);
		//	if (kw_res == AC_SYMBOL_TABLE_SYMBOL_FOUND)
		//		tok_type = st_lookup->tok_type;
		//	else
		//		tok_type = AC_IDENTIFIER;	
		//	break;	
		

		/*
		//
		// SINGLE CHARACTER LEXEMES 
		//
		//case '(':
		//case ')':
		case ';':
		//case ':':
		//case '[':
		//case ']':
		//case '{':
		//case '}':
		//case ',':
			lexeme[1] = '\0';
			tok_type = SEPARATOR;
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
				lexeme[1] = peek_char;
				lexeme[2] = '\0';
				next_ptr++;
			}
			else 
				lexeme[1] = '\0';
			break;
		
		//
		// LEXEMES THAT STAND ALONE, CAN BE DUPLICATED '++', 
		// OR CAN BE PAIRED WITH '=' 
		//
		case '<': // '<' and '<<' and '<='
		case '>': // '>' and '>>' and '>='
		case '+': // '+' and '++' and '+='
			if (peek_char == '=' || curr_char == peek_char) {
				lexeme[1] = peek_char;
				lexeme[2] = '\0';
				next_ptr++;
				char_num++;
			}
			else 
				lexeme[1] = '\0';
			break;

		//
		// LEXEMES THAT CAN STAND ALONE OR CAN BE DUPLICATED 
		//
		case '&':
		case '|':
			if (curr_char == peek_char) {
				lexeme[1] = peek_char;
				lexeme[2] = '\0';
				next_ptr++;
				char_num++;
			}
			else {
				lexeme[1] = '\0';
			}	
			break;
		
		//
		// STRING LITERALS
		//
		case '"':
			while (peek_char != '"') {
				AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
				char_num++;
			}

			AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
			char_num++;
			lexeme[char_count] = '\0';
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
				   (peek_char == '_')) {
				AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
				char_num++;
			}
			lexeme[char_count] = '\0';
			break;
		
		//
		// Handle '.' by itself and '.' at the beginning of a float:
		// (['.'])([0...9])*
		//
		case '.': 		
			while (AC_isNumeric(peek_char) == AC_SUCCESS) {
				AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
				char_num++;
			}
			lexeme[char_count] = '\0';
			break;	

		//
		// Handle integers and floats	
		//
		case '0': case '1': case '2': case '3': case '4': 
		case '5': case '6': case '7': case '8': case '9':
			while (AC_isNumeric(peek_char) == AC_SUCCESS) {
				AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
				char_num++;
			}
			if (peek_char == '.') {
				AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
				char_num++;
				while (AC_isNumeric(peek_char) == AC_SUCCESS){
					AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
			 		char_num++; 
				}
			}
			lexeme[char_count] = '\0';
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
				lexeme[1] = peek_char;
				char_count++;
				next_ptr++;
				char_num++;
			}
			
			//
			// (-)(.)([0...9])([0...9])* 
			//
			else if (peek_char == '.' && AC_isNumeric((*next_next_ptr)) == AC_SUCCESS) {
				AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
				char_num++;
				while (peek_char >= '0' && peek_char <= '9') {
					AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
					char_num++;
				}
			}
			
			//
			// ('-')([0...9])([0...9])* 
			// ('-')([0...9])('.')([0...9])*
			//
			else if (AC_isNumeric(peek_char) == AC_SUCCESS) {
				while (AC_isNumeric(peek_char) == AC_SUCCESS) {
					AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
					char_num++;
				}
				if (peek_char == '.') {
					AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count);
					char_num++;
				
					while (AC_isNumeric(peek_char) == AC_SUCCESS) {
						AC_lexCatChar(lexeme, &peek_char, &next_ptr, &char_count); 	
						char_num++;
					}
				}
			}
			lexeme[char_count] = '\0';
			break;
		*/
	}

	AC_generateToken(token_ptr, lexeme, ln_num, char_start_num, tok_type);

	return AC_SUCCESS;
}

//
// Malloc a buffer and then read the contents of a file into the buffer.
//
// Returns AC_Return_Type Enum
//
static AC_Result AC_readFile(
	const char *file_name, 
	char **char_buffer, 
	size_t *size
)
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

//
// AC_getToken Helper Function
// 
// Concatenates a character 'new_char' to the end of 'lexeme'
// Increments next_ptr, updates new_char value, and increments char_count
//
static AC_Result AC_lexCatChar(
	char *lexeme,
	char *new_char, 
	char **next_ptr, 
	uint32_t *char_count
) 
{
	lexeme[*char_count] = (*new_char);

	(*next_ptr)++;
	(*new_char) = (**next_ptr);
	(*char_count)++;
	return AC_SUCCESS;
}

//
// AC_getToken Helper Function
//
static AC_Result AC_isNumeric(
	char check_char
)
{
	if (check_char >= '0' && check_char <= '9')
		return AC_SUCCESS;
	else
		return AC_NOT_NUMERIC;
}
