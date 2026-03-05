#include "c_api/parser_abi.h"

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
#include "parser/query/regular_query.h"
#include "parser/query/single_query.h"
#include "parser/query/query_part.h"
#include "parser/query/reading_clause/reading_clause.h"
#include "parser/query/updating_clause/updating_clause.h"
#include "parser/query/return_with_clause/return_clause.h"
#include "parser/query/return_with_clause/with_clause.h"

using namespace gorgonzola;
using namespace gorgonzola::parser;

// --- Static asserts to guarantee ABI compatibility ---
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

// --- Wrapper structs for lifetime management ---
struct ParsedResultWrapper {
    std::vector<std::shared_ptr<Statement>> statements;
};

// --- C API Implementation ---

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
    if (!result) return;
    if (result->_parsed_result) {
        delete static_cast<ParsedResultWrapper*>(result->_parsed_result);
        result->_parsed_result = nullptr;
    }
    if (result->_error_message) {
        free(result->_error_message);
        result->_error_message = nullptr;
    }
}

uint64_t gorgonzola_parsed_result_num_statements(gorgonzola_parsed_result result) {
    if (!result._parsed_result) return 0;
    auto wrapper = static_cast<ParsedResultWrapper*>(result._parsed_result);
    return wrapper->statements.size();
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

gorgonzola_statement_type gorgonzola_statement_get_type(gorgonzola_statement stmt) {
    if (!stmt._statement) return (gorgonzola_statement_type)255;
    auto statement = static_cast<Statement*>(stmt._statement);
    return static_cast<gorgonzola_statement_type>(statement->getStatementType());
}

bool gorgonzola_statement_is_internal(gorgonzola_statement stmt) {
    if (!stmt._statement) return false;
    auto statement = static_cast<Statement*>(stmt._statement);
    return statement->isInternal();
}

void gorgonzola_destroy_string(char* str) {
    if (str) {
        free(str);
    }
}

// --- DDL Statement accessors ---

char* gorgonzola_create_table_get_name(gorgonzola_statement stmt) {
    if (!stmt._statement) return nullptr;
    auto createTable = static_cast<CreateTable*>(stmt._statement);
    return strdup(createTable->getInfo()->tableName.c_str());
}

uint64_t gorgonzola_create_table_get_num_properties(gorgonzola_statement stmt) {
    if (!stmt._statement) return 0;
    auto createTable = static_cast<CreateTable*>(stmt._statement);
    return createTable->getInfo()->propertyDefinitions.size();
}

char* gorgonzola_create_table_get_property_name(gorgonzola_statement stmt, uint64_t index) {
    if (!stmt._statement) return nullptr;
    auto createTable = static_cast<CreateTable*>(stmt._statement);
    if (index >= createTable->getInfo()->propertyDefinitions.size()) return nullptr;
    return strdup(createTable->getInfo()->propertyDefinitions[index].getName().c_str());
}

char* gorgonzola_create_table_get_property_type(gorgonzola_statement stmt, uint64_t index) {
    if (!stmt._statement) return nullptr;
    auto createTable = static_cast<CreateTable*>(stmt._statement);
    if (index >= createTable->getInfo()->propertyDefinitions.size()) return nullptr;
    return strdup(createTable->getInfo()->propertyDefinitions[index].getType().c_str());
}

char* gorgonzola_drop_get_name(gorgonzola_statement stmt) {
    if (!stmt._statement) return nullptr;
    auto drop = static_cast<Drop*>(stmt._statement);
    return strdup(drop->getDropInfo().name.c_str());
}

char* gorgonzola_alter_get_table_name(gorgonzola_statement stmt) {
    if (!stmt._statement) return nullptr;
    auto alter = static_cast<Alter*>(stmt._statement);
    return strdup(alter->getInfo()->tableName.c_str());
}

// --- Copy Statement accessors ---

char* gorgonzola_copy_from_get_table_name(gorgonzola_statement stmt) {
    if (!stmt._statement) return nullptr;
    auto copyFrom = static_cast<CopyFrom*>(stmt._statement);
    return strdup(copyFrom->getTableName().c_str());
}

char* gorgonzola_copy_to_get_file_path(gorgonzola_statement stmt) {
    if (!stmt._statement) return nullptr;
    auto copyTo = static_cast<CopyTo*>(stmt._statement);
    return strdup(copyTo->getFilePath().c_str());
}

// --- Database Statement accessors ---

char* gorgonzola_attach_database_get_db_path(gorgonzola_statement stmt) {
    if (!stmt._statement) return nullptr;
    auto attach = static_cast<AttachDatabase*>(stmt._statement);
    return strdup(attach->getAttachInfo().dbPath.c_str());
}

char* gorgonzola_attach_database_get_db_alias(gorgonzola_statement stmt) {
    if (!stmt._statement) return nullptr;
    auto attach = static_cast<AttachDatabase*>(stmt._statement);
    return strdup(attach->getAttachInfo().dbAlias.c_str());
}

char* gorgonzola_detach_database_get_db_name(gorgonzola_statement stmt) {
    if (!stmt._statement) return nullptr;
    auto detach = static_cast<DetachDatabase*>(stmt._statement);
    return strdup(detach->getDBName().c_str());
}

char* gorgonzola_use_database_get_db_name(gorgonzola_statement stmt) {
    if (!stmt._statement) return nullptr;
    auto use = static_cast<UseDatabase*>(stmt._statement);
    return strdup(use->getDBName().c_str());
}

// --- Query Statement accessors ---

uint64_t gorgonzola_query_get_num_single_queries(gorgonzola_statement stmt) {
    if (!stmt._statement) return 0;
    auto query = static_cast<RegularQuery*>(stmt._statement);
    return query->getNumSingleQueries();
}

gorgonzola_single_query gorgonzola_query_get_single_query(gorgonzola_statement stmt, uint64_t index) {
    gorgonzola_single_query result = {nullptr};
    if (!stmt._statement) return result;
    auto query = static_cast<RegularQuery*>(stmt._statement);
    if (index >= query->getNumSingleQueries()) return result;
    // Cast away constness since C API handles don't track const.
    // The C API user is expected not to modify through handles anyway.
    result._single_query = const_cast<SingleQuery*>(query->getSingleQuery(index));
    return result;
}

bool gorgonzola_query_is_union_all(gorgonzola_statement stmt, uint64_t index) {
    if (!stmt._statement) return false;
    auto query = static_cast<RegularQuery*>(stmt._statement);
    auto flags = query->getIsUnionAll();
    if (index >= flags.size()) return false;
    return flags[index];
}

// --- Single Query accessors ---

uint64_t gorgonzola_single_query_get_num_query_parts(gorgonzola_single_query sq) {
    if (!sq._single_query) return 0;
    auto singleQuery = static_cast<SingleQuery*>(sq._single_query);
    return singleQuery->getNumQueryParts();
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
    if (!sq._single_query) return 0;
    auto singleQuery = static_cast<SingleQuery*>(sq._single_query);
    return singleQuery->getNumReadingClauses();
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
    if (!sq._single_query) return 0;
    auto singleQuery = static_cast<SingleQuery*>(sq._single_query);
    return singleQuery->getNumUpdatingClauses();
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
    if (!sq._single_query) return false;
    auto singleQuery = static_cast<SingleQuery*>(sq._single_query);
    return singleQuery->hasReturnClause();
}

gorgonzola_return_clause gorgonzola_single_query_get_return_clause(gorgonzola_single_query sq) {
    gorgonzola_return_clause result = {nullptr};
    if (!sq._single_query) return result;
    auto singleQuery = static_cast<SingleQuery*>(sq._single_query);
    if (!singleQuery->hasReturnClause()) return result;
    result._return_clause = const_cast<ReturnClause*>(singleQuery->getReturnClause());
    return result;
}

// --- Query Part accessors ---

uint64_t gorgonzola_query_part_get_num_reading_clauses(gorgonzola_query_part qp) {
    if (!qp._query_part) return 0;
    auto queryPart = static_cast<QueryPart*>(qp._query_part);
    return queryPart->getNumReadingClauses();
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
    if (!qp._query_part) return 0;
    auto queryPart = static_cast<QueryPart*>(qp._query_part);
    return queryPart->getNumUpdatingClauses();
}

gorgonzola_updating_clause gorgonzola_query_part_get_updating_clause(gorgonzola_query_part qp, uint64_t index) {
    gorgonzola_updating_clause result = {nullptr};
    if (!qp._query_part) return result;
    auto queryPart = static_cast<QueryPart*>(qp._query_part);
    if (index >= queryPart->getNumUpdatingClauses()) return result;
    result._updating_clause = queryPart->getUpdatingClause(index);
    return result;
}

bool gorgonzola_query_part_has_with_clause(gorgonzola_query_part qp) {
    if (!qp._query_part) return false;
    auto queryPart = static_cast<QueryPart*>(qp._query_part);
    return queryPart->hasWithClause();
}

gorgonzola_with_clause gorgonzola_query_part_get_with_clause(gorgonzola_query_part qp) {
    gorgonzola_with_clause result = {nullptr};
    if (!qp._query_part) return result;
    auto queryPart = static_cast<QueryPart*>(qp._query_part);
    if (!queryPart->hasWithClause()) return result;
    result._with_clause = const_cast<WithClause*>(queryPart->getWithClause());
    return result;
}

