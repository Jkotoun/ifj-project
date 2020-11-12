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

/* ------------------------------- PUBLIC LOGIC ---------------------------------------*/

/**
 * @brief Top-level function for expression handling
 * 
 * Parses expression using a precedence analysis, checks if the expression 
 * is semantically correct and generates code for the interpreter.
 *
 * Should be called when expression was syntactically checked
 * @param sym_table TO BE ADD.
 * @param token_arr TO BE ADD.
 * @param token_count TO BE ADD.
 * @return EXIT_CODE
 * @see error_codes.h for EXIT_CODE details
 */
int parse_expression(table sym_table, 
    token *token_arr, 
    int token_count);

/* ------------------------------- INNER LOGIC ---------------------------------------*/

/**
 * @brief Gets expression symbol from the token
 * 
 * @param token represents current input token
 * @param out_expression_symbol represents the expression symbol
 * @return EXIT_CODE see error_codes.h
 */
int convert_token_to_expression_symbol(token token, 
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
    plus_minus = 0, /// +, -
    mul_div = 1,    /// *, /
    lbrack = 2,     /// (
    rbrack = 3,     /// )
    rel_op = 4,     /// ==, !=, < , <=, >, >=
    id_lit = 5,     /// id, int, string, float64
    dollar = 6,     /// $
} terminal_group;

/**
 * @brief Represents the precedence table
 * rows represent stack terminals, colums represent input terminals
 */
int precedence_table[7][7] = {
//  plus_minus | mul_div | lbrack | rbrack | rel_op | id_lit | dollar
    { R, S, S, R, R, S, R },    // plus_minus
    { R, R, S, R, R, S, R },    // mul_div
    { S, S, S, Eq, S, S, Er },  // lbrack
    { R, R, Er, R, R, Er, R },  // rbrack
    { S, S, S, R, Er, S, R },   // rel_op
    { R, R, Er, R, R, Er, R },  // id_lit
    { S, S, S, Er, S, S, Er}    // dollar
}

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
 * @enum Represents an expression symbol type
 */
typedef enum{
    plus,           /// +
    minus,          /// -
    mul,            /// *    
    div,            /// /
    lbrack,         /// (
    rbrack,         /// )
    eq,             /// =
    neq,            /// !=
    less,           /// <
    less_eq,        /// <=
    greater,        /// >
    greater_eq      /// >=
    id,             /// id
    int_lit,        /// int literal
    string_lit,     /// string literal
    float64_lit,    /// float64 literal
    nt,             /// non-terminal
    reduce_br,      /// symbol that determinest stop of the reduction (<)
    dollar         /// $
} expression_symbol;