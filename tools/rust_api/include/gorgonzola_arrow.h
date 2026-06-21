#pragma once

#include "rust/cxx.h"
#ifdef GORGONZOLA_BUNDLED
#include "main/gorgonzola.h"
#else
#include <gorgonzola.hpp>
#endif

namespace gorgonzola_arrow {

ArrowSchema query_result_get_arrow_schema(const gorgonzola::main::QueryResult& result);
ArrowArray query_result_get_next_arrow_chunk(gorgonzola::main::QueryResult& result, uint64_t chunkSize);

} // namespace gorgonzola_arrow
