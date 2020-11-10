/**
 * @file expression.c
 * @brief Expressions processing implementation
 * @author Vít Tlustoš - xtlust05@stud.fit.vutbr.cz
 *
 * Processing of expressions by precedent analysis
 */

#include "expression.h"
#include "scanner.h"
#include "expression_stack.h"
#include "symtable.h"

// Global variables
expression_stack stack;

void convert_token_to_expression_symbol(token token, 
    expression_symbol *out_expression_symbol)
{
    switch(token->type){
        // Variable
        case ID_TOKEN:
            *out_expression_symbol = id;
            break;
        // Literals
        case STRING_LITERAL_TOKEN:
            *out_expression_symbol = string_lit;
            break;
        case DECIMAL_LITERAL_TOKEN:
            *out_expression_symbol = float64_lit;
            break;
        case INTEGER_LITERAL_TOKEN:
            *out_expression_symbol = int_lit;
            break;
        // Brackets
        case LEFT_BRACKET_TOKEN:
            *out_expression_symbol = lbrack;
            break;
        case RIGHT_BRACKET_TOKEN:
            *out_expression_symbol = rbrack;
            break;
        // Arithmetic operators
        case PLUS_TOKEN:
            *out_expression_symbol = plus;
            break;
        case MINUS_TOKEN:
            *out_expression_symbol = minus;
            break;
        case MULTIPLICATION_TOKEN:
            *out_expression_symbol = mul;
            break;
        case DIVISON_TOKEN:
            *out_expression_symbol = div;
            break;
        // Relational operators
        case EQUALS_TOKEN:
            *out_expression_symbol = eq;
            break;
        case NOT_EQUALS_TOKEN:
            *out_expression_symbol = neq;
            break;
        case LESS_TOKEN:
            *out_expression_symbol = less;
            break;
        case LESS_EQUAL_TOKEN:
            *out_expression_symbol = less_eq;
            break;
        case GREATER_TOKEN:
            *out_expression_symbol = greater;
            break;
        case GREATER_EQUAL_TOKEN:
            *out_expression_symbol = greater_eq;
            break;
        // Unexpected token
        default:
            *out_expression_symbol = invalid;
    }
}

void convert_expression_symbol_to_terminal_group(expression_symbol expression_symbol,
    terminal_group *out_terminal_group){
    switch(expression_symbol){
        // Variable and literals
        case id:
        case string_lit:
        case float64_lit:
        case int_lit:
            *out_terminal_group = id_lit;
            break;
        // Brackets
        case lbrack:
            *out_terminal_group = lbrack;
            break;
        case rbrack:
            *out_terminal_group = rbrack;
            break;
        // Arithmetic operators
        case plus:
        case minus:
            *out_terminal_group = plus_minus;
            break;
        case mul:
        case div:
            *out_terminal_group = mul_div;
            break;
        // Relational operators
        case eq:
        case neq:
        case less:
        case less_eq:
        case greater:
        case greater_eq:
            *out_terminal_group = rel_op;
            break;
        // Unexpected token
        default:
            *out_terminal_group = invalid;
    }
}

int parse_expression(table sym_table, token *token_arr, int token_count)
{    
    // Initializes the stack and pushes stop element ($) to the stack
    stack_init(&stack);
    stack_push(&stack, dollar, NULL);

    for(int i = 0; i < token_count; i++){
        // Converts token to symbol
        expression_symbol input_symbol;
        convert_token_to_expression_symbol(token_arr + i, &input_symbol);

        // Gets top terminal from the stack
        expression_stack_node *top_stack_terminal;
        stack_top_terminal(&stack, top_stack_terminal);

        // Converts symbols to terminal groups
        terminal_group input_terminal_group;
        terminal_group top_stack_terminal_group;
        convert_expression_symbol_to_terminal_group(input_symbol, &input_terminal_group);
        convert_expression_symbol_to_terminal_group(top_stack_terminal->symbol, &top_stack_terminal_group);

        // Gets the precedence from the table and perform suitable operation
        precedence precedence_evaluation = (precedence)precedence_table[top_stack_terminal_group][input_terminal_group];
        switch(precedence_evaluation){
            case R:
                // REDUCTION - >
                // Gets the rule to use and its elements 
                reduce_rule reduce_rule;
                expression_stack_node *reduce_element_0;
                expression_stack_node *reduce_element_1;
                expression_stack_node *reduce_element_2;
                stack_reduce_rule(&stack, &reduce_rule, reduce_element_0, 
                    reduce_element_1, reduce_element_2);
                
                // Does reduction and code generation
                if(reduce_rule != unknown){
                    // TO DO: SEMANTICS CONTROL AND EXPLICIT CAST
                    // TO DO: GENERATE CODE

                    // Updates the stack 
                    // Pop - if rule had 3 elements (e.g. E -> E ... E) pops 4x (<E+E) else E -> i pops 2x (<i)
                    stack_pop(&stack, reduce_element_2 != NULL ? 4 : 2);
                    // Pushes non-terminal to symtable and pushes reduced non-terminal to the stack (E -> ...)
                    // stack_push(&stack, nt, );
                }
                else{
                    // TO DO: ERROR!!!!
                }
                break;
        }
    }

}