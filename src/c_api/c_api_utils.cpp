#include "c_api_utils.h"
#include "common/exception/exception.h"

namespace gorgonzola {
namespace c_api {

void translate_exception() {
    try {
        throw;
    } catch (const gorgonzola::common::Exception& e) {
        fprintf(stderr, "translate_exception caught Exception: %s\n", e.what());
    } catch (const std::exception& e) {
        fprintf(stderr, "translate_exception caught std::exception: %s\n", e.what());
    } catch (...) {
        fprintf(stderr, "translate_exception caught unknown exception\n");
    }
}

HandleRegistry& HandleRegistry::getInstance() {
    static HandleRegistry instance;
    return instance;
}

} // namespace c_api
} // namespace gorgonzola
