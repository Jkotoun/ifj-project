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
        switch(precedence_evaluation)
        {
            case Eq:
                // ---------------------------------- EQUAL - = --------------------------------- //
                // May happen only if top_stack_terminal_group = '(' and input_terminal_group = ')'
                // NO_CODE_GENERATION and NO_SEMANTIC_CHECK needed = (nt)
                stack_push(&stack, input_symbol, UNDEFINED);
                break;
                
            case S:
                // ---------------------------------- SHIFT - < --------------------------------- //
                // PUSHES the reduce_br
                stack_push_after_top_terminal(&stack, reduce_br, UNDEFINED);

                // PUSHES the input_symbol
                switch(input_symbol){
                    case id:
                        // variable node
                        node* sym_node = NULL;
                        if(search(sym_table->root_ptr, input_token->str, sym_node)){
                            stack_push(&stack, input_symbol, ((symbol_variable)sym_node->data)->var_type);
                            // TO DO: GENERATE_CODE(PUSHS(sym_node)) - pushes to the stack variable defined in sym_node
                        }
                        else{
                            // TO DO: SEMANTIC_ERROR - UNDEFINED_VAR
                        }
                        break;
                    case int_lit:
                        stack_push(&stack, input_symbol, INT);
                        // TO DO: GENERATE_CODE(PUSHS(input_token->integer)) - pushes to the stack
                        break;
                    case float64_lit:
                        stack_push(&stack, input_symbol, FLOAT);
                        // TO DO: GENERATE_CODE(PUSHS(input_token->decimal)) - pushes to the stack
                        break;
                    case string_lit:
                        stack_push(&stack, input_symbol, STRING);
                        // TO DO: GENERATE_CODE(PUSHS(input_token->str)) - pushes to the stack
                        break;
                    default:
                        // other possible input (e.g. +, -, ...)
                        stack_push(&stack, input_symbol, UNDEFINED);
                        break;
                }
                break;               

            case R:
                // ---------------------------------- REDUCE - > --------------------------------- //

                // RULE IDENTIFICATION AND REDUCTION ELEMENTS
                reduce_rule reduce_rule;
                expression_stack_node *reduce_element_0;
                expression_stack_node *reduce_element_1;
                expression_stack_node *reduce_element_2;
                stack_reduce_rule(&stack, &reduce_rule, reduce_element_0, 
                    reduce_element_1, reduce_element_2);
                
                // CODE GENERATION AND SEMANTIC CHECK
                varType reduced_type = UNDEFINED;
                switch(reduce_rule){
                    case operand:
                        // NO_CODE_GENERATION - value has already been pushed to the stack
                        if(reduce_element_0->type == UNDEFINED){
                            // TO DO: SEMANTIC_ERROR - UNDEFINED_VAR
                        }
                        reduced_type = reduce_element_0->type;
                        break;
                    case lbrack_nt_rbrack:
                        // NO_CODE_GENERATION - value has already been pushed to the stack
                        if(reduce_element_1->type == UNDEFINED){
                            // TO DO: SEMANTIC_ERROR - UNDEFINED_VAR
                        }
                        reduced_type = reduce_element_1->type;
                        break;
                    case nt_plus_nt:
                        if(reduce_element_0->type == STRING && reduce_element_2->type == STRING){
                            // TO DO: GENERATE_CODE(CONCAT) and GENERATE_CODE(PUSHS(result of concat)) 
                            // concates strings (symbols on stack) and pushes output to the stack
                            reduced_type = STRING;
                        }
                        else if(reduce_element_0->type == reduce_element_2->type
                            && reduce_element_0->type != BOOL
                            && reduce_element_0->type != UNDEFINED){
                            // TO DO: GENERATE_CODE(reduce_rule) - performs ADDS on the stack
                            reduced_type = reduce_element_0->type;
                        }
                        else{
                            // TO DO: SEMANTIC_ERROR - INCORRECT_TYPE_COMBINATION
                        }
                        break;
                    case nt_minus_nt:
                    case nt_mul_nt:
                        if(reduce_element_0->type == reduce_element_2->type
                            && reduce_element_0->type != BOOL 
                            && reduce_element_0->type != STRING
                            && reduce_element_0->type != UNDEFINED){
                            // TO DO: GENERATE_CODE(reduce_rule) - performs SUBS/MULS on the stack
                            reduced_type = reduce_element_0->type;
                        }
                        else{
                            // TO DO: SEMANTIC_ERROR - INCORRECT_TYPE_COMBINATION
                        }
                        break;
                    case nt_div_nt:
                        if(reduce_element_0->type == reduce_element_2->type
                            && reduce_element_0->type == INT){
                            // TO DO: GENERATE_CODE(reduce_rule) - performs IDIVS - integer division on the stack
                            reduced_type = INT;
                        }
                        else if(reduce_element_0->type == reduce_element_2->type
                            && reduce_element_0->type == FLOAT){
                            // TO DO: GENERATE_CODE(reduce_rule) - performs DIVS - floating point division on the stack
                            reduced_type = FLOAT;
                        }
                        else{
                            // TO DO: SEMANTIC_ERROR - INCORRECT_TYPE_COMBINATION
                        }
                        break;
                    case nt_eq_nt:
                    case nt_neq_nt:
                    case nt_less_nt:
                    case nt_less_eq_nt:
                    case nt_more_nt:
                    case nt_more_eq_nt:
                        if(reduce_element_0->type == reduce_element_2->type
                            && reduce_element_0->type != BOOL
                            && reduce_element_0->type != UNDEFINED){
                            // TO DO: GENERATE_CODE(reduce_rule) - performs comparision on the stack
                            // uses LTS/GTS/EQS determined by the reduce_rule
                            // uses NOTS if negation needed nt_neq_nt (!=) -> EQS + NOTS 
                            reduced_type = BOOL;
                        }
                        else{
                            // TO DO: SEMANTIC_ERROR - INCORRECT_TYPE_COMBINATION
                        }
                        break;
                    default:
                        // TO DO: SEMANTIC_ERROR - UNKNOWN_REDUCTION_RULE
                        break;
                }             

                // STACK UPDATE
                // Pop - if rule had 3 elements (e.g. E -> E ... E) pops 4x (<E+E) else E -> i pops 2x (<i)
                stack_pop(&stack, reduce_element_2 != NULL ? 4 : 2);
                // Pushes reduced non-terminal to the stack
                stack_push(&stack, nt, reduced_type);
                break;

            case Er:
                // ---------------------------------- REDUCE - {}  --------------------------------- //
                // May happen only if the input is invalid or when the end of the expression is reached
                
                if (input_symbol == dollar && top_stack_terminal->symbol == dollar){
                    // Expression has been successfuly parsed
                    // TO DO: OUT - PARSING_AND_CODE_GENERATION_OK
                }
                else{
                    // TO DO: SYNTAX_ERROR
                }
                break;
        }
    }
}