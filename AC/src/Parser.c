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

    // OPTION 1:
	//  - CHECK FOR EMPTY
	if (token_stream->end_reached)
		AC_PARSE_ERROR_MSG("Expected a Valid Program", token_stream->curr_line_num, token_stream->curr_char_num)

    res = AC_nextTokenTokenStream(token_stream, &curr_token);

    // OPTION 2:
    //  - CHECK FOR LEFT BRACKET 
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_FMT(AC_FINE, "Checking token: %s", curr_token->lexeme)
        if (curr_token->tok_type != AC_L_BRACKET) {
            AC_PARSE_ERROR_MSG("Expected a '{'", token_stream->curr_line_num, token_stream->curr_char_num)
            return AC_SYNTAX_ERROR;
        }
    }
    else {
        AC_EXIT_FAILURE("Error reading from token stream!")
    }
        
	//  - CHECK FOR EXPRESSION
    res = AC_isExpression(token_stream);
    if (res != AC_SUCCESS) {
        AC_PARSE_ERROR_MSG("Expected an Expression", curr_token->ln_num, curr_token->char_num)
    }

	//  - CHECK FOR RIGHT BRACKET
    res = AC_nextTokenTokenStream(token_stream, &curr_token);        
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_FMT(AC_FINE, "Checking token: %s", curr_token->lexeme)
        if (curr_token->tok_type != AC_R_BRACKET) {
            AC_PARSE_ERROR_MSG("Expected a '}'", token_stream->curr_line_num, token_stream->curr_char_num)
            return AC_SYNTAX_ERROR;
        }
    }
    else {
        AC_EXIT_FAILURE("Error reading from token stream!")
    }
    return AC_SUCCESS;
}

//AC_readAndCheckTokenStream(token_stream, tok_check_type, error_message)
/*
AC_Token *curr_token;
AC_Result res;
res = AC_nextTokenTokenStream(token_stream, &curr_token);      
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_FMT(AC_FINE, "Checking token: %s", curr_token->lexeme)
        if (curr_token->tok_type != AC_OPERATOR) {
            AC_PARSE_ERROR_MSG("Expected a Operator", token_stream->curr_line_num, token_stream->curr_char_num)
            return AC_SYNTAX_ERROR;
        }
    }
    else {
        AC_EXIT_FAILURE("Error reading from token stream!")
    }
*/

AC_Result AC_isExpression(
	AC_TokenStream *token_stream
)
{
    AC_Token *curr_token;
    AC_Result res;
    // OPTION 1:
    //  - CHECK FOR TERM
    res = AC_isTerm(token_stream);
    if (res != AC_SUCCESS) {
        AC_PARSE_ERROR_MSG("Expected a Term", token_stream->curr_line_num, token_stream->curr_char_num)
    }

    //  - CHECK FOR OP
    res = AC_nextTokenTokenStream(token_stream, &curr_token);      
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_FMT(AC_FINE, "Checking token: %s", curr_token->lexeme)
        if (curr_token->tok_type != AC_OPERATOR) {
            AC_PARSE_ERROR_MSG("Expected a Operator", token_stream->curr_line_num, token_stream->curr_char_num)
            return AC_SYNTAX_ERROR;
        }
    }
    else {
        AC_PARSE_ERROR_MSG("Expected a Valid Program", token_stream->curr_line_num, token_stream->curr_char_num)
    }

    //  - CHECK FOR TERM
    res = AC_isTerm(token_stream);
    if (res != AC_SUCCESS) {
        AC_PARSE_ERROR_MSG("Expected a Term", token_stream->curr_line_num, token_stream->curr_char_num)
    }

    // - CHECK FOR SEMI COLON
    res = AC_nextTokenTokenStream(token_stream, &curr_token);      
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_FMT(AC_FINE, "Checking token: %s", curr_token->lexeme)
        if (curr_token->tok_type != AC_SEMI_COLON) {
            AC_PARSE_ERROR_MSG("Expected a ';'", token_stream->curr_line_num, token_stream->curr_char_num)
            return AC_SYNTAX_ERROR;
        }
    }
    else {
        AC_PARSE_ERROR_MSG("Expected a Valid Program", token_stream->curr_line_num, token_stream->curr_char_num)
    }

    return AC_SUCCESS;
}

AC_Result AC_isTerm(
	AC_TokenStream *token_stream
)
{
    AC_Token *curr_token;
    AC_Result res;
    // OPTION 1:
    //  - CHECK FOR FACTOR
    res = AC_nextTokenTokenStream(token_stream, &curr_token);  
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_FMT(AC_FINE, "Checking token: %s", curr_token->lexeme)
        if (curr_token->tok_type == AC_FACTOR)
            return AC_SUCCESS;
    }
    else {
        AC_PARSE_ERROR_MSG("Expected a Expression", token_stream->curr_line_num, token_stream->curr_char_num)
    }
    //AC_backTrackTokenStream(token_stream);

    // OPTION 2:
    //  - CHECK FOR TERM
    res = AC_isTerm(token_stream);
    if (res != AC_SUCCESS)
        AC_PARSE_ERROR_MSG("Expected a Term", token_stream->curr_line_num, token_stream->curr_char_num)

    //  - CHECK FOR OPERATOR
    res = AC_nextTokenTokenStream(token_stream, &curr_token);  
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_FMT(AC_FINE, "Checking token: %s", curr_token->lexeme)
        if (curr_token->tok_type == AC_OPERATOR)
            return AC_SUCCESS;
        else
            AC_PARSE_ERROR_MSG("Expected an Operator", token_stream->curr_line_num, token_stream->curr_char_num)
    }
    else {
        AC_PARSE_ERROR_MSG("Expected a Expression", token_stream->curr_line_num, token_stream->curr_char_num)
    }

    //  - CHECK FOR FACTOR
    res = AC_nextTokenTokenStream(token_stream, &curr_token);  
    if (res == AC_SUCCESS) {
        AC_DEBUG_TRACE_FMT(AC_FINE, "Checking token: %s", curr_token->lexeme)
        if (curr_token->tok_type == AC_FACTOR)
            return AC_SUCCESS;
        else
            AC_PARSE_ERROR_MSG("Expected a Factor", token_stream->curr_line_num, token_stream->curr_char_num)
    }
    else {
        AC_PARSE_ERROR_MSG("Expected a Expression", token_stream->curr_line_num, token_stream->curr_char_num)
    }

    return AC_SUCCESS;
}


AC_Result AC_parseTokenStream(
	AC_TokenStream *token_stream
)
{
    //AC_printTokenStream(token_stream);

    if (AC_isProgram(token_stream) == AC_SUCCESS) {
        AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "PARSING COMPLETED SUCCESSFULLY!")
        return AC_SUCCESS;
    }
    else {
        AC_PARSE_ERROR_MSG("Expected a Program!", 0, 0)
    }

    return AC_SUCCESS;
}
