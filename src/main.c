#include "AC.h"

int main(int argc, char* argv[]) 
{
	if (argc < 2) {
		printf("Please provide input file\n");
		return -1;
	}

	const char *file_name = argv[1];

	AC_Result result;

	result = AC_sourceToTokenStream(file_name); 
	//AC_printResult(result);

	//AC_initSymbolTable();
	//AC_printSymbolTable();	

	return 0;
}
