#include "parser/transformer_impl.h"
#include "parser/use_database.h"

namespace gorgonzola {
namespace parser {

std::unique_ptr<Statement> Transformer::transformUseDatabase(
    CypherParser::KU_UseDatabaseContext& ctx) {
    auto dbName = transformSchemaName(*ctx.oC_SchemaName());
    return std::make_unique<UseDatabase>(std::move(dbName));
}

} // namespace parser
} // namespace gorgonzola
