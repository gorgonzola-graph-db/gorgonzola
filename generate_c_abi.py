import os

statements = [
    "QUERY", "CREATE_TABLE", "DROP", "ALTER", "COPY_TO", "COPY_FROM",
    "STANDALONE_CALL", "STANDALONE_CALL_FUNCTION", "EXPLAIN", "CREATE_MACRO",
    "TRANSACTION", "EXTENSION", "ATTACH_DATABASE", "DETACH_DATABASE", "USE_DATABASE"
]

header_path = "src/include/c_api/parser_flat_abi.h"
cpp_path = "src/parser/c_ast_translator.cpp"
h_path = "src/parser/c_ast_translator.h"

with open(header_path, "w") as f:
    f.write("#pragma once\n\n#include <stdbool.h>\n#include <stdint.h>\n\n#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n")
    f.write("typedef struct gorgonzola_flat_expression gorgonzola_flat_expression;\n")
    
    for stmt in statements:
        name = stmt.lower()
        f.write(f"typedef struct gorgonzola_flat_{name} gorgonzola_flat_{name};\n")
    
    f.write("\nstruct gorgonzola_flat_statement {\n    uint8_t type;\n    union {\n")
    for stmt in statements:
        name = stmt.lower()
        f.write(f"        gorgonzola_flat_{name}* {name};\n")
    f.write("    } stmt;\n};\n\n")
    
    for stmt in statements:
        name = stmt.lower()
        f.write(f"struct gorgonzola_flat_{name} {{\n    // Fields for {stmt}\n    int dummy;\n}};\n\n")

    f.write("#ifdef __cplusplus\n}\n#endif\n")

with open(h_path, "w") as f:
    f.write("#pragma once\n\n#include <memory>\n#include <vector>\n#include \"c_api/parser_flat_abi.h\"\n#include \"parser/statement.h\"\n\n")
    f.write("namespace gorgonzola {\nnamespace parser {\n\nclass CASTTranslator {\npublic:\n")
    f.write("    static std::vector<std::shared_ptr<Statement>> translateStatement(const gorgonzola_flat_statement* c_stmt);\n\nprivate:\n")
    
    for stmt in statements:
        name = stmt.lower()
        f.write(f"    static std::shared_ptr<Statement> translate_{name}(const gorgonzola_flat_{name}* c_stmt);\n")
    f.write("};\n\n}\n}\n")

with open(cpp_path, "w") as f:
    f.write("#include \"parser/c_ast_translator.h\"\n#include <stdexcept>\n\n")
    f.write("namespace gorgonzola {\nnamespace parser {\n\n")
    f.write("std::vector<std::shared_ptr<Statement>> CASTTranslator::translateStatement(const gorgonzola_flat_statement* c_stmt) {\n")
    f.write("    std::vector<std::shared_ptr<Statement>> result;\n    if (!c_stmt) return result;\n\n")
    f.write("    switch (c_stmt->type) {\n")
    for i, stmt in enumerate(statements):
        name = stmt.lower()
        f.write(f"        case {i}: // {stmt}\n")
        f.write(f"            result.push_back(translate_{name}(c_stmt->stmt.{name}));\n")
        f.write("            break;\n")
    f.write("        default:\n            throw std::runtime_error(\"Unsupported flat statement type in translator\");\n")
    f.write("    }\n    return result;\n}\n\n")
    
    for stmt in statements:
        name = stmt.lower()
        f.write(f"std::shared_ptr<Statement> CASTTranslator::translate_{name}(const gorgonzola_flat_{name}* c_stmt) {{\n")
        f.write(f"    // Translation logic for {stmt}\n")
        f.write("    return nullptr;\n}\n\n")
        
    f.write("}\n}\n")

print("Generated AST Translation Boilerplate")
