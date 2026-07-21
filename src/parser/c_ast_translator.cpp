#include "parser/c_ast_translator.h"
#include <stdexcept>

namespace gorgonzola {
namespace parser {

std::vector<std::shared_ptr<Statement>> CASTTranslator::translateStatement(const gorgonzola_flat_statement* c_stmt) {
    std::vector<std::shared_ptr<Statement>> result;
    if (!c_stmt) return result;

    switch (c_stmt->type) {
        case 0: // QUERY
            result.push_back(translate_query(c_stmt->stmt.query));
            break;
        case 1: // CREATE_TABLE
            result.push_back(translate_create_table(c_stmt->stmt.create_table));
            break;
        case 2: // DROP
            result.push_back(translate_drop(c_stmt->stmt.drop));
            break;
        case 3: // ALTER
            result.push_back(translate_alter(c_stmt->stmt.alter));
            break;
        case 4: // COPY_TO
            result.push_back(translate_copy_to(c_stmt->stmt.copy_to));
            break;
        case 5: // COPY_FROM
            result.push_back(translate_copy_from(c_stmt->stmt.copy_from));
            break;
        case 6: // STANDALONE_CALL
            result.push_back(translate_standalone_call(c_stmt->stmt.standalone_call));
            break;
        case 7: // STANDALONE_CALL_FUNCTION
            result.push_back(translate_standalone_call_function(c_stmt->stmt.standalone_call_function));
            break;
        case 8: // EXPLAIN
            result.push_back(translate_explain(c_stmt->stmt.explain));
            break;
        case 9: // CREATE_MACRO
            result.push_back(translate_create_macro(c_stmt->stmt.create_macro));
            break;
        case 10: // TRANSACTION
            result.push_back(translate_transaction(c_stmt->stmt.transaction));
            break;
        case 11: // EXTENSION
            result.push_back(translate_extension(c_stmt->stmt.extension));
            break;
        case 12: // ATTACH_DATABASE
            result.push_back(translate_attach_database(c_stmt->stmt.attach_database));
            break;
        case 13: // DETACH_DATABASE
            result.push_back(translate_detach_database(c_stmt->stmt.detach_database));
            break;
        case 14: // USE_DATABASE
            result.push_back(translate_use_database(c_stmt->stmt.use_database));
            break;
        default:
            throw std::runtime_error("Unsupported flat statement type in translator");
    }
    return result;
}

std::shared_ptr<Statement> CASTTranslator::translate_query(const gorgonzola_flat_query* c_stmt) {
    // Translation logic for QUERY
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_create_table(const gorgonzola_flat_create_table* c_stmt) {
    // Translation logic for CREATE_TABLE
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_drop(const gorgonzola_flat_drop* c_stmt) {
    // Translation logic for DROP
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_alter(const gorgonzola_flat_alter* c_stmt) {
    // Translation logic for ALTER
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_copy_to(const gorgonzola_flat_copy_to* c_stmt) {
    // Translation logic for COPY_TO
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_copy_from(const gorgonzola_flat_copy_from* c_stmt) {
    // Translation logic for COPY_FROM
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_standalone_call(const gorgonzola_flat_standalone_call* c_stmt) {
    // Translation logic for STANDALONE_CALL
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_standalone_call_function(const gorgonzola_flat_standalone_call_function* c_stmt) {
    // Translation logic for STANDALONE_CALL_FUNCTION
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_explain(const gorgonzola_flat_explain* c_stmt) {
    // Translation logic for EXPLAIN
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_create_macro(const gorgonzola_flat_create_macro* c_stmt) {
    // Translation logic for CREATE_MACRO
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_transaction(const gorgonzola_flat_transaction* c_stmt) {
    // Translation logic for TRANSACTION
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_extension(const gorgonzola_flat_extension* c_stmt) {
    // Translation logic for EXTENSION
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_attach_database(const gorgonzola_flat_attach_database* c_stmt) {
    // Translation logic for ATTACH_DATABASE
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_detach_database(const gorgonzola_flat_detach_database* c_stmt) {
    // Translation logic for DETACH_DATABASE
    return nullptr;
}

std::shared_ptr<Statement> CASTTranslator::translate_use_database(const gorgonzola_flat_use_database* c_stmt) {
    // Translation logic for USE_DATABASE
    return nullptr;
}

}
}
