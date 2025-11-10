#pragma once

#include "c_api/gorgonzola.h"
#include "graph_test/base_graph_test.h"

namespace gorgonzola {
namespace testing {

// This class starts database in on-disk mode.
class APIDBTest : public BaseGraphTest {
public:
    void SetUp() override {
        BaseGraphTest::SetUp();
        createDBAndConn();
        initGraph();
    }
};

class CApiTest : public APIDBTest {
public:
    gorgonzola_database _database;
    gorgonzola_connection connection;

    void SetUp() override {
        APIDBTest::SetUp();
        auto* connCppPointer = conn.release();
        auto* databaseCppPointer = database.release();
        connection = gorgonzola_connection{connCppPointer};
        _database = gorgonzola_database{databaseCppPointer};
    }

    std::string getDatabasePath() { return databasePath; }

    gorgonzola_database* getDatabase() { return &_database; }

    gorgonzola_connection* getConnection() { return &connection; }

    void TearDown() override {
        gorgonzola_connection_destroy(&connection);
        gorgonzola_database_destroy(&_database);
        APIDBTest::TearDown();
    }
};

} // namespace testing
} // namespace gorgonzola
