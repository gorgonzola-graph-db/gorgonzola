#include "gorgonzola/database.h"
#include "api_impl.h"
#include "main/client_context.h"
#include "transaction/transaction_manager.h"

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

Status Database::flush() {
    if (!impl_) {
        return Status::Error("Database is already closed");
    }
    
    auto* internal_db = impl_->getInternalDatabase();
    if (internal_db->getConfig().readOnly) {
        return Status::Success(); // Nothing to flush if read-only
    }

    try {
        main::ClientContext clientContext(internal_db);
        internal_db->getTransactionManager()->checkpoint(clientContext);
        return Status::Success();
    } catch (const std::exception& e) {
        return Status::Error(e.what());
    }
}

void Database::close() {
    // If impl_ is non-null, destroying it will trigger main::Database destructor,
    // which automatically joins threads and cleans up.
    if (impl_) {
        // We can do an explicit flush here if needed, but main::Database::~Database 
        // already calls checkpoint() if forceCheckpointOnClose is true. 
        // We just let it run.
        impl_.reset();
    }
}

} // namespace gorgonzola


