#include "Parser.h"

//
// Generate an Abstract Syntax Tree given a token stream
//
AC_Result AC_parseTokenStream(
	AC_TokenStream *token_stream
)
{
    AC_PARSE_FUNCTION_BEGIN

    AC_resetTokenStream(token_stream);
    //AC_printTokenStream(token_stream);

    if (AC_isProgram(token_stream) == AC_SUCCESS) {
        AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "PARSING COMPLETED SUCCESSFULLY!")
        return AC_SUCCESS;
    }
    else 
        AC_EXIT_FAILURE("Invalid Program!")
}

//
// Validates grammar rule: Program -> Expression
//
AC_Result AC_isProgram(
	AC_TokenStream *token_stream
)
{
    AC_PARSE_FUNCTION_BEGIN

    if (AC_isExpression(token_stream) == AC_SUCCESS) {
        AC_DEBUG_TRACE_ARG(AC_ALL_GOOD, "Found a valid Expression!")
        return AC_SUCCESS;
    }
    else 
        AC_EXIT_FAILURE("Expected a Valid Expression")
}

//
// Validates grammar rule: Expression -> Term Expression'
//
AC_Result AC_isExpression(
    AC_TokenStream *token_stream
)
{
    AC_PARSE_FUNCTION_BEGIN

    // Error: Op(+|-), Op(*|/), Terminal
    if (token_stream->curr_token->tok_type == AC_ADD_SUBTRACT
        || token_stream->curr_token->tok_type == AC_MULTIPLY_DIVIDE
        || token_stream->curr_token->tok_type == AC_TERMINAL)
    {
        AC_EXIT_FAILURE("Expected a Term!")
    }
    // Check for id
    else if (token_stream->curr_token->tok_type == AC_NUMBER) {

        // CHECK FOR TERM
        if (AC_isTerm(token_stream) != AC_SUCCESS)
            AC_EXIT_FAILURE("Invalid Term!")

        // CHECK FOR EXPRESSION PRIME
        if (AC_isExpressionPrime(token_stream) != AC_SUCCESS)
            AC_EXIT_FAILURE("Invalid Term!")

        return AC_SUCCESS;
    }
}

//
// Validate grammar rules:
//  - Expression' -> epsilon
//  - Expression' -> + Term Expression'
//
AC_Result AC_isExpressionPrime(
    AC_TokenStream *token_stream
)
{
    AC_PARSE_FUNCTION_BEGIN

    // error: number, Op(*|/)
    if (token_stream->curr_token->tok_type == AC_NUMBER
        || token_stream->curr_token->tok_type == AC_MULTIPLY_DIVIDE)
    {
        AC_EXIT_FAILURE("Expected an Operator or epsilon!")
    }
    // success: epsilon
    else if (token_stream->curr_token->tok_type == AC_TERMINAL) {
        return AC_SUCCESS;
    }
    // success: Op(+|-) -> Term, Expression'
    else {
        AC_bumpTokenStream(token_stream);

        // CHECK FOR TERM
        if (AC_isTerm(token_stream) != AC_SUCCESS)
            AC_EXIT_FAILURE("Expected a Term!")

        // CHECK FOR EXPRESSION PRIME
        if (AC_isExpressionPrime(token_stream) != AC_SUCCESS)
            AC_EXIT_FAILURE("Expected an Expression Prime!")
        
        return AC_SUCCESS;
    }
}

//
// Validate grammar rule: Term -> Factor Term'
//
AC_Result AC_isTerm(
    AC_TokenStream *token_stream
)
{
    AC_PARSE_FUNCTION_BEGIN

    // error: Op(+|-), Op(*|/), epsilon
    if (token_stream->curr_token->tok_type == AC_ADD_SUBTRACT 
        || token_stream->curr_token->tok_type == AC_MULTIPLY_DIVIDE
        || token_stream->curr_token->tok_type == AC_TERMINAL) 
    {
        AC_EXIT_FAILURE("Expected an Operator or epsilon!")
    }
    else {
        // CHECK FOR FACTOR
        if (AC_isFactor(token_stream) != AC_SUCCESS)
            AC_EXIT_FAILURE("Expected a Factor!")

        // CHECK FOR TERM PRIM
        if (AC_isTermPrime(token_stream) != AC_SUCCESS)
            AC_EXIT_FAILURE("Expected a Term Prime!")

        return AC_SUCCESS;
    }
}

//
// Validate grammar rules:
//  - Term' -> epsilon
//  - Term' -> * Factor Term'
//
AC_Result AC_isTermPrime(
    AC_TokenStream *token_stream
)
{
    AC_PARSE_FUNCTION_BEGIN

    // error: number
    if (token_stream->curr_token->tok_type == AC_NUMBER) {
        AC_EXIT_FAILURE("Expected an Operator or Epsilon!")
    }
    // success: Op(+|-), epsilon
    else if (token_stream->curr_token->tok_type == AC_ADD_SUBTRACT
             || token_stream->curr_token->tok_type == AC_TERMINAL)
    {
        return AC_SUCCESS;
    }
    // success: Op(*|/) -> Factor Term'
    else {
        AC_bumpTokenStream(token_stream);

        // CHECK FOR FACTOR
        if (AC_isFactor(token_stream) != AC_SUCCESS)
            AC_EXIT_FAILURE("Expected a Factor!")

        // CHECK FOR TERM PRIM
        if (AC_isTermPrime(token_stream) != AC_SUCCESS)
            AC_EXIT_FAILURE("Expected a Term Prime!")
    }
}

//
// Validates grammar rule: Factor -> number
//
AC_Result AC_isFactor(
    AC_TokenStream *token_stream
)
{
    AC_PARSE_FUNCTION_BEGIN

    if (token_stream->curr_token->tok_type == AC_NUMBER) {
        AC_bumpTokenStream(token_stream);
        return AC_SUCCESS;
    }
    else {
        AC_EXIT_FAILURE("Expected a Factor!")
    }
}