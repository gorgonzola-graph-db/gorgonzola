#include "gorgonzola_arrow.h"

namespace gorgonzola_arrow {

ArrowSchema query_result_get_arrow_schema(const gorgonzola::main::QueryResult& result) {
    // Could use directly, except that we can't (yet) mark ArrowSchema as being safe to store in a
    // cxx::UniquePtr
    return *result.getArrowSchema();
}

ArrowArray query_result_get_next_arrow_chunk(gorgonzola::main::QueryResult& result, uint64_t chunkSize) {
    return *result.getNextArrowChunk(chunkSize);
}

} // namespace gorgonzola_arrow
