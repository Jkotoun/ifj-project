/**
 * @file expression_stack.h
 * @brief Expressions stack definition
 * @author Vít Tlustoš - xtlust05@stud.fit.vutbr.cz
 *
 * Represents expression stack for precedence analysis
 */

#include "expression.h"
#include "symtable.h"

/**
 * @brief Represents stack node type
 */
typedef struct {
    expression_symbol symbol; // expression symbol type, e.g. id, $, ....
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
void stack_push(expression_stack *stack, expression_symbol symbol, node **id_node);

/**
 * @brief Pops the stack
 * @param symbol_type represents the actual "symbol"
 * @param amount amount of pops, default = 1 more if multiple is required
 */
void stack_pop(expression_stack *stack, int amount = 1);

/**
 * @brief Gets the top terminal from stack
 * @param stack represents the expression_stack
 * @param out_top_terminal represents the output top terminal
 */
void stack_top_terminal(expression_stack *stack, expression_stack_node *out_top_terminal);

/**
 * @brief Detects wich rule should be used for reduction and return element(s) appearing in the rule
 * @param out_reduce_rule returns rule that should be used for reduction
 * @param out_reduce_element_0 returns the last or an only element appearing in the rule if only one element in the rule
 * @param out_reduce_element_1 returns the seccond element appearing in the rule if any
 * @param out_reduce_element_2 returns the first element appearing in the rule if any
 */
void stack_reduce_rule(expression_stack *stack, reduce_rule *out_reduce_rule, 
    expression_stack_node *out_reduce_element_0, expression_stack_node *out_reduce_element_1,
    expression_stack_node *out_reduce_element_2)

/**
 * @brief Frees all alocated resources
 */
void stack_dispose(expression_stack *stack);