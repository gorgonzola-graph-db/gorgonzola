#include "gorgonzola/database.h"
#include "gorgonzola/session.h"
#include "gorgonzola/options.h"
#include "gorgonzola/result.h"
#include <iostream>

using namespace gorgonzola;

int main() {
    std::cout << "Starting Gorgonzola Edge Deployment Profile..." << std::endl;

    // The explicit 24GB memory budget for 4c/4t edge devices
    SystemConfig cfg;
    cfg.bufferPoolSize = 12ULL * 1024 * 1024 * 1024;  // 12 GB explicit cap
    cfg.maxNumThreads = 3;                            // Leave 1 core for OS/embedding client
    cfg.enableCompression = true;                     // Zstd compression enabled
    cfg.maxDBSize = 50ULL * 1024 * 1024 * 1024;       // 50 GB on-disk ceiling
    cfg.autoCheckpoint = true;
    cfg.checkpointThreshold = 8 * 1024 * 1024;        // 8 MB (down from 16 MB default)
    cfg.forceCheckpointOnClose = true;

    // Path must be updated for real deployments (e.g. /var/lib/gorgonzola)
    const std::string db_path = "./gorgonzola_edge_db";
    std::cout << "Opening database at: " << db_path << std::endl;

    Database db(db_path, cfg);
    Session session(db);

    // Test a basic query
    auto result = session.execute("RETURN 'Edge deployment initialized successfully' AS status");
    std::cout << result.toString() << std::endl;

    return 0;
}
