#include "common/data_chunk/data_chunk.h"

namespace gorgonzola {
namespace common {

thread_local BumpAllocator tl_alloc(1 << 20); // 1MB per thread

void* DataChunk::operator new(size_t s) {
    return tl_alloc.alloc(s);
}
void DataChunk::operator delete(void* p) {}
void DataChunk::resetBumpAllocator() {
    tl_alloc.reset();
}

void DataChunk::insert(uint32_t pos, std::shared_ptr<ValueVector> valueVector) {
    valueVector->setState(state);
    KU_ASSERT(valueVectors.size() > pos);
    valueVectors[pos] = std::move(valueVector);
}

void DataChunk::resetAuxiliaryBuffer() {
    for (auto& valueVector : valueVectors) {
        valueVector->resetAuxiliaryBuffer();
    }
}

} // namespace common
} // namespace gorgonzola
