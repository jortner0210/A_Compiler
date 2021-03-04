#include "Common.h"
#include "Core.h"

void AC_printResult(
	AC_Result result
)
{
	switch (result)
	{
		case AC_SUCCESS:
			printf("ID: %d, Type: Success.\n", AC_SUCCESS);
			break;		
		
		case AC_IO_ERROR:
			printf("ID: %d, Type: I/O Error!\n", AC_IO_ERROR);
			break;
	}
}
