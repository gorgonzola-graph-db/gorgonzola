#include <algorithm>
#include "binder/binder.h"
#include "binder/bound_create_macro.h"
#include "catalog/catalog.h"
#include "common/exception/binder.h"
#include "common/string_format.h"
#include "common/string_utils.h"
#include "parser/create_macro.h"
#include "transaction/transaction.h"

using namespace gorgonzola::common;
using namespace gorgonzola::parser;

namespace gorgonzola {
namespace binder {

std::unique_ptr<BoundStatement> Binder::bindCreateMacro(const Statement& statement) const {
    auto& createMacro = ku_dynamic_cast<const CreateMacro&>(statement);
    auto macroName = createMacro.getMacroName();
    StringUtils::toUpper(macroName);
    if (catalog::Catalog::Get(*clientContext)
            ->containsMacro(transaction::Transaction::Get(*clientContext), macroName)) {
        throw BinderException{stringFormat("Macro {} already exists.", macroName)};
    }
    parser::default_macro_args defaultArgs;
    defaultArgs.reserve(createMacro.getDefaultArgs().size());
    std::transform(createMacro.getDefaultArgs().begin(), createMacro.getDefaultArgs().end(), std::back_inserter(defaultArgs),
        [](const auto& defaultArg) { return std::make_pair(defaultArg.first, defaultArg.second->copy()); });
    auto scalarMacro =
        std::make_unique<function::ScalarMacroFunction>(createMacro.getMacroExpression()->copy(),
            createMacro.getPositionalArgs(), std::move(defaultArgs));
    return std::make_unique<BoundCreateMacro>(std::move(macroName), std::move(scalarMacro));
}

} // namespace binder
} // namespace gorgonzola
