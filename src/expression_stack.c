/**
 * @file expression_stack.c
 * @brief Expressions stack implementation
 * @author Vít Tlustoš - xtlust05@stud.fit.vutbr.cz
 *
 * Represents expression stack for precedence analysis
 */

#include "expression.h"
#include "symtable.h"
#include "expression_stack.h"

void stack_init(expression_stack *stack){
    stack->top = NULL;
}

void stack_push(expression_stack *stack, expression_symbol_type symbol, varType type){
    expression_stack_node* push_node = (expression_stack_node*)malloc(sizeof(expression_stack_node));
    if(push_node != NULL){
        push_node->symbol = symbol;
        push_node->type = type;
        push_node->next = stack->top;
        stack->top = push_node;
    }
}

void stack_push_after_top_terminal(expression_stack *stack, expression_symbol symbol, varType type){
    // Gets the top terminal
    expression_stack_node *top_terminal;
    stack_top_terminal(stack, top_terminal);
    // Pushes new node
    expression_stack_node* push_node = (expression_stack_node*)malloc(sizeof(expression_stack_node));
    if(push_node != NULL){
        push_node->symbol = symbol;
        push_node->type = type;
        push_node->next = top_terminal->next;
        top_terminal->next = push_node;
    }
}

void stack_pop(expression_stack *stack, int amount = 1){
    while(amount != 0){
        if(stack->top != NULL){
            expression_stack_node *pop_node = stack->top;
            stack->top = pop_node->next;
            free(pop_node);
        }
        amount -= 1;
    }
}

void stack_dispose(expression_stack *stack){
    while(stack->top != NULL){
        expression_stack_node* next_node = stack->top->next;
        free(stack->top);
        stack->top = next_node;
    }
}

void stack_top_terminal(expression_stack *stack, expression_stack_node *out_top_terminal){
    expression_stack_node *top_terminal = stack->top;
    while(top_terminal != NULL && (top_terminal->symbol == nt || top_terminal->symbol == reduce_br)){
        top_terminal = top_terminal->next;
    }
    *out_top_terminal = top_terminal;
}

void stack_reduce_rule(expression_stack *stack, reduce_rule *out_reduce_rule, 
    expression_stack_node *out_reduce_element_0, expression_stack_node *out_reduce_element_1,
    expression_stack_node *out_reduce_element_2)
{
    // Gets elements that should be reduced
    *out_reduce_element_0 = stack->top;
    *out_reduce_element_1 = (out_reduce_element_0->next != NULL 
        && out_reduce_element_0->next->symbol != reduce_br) ?  out_reduce_element_0->next : NULL;
    *out_reduce_element_2 = (out_reduce_element_1 != NULL && out_reduce_element_1->next != NULL 
        && out_reduce_element_1->next->symbol != reduce_br) ? out_reduce_element_1->next : NULL;
    
    *out_reduce_rule = unknown;       
    if(out_reduce_element_0 != NULL && out_reduce_element_1 != NULL && out_reduce_element_2 != NULL 
        && out_reduce_element_2->next != NULL && out_reduce_element_2->next->symbol == reduce_br){        
        // Rule with 3 elements
        if(out_reduce_element_0->symbol == nt && out_reduce_element_2->symbol == nt){
            // E -> E operation E
            switch(out_reduce_element_1->symbol){
                case plus:
                    // E -> E + E
                    *out_reduce_rule = nt_plus_nt;
                    break;
                case minus:
                    // E -> E - E
                    *out_reduce_rule = nt_minus_nt;
                    break;
                case mul:
                    // E -> E * E
                    *out_reduce_rule = nt_mul_nt;
                    break;
                case div:
                    // E -> E / E
                    *out_reduce_rule = nt_div_nt;
                    break;
                case eq:
                    // E -> E == E
                    *out_reduce_rule = nt_eq_nt;
                    break;
                case neq:
                    // E -> E != E
                    *out_reduce_rule = nt_neq_nt;
                    break;
                case less:
                    // E -> E < E
                    *out_reduce_rule = nt_less_nt;
                    break;
                case less_eq:
                    // E -> E <= E
                    *out_reduce_rule = nt_less_eq_nt;
                    break;
                case more:
                    // E -> E > E
                    *out_reduce_rule = nt_more_nt;
                    break;
                case more_eq:
                    // E -> E >= E
                    *out_reduce_rule = nt_more_eq_nt;
                    break;              
                default:
                    break;
            }
        }
        else if(out_reduce_element_0->symbol == rbrack && out_reduce_element_1->symbol == nt
            && out_reduce_element_2->symbol == lbrack){
            // E -> (E)
            *out_reduce_rule = lbrack_nt_rbrack;       
        }
    }
    else if(out_reduce_element_0 != NULL && out_reduce_element_0->next != NULL 
        && out_reduce_element_0->next->symbol == reduce_br){
        // Rule with 1 element
        if (out_reduce_element_0->symbol == id 
            || out_reduce_element_0->symbol == int_lit 
            || out_reduce_element_0->symbol == string_lit 
            || out_reduce_element_0->symbol == float64_lit){
                // E -> i
                *out_reduce_rule = operand;
        }
    }

    if(*out_reduce_rule == unknown){
        // if unknown rule -> out_reduce elements = NULL
        *out_reduce_element_0 = NULL;
        *out_reduce_element_1 = NULL;
        *out_reduce_element_2 = NULL;
    }
}