#include "AC.h"

int main(int argc, char* argv[]) 
{
	if (argc < 2) {
		printf("Please provide input file\n");
		return -1;
	}

	const char *file_name = argv[1];

	AC_Result result;
	AC_TokenStream *token_stream;

	result = AC_sourceToTokenStream(file_name, &token_stream); 

	AC_printTokenStream(token_stream);
	AC_destroyTokenStream(token_stream);

	AC_initSymbolTable();
	AC_printSymbolTable();	
	AC_destroySymbolTable();

	return 0;
}
