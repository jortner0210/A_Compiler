#include "Core.h"
#include "SymbolTable.h"

///////////////////////////// TOKEN FUNCTIONS

AC_Result 
AC_getTokenInfo
(
	const char *lexeme,
	AC_TokenInfo *tok_info
)
{
	if (strcmp(lexeme, "if") == 0) {
		tok_info->tok_family = AC_CONDITIONAL;
		tok_info->tok_type 	 = AC_IF;
		tok_info->is_keyword = 1;
	}
}


///////////////////////////// SYMBOL TABLE GLOBAL STATE

AC_SymbolTableItem *_ac_hash_array[AC_MAX_HASH_ARRAY_SIZE] = {NULL};

///////////////////////////// SYMBOL TABLE FUNCTIONS

AC_Result
AC_printSymbolTableItem
(
	AC_SymbolTableItem *item
)
{
	printf("Key         : %s\n", item->key);
	printf("Token Type  : %d\n", item->info.tok_type);
	printf("Token Family: %d\n", item->info.tok_type);
	printf("/////////////////////////////\n\n"); 
}

///////////////////////////// SYMBOL TABLE FUNCTIONS

AC_Result
AC_hashStringSymbolTable
(
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
AC_Result
AC_insertSymbolTable
(
	char *key,
	AC_TokenType tok_type,
	AC_TokenFamily tok_family
)
{
	uint32_t hash_arr_idx;
	
	AC_hashStringSymbolTable(key, &hash_arr_idx);
	AC_SymbolTableItem *new_item = (AC_SymbolTableItem *)malloc(sizeof(AC_SymbolTableItem));
	
	strcpy(new_item->key, key);
	new_item->info.tok_type   = tok_type;
	new_item->info.tok_family = tok_family;	
	new_item->next 		 = NULL;
	
	if (_ac_hash_array[hash_arr_idx] == NULL) {
		_ac_hash_array[hash_arr_idx] = new_item;		
	}	
	else {
		AC_SymbolTableItem *curr_item = _ac_hash_array[hash_arr_idx];
		while (curr_item->next != NULL)
			curr_item = curr_item->next;
		curr_item->next = new_item;
	}
}

//
// Initialize the Symbol Table
//
AC_Result 
AC_initSymbolTable()
{
	AC_insertSymbolTable("T", AC_IF, AC_LOOP);
	AC_insertSymbolTable("Te", AC_IF, AC_LOOP);
	AC_insertSymbolTable("Tes", AC_IF, AC_LOOP);
	AC_insertSymbolTable("Test", AC_IF, AC_LOOP);
	AC_insertSymbolTable("Test_", AC_IF, AC_LOOP);
	AC_insertSymbolTable("Test_I", AC_IF, AC_LOOP);
	AC_insertSymbolTable("Test_ID", AC_IF, AC_LOOP);
}

//
// Print the contents of the Symbol Table to the terminal
// 
AC_Result
AC_printSymbolTable()
{
	printf("/----- AC Symbol Table -----/\n\n");
	for (int i = 0; i < AC_MAX_HASH_ARRAY_SIZE; i++) {
		if( _ac_hash_array[i] != NULL) {
			AC_SymbolTableItem *item_print = _ac_hash_array[i];;
			
			while (item_print != NULL) {
				AC_printSymbolTableItem(item_print);
				item_print = item_print->next;
			}
		}
		else {
			printf("Idx: %d, Val: %p\n", i, _ac_hash_array[i]);
			printf("/////////////////////////////\n\n"); 
		}
	}
}

