#include "Parser.h"

//
//  Program -> { Statement_List } eof
//
AC_Result AC_isProgram(
	AC_TokenStream *token_stream
)
{
    AC_Token *curr_token;
    AC_Result res;
	
	printf("end reached: %d\n", token_stream->end_reached);

	// CHECK FOR EMPTY
	if (token_stream->end_reached)
		return AC_SUCCESS;

    res = AC_nextTokenTokenStream(token_stream, &curr_token);

    // CHECK FOR LEFT BRACKET 
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_FMT(AC_FINE, "Checking token: %s", curr_token->lexeme)
        if (curr_token->tok_type != AC_L_BRACKET) {
            AC_DEBUG_TRACE_ARG(AC_NOT_GOOD, "Syntax Error! Expected a '{'")
            return AC_SYNTAX_ERROR;
        }
    }
    else {
        AC_EXIT_FAILURE("Error reading from token stream!")
    }
        
	// CHECK FOR STATEMENT LIST
	res = AC_isStatementList(token_stream);
	if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_ARG(AC_FINE, "Valid StatementList found.")
	}

	// CHECK FOR RIGHT BRACKET        
    res = AC_nextTokenTokenStream(token_stream, &curr_token);
	if (res == AC_SUCCESS) {
		AC_DEBUG_TRACE_FMT(AC_FINE, "Checking token: %s", curr_token->lexeme)
        if (curr_token->tok_type != AC_R_BRACKET) {
            AC_DEBUG_TRACE_ARG(AC_NOT_GOOD, "Syntax Error! Expected a '}'")
            return AC_SYNTAX_ERROR;
        }
    }
    else {
        AC_EXIT_FAILURE("Error reading from token stream!")
    }

    return AC_SUCCESS;
}

AC_Result AC_isStatementList(
	AC_TokenStream *token_stream
)
{
	// CHECK FOR EMPTY
	if (token_stream->end_reached)
		return AC_SUCCESS;

	// CHECK FOR STATEMENT
	AC_Result res;
	res = AC_isStatement(token_stream);
	printf("statement check\n");
	if (res != AC_SUCCESS) {
		// Valid statment not found
		AC_DEBUG_TRACE_ARG(AC_NOT_GOOD, "Syntax Error! Expected valid Statement");
		return AC_SYNTAX_ERROR;
	}

	// CHECK FOR STATEMENT LIST
	res = AC_isStatementList(token_stream);
	if (res != AC_SUCCESS) {
		// Valid statment not found
		AC_DEBUG_TRACE_ARG(AC_NOT_GOOD, "Syntax Error! Expected valid StatementList");
		return AC_SYNTAX_ERROR;
	}

	return AC_SUCCESS;
}

AC_Result AC_isStatement(
	AC_TokenStream *token_stream
)
{
	token_stream->end_reached = 1;
	return AC_SUCCESS;
}

AC_Result AC_parseTokenStream(
	AC_TokenStream *token_stream
)
{
    //AC_printTokenStream(token_stream);

    if (AC_isProgram(token_stream) == AC_SUCCESS) {
        return AC_SUCCESS;
    }
    else {
        AC_EXIT_FAILURE("Program was not recognized by parser!")
    }

    AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "PARSING COMPLETED SUCCESSFULLY!")
    return AC_SUCCESS;
}
