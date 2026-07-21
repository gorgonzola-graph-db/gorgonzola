🛠️ Core Storage & Buffers

     disk_array.h/.cpp: Rework DiskArray to use the memoryManager instead of pinning pages and updating in-place.
     disk_array.cpp: Add support for DiskArrays that don't grow in size (currently only growing arrays are supported).
     buffer_manager.cpp: Spill to disk in a different location specifically for remote file workloads.
     vm_region.cpp: Investigate if VirtualAlloc(..., MEM_RESET, ...) is faster on Windows.
     page_state.h: (Beyond the P0 bug) Track down the second rare bug disabling another assert (#2289).
     column_chunk_data.cpp: Fix enableCompression logic. 
     column_chunk_data.h: Stop caching datatype in the chunk; let getNumValues read from metadata; hide direct variable access.
     column_chunk.h: Implement binary search for large numbers of entries; make the ALP exception function const; allow segments to share a single datatype; remove the storage_info workaround.
     column.cpp: Adapt offsets to current node group; ensure all writes update numValues; predict if splitting is needed based on compression type.
     string_column.cpp: Replace indices with ValueVector; optimize index scans by grouping adjacent ones.
     struct_column.cpp: Remove the necessity for the current struct handling; ensure child columns are split together as a group.
     dictionary_column.cpp: Scan batches of adjacent values instead of one by one.
     csr_node_group.cpp/.h: Simplify multiple internal checks; optimize loops with batch appends; vectorize length chunk processing; skip deleted rows; skip early if no changes; find max node offset; use finalizeCheckpoint.
     csr_node_group.h: Split CSRIndex into two levels (per csr leaf region and per node); serialize index info to disk; optimize rowIndices vector for space efficiency.
     csr_chunked_node_group.cpp: Vectorize length chunk processing; reuse ChunkedNodeGroup::deserialize().
     node_group.cpp: Move locked part of figuring out chunked group to initScan; optimize loop accessing version info; optimize local storage loop.
     node_group_collection.cpp: Directly figure out startRowIdx to optimize scanning.
     node_table.cpp: Assume local storage is small and optimize the loop away.
     rel_table.cpp: Remove the copy of rowIndices used to simplify implementation; support unflat vectors in delete_().
     table.h: Find a better way to skip WAL logging for FTS (currently hardcoded).
     update_info.cpp: Move VectorUpdateInfo into UndoBuffer; ensure rowsInVector is sorted.
     version_info.cpp: Add ALWAYS_INSERTED optimization; keep an additional insertion/deletion field to avoid extra work.
     overflow_file.cpp: Allow parallelization of HashIndex operations.
     shadow_file.cpp: Remove the shadow file entirely (requires broader storage changes).
     storage_manager.cpp: Remove the API added to add a single item (should be handled elsewhere).
     database_header.cpp: Add a test case for header logic.

⚙️ Query Processor & Execution

     factorized_table.cpp/.h: Stop adding to the end to prevent empty blocks in the middle; set up a rule for when to reset; change function to not use dataChunkPos in ColumnSchema.
     factorized_table_util.h: Review functions used to store copy messages.
     hash_aggregate.cpp: Merge functions into a single function.
     base_aggregate.cpp: Flush queuedTuples if it reaches a certain size.
     hash_join_probe.cpp: Review setToUnflat logic in restore/save state.
     intersect.cpp: Remove the assumption requiring keys to be stored in a specific order.
     order_by_merge.cpp: Directly feed sharedState to merger and dispatcher.
     sort_state.cpp: Remove hacky use of factorizedTable::lookup function.
     path_property_probe.cpp: Revisit printing order (src->dst vs left->right).
     insert_executor.cpp: Reference data vector instead of copying it.
     merge.cpp: Remove types parameter.
     node_batch_insert.cpp: Re-use the chunk and shift the values.
     rel_batch_insert.cpp: Get rid of hard-coded nbr and rel column ID 0/1; handle concurrency between COPY and other insertions.
     result_collector.cpp: Add an interface in factorized table for specific operations.
     export_db.cpp: Pass fileName from binder phase to processor (currently done later).
     import_db.cpp: Refactor special "Import database" logic after supporting proper syntax.
     map_dummy_scan.cpp: Remove vectors after dataChunk refactor.
     map_copy_to.cpp: Fix datatype inference for COPY (RETURN null) TO....

🧠 Planner & Optimizer

     acc_hash_join_optimizer.cpp: Check if SIP (Side-way Info Passing) can be applied to ScanNodeTable & RecursiveJoin; review if passing semi mask is optimal; apply SIP from build to probe.
     factorization_rewriter.cpp: Correctly set cardinality here.
     projection_push_down_optimizer.cpp: Replace current logic with a separate optimizer.
     remove_unnecessary_join_optimizer.cpp: Double check recent changes here.
     top_k_optimizer.cpp: Remove the projection between ORDER BY and MULTIPLICITY REDUCER.
     cost_model.cpp: Calculate intersect cost so it will be picked in worst-case scenarios.
     join_plan_solver.cpp: Provide an interface to append operator to resultPlan.
     logical_hash_join.cpp: Fix the requirement to flatten; consider getting rid of payloads when build side is flat.
     plan_subquery.cpp: Decide if we want to plan as a correlated subquery.

📥 Import/Export & File Formats

     base_csv_reader.cpp: Reduce the number of fields for performance reasons.
     csv_reader_config.h: Add newline character option; allow delimiter to be a string; support transform special options like \.
     parquet_reader.cpp/.h: Check return value of derive type; add prefetching support for remote file systems.
     parquet_column_reader.cpp: Optimize bitunpack offsets; keep state in the state object; fix dict_width being 0.
     parquet_rle_bp_decoder.h: Complain/throw error if we run out of buffer.
     parquet_dbp_decoder.h: Handle what happens if width is 0.
     parquet_column_writer.h: Add statistics to indicate column characteristics.
     npy_reader.cpp: Set npy reader data type to ARRAY to simplify checks; double check NpyReader instantiation.
     arrow_array_scan.cpp: Implement pure time type scanning.
     arrow_row_batch.cpp: Refactor into a function.
     arrow_type.cpp: Implement pure time type; add timezone support.

🔌 API Bindings (Python, Rust, C++, Node, Java)

     gorgonzola_java.cpp: Review JNI code for integer overflow.
     node_util.cpp: Review Node.js date difference handling (milliseconds vs specific units).
     py_connection.cpp: Remove ROLLBACK once database deletion after connection closure is guaranteed.
     pyarrow_scan.cpp: Drastically improve the binding step.
     value.rs (Rust): Add better error messages for unsupported types; test equivalence to value constructed inside a query.
     connection.rs (Rust): Instead of having a Value enum in results, perhaps QueryResult should handle it differently.
     gorgonzola.h (C API): Refactor datatype constructor to follow the C++ way of creating DataTypes.

🧩 Functions, Casting, & Types

     base_lower_upper_operation.cpp: Prevent invalid UTF-8 from entering a string column.
     built_in_function_utils.h/.cpp: Check any type; create unified interface for table/aggregate/scalar matching.
     cast_from_string_functions.cpp: Handle escape characters.
     vector_cast_functions.cpp: Handle remaining special cases; unify binding of casting function with other scalar functions.
     vector_hash_functions.cpp: Allow selection size to be greater than default vector capacity; have evaluators resolve state for result vector.
     list_filter.cpp: Make the output pos respect resultSelVector.
     show_sequences.cpp: Uncomment test logic when testable.
     int128_t.cpp: Add isFinite function in value.h to check all types.
     timestamp_t.cpp: Add the tests promised in the timestamp PR.
     types.cpp/.h: Use UINT8 to represent tag value; remove val suffix from all values in Val (redundant).
     value_vector.h: Rename to getValueRef; merge setDataVector & copyListEntryAndBufferMetaData.
     value.cpp (C API): Bind all int128_t supported functions.
     cast_string_non_nested_functions.h: Handle decimals; support exponent + decimal.

🗂️ Graph, GDS, & Algorithms

     on_disk_graph.cpp: Handle conflicts between predicate and custom edgePropertyIndex; remove the need to provide nbr info at prepare stage.
     graph.h: Avoid scanning if all that's necessary is to count the results.
     bfs_graph.h: Optimize if edgeID is not needed.
     betweenness_centrality.cpp: Use vertex compute (appears 4 times in algo/core); use function from PR #5916.

📦 DB Extensions

     httpfs.cpp/.h: Fix Windows CI missing certificates; make timeout/retry configurations exposed.
     http.test: Automate test file upload if storage has changed.
     iceberg.test: Handle nested struct in iceberg (currently blocked by DUCKDB).
     json_scan.cpp: Make maximum object size configurable; fix implementation issues.
     json_merge_patch.test: Support multiple paths merge.
     json_utils.test: Support INF in the database.
     create_fts_index.cpp: Allow Copy statement to be wrapped in manual transaction.
     fts_index.cpp: Fix StringUtils::split() bug ignoring empty parts.
     postgres.test: Implement regex in testing framework to handle platform-dependent error messages.
     sqlite.test: Apply casting from int64 -> bool when doing copy from.
     provider.h (LLM): When docs are created, validate the URL in the string.

📝 Miscellaneous & Cleanup

     CMakeLists.txt: Figure out if a specific flag can be set automatically by CMake.
     bind_import_database.cpp: Remove temporary workaround for parser requiring Cypher queries.
     bind_projection_clause.cpp: Remove augment group by (ensure sufficient testing first).
     bind_function_expression.cpp: Return a deep copy of the expression.
     bind_property_expression.cpp: Remove l97-l100 after removing propertyDataExprs from node.
     binder.cpp: Assert name not in scope (resolve the developer note).
     expression_binder.h: Move logic to an expression rewriter.
     rel_group_catalog_entry.h: Avoid using extend direction for storage direction.
     data_chunk_collection.h: Rework to use ColumnChunk.
     data_chunk_state.h: Get rid of fStateType and merge DataChunkState with SelectionVector.
     scan_file_function.h: Unify the design on how we calculate totalSize.
     table_function.h: Find a better way to pass semiMasks to QueryHNSWIndex.
     unary_function_executor.h: Fix unsafe reinterpret_cast.
     logical_operator_collector.h: Rename this file/class.
     logical_recursive_extend.h: Remove limitNum once recursive extend is pipelined.
     side_way_info_passing.h: Think if we should simply put outer plan always on the build side.
     expression_mapper.h: Add a comment explaining the logic.
     standalone_call.h: Remove this class/structure.
     transaction.h: These shouldn't need to be exported (make internal).
     query_result.cpp: Introduce an error query result class.
     settings.cpp: Remove temporary solution for multiple write transactions testing.
     embedded_shell.cpp / json_printer.cpp: Move JSON printing logic into QueryResult interface; expose unified printing logic.