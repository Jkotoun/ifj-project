/**
 * @file expression.h 
 * @brief Expressions processing
 * @author Vít Tlustoš - xtlust05@stud.fit.vutbr.cz
 *
 * Processing of expressions by precedent analysis
 */

#pragma once

#include "scanner.h"
#include "symtable.h"
#include "expression_stack.h"

/* ------------------------------- INNER LOGIC ------------------------------------*/

/**
 * @enum Represents the precedence of rules 
 * in relation with current the stack state and the input token
 */
typedef enum {
    R,  /// > reduce
    S,  /// < shift
    Eq, /// = equal
    Er  /// error
} precedence;

/**
 * @enum Represents a terminal group
 * that input may contain
 */
typedef enum{
    plus_minus_gr = 0, /// +, -
    mul_div_gr = 1,    /// *, /
    lbrack_gr = 2,     /// (
    rbrack_gr = 3,     /// )
    rel_op_gr = 4,     /// ==, !=, < , <=, >, >=
    id_lit_gr = 5,     /// id, int, string, float64
    dollar_gr = 6,     /// $
} terminal_group;

/**
 * @enum Represents a reduction rule
 */
typedef enum{
    nt_plus_nt,         /// E -> E + E
    nt_minus_nt,        /// E -> E - E
    nt_mul_nt,          /// E -> E * E
    nt_div_nt,          /// E -> E / E
    nt_eq_nt,           /// E -> E == E
    nt_neq_nt,          /// E -> E != E
    nt_less_nt,         /// E -> E < E
    nt_less_eq_nt,      /// E -> E <= E
    nt_more_nt,         /// E -> E > E
    nt_more_eq_nt,      /// E -> E >= E
    lbrack_nt_rbrack,   /// E -> (E)
    operand             /// E -> i
} reduction_rule;

/**
 * @brief Represents one reduction step
 */
typedef struct instrumented_node{
    reduction_rule rule;
    varType type;
    struct instrumented_node *next;
}instrumented_node;

/**
 * @brief Gets expression symbol from the token
 * 
 * @param token represents current input token
 * @param out_expression_symbol represents the expression symbol
 * @return EXIT_CODE see error_codes.h
 */
int convert_token_to_expression_symbol(token *token, 
    expression_symbol *out_expression_symbol);

/**
 * @brief Converts expression symbol to the expression symbol group
 * 
 * helper so the precedence table can be smaller 
 * @param expression_symbol represents the symbol to be converted
 * @param out_terminal_group represents the converted symbol group
 * @return EXIT_CODE see error_codes.h
 */
int convert_expression_symbol_to_terminal_group(expression_symbol expression_symbol,
    terminal_group *out_terminal_group);

/**
 * @brief Gets the reduction rule based on current stack state and input
 * 
 * @param reduce_element_0 the last or an only element appearing in the rule if only one element in the rule
 * @param reduce_element_1 the seccond element appearing in the rule if any
 * @param reduce_element_2 the first element appearing in the rule if more than 1
 * @param out_reduction_rule the output reduction rule
 * @return EXIT_CODE see error_codes.h
 */ 
int get_reduction_rule(expression_stack_node *reduce_element_0, 
    expression_stack_node *reduce_element_1,
    expression_stack_node *reduce_element_2,
    reduction_rule *out_reduction_rule);

/**
 * @brief Monitors derived value of float and int literals
 * 
 * Enables the zero division check feature.
 * @param reduce_element_0 the last or an only element appearing in the rule if only one element in the rule
 * @param reduce_element_1 the seccond element appearing in the rule if any
 * @param reduce_element_2 the first element appearing in the rule if more than 1
 * @param rule used reduction rule
 * @return calculated derived value or default derived value if not possible to determine
 */ 
derived_value get_reduced_const_value(expression_stack_node *reduce_element_0,
    expression_stack_node *reduce_element_1,
    expression_stack_node *reduce_element_2,
    reduction_rule rule);

/**
 * @brief Inner function for expression handling
 * 
 * @param scoped_symtables representing scoped symtables
 * @param token_arr first token in the expression
 * @param token_count amount of tokens in the expression
 * @param out_type reduced type during the expression parsing
 * @param out_instrumented_arr nullable sequence of used reduction rules 
 * @return EXIT_CODE error_codes.h for details
 */
int parse_expression_inner(tDLList *scoped_symtables,
    token *token_arr, 
    int token_count,
    varType *out_type,
    instrumented_node **out_instrumented_arr);

/* ------------------------------- PUBLIC LOGIC ---------------------------------------*/

/**
 * @brief Top-level function for expression handling
 * 
 * Parses expression using a precedence analysis, checks if the expression 
 * is semantically correct and generates code for the interpreter.
 *
 * Should be called when expression was syntactically checked. 
 * Calls parse_expression_inner internaly.
 * @param scoped_symtables representing scoped symtables
 * @param token_arr first token in the expression
 * @param token_count amount of tokens in the expression
 * @param out_type reduced type during the expression parsing
 * @return EXIT_CODE error_codes.h for details
 */
int parse_expression(tDLList *scoped_symtables,
    token *token_arr, 
    int token_count,
    varType *out_type);
    
/* ---------------------------- INSTRUMENTATION LOGIC ----------------------------------*/

/**
 * @brief Top-level function for expression handling with instrumentation
 * 
 * Parses expression using a precedence analysis, checks if the expression 
 * is semantically correct and generates code for the interpreter.
 *
 * Calls parse_expression_inner internaly.
 * @param scoped_symtables representing scoped symtables
 * @param token_arr first token in the expression
 * @param token_count amount of tokens in the expression
 * @param out_type reduced type during the expression parsing
 * @param out_instrumented_arr sequence of used reduction rules 
 * @return EXIT_CODE error_codes.h for details
 */
int parse_instrumented_expression(tDLList *scoped_symtables,
    token *token_arr, 
    int token_count,
    varType *out_type,
    instrumented_node **out_instrumented_arr);