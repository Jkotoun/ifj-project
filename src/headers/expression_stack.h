/**
 * @file expression_stack.h
 * @brief Expressions stack
 * @author Vít Tlustoš - xtlust05@stud.fit.vutbr.cz
 *
 * Reprpesents expression stack for precedence analysis
 */

#include "expression.h"
#include "symtable.h"

/**
 * @brief Represents stack node type
 */
typedef struct {
    expression_symbol_type symbol_type; // expression symbol type, e.g. id, $, ....
    node **id_node; /// tree node of the variable in case of symbol type = id
    struct expression_stack_node *next; /// ptr to next expression_stack_node
} expression_stack_node;

/**
 * @brief Represents stack that is used when analysing expressions
 */
typedef struct {
    expression_stack_node *top;
} expression_stack;

/**
 * @brief Initializes the stack
 */
void stack_init(expression_stack *stack);

/**
 * @brief Pushes to the stack
 * @param symbol_type represents the actual "symbol"
 * @param id_node id_node from symtable if symbol = id
 */
void stack_push(expression_stack *stack, expression_symbol_type symbol_type, node **id_node);

/**
 * @brief Gets the top terminal from stack
 * @param stack represents the expression_stack
 * @param top_terminal represents the output top terminal
 */
void stack_top_terminal(expression_stack *stack, expression_stack_node *top_terminal);

/**
 * @brief Gets the top terminal from stack
 * @param reduce_array represents the first "symbol" that should be reduced
 * @param reduce_array_size represents the amount of element that should be reduced (most likely to be 1 or 3)
 */
void stack_reduce_array(expression_stack *stack, expression_stack_node *reduce_array, int *reduce_array_size);