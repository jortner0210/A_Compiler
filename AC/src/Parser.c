#include "Parser.h"

AC_Result AC_isFactor(
    AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isFactor")
    
    AC_Token *curr_token;

    // Error States: eof, plus, multiply
    if (AC_nextTokenTokenStream(token_stream, &curr_token) == AC_SUCCESS) {
        if (curr_token->tok_type != AC_NUMBER) {   
            AC_EXIT_FAILURE("Unexpected end a number!")    
        }
        else {
            return AC_SUCCESS;
        }
    }
    else {
        AC_EXIT_FAILURE("Unexpected end of token stream!")
    }
}

AC_Result AC_isTermPrime(
    AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isTermPrime")
    
    AC_Token *curr_token;

    // Error States: number
    if (AC_nextTokenTokenStream(token_stream, &curr_token) == AC_SUCCESS) {
        if (curr_token->tok_type == AC_NUMBER) {   
            AC_EXIT_FAILURE("Expected a valid Term Prime!")    
        }
        else if (curr_token->tok_type == AC_ADD_SUBTRACT
                 || curr_token->tok_type == AC_TERMINAL) {   
            AC_backTrackTokenStream(token_stream);
            return AC_SUCCESS;
        }
        else if (curr_token->tok_type == AC_MULTIPLY_DIVIDE) {
            // CHECK FOR FACTOR
            if (AC_isFactor(token_stream) != AC_SUCCESS)
                AC_EXIT_FAILURE("Expected a Factor!")

            // CHECK FOR TERM PRIM
            if (AC_isTermPrime(token_stream) != AC_SUCCESS)
                AC_EXIT_FAILURE("Expected a Term Prime!")
        }
    }
    else {
        AC_EXIT_FAILURE("Unexpected end of token stream!")
    }

}

AC_Result AC_isTerm(
    AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isTerm")
    
    AC_Token *curr_token;

    // Error States: eof, plus, multiply
    if (AC_nextTokenTokenStream(token_stream, &curr_token) == AC_SUCCESS) {
        if (curr_token->tok_type == AC_ADD_SUBTRACT 
            || curr_token->tok_type == AC_MULTIPLY_DIVIDE
            || curr_token->tok_type == AC_TERMINAL) 
        {
            AC_EXIT_FAILURE("Expected a Term!")
        }
        else {
            AC_backTrackTokenStream(token_stream);
        }
    }
    else {
        AC_EXIT_FAILURE("Unexpected end of token stream!")
    }

    // CHECK FOR FACTOR
    if (AC_isFactor(token_stream) != AC_SUCCESS)
        AC_EXIT_FAILURE("Expected a Factor!")

    // CHECK FOR TERM PRIM
    if (AC_isTermPrime(token_stream) != AC_SUCCESS)
        AC_EXIT_FAILURE("Expected a Term Prime!")
}

AC_Result AC_isExpressionPrime(
    AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isExpressionPrime")

    AC_Token *curr_token;

    // Error States: mulitply, number
    if (AC_nextTokenTokenStream(token_stream, &curr_token) == AC_SUCCESS) {
        if (curr_token->tok_type == AC_MULTIPLY_DIVIDE
            || curr_token->tok_type == AC_NUMBER) {
            AC_EXIT_FAILURE("Expected an Expression Prime!")
        }
        else if (curr_token->tok_type == AC_TERMINAL) {
            AC_backTrackTokenStream(token_stream);
            return AC_SUCCESS;
        }
        else if (curr_token->tok_type == AC_ADD_SUBTRACT) {
            // CHECK FOR TERM
            if (AC_isTerm(token_stream) != AC_SUCCESS)
                AC_EXIT_FAILURE("Expected a Term!")

            // CHECK FOR EXPRESSION PRIME
            if (AC_isExpressionPrime(token_stream) != AC_SUCCESS)
                AC_EXIT_FAILURE("Expected an Expression Prime!")
            return AC_SUCCESS;
        }
        else {
            AC_backTrackTokenStream(token_stream);
        }
    }
    else {
        AC_EXIT_FAILURE("Unexpected end of token stream!")
    }
}

//
// Expression -> Term Expression'
//
AC_Result AC_isExpression(
    AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isExpression")

    AC_Token *curr_token;

    // Error States: eof, plus, multiply
    if (AC_nextTokenTokenStream(token_stream, &curr_token) == AC_SUCCESS) {
        if (curr_token->tok_type == AC_ADD_SUBTRACT 
            || curr_token->tok_type == AC_MULTIPLY_DIVIDE
            || curr_token->tok_type == AC_TERMINAL) 
        {
            AC_EXIT_FAILURE("Expected a Term!")
        }
        else { 
            AC_backTrackTokenStream(token_stream);
        }
    }
    else {
        AC_EXIT_FAILURE("Unexpected end of token stream!")
    }

    // CHECK FOR TERM
    if (AC_isTerm(token_stream) != AC_SUCCESS)
        AC_EXIT_FAILURE("Invalid Term!")

    // CHECK FOR EXPRESSION PRIME
    if (AC_isExpressionPrime(token_stream) != AC_SUCCESS)
        AC_EXIT_FAILURE("Invalid Term!")

    return AC_SUCCESS;
}

//
// Current Supported Grammer: Aim: LL(1) Grammar
//
// Program     -> Expression
//
// Expression  -> Term Expression' 
//
// Expression' -> empty
// Expression' -> + Term Expression'
//
// Term        -> Factor Term'
//
// Term'       -> empty
// Term'       -> * Factor Term'
//
// Factor 	   -> number
//
AC_Result AC_isProgram(
	AC_TokenStream *token_stream
)
{
    AC_DEBUG_TRACE_ARG(AC_FINE, "AC_isProgram")

    if (AC_isExpression(token_stream) == AC_SUCCESS) {
        AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Looking for and found a valid Expression!")
        return AC_SUCCESS;
    }
    else 
        AC_EXIT_FAILURE("Expected a Valid Statement")
}


AC_Result AC_parseTokenStream(
	AC_TokenStream *token_stream
)
{
    AC_printTokenStream(token_stream);

    if (AC_isProgram(token_stream) == AC_SUCCESS) {
        AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "PARSING COMPLETED SUCCESSFULLY!")
        return AC_SUCCESS;
    }
    else 
        AC_PARSE_ERROR_MSG("Expected a Program!", 0, 0)
}
