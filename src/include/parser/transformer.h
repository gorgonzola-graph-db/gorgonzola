#pragma once

#include <memory>
#include <vector>

#include "common/enums/conflict_action.h"
#include "statement.h"

namespace antlr4 {
namespace tree {
class TerminalNode;
} // namespace tree
} // namespace antlr4

class CypherParser;

namespace gorgonzola {
namespace extension {
class TransformerExtension;
}
namespace parser {

// Slim public interface for the Cypher AST transformer.
// Include "parser/transformer_impl.h" for the full method set
// (needed only inside src/parser/ translation units).
class Transformer {
public:
    Transformer(CypherParser::Ku_StatementsContext& root,
        std::vector<extension::TransformerExtension*> transformerExtensions);

    std::vector<std::shared_ptr<Statement>> transform();

    void registerTransformExtension(
        std::unique_ptr<extension::TransformerExtension> transformerExtension);

private:
    CypherParser::Ku_StatementsContext& root;
    std::vector<extension::TransformerExtension*> transformerExtensions;
};

} // namespace parser
} // namespace gorgonzola
