#include "c_api_test/c_api_test.h"

using namespace gorgonzola::main;
using namespace gorgonzola::testing;

class CApiPreparedStatementTest : public CApiTest {
public:
    std::string getInputDir() override {
        return TestHelper::appendGorgonzolaRootPath("dataset/tinysnb/");
    }
};

TEST_F(CApiPreparedStatementTest, IsSuccess) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query = "MATCH (a:person) WHERE a.isStudent = $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(preparedStatement._prepared_statement, nullptr);
    ASSERT_TRUE(gorgonzola_prepared_statement_is_success(&preparedStatement));
    gorgonzola_prepared_statement_destroy(&preparedStatement);

    query = "MATCH (a:personnnn) WHERE a.isStudent = $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(preparedStatement._prepared_statement, nullptr);
    ASSERT_FALSE(gorgonzola_prepared_statement_is_success(&preparedStatement));
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, GetErrorMessage) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query = "MATCH (a:person) WHERE a.isStudent = $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(preparedStatement._prepared_statement, nullptr);
    ASSERT_EQ(gorgonzola_prepared_statement_get_error_message(&preparedStatement), nullptr);
    gorgonzola_prepared_statement_destroy(&preparedStatement);

    query = "MATCH (a:personnnn) WHERE a.isStudent = $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(preparedStatement._prepared_statement, nullptr);
    char* message = gorgonzola_prepared_statement_get_error_message(&preparedStatement);
    ASSERT_EQ(std::string(message), "Binder exception: Table personnnn does not exist.");
    gorgonzola_prepared_statement_destroy(&preparedStatement);
    gorgonzola_destroy_string(message);
}

TEST_F(CApiPreparedStatementTest, BindBool) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query = "MATCH (a:person) WHERE a.isStudent = $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_bool(&preparedStatement, "1", true), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 3);
    gorgonzola_query_result_destroy(&result);
    // Bind a different parameter
    ASSERT_EQ(gorgonzola_prepared_statement_bind_bool(&preparedStatement, "1", false), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    resultCpp = static_cast<QueryResult*>(result._query_result);
    tuple = resultCpp->getNext();
    value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 5);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindInt64) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query = "MATCH (a:person) WHERE a.age > $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_int64(&preparedStatement, "1", 30), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 4);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindInt32) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query = "MATCH (a:movies) WHERE a.length > $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_int32(&preparedStatement, "1", 200), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 2);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindInt16) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query =
        "MATCH (a:person) -[s:studyAt]-> (b:organisation) WHERE s.length > $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_int16(&preparedStatement, "1", 10), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 2);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindInt8) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query =
        "MATCH (a:person) -[s:studyAt]-> (b:organisation) WHERE s.level > $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_int8(&preparedStatement, "1", 3), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 2);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindUInt64) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query =
        "MATCH (a:person) -[s:studyAt]-> (b:organisation) WHERE s.code > $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_uint64(&preparedStatement, "1", 100), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 2);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindUInt32) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query =
        "MATCH (a:person) -[s:studyAt]-> (b:organisation) WHERE s.temperature> $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_uint32(&preparedStatement, "1", 10), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 2);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindUInt16) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query =
        "MATCH (a:person) -[s:studyAt]-> (b:organisation) WHERE s.ulength> $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_uint16(&preparedStatement, "1", 100), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 2);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindUInt8) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query =
        "MATCH (a:person) -[s:studyAt]-> (b:organisation) WHERE s.ulevel> $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_uint8(&preparedStatement, "1", 14), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 2);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindDouble) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query = "MATCH (a:person) WHERE a.eyeSight > $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_double(&preparedStatement, "1", 4.5), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 7);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindFloat) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query = "MATCH (a:person) WHERE a.height < $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_float(&preparedStatement, "1", 1.0), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 1);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindString) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query = "MATCH (a:person) WHERE a.fName = $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_prepared_statement_is_success(&preparedStatement));
    ASSERT_EQ(gorgonzola_prepared_statement_bind_string(&preparedStatement, "1", "Alice"), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 1);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindDate) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query = "MATCH (a:person) WHERE a.birthdate > $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_prepared_statement_is_success(&preparedStatement));
    auto date = gorgonzola_date_t{0};
    ASSERT_EQ(gorgonzola_prepared_statement_bind_date(&preparedStatement, "1", date), GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 4);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindTimestamp) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query = "MATCH (a:person) WHERE a.registerTime > $1 and cast(a.registerTime, "
                 "\"timestamp_ns\") > $2 and cast(a.registerTime, \"timestamp_ms\") > "
                 "$3 and cast(a.registerTime, \"timestamp_sec\") > $4 and cast(a.registerTime, "
                 "\"timestamp_tz\") > $5 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_prepared_statement_is_success(&preparedStatement));
    auto timestamp = gorgonzola_timestamp_t{0};
    auto timestamp_ns = gorgonzola_timestamp_ns_t{1};
    auto timestamp_ms = gorgonzola_timestamp_ms_t{2};
    auto timestamp_sec = gorgonzola_timestamp_sec_t{3};
    auto timestamp_tz = gorgonzola_timestamp_tz_t{4};
    ASSERT_EQ(gorgonzola_prepared_statement_bind_timestamp(&preparedStatement, "1", timestamp),
        GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_timestamp_ns(&preparedStatement, "2", timestamp_ns),
        GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_timestamp_ms(&preparedStatement, "3", timestamp_ms),
        GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_timestamp_sec(&preparedStatement, "4", timestamp_sec),
        GorgonzolaSuccess);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_timestamp_tz(&preparedStatement, "5", timestamp_tz),
        GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 7);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindInteval) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query = "MATCH (a:person) WHERE a.lastJobDuration > $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_prepared_statement_is_success(&preparedStatement));
    auto interval = gorgonzola_interval_t{0, 0, 0};
    ASSERT_EQ(gorgonzola_prepared_statement_bind_interval(&preparedStatement, "1", interval),
        GorgonzolaSuccess);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 8);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}

TEST_F(CApiPreparedStatementTest, BindValue) {
    gorgonzola_prepared_statement preparedStatement;
    gorgonzola_query_result result;
    gorgonzola_state state;
    auto connection = getConnection();
    auto query = "MATCH (a:person) WHERE a.registerTime > $1 RETURN COUNT(*)";
    state = gorgonzola_connection_prepare(connection, query, &preparedStatement);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_TRUE(gorgonzola_prepared_statement_is_success(&preparedStatement));
    auto timestamp = gorgonzola_timestamp_t{0};
    auto timestampValue = gorgonzola_value_create_timestamp(timestamp);
    ASSERT_EQ(gorgonzola_prepared_statement_bind_value(&preparedStatement, "1", timestampValue),
        GorgonzolaSuccess);
    gorgonzola_value_destroy(timestampValue);
    state = gorgonzola_connection_execute(connection, &preparedStatement, &result);
    ASSERT_EQ(state, GorgonzolaSuccess);
    ASSERT_NE(result._query_result, nullptr);
    ASSERT_EQ(gorgonzola_query_result_get_num_tuples(&result), 1);
    ASSERT_EQ(gorgonzola_query_result_get_num_columns(&result), 1);
    ASSERT_TRUE(gorgonzola_query_result_is_success(&result));
    ASSERT_TRUE(gorgonzola_query_result_has_next(&result));
    auto resultCpp = static_cast<QueryResult*>(result._query_result);
    auto tuple = resultCpp->getNext();
    auto value = tuple->getValue(0)->getValue<int64_t>();
    ASSERT_EQ(value, 7);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_prepared_statement_destroy(&preparedStatement);
}
