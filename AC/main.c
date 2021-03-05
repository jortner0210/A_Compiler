#include "AC.h"

int main(int argc, char* argv[]) 
{
	if (argc < 2) {
		printf("Please provide input file\n");
		return -1;
	}
	//
	// Initialize globals
	//
	AC_initSymbolTable();	

	const char *file_name = argv[1];
	AC_Result result;

	//
	// Generate Token Stream
	//
	AC_TokenStream *token_stream;
	result = AC_sourceToTokenStream(file_name, &token_stream); 
	//AC_printTokenStream(token_stream);

	//
	// Parse Token Stream
	//
	result = AC_parseTokenStream(token_stream);

	//
	// Deallocate Resources
	//
	AC_destroyTokenStream(token_stream);
	AC_destroySymbolTable();
	
	return 0;
}
