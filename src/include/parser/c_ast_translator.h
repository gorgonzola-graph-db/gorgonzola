#pragma once

#include <memory>
#include <vector>

#include "c_api/parser_flat_abi.h"
#include "parser/statement.h"

namespace gorgonzola {
namespace parser {

class CASTTranslator {
public:
    static std::vector<std::shared_ptr<Statement>> translateStatement(
        const gorgonzola_flat_statement* c_stmt);

private:
    static std::shared_ptr<Statement> translate_query(const gorgonzola_flat_query* c_stmt);
    static std::shared_ptr<Statement> translate_create_table(
        const gorgonzola_flat_create_table* c_stmt);
    static std::shared_ptr<Statement> translate_drop(const gorgonzola_flat_drop* c_stmt);
    static std::shared_ptr<Statement> translate_alter(const gorgonzola_flat_alter* c_stmt);
    static std::shared_ptr<Statement> translate_copy_to(const gorgonzola_flat_copy_to* c_stmt);
    static std::shared_ptr<Statement> translate_copy_from(const gorgonzola_flat_copy_from* c_stmt);
    static std::shared_ptr<Statement> translate_standalone_call(
        const gorgonzola_flat_standalone_call* c_stmt);
    static std::shared_ptr<Statement> translate_standalone_call_function(
        const gorgonzola_flat_standalone_call_function* c_stmt);
    static std::shared_ptr<Statement> translate_explain(const gorgonzola_flat_explain* c_stmt);
    static std::shared_ptr<Statement> translate_create_macro(
        const gorgonzola_flat_create_macro* c_stmt);
    static std::shared_ptr<Statement> translate_transaction(
        const gorgonzola_flat_transaction* c_stmt);
    static std::shared_ptr<Statement> translate_extension(const gorgonzola_flat_extension* c_stmt);
    static std::shared_ptr<Statement> translate_attach_database(
        const gorgonzola_flat_attach_database* c_stmt);
    static std::shared_ptr<Statement> translate_detach_database(
        const gorgonzola_flat_detach_database* c_stmt);
    static std::shared_ptr<Statement> translate_use_database(
        const gorgonzola_flat_use_database* c_stmt);
};

} // namespace parser
} // namespace gorgonzola
