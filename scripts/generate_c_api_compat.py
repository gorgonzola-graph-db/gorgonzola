import re
from pathlib import Path

# Paths
c_api_dir = Path("/run/media/lechibang/cb09d199-3769-4ec8-9af5-954929515428/projects/gorgonzola/src/include/c_api")
gorgonzola_h = c_api_dir / "gorgonzola.h"
kuzu_h = c_api_dir / "kuzu.h"

content = gorgonzola_h.read_text()

# Find all unique tokens starting with gorgonzola_ or Gorgonzola
tokens = set(re.findall(r'\b(?:gorgonzola_[a-zA-Z0-9_]+|Gorgonzola[a-zA-Z0-9_]*)\b', content))

macros = []
for tok in sorted(tokens):
    if tok.startswith("gorgonzola_"):
        kuzu_tok = tok.replace("gorgonzola_", "kuzu_", 1)
        macros.append(f"#define {kuzu_tok} {tok}")
    elif tok.startswith("Gorgonzola"):
        kuzu_tok = tok.replace("Gorgonzola", "Kuzu", 1)
        macros.append(f"#define {kuzu_tok} {tok}")

# Add manual mappings if any
macros.append("#define GORGONZOLA_API KUZU_API")

output = f"""#pragma once
#include "c_api/gorgonzola.h"

/* Gorgonzola C API compatibility mappings to Kuzu */
""" + "\n".join(macros) + "\n"

kuzu_h.write_text(output)
print(f"Generated {len(macros)} compatibility macros in {kuzu_h}")
