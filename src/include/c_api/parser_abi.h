#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "c_api/gorgonzola.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GORGONZOLA_C_API
#ifdef __cplusplus
#define GORGONZOLA_C_API extern "C" GORGONZOLA_API
#else
#define GORGONZOLA_C_API GORGONZOLA_API
#endif
#endif


// --- Opaque handles ---
typedef struct { void* _parsed_result; char* _error_message; } gorgonzola_parsed_result;
typedef struct { void* _statement; }     gorgonzola_statement;
typedef struct { void* _expression; }    gorgonzola_expression;

// Query part handles
typedef struct { void* _single_query; }    gorgonzola_single_query;
typedef struct { void* _query_part; }      gorgonzola_query_part;
typedef struct { void* _reading_clause; }  gorgonzola_reading_clause;
typedef struct { void* _updating_clause; } gorgonzola_updating_clause;
typedef struct { void* _return_clause; }   gorgonzola_return_clause;
typedef struct { void* _with_clause; }     gorgonzola_with_clause;

// --- Statement type enum (mirrors gorgonzola::common::StatementType) ---
typedef uint8_t gorgonzola_statement_type;
#define GORGONZOLA_STMT_QUERY 0
#define GORGONZOLA_STMT_CREATE_TABLE 1
#define GORGONZOLA_STMT_DROP 2
#define GORGONZOLA_STMT_ALTER 3
#define GORGONZOLA_STMT_COPY_TO 19
#define GORGONZOLA_STMT_COPY_FROM 20
#define GORGONZOLA_STMT_STANDALONE_CALL 21
#define GORGONZOLA_STMT_STANDALONE_CALL_FUNCTION 22
#define GORGONZOLA_STMT_EXPLAIN 23
#define GORGONZOLA_STMT_CREATE_MACRO 24
#define GORGONZOLA_STMT_TRANSACTION 30
#define GORGONZOLA_STMT_EXTENSION 31
#define GORGONZOLA_STMT_EXPORT_DATABASE 32
#define GORGONZOLA_STMT_IMPORT_DATABASE 33
#define GORGONZOLA_STMT_ATTACH_DATABASE 34
#define GORGONZOLA_STMT_DETACH_DATABASE 35
#define GORGONZOLA_STMT_USE_DATABASE 36
#define GORGONZOLA_STMT_CREATE_SEQUENCE 37
#define GORGONZOLA_STMT_CREATE_TYPE 39
#define GORGONZOLA_STMT_EXTENSION_CLAUSE 40

// --- Expression type enum (mirrors gorgonzola::common::ExpressionType) ---
typedef uint8_t gorgonzola_expression_type;

// --- Core parsing ---
/**
 * @brief Parses a Cypher query into an AST.
 * @param query The Cypher query string.
 * @return A handle to the parsed result. The caller must free it with gorgonzola_parsed_result_destroy.
 *         If parsing fails, _parsed_result will be NULL and _error_message will contain the error.
 */
GORGONZOLA_C_API gorgonzola_parsed_result gorgonzola_parse(const char* query);

/**
 * @brief Destroys a parsed result, freeing all associated AST memory and error strings.
 */
GORGONZOLA_C_API void gorgonzola_parsed_result_destroy(gorgonzola_parsed_result* result);

/**
 * @brief Returns the number of statements in the parsed result.
 */
GORGONZOLA_C_API uint64_t gorgonzola_parsed_result_num_statements(gorgonzola_parsed_result result);

/**
 * @brief Gets a specific statement from the parsed result.
 * @param index 0-based index of the statement.
 * @return A handle to the statement. The memory is owned by the parsed result.
 */
GORGONZOLA_C_API gorgonzola_statement gorgonzola_parsed_result_get_statement(gorgonzola_parsed_result result, uint64_t index);


// --- Statement accessors ---
GORGONZOLA_C_API gorgonzola_statement_type gorgonzola_statement_get_type(gorgonzola_statement stmt);
GORGONZOLA_C_API bool                      gorgonzola_statement_is_internal(gorgonzola_statement stmt);

// --- DDL Statement accessors ---
GORGONZOLA_C_API char*    gorgonzola_create_table_get_name(gorgonzola_statement stmt);
GORGONZOLA_C_API uint64_t gorgonzola_create_table_get_num_properties(gorgonzola_statement stmt);
GORGONZOLA_C_API char*    gorgonzola_create_table_get_property_name(gorgonzola_statement stmt, uint64_t index);
GORGONZOLA_C_API char*    gorgonzola_create_table_get_property_type(gorgonzola_statement stmt, uint64_t index);

GORGONZOLA_C_API char*    gorgonzola_drop_get_name(gorgonzola_statement stmt);
GORGONZOLA_C_API char*    gorgonzola_alter_get_table_name(gorgonzola_statement stmt);

// --- Copy Statement accessors ---
GORGONZOLA_C_API char*    gorgonzola_copy_from_get_table_name(gorgonzola_statement stmt);
GORGONZOLA_C_API char*    gorgonzola_copy_to_get_file_path(gorgonzola_statement stmt);

// --- Database Statement accessors ---
GORGONZOLA_C_API char*    gorgonzola_attach_database_get_db_path(gorgonzola_statement stmt);
GORGONZOLA_C_API char*    gorgonzola_attach_database_get_db_alias(gorgonzola_statement stmt);
GORGONZOLA_C_API char*    gorgonzola_detach_database_get_db_name(gorgonzola_statement stmt);
GORGONZOLA_C_API char*    gorgonzola_use_database_get_db_name(gorgonzola_statement stmt);

// --- Query Statement accessors ---
GORGONZOLA_C_API uint64_t                gorgonzola_query_get_num_single_queries(gorgonzola_statement stmt);
GORGONZOLA_C_API gorgonzola_single_query gorgonzola_query_get_single_query(gorgonzola_statement stmt, uint64_t index);
GORGONZOLA_C_API bool                    gorgonzola_query_is_union_all(gorgonzola_statement stmt, uint64_t index);

// --- Single Query accessors ---
GORGONZOLA_C_API uint64_t                   gorgonzola_single_query_get_num_query_parts(gorgonzola_single_query sq);
GORGONZOLA_C_API gorgonzola_query_part      gorgonzola_single_query_get_query_part(gorgonzola_single_query sq, uint64_t index);
GORGONZOLA_C_API uint64_t                   gorgonzola_single_query_get_num_reading_clauses(gorgonzola_single_query sq);
GORGONZOLA_C_API gorgonzola_reading_clause  gorgonzola_single_query_get_reading_clause(gorgonzola_single_query sq, uint64_t index);
GORGONZOLA_C_API uint64_t                   gorgonzola_single_query_get_num_updating_clauses(gorgonzola_single_query sq);
GORGONZOLA_C_API gorgonzola_updating_clause gorgonzola_single_query_get_updating_clause(gorgonzola_single_query sq, uint64_t index);
GORGONZOLA_C_API bool                       gorgonzola_single_query_has_return_clause(gorgonzola_single_query sq);
GORGONZOLA_C_API gorgonzola_return_clause   gorgonzola_single_query_get_return_clause(gorgonzola_single_query sq);

// --- Query Part accessors ---
GORGONZOLA_C_API uint64_t                   gorgonzola_query_part_get_num_reading_clauses(gorgonzola_query_part qp);
GORGONZOLA_C_API gorgonzola_reading_clause  gorgonzola_query_part_get_reading_clause(gorgonzola_query_part qp, uint64_t index);
GORGONZOLA_C_API uint64_t                   gorgonzola_query_part_get_num_updating_clauses(gorgonzola_query_part qp);
GORGONZOLA_C_API gorgonzola_updating_clause gorgonzola_query_part_get_updating_clause(gorgonzola_query_part qp, uint64_t index);
GORGONZOLA_C_API bool                       gorgonzola_query_part_has_with_clause(gorgonzola_query_part qp);
GORGONZOLA_C_API gorgonzola_with_clause     gorgonzola_query_part_get_with_clause(gorgonzola_query_part qp);

// --- String cleanup ---
/**
 * @brief Frees a string returned by any of the getter functions (other than _error_message inside the result struct).
 */
GORGONZOLA_C_API void gorgonzola_destroy_string(char* str);

#ifdef __cplusplus
}
#endif
