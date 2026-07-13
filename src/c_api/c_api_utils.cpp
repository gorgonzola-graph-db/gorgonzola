#include "c_api_utils.h"
#include "common/exception/exception.h"

namespace gorgonzola {
namespace c_api {

void translate_exception() {
    try {
        throw;
    } catch (const gorgonzola::common::Exception& /*e*/) {
        // Can optionally log here in the future
    } catch (const std::exception& /*e*/) {
        // Generic C++ exception
    } catch (...) {
        // Unknown exception
    }
}

} // namespace c_api
} // namespace gorgonzola
