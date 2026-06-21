#include "processor/operator/empty_result.h"

namespace gorgonzola {
namespace processor {

bool EmptyResult::getNextTuplesInternal(ExecutionContext*) {
    return false;
}

} // namespace processor
} // namespace gorgonzola
