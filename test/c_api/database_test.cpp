#include "c_api/gorgonzola.h"
#include "graph_test/base_graph_test.h"
#include "gtest/gtest.h"

using namespace gorgonzola::main;
using namespace gorgonzola::testing;

// This class starts database without initializing graph.
class APIEmptyDBTest : public BaseGraphTest {
    std::string getInputDir() override { KU_UNREACHABLE; }
};

class CApiDatabaseTest : public APIEmptyDBTest {
public:
    void SetUp() override {
        APIEmptyDBTest::SetUp();
        defaultSystemConfig = gorgonzola_default_system_config();

        // limit memory usage by keeping max number of threads small
        defaultSystemConfig.max_num_threads = 2;
        auto maxDBSizeEnv = TestHelper::getSystemEnv("MAX_DB_SIZE");
        if (!maxDBSizeEnv.empty()) {
            defaultSystemConfig.max_db_size = std::stoull(maxDBSizeEnv);
        }
    }

    gorgonzola_system_config defaultSystemConfig;
};

TEST_F(CApiDatabaseTest, CreationAndDestroy) {
    gorgonzola_database database;
    gorgonzola_state state;
    auto databasePathCStr = databasePath.c_str();
    auto systemConfig = defaultSystemConfig;
    state = gorgonzola_database_init(databasePathCStr, systemConfig, &database);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(database._database, nullptr);
    auto databaseCpp = static_cast<Database*>(database._database);
    ASSERT_NE(databaseCpp, nullptr);
    gorgonzola_database_destroy(&database);
}

TEST_F(CApiDatabaseTest, CreationReadOnly) {
    gorgonzola_database database;
    gorgonzola_connection connection;
    gorgonzola_query_result queryResult;
    gorgonzola_state state;
    auto databasePathCStr = databasePath.c_str();
    auto systemConfig = defaultSystemConfig;
    // First, create a read-write database.
    state = gorgonzola_database_init(databasePathCStr, systemConfig, &database);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(database._database, nullptr);
    auto databaseCpp = static_cast<Database*>(database._database);
    ASSERT_NE(databaseCpp, nullptr);
    gorgonzola_database_destroy(&database);
    // Now, access the same database read-only.
    systemConfig.read_only = true;
    state = gorgonzola_database_init(databasePathCStr, systemConfig, &database);
    if (databasePath == "" || databasePath == ":memory:") {
        ASSERT_EQ(state, GorgonzolaError);
        ASSERT_EQ(database._database, nullptr);
        return;
    }
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(database._database, nullptr);
    databaseCpp = static_cast<Database*>(database._database);
    ASSERT_NE(databaseCpp, nullptr);
    // Try to write to the database.
    state = gorgonzola_connection_init(&database, &connection);
    ASSERT_EQ(state, GorgonzolaSuccess);
    state = gorgonzola_connection_query(&connection,
        "CREATE NODE TABLE User(name STRING, age INT64, reg_date DATE, PRIMARY KEY (name))",
        &queryResult);
    ASSERT_EQ(state, GorgonzolaError);
    ASSERT_FALSE(gorgonzola_query_result_is_success(&queryResult));
    gorgonzola_query_result_destroy(&queryResult);
    gorgonzola_connection_destroy(&connection);
    gorgonzola_database_destroy(&database);
}

TEST_F(CApiDatabaseTest, CreationInMemory) {
    gorgonzola_database database;
    gorgonzola_state state;
    auto databasePathCStr = (char*)"";
    state = gorgonzola_database_init(databasePathCStr, defaultSystemConfig, &database);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_database_destroy(&database);
    databasePathCStr = (char*)":memory:";
    state = gorgonzola_database_init(databasePathCStr, defaultSystemConfig, &database);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_database_destroy(&database);
}

#ifndef __WASM__ // home directory is not available in WASM
TEST_F(CApiDatabaseTest, CreationHomeDir) {
    gorgonzola_database database;
    gorgonzola_connection connection;
    gorgonzola_state state;
    auto databasePathCStr = (char*)"~/ku_test.db";
    state = gorgonzola_database_init(databasePathCStr, defaultSystemConfig, &database);
    ASSERT_EQ(state, GorgonzolaSuccess);
    state = gorgonzola_connection_init(&database, &connection);
    ASSERT_EQ(state, GorgonzolaSuccess);
    auto homePath =
        getClientContext(*(Connection*)(connection._connection))->getClientConfig()->homeDirectory;
    gorgonzola_connection_destroy(&connection);
    gorgonzola_database_destroy(&database);
    std::filesystem::remove_all(homePath + "/ku_test.db");
}
#endif

TEST_F(CApiDatabaseTest, CloseQueryResultAndConnectionAfterDatabaseDestroy) {
    gorgonzola_database database;
    auto databasePathCStr = (char*)":memory:";
    auto systemConfig = gorgonzola_default_system_config();
    systemConfig.buffer_pool_size = 10 * 1024 * 1024; // 10MB
    systemConfig.max_db_size = 1 << 30;               // 1GB
    systemConfig.max_num_threads = 2;
    gorgonzola_state state = gorgonzola_database_init(databasePathCStr, systemConfig, &database);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(database._database, nullptr);
    gorgonzola_connection conn;
    gorgonzola_query_result queryResult;
    state = gorgonzola_connection_init(&database, &conn);
    ASSERT_EQ(state, GorgonzolaSuccess);
    state = gorgonzola_connection_query(&conn, "RETURN 1+1", &queryResult);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&queryResult));
    gorgonzola_flat_tuple tuple;
    gorgonzola_state resultState = gorgonzola_query_result_get_next(&queryResult, &tuple);
    ASSERT_EQ(resultState, GorgonzolaSuccess);
    gorgonzola_value value;
    gorgonzola_state valueState = gorgonzola_flat_tuple_get_value(&tuple, 0, &value);
    ASSERT_EQ(valueState, GorgonzolaSuccess);
    int64_t valueInt = INT64_MAX;
    gorgonzola_state valueIntState = gorgonzola_value_get_int64(&value, &valueInt);
    ASSERT_EQ(valueIntState, GorgonzolaSuccess);
    ASSERT_EQ(valueInt, 2);
    // Destroy database first, this should not crash
    gorgonzola_database_destroy(&database);
    // Call gorgonzola_connection_query should not crash, but return an error
    state = gorgonzola_connection_query(&conn, "RETURN 1+1", &queryResult);
    ASSERT_EQ(state, GorgonzolaError);
    // Call gorgonzola_query_result_get_next should not crash, but return an error
    resultState = gorgonzola_query_result_get_next(&queryResult, &tuple);
    ASSERT_EQ(resultState, GorgonzolaError);
    // Now destroy everything, this should not crash
    gorgonzola_query_result_destroy(&queryResult);
    gorgonzola_connection_destroy(&conn);
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&tuple);
}

TEST_F(CApiDatabaseTest, UseConnectionAfterDatabaseDestroy) {
    gorgonzola_database db;
    gorgonzola_connection conn;
    gorgonzola_query_result result;

    auto systemConfig = gorgonzola_default_system_config();
    systemConfig.buffer_pool_size = 10 * 1024 * 1024; // 10MB
    systemConfig.max_db_size = 1 << 30;               // 1GB
    systemConfig.max_num_threads = 2;
    auto state = gorgonzola_database_init("", systemConfig, &db);
    ASSERT_EQ(state, GorgonzolaSuccess);
    state = gorgonzola_connection_init(&db, &conn);
    ASSERT_EQ(state, GorgonzolaSuccess);
    gorgonzola_database_destroy(&db);
    state = gorgonzola_connection_query(&conn, "RETURN 0", &result);
    ASSERT_EQ(state, GorgonzolaError);

    gorgonzola_connection_destroy(&conn);
}
