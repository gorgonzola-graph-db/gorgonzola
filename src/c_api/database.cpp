#include "c_api/gorgonzola.h"
#include "common/exception/exception.h"
#include "main/gorgonzola.h"
using namespace gorgonzola::main;
using namespace gorgonzola::common;

#include "c_api_utils.h"

gorgonzola_state gorgonzola_database_init(const char* database_path,
    gorgonzola_system_config config, gorgonzola_database* out_database) {
    if (!out_database) {
        return GorgonzolaError;
}
    GORGONZOLA_C_API_BEGIN
    std::string database_path_str = database_path;
    auto systemConfig = SystemConfig(config.buffer_pool_size, config.max_num_threads,
        config.enable_compression, config.read_only, config.max_db_size, config.auto_checkpoint,
        config.checkpoint_threshold);

#if defined(__APPLE__)
    systemConfig.threadQos = config.thread_qos;
#endif
    out_database->_database = new Database(database_path_str, systemConfig);
    gorgonzola::c_api::HandleRegistry::getInstance().registerHandle(out_database->_database,
        gorgonzola::c_api::HandleType::Database);
    return GorgonzolaSuccess;
    GORGONZOLA_C_API_END(GorgonzolaError)
}

void gorgonzola_database_destroy(gorgonzola_database* database) {
    if (database == nullptr || database->_database == nullptr) {
        return;
    }
    GORGONZOLA_C_API_BEGIN
    VALIDATE_HANDLE_VOID(database->_database, Database)
    gorgonzola::c_api::HandleRegistry::getInstance().unregisterHandle(database->_database);
    delete static_cast<Database*>(database->_database);
    database->_database = nullptr;
    GORGONZOLA_C_API_END_VOID
}

gorgonzola_system_config gorgonzola_default_system_config() {
    SystemConfig config = SystemConfig();
    auto cSystemConfig = gorgonzola_system_config();
    GORGONZOLA_C_API_BEGIN
    cSystemConfig.buffer_pool_size = config.bufferPoolSize;
    cSystemConfig.max_num_threads = config.maxNumThreads;
    cSystemConfig.enable_compression = config.enableCompression;
    cSystemConfig.read_only = config.readOnly;
    cSystemConfig.max_db_size = config.maxDBSize;
    cSystemConfig.auto_checkpoint = config.autoCheckpoint;
    cSystemConfig.checkpoint_threshold = config.checkpointThreshold;
#if defined(__APPLE__)
    cSystemConfig.thread_qos = config.threadQos;
#endif
    GORGONZOLA_C_API_END(cSystemConfig)
    return cSystemConfig;
}
