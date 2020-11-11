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

        // Converts input token to the symbol
        token *input_token = (token_arr + i);
        expression_symbol input_symbol;
        convert_token_to_expression_symbol(input_token, &input_symbol);

        // Gets top terminal from the stack
        expression_stack_node *top_stack_terminal;
        stack_top_terminal(&stack, top_stack_terminal);

        // Converts symbols to the terminal groups
        terminal_group input_terminal_group;
        terminal_group top_stack_terminal_group;
        convert_expression_symbol_to_terminal_group(input_symbol, &input_terminal_group);
        convert_expression_symbol_to_terminal_group(top_stack_terminal->symbol, &top_stack_terminal_group);

        // Gets the precedence from the table and perform suitable operation
        precedence precedence_evaluation = (precedence)precedence_table[top_stack_terminal_group][input_terminal_group];
        switch(precedence_evaluation){
            case Eq:
                // ---------------------------------- EQUAL - = --------------------------------- //
                stack_push(&stack, input_symbol, UNDEFINED);
                
            case S:
                // ---------------------------------- SHIFT - < --------------------------------- //
                // PUSHES the reduce_br
                stack_push_after_top_terminal(&stack, reduce_br, UNDEFINED);

                // PUSHES the input_symbol
                if(input_symbol == id){
                    // variable node
                    node* sym_node = NULL;
                    if(search(sym_table->root_ptr, input_token->str, sym_node)){
                        stack_push(&stack, input_symbol, ((symbol_variable)sym_node->data)->var_type);
                        // TO DO: GENERATE_CODE(PUSHS(sym_node)) - pushes to the stack variable defined in sym_node
                    }
                    else{
                        // TO DO: SEMANTIC_ERROR - UNDEFINED_VAR
                    }
                }
                else if(input_symbol == int_lit){
                    // literal node
                    stack_push(&stack, input_symbol, INT);
                    // TO DO: GENERATE_CODE(PUSHS(input_token->integer)) - pushes int literal to the stack
                }
                else if(input_symbol == string_lit){
                     // literal node
                    stack_push(&stack, input_symbol, STRING);
                    // TO DO: GENERATE_CODE(PUSHS(input_token->integer)) - pushes string literal to the stack
                }
                else if(input_symbol == float64_lit){
                     // literal node
                    stack_push(&stack, input_symbol, FLOAT);
                    // TO DO: GENERATE_CODE(PUSHS(input_token->integer)) - pushes float literal to the stack
                }
                else{
                    // other possible input (e.g. +, -, ...)
                    stack_push(&stack, input_symbol, UNDEFINED);
                }

            case R:
                // ---------------------------------- REDUCE - > --------------------------------- //

                // Gets the rule to use and its elements 
                reduce_rule reduce_rule;
                expression_stack_node *reduce_element_0;
                expression_stack_node *reduce_element_1;
                expression_stack_node *reduce_element_2;
                stack_reduce_rule(&stack, &reduce_rule, reduce_element_0, 
                    reduce_element_1, reduce_element_2);
                
                // SEMANTIC CHECK AND CODE_GENERATION
                if(reduce_rule == operand){
                    // NO_CODE_GENERATION - value has already been pushed to the stack
                    if(reduce_element_0->type == UNDEFINED){
                        // TO DO: SEMANTIC_ERROR - UNDEFINED_VAR
                    }
                }
                else if(reduce_rule == lbrack_nt_rbrack){
                    // NO_CODE_GENERATION - value has already been pushed to the stack
                    if(reduce_element_1->type == UNDEFINED){
                        // TO DO: SEMANTIC_ERROR - UNDEFINED_VAR
                    }
                }

                // TO DO: SEMANTICS CONTROL AND EXPLICIT CAST
                // TO DO: GENERATE CODE
                else if(reduce_rule == nt_plus_nt || reduce_rule == nt_minus_nt
                    || reduce_rule == nt_mul_nt || reduce_rule == nt_div_nt){
                        if(reduce_element_0->type == UNDEFINED
                            || reduce_element_0->type == UNDEFINED){
                            // TO DO: SEMANTIC_ERROR - UNDEFINED_VAR
                        }
                    }
                    else{
                        // If reduce_rule == operand -> NO_CODE_GENERATION
                        // NO_CODE_GENERATION - value has already been pushed during the stack shift
                        if(reduce_element_0->type == UNDEFINED){
                            // TO DO: SEMANTIC_ERROR - UNDEFINED_VAR
                        }
                    }

                    // STACK UPDATE
                    // Pop - if rule had 3 elements (e.g. E -> E ... E) pops 4x (<E+E) else E -> i pops 2x (<i)
                    stack_pop(&stack, reduce_element_2 != NULL ? 4 : 2);
                    // Pushes reduced non-terminal to the stack
                    stack_push(&stack, nt, UNDEFINED); // !!!!!!!!!! not accurete !!!!!!!!!!!!!!!!!
                }
                else{
                    // TO DO: SEMANTIC_ERROR - UNKNOWN_REDUCTION_RULE
                }
                break;
        }
    }

}