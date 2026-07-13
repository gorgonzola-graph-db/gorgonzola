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

// Query structure handles
typedef struct { void* _single_query; }    gorgonzola_single_query;
typedef struct { void* _query_part; }      gorgonzola_query_part;
typedef struct { void* _reading_clause; }  gorgonzola_reading_clause;
typedef struct { void* _updating_clause; } gorgonzola_updating_clause;
typedef struct { void* _return_clause; }   gorgonzola_return_clause;
typedef struct { void* _with_clause; }     gorgonzola_with_clause;
typedef struct { void* _projection_body; } gorgonzola_projection_body;

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
#define GORGONZOLA_EXPR_OR 0
#define GORGONZOLA_EXPR_XOR 1
#define GORGONZOLA_EXPR_AND 2
#define GORGONZOLA_EXPR_NOT 3
#define GORGONZOLA_EXPR_EQUALS 10
#define GORGONZOLA_EXPR_NOT_EQUALS 11
#define GORGONZOLA_EXPR_GREATER_THAN 12
#define GORGONZOLA_EXPR_GREATER_THAN_EQUALS 13
#define GORGONZOLA_EXPR_LESS_THAN 14
#define GORGONZOLA_EXPR_LESS_THAN_EQUALS 15
#define GORGONZOLA_EXPR_IS_NULL 50
#define GORGONZOLA_EXPR_IS_NOT_NULL 51
#define GORGONZOLA_EXPR_PROPERTY 60
#define GORGONZOLA_EXPR_LITERAL 70
#define GORGONZOLA_EXPR_STAR 80
#define GORGONZOLA_EXPR_VARIABLE 90
#define GORGONZOLA_EXPR_PATH 91
#define GORGONZOLA_EXPR_PATTERN 92
#define GORGONZOLA_EXPR_PARAMETER 100
#define GORGONZOLA_EXPR_FUNCTION 110
#define GORGONZOLA_EXPR_AGGREGATE_FUNCTION 130
#define GORGONZOLA_EXPR_SUBQUERY 190
#define GORGONZOLA_EXPR_CASE_ELSE 200
#define GORGONZOLA_EXPR_GRAPH 210
#define GORGONZOLA_EXPR_LAMBDA 220
#define GORGONZOLA_EXPR_INVALID 255

// --- Clause type enum (mirrors gorgonzola::common::ClauseType) ---
typedef uint8_t gorgonzola_clause_type;
#define GORGONZOLA_CLAUSE_SET 0
#define GORGONZOLA_CLAUSE_DELETE 1
#define GORGONZOLA_CLAUSE_INSERT 2
#define GORGONZOLA_CLAUSE_MERGE 3
#define GORGONZOLA_CLAUSE_MATCH 10
#define GORGONZOLA_CLAUSE_UNWIND 11
#define GORGONZOLA_CLAUSE_IN_QUERY_CALL 12
#define GORGONZOLA_CLAUSE_TABLE_FUNCTION_CALL 13
#define GORGONZOLA_CLAUSE_GDS_CALL 14
#define GORGONZOLA_CLAUSE_LOAD_FROM 15

// --- Match clause type enum ---
typedef uint8_t gorgonzola_match_clause_type;
#define GORGONZOLA_MATCH_CLAUSE_MATCH 0
#define GORGONZOLA_MATCH_CLAUSE_OPTIONAL_MATCH 1

// ============================================================================
// Core parsing
// ============================================================================

GORGONZOLA_C_API gorgonzola_parsed_result gorgonzola_parse(const char* query);
GORGONZOLA_C_API void gorgonzola_parsed_result_destroy(gorgonzola_parsed_result* result);
GORGONZOLA_C_API uint64_t gorgonzola_parsed_result_num_statements(gorgonzola_parsed_result result);
GORGONZOLA_C_API gorgonzola_statement gorgonzola_parsed_result_get_statement(gorgonzola_parsed_result result, uint64_t index);

// ============================================================================
// Statement accessors
// ============================================================================

GORGONZOLA_C_API gorgonzola_statement_type gorgonzola_statement_get_type(gorgonzola_statement stmt);
GORGONZOLA_C_API bool gorgonzola_statement_is_internal(gorgonzola_statement stmt);

// DDL
GORGONZOLA_C_API char*    gorgonzola_create_table_get_name(gorgonzola_statement stmt);
GORGONZOLA_C_API uint64_t gorgonzola_create_table_get_num_properties(gorgonzola_statement stmt);
GORGONZOLA_C_API char*    gorgonzola_create_table_get_property_name(gorgonzola_statement stmt, uint64_t index);
GORGONZOLA_C_API char*    gorgonzola_create_table_get_property_type(gorgonzola_statement stmt, uint64_t index);
GORGONZOLA_C_API char*    gorgonzola_drop_get_name(gorgonzola_statement stmt);
GORGONZOLA_C_API char*    gorgonzola_alter_get_table_name(gorgonzola_statement stmt);

// Copy
GORGONZOLA_C_API char*    gorgonzola_copy_from_get_table_name(gorgonzola_statement stmt);
GORGONZOLA_C_API char*    gorgonzola_copy_to_get_file_path(gorgonzola_statement stmt);

// Database
GORGONZOLA_C_API char*    gorgonzola_attach_database_get_db_path(gorgonzola_statement stmt);
GORGONZOLA_C_API char*    gorgonzola_attach_database_get_db_alias(gorgonzola_statement stmt);
GORGONZOLA_C_API char*    gorgonzola_detach_database_get_db_name(gorgonzola_statement stmt);
GORGONZOLA_C_API char*    gorgonzola_use_database_get_db_name(gorgonzola_statement stmt);

// ============================================================================
// Query structure navigation
// ============================================================================

GORGONZOLA_C_API uint64_t                gorgonzola_query_get_num_single_queries(gorgonzola_statement stmt);
GORGONZOLA_C_API gorgonzola_single_query gorgonzola_query_get_single_query(gorgonzola_statement stmt, uint64_t index);
GORGONZOLA_C_API bool                    gorgonzola_query_is_union_all(gorgonzola_statement stmt, uint64_t index);

GORGONZOLA_C_API uint64_t                   gorgonzola_single_query_get_num_query_parts(gorgonzola_single_query sq);
GORGONZOLA_C_API gorgonzola_query_part      gorgonzola_single_query_get_query_part(gorgonzola_single_query sq, uint64_t index);
GORGONZOLA_C_API uint64_t                   gorgonzola_single_query_get_num_reading_clauses(gorgonzola_single_query sq);
GORGONZOLA_C_API gorgonzola_reading_clause  gorgonzola_single_query_get_reading_clause(gorgonzola_single_query sq, uint64_t index);
GORGONZOLA_C_API uint64_t                   gorgonzola_single_query_get_num_updating_clauses(gorgonzola_single_query sq);
GORGONZOLA_C_API gorgonzola_updating_clause gorgonzola_single_query_get_updating_clause(gorgonzola_single_query sq, uint64_t index);
GORGONZOLA_C_API bool                       gorgonzola_single_query_has_return_clause(gorgonzola_single_query sq);
GORGONZOLA_C_API gorgonzola_return_clause   gorgonzola_single_query_get_return_clause(gorgonzola_single_query sq);

GORGONZOLA_C_API uint64_t                   gorgonzola_query_part_get_num_reading_clauses(gorgonzola_query_part qp);
GORGONZOLA_C_API gorgonzola_reading_clause  gorgonzola_query_part_get_reading_clause(gorgonzola_query_part qp, uint64_t index);
GORGONZOLA_C_API uint64_t                   gorgonzola_query_part_get_num_updating_clauses(gorgonzola_query_part qp);
GORGONZOLA_C_API gorgonzola_updating_clause gorgonzola_query_part_get_updating_clause(gorgonzola_query_part qp, uint64_t index);
GORGONZOLA_C_API gorgonzola_with_clause     gorgonzola_query_part_get_with_clause(gorgonzola_query_part qp);

// ============================================================================
// Clause accessors
// ============================================================================

// Reading clause (base)
GORGONZOLA_C_API gorgonzola_clause_type gorgonzola_reading_clause_get_type(gorgonzola_reading_clause rc);
GORGONZOLA_C_API bool                   gorgonzola_reading_clause_has_where(gorgonzola_reading_clause rc);
GORGONZOLA_C_API gorgonzola_expression  gorgonzola_reading_clause_get_where(gorgonzola_reading_clause rc);

// Match clause
GORGONZOLA_C_API gorgonzola_match_clause_type gorgonzola_match_clause_get_type(gorgonzola_reading_clause rc);
GORGONZOLA_C_API uint64_t                     gorgonzola_match_clause_get_num_pattern_elements(gorgonzola_reading_clause rc);

// Unwind clause
GORGONZOLA_C_API gorgonzola_expression gorgonzola_unwind_clause_get_expression(gorgonzola_reading_clause rc);
GORGONZOLA_C_API char*                 gorgonzola_unwind_clause_get_alias(gorgonzola_reading_clause rc);

// Updating clause (base)
GORGONZOLA_C_API gorgonzola_clause_type gorgonzola_updating_clause_get_type(gorgonzola_updating_clause uc);

// Delete clause
GORGONZOLA_C_API uint64_t              gorgonzola_delete_clause_get_num_expressions(gorgonzola_updating_clause uc);
GORGONZOLA_C_API gorgonzola_expression gorgonzola_delete_clause_get_expression(gorgonzola_updating_clause uc, uint64_t index);

// Set clause
GORGONZOLA_C_API uint64_t gorgonzola_set_clause_get_num_items(gorgonzola_updating_clause uc);

// ============================================================================
// Projection body (shared by RETURN and WITH clauses)
// ============================================================================

GORGONZOLA_C_API gorgonzola_projection_body gorgonzola_return_clause_get_projection_body(gorgonzola_return_clause rc);
GORGONZOLA_C_API gorgonzola_projection_body gorgonzola_with_clause_get_projection_body(gorgonzola_with_clause wc);

GORGONZOLA_C_API bool                  gorgonzola_projection_body_is_distinct(gorgonzola_projection_body pb);
GORGONZOLA_C_API uint64_t              gorgonzola_projection_body_get_num_expressions(gorgonzola_projection_body pb);
GORGONZOLA_C_API gorgonzola_expression gorgonzola_projection_body_get_expression(gorgonzola_projection_body pb, uint64_t index);
GORGONZOLA_C_API bool                  gorgonzola_projection_body_has_order_by(gorgonzola_projection_body pb);
GORGONZOLA_C_API uint64_t              gorgonzola_projection_body_get_num_order_by(gorgonzola_projection_body pb);
GORGONZOLA_C_API gorgonzola_expression gorgonzola_projection_body_get_order_by(gorgonzola_projection_body pb, uint64_t index);
GORGONZOLA_C_API bool                  gorgonzola_projection_body_get_order_by_is_asc(gorgonzola_projection_body pb, uint64_t index);
GORGONZOLA_C_API bool                  gorgonzola_projection_body_has_skip(gorgonzola_projection_body pb);
GORGONZOLA_C_API gorgonzola_expression gorgonzola_projection_body_get_skip(gorgonzola_projection_body pb);
GORGONZOLA_C_API bool                  gorgonzola_projection_body_has_limit(gorgonzola_projection_body pb);
GORGONZOLA_C_API gorgonzola_expression gorgonzola_projection_body_get_limit(gorgonzola_projection_body pb);

// ============================================================================
// Expression tree walking
// ============================================================================

GORGONZOLA_C_API gorgonzola_expression_type gorgonzola_expression_get_type(gorgonzola_expression expr);
GORGONZOLA_C_API char*                      gorgonzola_expression_get_raw_name(gorgonzola_expression expr);
GORGONZOLA_C_API bool                       gorgonzola_expression_has_alias(gorgonzola_expression expr);
GORGONZOLA_C_API char*                      gorgonzola_expression_get_alias(gorgonzola_expression expr);
GORGONZOLA_C_API uint64_t                   gorgonzola_expression_get_num_children(gorgonzola_expression expr);
GORGONZOLA_C_API gorgonzola_expression      gorgonzola_expression_get_child(gorgonzola_expression expr, uint64_t index);

// ============================================================================
// Memory management
// ============================================================================

GORGONZOLA_C_API void gorgonzola_destroy_string(char* str);

#ifdef __cplusplus
}
#endif
