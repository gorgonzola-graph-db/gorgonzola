#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gorgonzola_flat_expression gorgonzola_flat_expression;
typedef struct gorgonzola_flat_query gorgonzola_flat_query;
typedef struct gorgonzola_flat_create_table gorgonzola_flat_create_table;
typedef struct gorgonzola_flat_drop gorgonzola_flat_drop;
typedef struct gorgonzola_flat_alter gorgonzola_flat_alter;
typedef struct gorgonzola_flat_copy_to gorgonzola_flat_copy_to;
typedef struct gorgonzola_flat_copy_from gorgonzola_flat_copy_from;
typedef struct gorgonzola_flat_standalone_call gorgonzola_flat_standalone_call;
typedef struct gorgonzola_flat_standalone_call_function gorgonzola_flat_standalone_call_function;
typedef struct gorgonzola_flat_explain gorgonzola_flat_explain;
typedef struct gorgonzola_flat_create_macro gorgonzola_flat_create_macro;
typedef struct gorgonzola_flat_transaction gorgonzola_flat_transaction;
typedef struct gorgonzola_flat_extension gorgonzola_flat_extension;
typedef struct gorgonzola_flat_attach_database gorgonzola_flat_attach_database;
typedef struct gorgonzola_flat_detach_database gorgonzola_flat_detach_database;
typedef struct gorgonzola_flat_use_database gorgonzola_flat_use_database;

struct gorgonzola_flat_statement {
    uint8_t type;
    union {
        gorgonzola_flat_query* query;
        gorgonzola_flat_create_table* create_table;
        gorgonzola_flat_drop* drop;
        gorgonzola_flat_alter* alter;
        gorgonzola_flat_copy_to* copy_to;
        gorgonzola_flat_copy_from* copy_from;
        gorgonzola_flat_standalone_call* standalone_call;
        gorgonzola_flat_standalone_call_function* standalone_call_function;
        gorgonzola_flat_explain* explain;
        gorgonzola_flat_create_macro* create_macro;
        gorgonzola_flat_transaction* transaction;
        gorgonzola_flat_extension* extension;
        gorgonzola_flat_attach_database* attach_database;
        gorgonzola_flat_detach_database* detach_database;
        gorgonzola_flat_use_database* use_database;
    } stmt;
};

struct gorgonzola_flat_query {
    // Fields for QUERY
    int dummy;
};

struct gorgonzola_flat_create_table {
    // Fields for CREATE_TABLE
    int dummy;
};

struct gorgonzola_flat_drop {
    // Fields for DROP
    int dummy;
};

struct gorgonzola_flat_alter {
    // Fields for ALTER
    int dummy;
};

struct gorgonzola_flat_copy_to {
    // Fields for COPY_TO
    int dummy;
};

struct gorgonzola_flat_copy_from {
    // Fields for COPY_FROM
    int dummy;
};

struct gorgonzola_flat_standalone_call {
    // Fields for STANDALONE_CALL
    int dummy;
};

struct gorgonzola_flat_standalone_call_function {
    // Fields for STANDALONE_CALL_FUNCTION
    int dummy;
};

struct gorgonzola_flat_explain {
    // Fields for EXPLAIN
    int dummy;
};

struct gorgonzola_flat_create_macro {
    // Fields for CREATE_MACRO
    int dummy;
};

struct gorgonzola_flat_transaction {
    // Fields for TRANSACTION
    int dummy;
};

struct gorgonzola_flat_extension {
    // Fields for EXTENSION
    int dummy;
};

struct gorgonzola_flat_attach_database {
    // Fields for ATTACH_DATABASE
    int dummy;
};

struct gorgonzola_flat_detach_database {
    // Fields for DETACH_DATABASE
    int dummy;
};

struct gorgonzola_flat_use_database {
    // Fields for USE_DATABASE
    int dummy;
};

#ifdef __cplusplus
}
#endif
