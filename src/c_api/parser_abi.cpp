#include "c_api/parser_abi.h"
#include "c_api_utils.h"

#include <cstdlib>
#include <cstring>
#include <vector>

#include "parser/parser.h"
#include "parser/statement.h"
#include "parser/ddl/create_table.h"
#include "parser/ddl/drop.h"
#include "parser/ddl/alter.h"
#include "parser/copy.h"
#include "parser/attach_database.h"
#include "parser/detach_database.h"
#include "parser/use_database.h"
#include "parser/query/regular_query.h"
#include "parser/query/single_query.h"
#include "parser/query/query_part.h"
#include "parser/query/reading_clause/reading_clause.h"
#include "parser/query/reading_clause/match_clause.h"
#include "parser/query/reading_clause/unwind_clause.h"
#include "parser/query/updating_clause/updating_clause.h"
#include "parser/query/updating_clause/delete_clause.h"
#include "parser/query/updating_clause/set_clause.h"
#include "parser/query/return_with_clause/return_clause.h"
#include "parser/query/return_with_clause/with_clause.h"
#include "parser/query/return_with_clause/projection_body.h"
#include "parser/expression/parsed_expression.h"

using namespace gorgonzola;
using namespace gorgonzola::parser;

// ============================================================================
// Static asserts — guarantee ABI compatibility at compile time
// ============================================================================

// StatementType
static_assert((uint8_t)common::StatementType::QUERY == GORGONZOLA_STMT_QUERY);
static_assert((uint8_t)common::StatementType::CREATE_TABLE == GORGONZOLA_STMT_CREATE_TABLE);
static_assert((uint8_t)common::StatementType::DROP == GORGONZOLA_STMT_DROP);
static_assert((uint8_t)common::StatementType::ALTER == GORGONZOLA_STMT_ALTER);
static_assert((uint8_t)common::StatementType::COPY_TO == GORGONZOLA_STMT_COPY_TO);
static_assert((uint8_t)common::StatementType::COPY_FROM == GORGONZOLA_STMT_COPY_FROM);
static_assert((uint8_t)common::StatementType::STANDALONE_CALL == GORGONZOLA_STMT_STANDALONE_CALL);
static_assert((uint8_t)common::StatementType::STANDALONE_CALL_FUNCTION == GORGONZOLA_STMT_STANDALONE_CALL_FUNCTION);
static_assert((uint8_t)common::StatementType::EXPLAIN == GORGONZOLA_STMT_EXPLAIN);
static_assert((uint8_t)common::StatementType::CREATE_MACRO == GORGONZOLA_STMT_CREATE_MACRO);
static_assert((uint8_t)common::StatementType::TRANSACTION == GORGONZOLA_STMT_TRANSACTION);
static_assert((uint8_t)common::StatementType::EXTENSION == GORGONZOLA_STMT_EXTENSION);
static_assert((uint8_t)common::StatementType::EXPORT_DATABASE == GORGONZOLA_STMT_EXPORT_DATABASE);
static_assert((uint8_t)common::StatementType::IMPORT_DATABASE == GORGONZOLA_STMT_IMPORT_DATABASE);
static_assert((uint8_t)common::StatementType::ATTACH_DATABASE == GORGONZOLA_STMT_ATTACH_DATABASE);
static_assert((uint8_t)common::StatementType::DETACH_DATABASE == GORGONZOLA_STMT_DETACH_DATABASE);
static_assert((uint8_t)common::StatementType::USE_DATABASE == GORGONZOLA_STMT_USE_DATABASE);
static_assert((uint8_t)common::StatementType::CREATE_SEQUENCE == GORGONZOLA_STMT_CREATE_SEQUENCE);
static_assert((uint8_t)common::StatementType::CREATE_TYPE == GORGONZOLA_STMT_CREATE_TYPE);
static_assert((uint8_t)common::StatementType::EXTENSION_CLAUSE == GORGONZOLA_STMT_EXTENSION_CLAUSE);

// ExpressionType
static_assert((uint8_t)common::ExpressionType::OR == GORGONZOLA_EXPR_OR);
static_assert((uint8_t)common::ExpressionType::AND == GORGONZOLA_EXPR_AND);
static_assert((uint8_t)common::ExpressionType::NOT == GORGONZOLA_EXPR_NOT);
static_assert((uint8_t)common::ExpressionType::EQUALS == GORGONZOLA_EXPR_EQUALS);
static_assert((uint8_t)common::ExpressionType::LITERAL == GORGONZOLA_EXPR_LITERAL);
static_assert((uint8_t)common::ExpressionType::VARIABLE == GORGONZOLA_EXPR_VARIABLE);
static_assert((uint8_t)common::ExpressionType::FUNCTION == GORGONZOLA_EXPR_FUNCTION);
static_assert((uint8_t)common::ExpressionType::PROPERTY == GORGONZOLA_EXPR_PROPERTY);
static_assert((uint8_t)common::ExpressionType::STAR == GORGONZOLA_EXPR_STAR);
static_assert((uint8_t)common::ExpressionType::INVALID == GORGONZOLA_EXPR_INVALID);

// ClauseType
static_assert((uint8_t)common::ClauseType::SET == GORGONZOLA_CLAUSE_SET);
static_assert((uint8_t)common::ClauseType::DELETE_ == GORGONZOLA_CLAUSE_DELETE);
static_assert((uint8_t)common::ClauseType::INSERT == GORGONZOLA_CLAUSE_INSERT);
static_assert((uint8_t)common::ClauseType::MERGE == GORGONZOLA_CLAUSE_MERGE);
static_assert((uint8_t)common::ClauseType::MATCH == GORGONZOLA_CLAUSE_MATCH);
static_assert((uint8_t)common::ClauseType::UNWIND == GORGONZOLA_CLAUSE_UNWIND);
static_assert((uint8_t)common::ClauseType::IN_QUERY_CALL == GORGONZOLA_CLAUSE_IN_QUERY_CALL);
static_assert((uint8_t)common::ClauseType::LOAD_FROM == GORGONZOLA_CLAUSE_LOAD_FROM);

// MatchClauseType
static_assert((uint8_t)common::MatchClauseType::MATCH == GORGONZOLA_MATCH_CLAUSE_MATCH);
static_assert((uint8_t)common::MatchClauseType::OPTIONAL_MATCH == GORGONZOLA_MATCH_CLAUSE_OPTIONAL_MATCH);

// ============================================================================
// Internal wrapper
// ============================================================================

struct ParsedResultWrapper {
    std::vector<std::shared_ptr<Statement>> statements;
};

// ============================================================================
// Core parsing
// ============================================================================

gorgonzola_parsed_result gorgonzola_parse(const char* query) {
    gorgonzola_parsed_result result = {nullptr, nullptr};
    try {
        auto statements = Parser::parseQuery(query);
        auto wrapper = new ParsedResultWrapper{std::move(statements)};
        result._parsed_result = wrapper;
    } catch (std::exception& e) {
        result._error_message = strdup(e.what());
    } catch (...) {
        result._error_message = strdup("Unknown parser exception occurred");
    }
    return result;
}

void gorgonzola_parsed_result_destroy(gorgonzola_parsed_result* result) {
        GORGONZOLA_C_API_BEGIN

    if (!result) return;
    if (result->_parsed_result) {
        delete static_cast<ParsedResultWrapper*>(result->_parsed_result);
        result->_parsed_result = nullptr;
    }
    if (result->_error_message) {
        free(result->_error_message);
        result->_error_message = nullptr;
    }

    } catch (...) {
        gorgonzola::c_api::translate_exception();
    }
}

uint64_t gorgonzola_parsed_result_num_statements(gorgonzola_parsed_result result) {
        GORGONZOLA_C_API_BEGIN

    if (!result._parsed_result) return {0};
    return static_cast<ParsedResultWrapper*>(result._parsed_result)->statements.size();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

gorgonzola_statement gorgonzola_parsed_result_get_statement(gorgonzola_parsed_result result, uint64_t index) {
    gorgonzola_statement stmt = {nullptr};
    if (!result._parsed_result) return stmt;
    auto wrapper = static_cast<ParsedResultWrapper*>(result._parsed_result);
    if (index < wrapper->statements.size()) {
        stmt._statement = wrapper->statements[index].get();
    }
    return stmt;
}

// ============================================================================
// Statement accessors
// ============================================================================

gorgonzola_statement_type gorgonzola_statement_get_type(gorgonzola_statement stmt) {
    if (!stmt._statement) return (gorgonzola_statement_type)255;
    return static_cast<gorgonzola_statement_type>(
        static_cast<Statement*>(stmt._statement)->getStatementType());
}

bool gorgonzola_statement_is_internal(gorgonzola_statement stmt) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return false;
    return static_cast<Statement*>(stmt._statement)->isInternal();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

// --- DDL ---

char* gorgonzola_create_table_get_name(gorgonzola_statement stmt) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return nullptr;
    return strdup(static_cast<CreateTable*>(stmt._statement)->getInfo()->tableName.c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

uint64_t gorgonzola_create_table_get_num_properties(gorgonzola_statement stmt) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return {0};
    return static_cast<CreateTable*>(stmt._statement)->getInfo()->propertyDefinitions.size();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

char* gorgonzola_create_table_get_property_name(gorgonzola_statement stmt, uint64_t index) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return nullptr;
    auto info = static_cast<CreateTable*>(stmt._statement)->getInfo();
    if (index >= info->propertyDefinitions.size()) return nullptr;
    return strdup(info->propertyDefinitions[index].getName().c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

char* gorgonzola_create_table_get_property_type(gorgonzola_statement stmt, uint64_t index) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return nullptr;
    auto info = static_cast<CreateTable*>(stmt._statement)->getInfo();
    if (index >= info->propertyDefinitions.size()) return nullptr;
    return strdup(info->propertyDefinitions[index].getType().c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

char* gorgonzola_drop_get_name(gorgonzola_statement stmt) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return nullptr;
    return strdup(static_cast<Drop*>(stmt._statement)->getDropInfo().name.c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

char* gorgonzola_alter_get_table_name(gorgonzola_statement stmt) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return nullptr;
    return strdup(static_cast<Alter*>(stmt._statement)->getInfo()->tableName.c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

// --- Copy ---

char* gorgonzola_copy_from_get_table_name(gorgonzola_statement stmt) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return nullptr;
    return strdup(static_cast<CopyFrom*>(stmt._statement)->getTableName().c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

char* gorgonzola_copy_to_get_file_path(gorgonzola_statement stmt) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return nullptr;
    return strdup(static_cast<CopyTo*>(stmt._statement)->getFilePath().c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

// --- Database ---

char* gorgonzola_attach_database_get_db_path(gorgonzola_statement stmt) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return nullptr;
    return strdup(static_cast<AttachDatabase*>(stmt._statement)->getAttachInfo().dbPath.c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

char* gorgonzola_attach_database_get_db_alias(gorgonzola_statement stmt) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return nullptr;
    return strdup(static_cast<AttachDatabase*>(stmt._statement)->getAttachInfo().dbAlias.c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

char* gorgonzola_detach_database_get_db_name(gorgonzola_statement stmt) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return nullptr;
    return strdup(static_cast<DetachDatabase*>(stmt._statement)->getDBName().c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

char* gorgonzola_use_database_get_db_name(gorgonzola_statement stmt) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return nullptr;
    return strdup(static_cast<UseDatabase*>(stmt._statement)->getDBName().c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

// ============================================================================
// Query structure navigation
// ============================================================================

uint64_t gorgonzola_query_get_num_single_queries(gorgonzola_statement stmt) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return {0};
    return static_cast<RegularQuery*>(stmt._statement)->getNumSingleQueries();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

gorgonzola_single_query gorgonzola_query_get_single_query(gorgonzola_statement stmt, uint64_t index) {
    gorgonzola_single_query result = {nullptr};
    if (!stmt._statement) return result;
    auto query = static_cast<RegularQuery*>(stmt._statement);
    if (index >= query->getNumSingleQueries()) return result;
    result._single_query = const_cast<SingleQuery*>(query->getSingleQuery(index));
    return result;
}

bool gorgonzola_query_is_union_all(gorgonzola_statement stmt, uint64_t index) {
        GORGONZOLA_C_API_BEGIN

    if (!stmt._statement) return false;
    auto flags = static_cast<RegularQuery*>(stmt._statement)->getIsUnionAll();
    if (index >= flags.size()) return false;
    return flags[index];

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

// --- Single Query ---

uint64_t gorgonzola_single_query_get_num_query_parts(gorgonzola_single_query sq) {
        GORGONZOLA_C_API_BEGIN

    if (!sq._single_query) return {0};
    return static_cast<SingleQuery*>(sq._single_query)->getNumQueryParts();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

gorgonzola_query_part gorgonzola_single_query_get_query_part(gorgonzola_single_query sq, uint64_t index) {
    gorgonzola_query_part result = {nullptr};
    if (!sq._single_query) return result;
    auto singleQuery = static_cast<SingleQuery*>(sq._single_query);
    if (index >= singleQuery->getNumQueryParts()) return result;
    result._query_part = const_cast<QueryPart*>(singleQuery->getQueryPart(index));
    return result;
}

uint64_t gorgonzola_single_query_get_num_reading_clauses(gorgonzola_single_query sq) {
        GORGONZOLA_C_API_BEGIN

    if (!sq._single_query) return {0};
    return static_cast<SingleQuery*>(sq._single_query)->getNumReadingClauses();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

gorgonzola_reading_clause gorgonzola_single_query_get_reading_clause(gorgonzola_single_query sq, uint64_t index) {
    gorgonzola_reading_clause result = {nullptr};
    if (!sq._single_query) return result;
    auto singleQuery = static_cast<SingleQuery*>(sq._single_query);
    if (index >= singleQuery->getNumReadingClauses()) return result;
    result._reading_clause = singleQuery->getReadingClause(index);
    return result;
}

uint64_t gorgonzola_single_query_get_num_updating_clauses(gorgonzola_single_query sq) {
        GORGONZOLA_C_API_BEGIN

    if (!sq._single_query) return {0};
    return static_cast<SingleQuery*>(sq._single_query)->getNumUpdatingClauses();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

gorgonzola_updating_clause gorgonzola_single_query_get_updating_clause(gorgonzola_single_query sq, uint64_t index) {
    gorgonzola_updating_clause result = {nullptr};
    if (!sq._single_query) return result;
    auto singleQuery = static_cast<SingleQuery*>(sq._single_query);
    if (index >= singleQuery->getNumUpdatingClauses()) return result;
    result._updating_clause = singleQuery->getUpdatingClause(index);
    return result;
}

bool gorgonzola_single_query_has_return_clause(gorgonzola_single_query sq) {
        GORGONZOLA_C_API_BEGIN

    if (!sq._single_query) return false;
    return static_cast<SingleQuery*>(sq._single_query)->hasReturnClause();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

gorgonzola_return_clause gorgonzola_single_query_get_return_clause(gorgonzola_single_query sq) {
    gorgonzola_return_clause result = {nullptr};
    if (!sq._single_query) return result;
    auto singleQuery = static_cast<SingleQuery*>(sq._single_query);
    if (!singleQuery->hasReturnClause()) return result;
    result._return_clause = const_cast<ReturnClause*>(singleQuery->getReturnClause());
    return result;
}

// --- Query Part ---

uint64_t gorgonzola_query_part_get_num_reading_clauses(gorgonzola_query_part qp) {
        GORGONZOLA_C_API_BEGIN

    if (!qp._query_part) return {0};
    return static_cast<QueryPart*>(qp._query_part)->getNumReadingClauses();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

gorgonzola_reading_clause gorgonzola_query_part_get_reading_clause(gorgonzola_query_part qp, uint64_t index) {
    gorgonzola_reading_clause result = {nullptr};
    if (!qp._query_part) return result;
    auto queryPart = static_cast<QueryPart*>(qp._query_part);
    if (index >= queryPart->getNumReadingClauses()) return result;
    result._reading_clause = queryPart->getReadingClause(index);
    return result;
}

uint64_t gorgonzola_query_part_get_num_updating_clauses(gorgonzola_query_part qp) {
        GORGONZOLA_C_API_BEGIN

    if (!qp._query_part) return {0};
    return static_cast<QueryPart*>(qp._query_part)->getNumUpdatingClauses();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

gorgonzola_updating_clause gorgonzola_query_part_get_updating_clause(gorgonzola_query_part qp, uint64_t index) {
    gorgonzola_updating_clause result = {nullptr};
    if (!qp._query_part) return result;
    auto queryPart = static_cast<QueryPart*>(qp._query_part);
    if (index >= queryPart->getNumUpdatingClauses()) return result;
    result._updating_clause = queryPart->getUpdatingClause(index);
    return result;
}

gorgonzola_with_clause gorgonzola_query_part_get_with_clause(gorgonzola_query_part qp) {
    gorgonzola_with_clause result = {nullptr};
    if (!qp._query_part) return result;
    result._with_clause = const_cast<WithClause*>(
        static_cast<QueryPart*>(qp._query_part)->getWithClause());
    return result;
}

// ============================================================================
// Clause accessors
// ============================================================================

// --- Reading Clause (base) ---

gorgonzola_clause_type gorgonzola_reading_clause_get_type(gorgonzola_reading_clause rc) {
    if (!rc._reading_clause) return (gorgonzola_clause_type)255;
    return static_cast<gorgonzola_clause_type>(
        static_cast<ReadingClause*>(rc._reading_clause)->getClauseType());
}

bool gorgonzola_reading_clause_has_where(gorgonzola_reading_clause rc) {
        GORGONZOLA_C_API_BEGIN

    if (!rc._reading_clause) return false;
    return static_cast<ReadingClause*>(rc._reading_clause)->hasWherePredicate();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

gorgonzola_expression gorgonzola_reading_clause_get_where(gorgonzola_reading_clause rc) {
        GORGONZOLA_C_API_BEGIN

    gorgonzola_expression result = {nullptr};
    if (!rc._reading_clause) return result;
    auto clause = static_cast<ReadingClause*>(rc._reading_clause);
    if (!clause->hasWherePredicate()) return result;
    result._expression = const_cast<ParsedExpression*>(clause->getWherePredicate());
    return result;

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

// --- Match Clause ---

gorgonzola_match_clause_type gorgonzola_match_clause_get_type(gorgonzola_reading_clause rc) {
    if (!rc._reading_clause) return (gorgonzola_match_clause_type)255;
    return static_cast<gorgonzola_match_clause_type>(
        static_cast<ReadingClause*>(rc._reading_clause)
            ->constCast<MatchClause>().getMatchClauseType());
}

uint64_t gorgonzola_match_clause_get_num_pattern_elements(gorgonzola_reading_clause rc) {
        GORGONZOLA_C_API_BEGIN

    if (!rc._reading_clause) return {0};
    return static_cast<ReadingClause*>(rc._reading_clause)
        ->constCast<MatchClause>().getPatternElementsRef().size();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

// --- Unwind Clause ---

gorgonzola_expression gorgonzola_unwind_clause_get_expression(gorgonzola_reading_clause rc) {
        GORGONZOLA_C_API_BEGIN

    gorgonzola_expression result = {nullptr};
    if (!rc._reading_clause) return result;
    auto expr = static_cast<ReadingClause*>(rc._reading_clause)
        ->constCast<UnwindClause>().getExpression();
    result._expression = const_cast<ParsedExpression*>(expr);
    return result;

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

char* gorgonzola_unwind_clause_get_alias(gorgonzola_reading_clause rc) {
        GORGONZOLA_C_API_BEGIN

    if (!rc._reading_clause) return nullptr;
    return strdup(static_cast<ReadingClause*>(rc._reading_clause)
        ->constCast<UnwindClause>().getAlias().c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

// --- Updating Clause (base) ---

gorgonzola_clause_type gorgonzola_updating_clause_get_type(gorgonzola_updating_clause uc) {
    if (!uc._updating_clause) return (gorgonzola_clause_type)255;
    return static_cast<gorgonzola_clause_type>(
        static_cast<UpdatingClause*>(uc._updating_clause)->getClauseType());
}

// --- Delete Clause ---

uint64_t gorgonzola_delete_clause_get_num_expressions(gorgonzola_updating_clause uc) {
        GORGONZOLA_C_API_BEGIN

    if (!uc._updating_clause) return {0};
    return static_cast<UpdatingClause*>(uc._updating_clause)
        ->constCast<DeleteClause>().getNumExpressions();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

gorgonzola_expression gorgonzola_delete_clause_get_expression(gorgonzola_updating_clause uc, uint64_t index) {
        GORGONZOLA_C_API_BEGIN

    gorgonzola_expression result = {nullptr};
    if (!uc._updating_clause) return result;
    auto& del = static_cast<UpdatingClause*>(uc._updating_clause)->constCast<DeleteClause>();
    if (index >= del.getNumExpressions()) return result;
    result._expression = del.getExpression(index);
    return result;

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

// --- Set Clause ---

uint64_t gorgonzola_set_clause_get_num_items(gorgonzola_updating_clause uc) {
        GORGONZOLA_C_API_BEGIN

    if (!uc._updating_clause) return {0};
    return static_cast<UpdatingClause*>(uc._updating_clause)
        ->constCast<SetClause>().getSetItemsRef().size();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

// ============================================================================
// Projection Body
// ============================================================================

gorgonzola_projection_body gorgonzola_return_clause_get_projection_body(gorgonzola_return_clause rc) {
    gorgonzola_projection_body result = {nullptr};
    if (!rc._return_clause) return result;
    result._projection_body = const_cast<ProjectionBody*>(
        static_cast<ReturnClause*>(rc._return_clause)->getProjectionBody());
    return result;
}

gorgonzola_projection_body gorgonzola_with_clause_get_projection_body(gorgonzola_with_clause wc) {
    gorgonzola_projection_body result = {nullptr};
    if (!wc._with_clause) return result;
    result._projection_body = const_cast<ProjectionBody*>(
        static_cast<WithClause*>(wc._with_clause)->getProjectionBody());
    return result;
}

bool gorgonzola_projection_body_is_distinct(gorgonzola_projection_body pb) {
        GORGONZOLA_C_API_BEGIN

    if (!pb._projection_body) return false;
    return static_cast<ProjectionBody*>(pb._projection_body)->getIsDistinct();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

uint64_t gorgonzola_projection_body_get_num_expressions(gorgonzola_projection_body pb) {
        GORGONZOLA_C_API_BEGIN

    if (!pb._projection_body) return {0};
    return static_cast<ProjectionBody*>(pb._projection_body)->getProjectionExpressions().size();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

gorgonzola_expression gorgonzola_projection_body_get_expression(gorgonzola_projection_body pb, uint64_t index) {
        GORGONZOLA_C_API_BEGIN

    gorgonzola_expression result = {nullptr};
    if (!pb._projection_body) return result;
    auto& exprs = static_cast<ProjectionBody*>(pb._projection_body)->getProjectionExpressions();
    if (index >= exprs.size()) return result;
    result._expression = exprs[index].get();
    return result;

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

bool gorgonzola_projection_body_has_order_by(gorgonzola_projection_body pb) {
        GORGONZOLA_C_API_BEGIN

    if (!pb._projection_body) return false;
    return static_cast<ProjectionBody*>(pb._projection_body)->hasOrderByExpressions();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

uint64_t gorgonzola_projection_body_get_num_order_by(gorgonzola_projection_body pb) {
        GORGONZOLA_C_API_BEGIN

    if (!pb._projection_body) return {0};
    return static_cast<ProjectionBody*>(pb._projection_body)->getOrderByExpressions().size();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

gorgonzola_expression gorgonzola_projection_body_get_order_by(gorgonzola_projection_body pb, uint64_t index) {
        GORGONZOLA_C_API_BEGIN

    gorgonzola_expression result = {nullptr};
    if (!pb._projection_body) return result;
    auto& exprs = static_cast<ProjectionBody*>(pb._projection_body)->getOrderByExpressions();
    if (index >= exprs.size()) return result;
    result._expression = exprs[index].get();
    return result;

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

bool gorgonzola_projection_body_get_order_by_is_asc(gorgonzola_projection_body pb, uint64_t index) {
        GORGONZOLA_C_API_BEGIN

    if (!pb._projection_body) return true;
    auto orders = static_cast<ProjectionBody*>(pb._projection_body)->getSortOrders();
    if (index >= orders.size()) return true;
    return orders[index];

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

bool gorgonzola_projection_body_has_skip(gorgonzola_projection_body pb) {
        GORGONZOLA_C_API_BEGIN

    if (!pb._projection_body) return false;
    return static_cast<ProjectionBody*>(pb._projection_body)->hasSkipExpression();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

gorgonzola_expression gorgonzola_projection_body_get_skip(gorgonzola_projection_body pb) {
        GORGONZOLA_C_API_BEGIN

    gorgonzola_expression result = {nullptr};
    if (!pb._projection_body) return result;
    auto body = static_cast<ProjectionBody*>(pb._projection_body);
    if (!body->hasSkipExpression()) return result;
    result._expression = body->getSkipExpression();
    return result;

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

bool gorgonzola_projection_body_has_limit(gorgonzola_projection_body pb) {
        GORGONZOLA_C_API_BEGIN

    if (!pb._projection_body) return false;
    return static_cast<ProjectionBody*>(pb._projection_body)->hasLimitExpression();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

gorgonzola_expression gorgonzola_projection_body_get_limit(gorgonzola_projection_body pb) {
        GORGONZOLA_C_API_BEGIN

    gorgonzola_expression result = {nullptr};
    if (!pb._projection_body) return result;
    auto body = static_cast<ProjectionBody*>(pb._projection_body);
    if (!body->hasLimitExpression()) return result;
    result._expression = body->getLimitExpression();
    return result;

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

// ============================================================================
// Expression tree walking
// ============================================================================

gorgonzola_expression_type gorgonzola_expression_get_type(gorgonzola_expression expr) {
        GORGONZOLA_C_API_BEGIN

    if (!expr._expression) return GORGONZOLA_EXPR_INVALID;
    return static_cast<gorgonzola_expression_type>(
        static_cast<ParsedExpression*>(expr._expression)->getExpressionType());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

char* gorgonzola_expression_get_raw_name(gorgonzola_expression expr) {
        GORGONZOLA_C_API_BEGIN

    if (!expr._expression) return nullptr;
    return strdup(static_cast<ParsedExpression*>(expr._expression)->getRawName().c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

bool gorgonzola_expression_has_alias(gorgonzola_expression expr) {
        GORGONZOLA_C_API_BEGIN

    if (!expr._expression) return false;
    return static_cast<ParsedExpression*>(expr._expression)->hasAlias();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

char* gorgonzola_expression_get_alias(gorgonzola_expression expr) {
        GORGONZOLA_C_API_BEGIN

    if (!expr._expression) return nullptr;
    auto e = static_cast<ParsedExpression*>(expr._expression);
    if (!e->hasAlias()) return nullptr;
    return strdup(e->getAlias().c_str());

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

uint64_t gorgonzola_expression_get_num_children(gorgonzola_expression expr) {
        GORGONZOLA_C_API_BEGIN

    if (!expr._expression) return {0};
    return static_cast<ParsedExpression*>(expr._expression)->getNumChildren();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

gorgonzola_expression gorgonzola_expression_get_child(gorgonzola_expression expr, uint64_t index) {
        GORGONZOLA_C_API_BEGIN

    gorgonzola_expression result = {nullptr};
    if (!expr._expression) return result;
    auto e = static_cast<ParsedExpression*>(expr._expression);
    if (index >= e->getNumChildren()) return result;
    result._expression = e->getChild(index);
    return result;

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return {0};
    }
}

// ============================================================================
// Memory management
// ============================================================================

