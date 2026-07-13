#include "gorgonzola/database.h"
#include "api_impl.h"
#include "main/client_context.h"
#include "transaction/transaction_manager.h"
#include "catalog/catalog.h"
#include "catalog/catalog_entry/table_catalog_entry.h"

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
    
    if (impl_->isPoisoned()) {
        return impl_->getLastError();
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
        auto status = Status::FatalIOError("Checkpoint failed. Database is now read-only. Please restart. Error: " + std::string(e.what()));
        impl_->poison(status);
        return status;
    }
}

std::vector<std::string> Database::getTableNames() const {
    if (!impl_ || impl_->isPoisoned()) return {};
    auto* internal_db = impl_->getInternalDatabase();
    
    main::ClientContext context(internal_db);
    auto* txManager = internal_db->getTransactionManager();
    auto* tx = txManager->beginTransaction(context, transaction::TransactionType::READ_ONLY);
    
    auto* catalog = internal_db->getCatalog();
    auto entries = catalog->getTableEntries(tx, false /* useInternal */);
    
    std::vector<std::string> names;
    names.reserve(entries.size());
    for (const auto* entry : entries) {
        names.push_back(entry->getName());
    }
    
    txManager->commit(context, tx);
    return names;
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


