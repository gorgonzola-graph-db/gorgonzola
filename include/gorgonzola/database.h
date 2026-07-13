#pragma once

#include <memory>
#include <string_view>
#include "gorgonzola/export.h"
#include "gorgonzola/options.h"
#include "gorgonzola/status.h"

namespace gorgonzola {

class DatabaseImpl;

/**
 * @brief Database is the root object that manages engine lifecycle and storage.
 * It is completely thread-safe and can be shared across multiple sessions.
 */
class GORGONZOLA_API Database {
public:
    /**
     * @brief Opens a database at the specified path.
     * @param path File path or ":memory:" for in-memory database.
     * @param config System configuration options.
     */
    explicit Database(std::string_view path, const SystemConfig& config = SystemConfig());
    
    ~Database();

    // Prevent copying
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    // Allow moving
    Database(Database&&) noexcept;
    Database& operator=(Database&&) noexcept;

    /**
     * @brief Forces a checkpoint/flush of the database to disk.
     * @return Status indicating success or failure of the flush operation.
     */
    Status flush();

    /**
     * @brief Explicitly closes the database, releasing all resources.
     * This is useful to capture exceptions, since the destructor cannot throw.
     * Multiple calls to close() are safe and will be ignored.
     */
    void close();

    /**
     * @brief Internal implementation accessor. 
     * DO NOT use directly in embedding applications.
     */
    DatabaseImpl* getImpl() const { return impl_.get(); }

private:
    std::unique_ptr<DatabaseImpl> impl_;
};

} // namespace gorgonzola
