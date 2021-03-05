#include "Core.h"
#include "SymbolTable.h"

///////////////////////////// SYMBOL TABLE ITEM STRUCT FUNCTIONS

AC_Result AC_printSymbolTableItem(
	AC_SymbolTableItem *item
)
{
	printf("Key         : %s\n", item->key);
	printf("Token Type  : %d\n", item->tok_type);
	printf("/////////////////////////////\n\n"); 
}


///////////////////////////// SYMBOL TABLE GLOBAL STATE

AC_SymbolTableItem *_ac_st_array[AC_MAX_HASH_ARRAY_SIZE] = {NULL};


///////////////////////////// SYMBOL TABLE FUNCTIONS

//
// Initialize the Symbol Table
//
AC_Result AC_initSymbolTable()
{
	AC_insertSymbolTable("int", AC_KEYWORD);

	AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "SYMBOL TABLE INITIALIZED!")
}

AC_Result AC_destroySymbolTable()
{
	for (int i = 0; i < AC_MAX_HASH_ARRAY_SIZE; i++) {
		if (_ac_st_array[i] != NULL) {
			AC_SymbolTableItem *del_item = _ac_st_array[i];
			AC_SymbolTableItem *next_item;
			while (del_item != NULL) {
				next_item = del_item->next;
				free(del_item);
				del_item = next_item;
			}
			_ac_st_array[i] = NULL;
		}
	}	
	AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "SYMBOL TABLE DESTORYED!")
}

//
// Print the contents of the Symbol Table to the terminal
// 
AC_Result AC_printSymbolTable()
{
	printf("/----- AC Symbol Table -----/\n\n");
	for (int i = 0; i < AC_MAX_HASH_ARRAY_SIZE; i++) {
		if( _ac_st_array[i] != NULL) {
			AC_SymbolTableItem *item_print = _ac_st_array[i];;
			
			while (item_print != NULL) {
				AC_printSymbolTableItem(item_print);
				item_print = item_print->next;
			}
		}
		else {
			printf("Idx: %d, Val: %p\n", i, _ac_st_array[i]);
			printf("/////////////////////////////\n\n"); 
		}
	}
}

//
// Hash function for symbol table
// TO DO: change sum of chars to something else
//		  - I think sum won't distribute evenly (use xor?)
//
AC_Result AC_hashStringSymbolTable(
	char *key,
	uint32_t *idx
)
{
	size_t key_len = strlen(key);
	uint32_t sum   = 0;
	// Cast chars to int and sum 
	for (size_t i = 0; i < key_len; i++) {
		sum += (uint8_t)key[i];
	}
	(*idx) = sum % AC_MAX_HASH_ARRAY_SIZE;
}

//
// Add Token to Symbol Table
//
AC_Result AC_insertSymbolTable(
	char *key,
	AC_TokenType tok_type
)
{
	uint32_t hash_arr_idx;
	
	AC_hashStringSymbolTable(key, &hash_arr_idx);
	AC_SymbolTableItem *new_item = (AC_SymbolTableItem *)malloc(sizeof(AC_SymbolTableItem));
	
	strcpy(new_item->key, key);
	new_item->tok_type = tok_type;
	new_item->next 	   = NULL;
	
	if (_ac_st_array[hash_arr_idx] == NULL) {
		_ac_st_array[hash_arr_idx] = new_item;		
	}	
	else {
		AC_SymbolTableItem *curr_item = _ac_st_array[hash_arr_idx];
		while (curr_item->next != NULL)
			curr_item = curr_item->next;
		curr_item->next = new_item;
	}
}

AC_Result AC_inSymbolTable(
	char *key
)
{
	uint32_t hash_arr_idx;
	uint32_t found = 0;

	AC_hashStringSymbolTable(key, &hash_arr_idx);

	if (_ac_st_array[hash_arr_idx] == NULL) {
		return AC_SYMBOL_TABLE_SYMBOL_NOT_FOUND;
	}
	AC_SymbolTableItem *curr_item = _ac_st_array[hash_arr_idx];
	while (curr_item != NULL) {
		if (strcmp(key, curr_item->key) == 0) {
			found = 1;
			break;
		}
		curr_item = curr_item->next;
	}
	if (found)
		return AC_SYMBOL_TABLE_SYMBOL_FOUND;
	else
		return AC_SYMBOL_TABLE_SYMBOL_NOT_FOUND;
}

AC_Result AC_getItemSymbolTable(
	char *key,
	AC_SymbolTableItem **ret_item
)
{
	if (AC_inSymbolTable(key) == AC_SYMBOL_TABLE_SYMBOL_FOUND) {
		uint32_t hash_arr_idx;

		AC_hashStringSymbolTable(key, &hash_arr_idx);

		AC_SymbolTableItem *curr_item = _ac_st_array[hash_arr_idx];
		while (curr_item != NULL) {
			if (strcmp(key, curr_item->key) == 0) {
				(*ret_item) = curr_item;
				return AC_SYMBOL_TABLE_SYMBOL_FOUND;
			}
			curr_item = curr_item->next;
		}

	}
	return AC_SYMBOL_TABLE_SYMBOL_NOT_FOUND;
}


///////////////////////////// TOKEN FUNCTIONS


/*
static AC_Result AC_getTokenType(
	const char *lexeme,
	AC_TokenType *tok_type
)
{
	if (strlen(lexeme) > 1) {

		// LITERAL
		if (lexeme[0] == '"') (*tok_type) = AC_STRING_LITERAL;	
	
		// CONDITIONALS 
		else if (strcmp(lexeme, "if") == 0)   (*tok_type) = AC_IF;	
		else if (strcmp(lexeme, "else") == 0) (*tok_type) = AC_ELSE;	
		
		// LOOPS 
		else if (strcmp(lexeme, "while") == 0) (*tok_type) = AC_WHILE;
		else if (strcmp(lexeme, "for") == 0)   (*tok_type) = AC_FOR;
		
		// COMPARISION 
		else if (strcmp(lexeme, "==") == 0) (*tok_type) = AC_EQUAL;
		else if (strcmp(lexeme, "!=") == 0) (*tok_type) = AC_NOT_EQUAL;
		else if (strcmp(lexeme, "<=") == 0) (*tok_type) = AC_LESS_THAN_EQUAL;
		else if (strcmp(lexeme, ">=") == 0) (*tok_type) = AC_GREATER_THAN_EQUAL;
			
		// DATA TYPE 
		else if (strcmp(lexeme, "i8") == 0)   (*tok_type) = AC_INT_8;
		else if (strcmp(lexeme, "i16") == 0)  (*tok_type) = AC_INT_16;
		else if (strcmp(lexeme, "i32") == 0)  (*tok_type) = AC_INT_32;
		else if (strcmp(lexeme, "u8") == 0)   (*tok_type) = AC_UINT_8;
		else if (strcmp(lexeme, "u16") == 0)  (*tok_type) = AC_UINT_16;
		else if (strcmp(lexeme, "u32") == 0)  (*tok_type) = AC_UINT_32;
		else if (strcmp(lexeme, "f32") == 0)  (*tok_type) = AC_FLOAT_32;
		else if (strcmp(lexeme, "f64") == 0)  (*tok_type) = AC_FLOAT_64;
		else if (strcmp(lexeme, "char") == 0) (*tok_type) = AC_CHARACTER;
			
		// OPERATOR 
		else if (strcmp(lexeme, "+=") == 0) (*tok_type) = AC_ADD_ASSIGN;
		else if (strcmp(lexeme, "-=") == 0) (*tok_type) = AC_SUBTRACT_ASSIGN;
		else if (strcmp(lexeme, "*=") == 0) (*tok_type) = AC_MULTIPLY_ASSIGN;
		else if (strcmp(lexeme, "/=") == 0) (*tok_type) = AC_DIVIDE_ASSIGN;
		else if (strcmp(lexeme, "%=") == 0) (*tok_type) = AC_MODULO_ASSIGN;
		else if (strcmp(lexeme, "--") == 0) (*tok_type) = AC_INCREMENT;
		else if (strcmp(lexeme, "++") == 0) (*tok_type) = AC_DECREMENT;
		else if (strcmp(lexeme, "&&") == 0) (*tok_type) = AC_AND;
		else if (strcmp(lexeme, "||") == 0) (*tok_type) = AC_OR;
			
		// BIT WISE OPERATORS 
		else if (strcmp(lexeme, "<<") == 0) (*tok_type) = AC_BIT_LEFT_SHIFT;
		else if (strcmp(lexeme, ">>") == 0) (*tok_type) = AC_BIT_RIGHT_SHIFT;

		// USER DEFINED IDENTIFIER
		else (*tok_type) = AC_USER_DEFINED_WORD;
	}
	else {
		switch ((*lexeme))
		{
			// PUNCTUATION 		
			case '(': (*tok_type) = AC_L_PAREN;   break;
			case ')': (*tok_type) = AC_R_PAREN;   break;
			case ';': (*tok_type) = AC_S_COLON;   break;
			case ':': (*tok_type) = AC_COLON;	  break;
			case '[': (*tok_type) = AC_L_SQUARE;  break;
			case ']': (*tok_type) = AC_R_SQUARE;  break;
			case '{': (*tok_type) = AC_L_BRACKET; break;
			case '}': (*tok_type) = AC_R_BRACKET; break;
			case ',': (*tok_type) = AC_COMMA; 	  break;
			case '.': (*tok_type) = AC_PERIOD;	  break;

			// COMPARISON 
			case '<': (*tok_type) = AC_LESS_THAN; 	 break;
			case '>': (*tok_type) = AC_GREATER_THAN; break;
			
			// OPERATOR 
			case '!': (*tok_type) = AC_NOT;	     break;
			case '=': (*tok_type) = AC_ASSIGN;   break;
			case '+': (*tok_type) = AC_ADD;	     break;
			case '-': (*tok_type) = AC_SUBTRACT; break;
			case '*': (*tok_type) = AC_MULTIPLY; break;
			case '/': (*tok_type) = AC_DIVIDE;	 break;
			case '%': (*tok_type) = AC_MODULO;	 break;
			
			// BIT WISE OPERATORS 
			case '&': (*tok_type) = AC_BIT_AND;  break;
			case '|': (*tok_type) = AC_BIT_OR;   break;
			case '^': (*tok_type) = AC_BIT_XOR;  break;
			case '~': (*tok_type) = AC_BIT_FLIP; break;
			
			// USER DEFINED IDENTIFIER
			default: (*tok_type) = AC_USER_DEFINED_WORD; break;
		}
	}
	
}

static AC_Result AC_getTokenFamily(
	AC_TokenType tok_type,
	AC_TokenFamily *tok_family,
	uint8_t *is_keyword
)
{
	if (tok_type == AC_STRING_LITERAL) 
	{
		(*tok_family) = AC_LITERAL;
		(*is_keyword) = 0;
	}
	else if (tok_type == AC_IF || tok_type == AC_ELSE) 
	{
		(*tok_family) = AC_CONDITIONAL;
		(*is_keyword) = 1;
	}
	else if (tok_type == AC_FOR || tok_type == AC_WHILE) 
	{
		(*tok_family) = AC_LOOP;
		(*is_keyword) = 1;
	}
	else if (tok_type == AC_EQUAL 		    
			 || tok_type == AC_NOT_EQUAL
			 || tok_type == AC_LESS_THAN_EQUAL 
			 || tok_type == AC_GREATER_THAN_EQUAL 
			 || tok_type == AC_LESS_THAN 		
			 || tok_type == AC_GREATER_THAN) 
	{
		(*tok_family) = AC_COMPARISON;
		(*is_keyword) = 0;
	}
	else if (tok_type == AC_INT_8 	 
			 || tok_type == AC_INT_16   
			 || tok_type == AC_INT_32  
			 || tok_type == AC_UINT_8
			 || tok_type == AC_UINT_16  
			 || tok_type == AC_UINT_32 
			 || tok_type == AC_FLOAT_32 
			 || tok_type == AC_FLOAT_64 
			 || tok_type == AC_CHARACTER )		 
	{
		(*tok_family) = AC_DATA_TYPE;
		(*is_keyword) = 1;
	}
	else if (tok_type == AC_NOT 
			 || tok_type == AC_ASSIGN 
			 || tok_type == AC_ADD 
			 || tok_type == AC_SUBTRACT 
			 || tok_type == AC_MULTIPLY 
			 || tok_type == AC_DIVIDE 
			 || tok_type == AC_MODULO 
			 || tok_type == AC_ADD_ASSIGN 
			 || tok_type == AC_SUBTRACT_ASSIGN 
			 || tok_type == AC_MULTIPLY_ASSIGN 
			 || tok_type == AC_DIVIDE_ASSIGN 
			 || tok_type == AC_MODULO_ASSIGN 
			 || tok_type == AC_INCREMENT 
			 || tok_type == AC_DECREMENT 
			 || tok_type == AC_AND 
			 || tok_type == AC_OR)		 
	{
		(*tok_family) = AC_OPERATOR;
		(*is_keyword) = 0;
	}
	else if (tok_type == AC_BIT_LEFT_SHIFT 
			 || tok_type == AC_BIT_RIGHT_SHIFT 
			 || tok_type == AC_BIT_AND 
			 || tok_type == AC_BIT_OR 
			 || tok_type == AC_BIT_XOR 
			 || tok_type == AC_BIT_FLIP)		 
	{
		(*tok_family) = AC_BIT_WISE_OPERATOR;
		(*is_keyword) = 0;
	}
	else if (tok_type == AC_L_PAREN
			 || tok_type == AC_R_PAREN
			 || tok_type == AC_S_COLON
			 || tok_type == AC_COLON
			 || tok_type == AC_L_SQUARE
			 || tok_type == AC_R_SQUARE
			 || tok_type == AC_L_BRACKET
			 || tok_type == AC_R_BRACKET
			 || tok_type == AC_PERIOD
			 || tok_type == AC_COMMA)		 
	{
		(*tok_family) = AC_PUNCUATION;
		(*is_keyword) = 0;
	}
}

AC_Result AC_getTokenInfo(
	const char *lexeme,
	AC_TokenInfo *tok_info
)
{

	AC_getTokenType(lexeme, &tok_info->tok_type);
	AC_getTokenFamily(tok_info->tok_type, &tok_info->tok_family, &tok_info->is_keyword);
	return AC_SUCCESS;
}*/
