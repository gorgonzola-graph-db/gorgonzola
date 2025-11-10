#include "common/exception/exception.h"

#ifdef GORGONZOLA_BACKTRACE
#include <cpptrace/cpptrace.hpp>
#endif

namespace gorgonzola {
namespace common {

Exception::Exception(std::string msg) : exception(), exception_message_(std::move(msg)) {
#ifdef GORGONZOLA_BACKTRACE
    cpptrace::generate_trace(1 /*skip this function's frame*/).print();
#endif
}

} // namespace common
} // namespace gorgonzola
