#include "gorgonzola/database.h"
#include "api_impl.h"

namespace gorgonzola {

Database::Database(std::string_view path, const SystemConfig& config) {
    main::SystemConfig internalConfig(
        config.bufferPoolSize,
        config.maxNumThreads,
        config.enableCompression,
        config.readOnly,
        config.maxDBSize,
        config.autoCheckpoint,
        config.checkpointThreshold,
        config.forceCheckpointOnClose,
        config.throwOnWalReplayFailure,
        config.enableChecksums
    );
    auto db = std::make_unique<main::Database>(path, internalConfig);
    impl_ = std::make_unique<DatabaseImpl>(std::move(db));
}

Database::~Database() = default;

Database::Database(Database&&) noexcept = default;
Database& Database::operator=(Database&&) noexcept = default;

} // namespace gorgonzola


