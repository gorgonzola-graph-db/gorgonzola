#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "gorgonzola.h"

int main() {
    gorgonzola_database db;
    gorgonzola_connection conn;
    gorgonzola_database_init("" /* fill db path */, gorgonzola_default_system_config(), &db);
    gorgonzola_connection_init(&db, &conn);

    // Create schema.
    gorgonzola_query_result result;
    gorgonzola_connection_query(
        &conn, "CREATE NODE TABLE Person(name STRING, age INT64, PRIMARY KEY(name));", &result);
    gorgonzola_query_result_destroy(&result);
    // Create nodes.
    gorgonzola_connection_query(&conn, "CREATE (:Person {name: 'Alice', age: 25});", &result);
    gorgonzola_query_result_destroy(&result);
    gorgonzola_connection_query(&conn, "CREATE (:Person {name: 'Bob', age: 30});", &result);
    gorgonzola_query_result_destroy(&result);

    // Execute a simple query.
    gorgonzola_connection_query(&conn, "MATCH (a:Person) RETURN a.name AS NAME, a.age AS AGE;", &result);

    // Fetch each value.
    gorgonzola_flat_tuple tuple;
    gorgonzola_value value;
    while (gorgonzola_query_result_has_next(&result)) {
        gorgonzola_query_result_get_next(&result, &tuple);

        gorgonzola_flat_tuple_get_value(&tuple, 0, &value);
        char* name;
        gorgonzola_value_get_string(&value, &name);

        gorgonzola_flat_tuple_get_value(&tuple, 1, &value);
        int64_t age;
        gorgonzola_value_get_int64(&value, &age);

        printf("name: %s, age: %" PRIi64 " \n", name, age);
        gorgonzola_destroy_string(name);
    }
    gorgonzola_value_destroy(&value);
    gorgonzola_flat_tuple_destroy(&tuple);

    // Print query result.
    char* result_string = gorgonzola_query_result_to_string(&result);
    printf("%s", result_string);
    gorgonzola_destroy_string(result_string);

    gorgonzola_query_result_destroy(&result);
    gorgonzola_connection_destroy(&conn);
    gorgonzola_database_destroy(&db);
    return 0;
}
