#include <memory>

#include "printer/printer.h"

namespace gorgonzola {
namespace main {

class PrinterFactory {
public:
    static std::unique_ptr<Printer> getPrinter(PrinterType type);
};

} // namespace main
} // namespace gorgonzola
