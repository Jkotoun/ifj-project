#pragma once
enum instruction_type {

    // Func calls, frames
    MOVE,
    CREATEFRAME,
    PUSHFRAME,
    POPFRAME,
    DEFVAR,
    CALL,
    RETURN,

    // Stack
    PUSHS,
    POPS,
    CLEARS,

    // Aritmetic instructions
    ADD,
    SUB,
    MUL,
    DIV,
    IDIV,
    ADDS,
    SUBS,
    MULS,
    DIVS,
    IDIVS,

    // Relational instructions
    LT,
    GT,
    EQ,
    LTS,
    GTS,
    EQS

    // Bool instructions
    AND,
    OR,
    NOT,
    ANDS,
    ORS,
    NOTS

    // Conversion instructions
    INT2FLOAT,
    FLOAT2INT,
    INT2CHAR,
    STRI2INT,
    INT2FLOATS,
    FLOAT2INTS,
    INT2CHARS,
    STRI2INTS

    // I/0 instructions
    READ,
    WRITE,

    // String instructions
    CONCAT,
    STRLEN,
    GETCHAR,
    SETCHAR

    // Types
    TYPE,

    // Program flow instructions
    LABEL,
    JUMP,
    JUMPIFEQ,
    JUMPIFNEQ,
    JUMPIFEQS,
    JUMPIFNEQS,
    EXIT,

    // Debugging instructions
    BREAK,
    DPRINT
};

