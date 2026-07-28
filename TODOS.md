Here is the complete, exhaustive list of all actionable items from the codebase, strictly sorted by importance from **Deal with Immediately** (critical bugs and broken tests) to **Long-Term** (architectural redesigns and micro-optimizations).

---

### 🚨 1. Deal With Immediately (Critical Bugs, Correctness, & Failing Tests)
*These items represent likely bugs, data corruption risks, or currently failing test suites.*

- [ ] **[BUG] Investigate disabled assertion in Buffer Manager (`page_state.h`)**: Track down the rare bug referenced in #2289 that caused an assertion to be disabled. Re-enable it.
- [ ] **[BUG] Track down the second rare bug (`page_state.h`)**: Investigate the second disabled assertion related to #2289.
- [ ] **[BUG] Fix LEFT JOIN discarding NULL keys (`hash_join_probe.cpp`)**: Fix the bug where LEFT JOIN improperly discards NULL keys during the probe phase.
- [ ] **[BUG] Fix key state restoration in Hash Join Probe (`hash_join_probe.cpp`)**: Address the potential bug where all keys' states are not properly restored.
- [ ] **[BUG] Investigate unfiltered state in Join Hash Table (`join_hash_table.cpp`)**: Review the logic where the state is set to un-filtered (noted as "probably wrong" by the developer).
- [ ] **[BUG] Enforce NULL constraints for InternalIDChunkData (`column_chunk_data.h`)**: Ensure `InternalIDChunkData` properly handles or prevents NULLs as intended.
- [ ] **[BUG] Prevent invalid UTF-8 from entering string columns (`base_lower_upper_operation.cpp`)**: Stop invalid UTF-8 data at the function level.
- [ ] **[BUG] Fix failing parameter test (`test_parameter.py`)**: Fix the case that is currently failing but "should be executed successfully."
- [ ] **[BUG] Re-enable Torch Geometric remote backend tests (`test_torch_geometric_remote_backend.py`)**: Fix the underlying `StorageDriver` issue and re-enable the tests.
- [ ] **[BUG] Resolve stale TODO in Binder scope assertion (`binder.cpp`)**: Review the note left for "Xiyang" regarding the failing assertion for name scopes. Fix the tests or remove the TODO.
- [ ] **[SAFETY] Replace `unwrap()` with `Result` in Rust API (`query_result.rs`)**: Remove `unwrap()` calls in FFI boundaries to prevent panics.
- [ ] **[SAFETY] Review JNI code for integer overflow (`gorgonzola_java.cpp`)**: Audit Java API bindings for integer overflows.
- [ ] **[TEST] Enable read-only mode tests on Windows (`test_database.py`)**: Implement read-only mode for Windows so these 3 disabled tests can run.
- [ ] **[FUNC] Support unflat vectors in `delete_()` (`rel_table.cpp`)**: Remove the current functional limitation preventing deletions with unflat vectors.
- [ ] **[BLOCKER] Fix Windows CI missing certificates (`httpfs.cpp`)**: Enable HTTPFS tests on Windows by resolving missing certificates in CI.

---

### 🏗️ 2. Short-Term (High-Impact Refactors, Core API Safety, & Major Features)
*These items block future development, cause significant maintenance burden, or address highly requested feature gaps.*

- [ ] **[REFACTOR] Enforce Type Safety in Rust API (`value.rs`)**: Enforce the type of contents in the `Value` enum to prevent type safety holes.
- [ ] **[REFACTOR] Unify Factorized Table Interfaces (`factorized_table.h`)**: Merge the two `readUnflatCol()` interfaces and unify `readUnflatCol` with `readCol`.
- [ ] **[REFACTOR] Split CSV Reader Driver Header (`driver.h`)**: Split this monolithic header file into logical, focused components.
- [ ] **[REFACTOR] Separate Shell Printers into Individual Files (`printer.h`, `embedded_shell.cpp`)**: Move printing logic out of the embedded shell and into separate classes.
- [ ] **[REFACTOR] Remove temporary workaround for parser (`bind_import_database.cpp`)**: Remove the workaround that requires input cypher queries.
- [ ] **[REFACTOR] Remove special "Import database" logic (`import_db.cpp`)**: Refactor after proper syntax is supported.
- [ ] **[REFACTOR] Remove temporary solution for write transaction tests (`settings.cpp`)**: Clean up testing workarounds.
- [ ] **[PERF] Optimize Hash Index Checkpointing (`hash_index.cpp`)**: Implement vacuuming of the index during checkpoint to prevent bloat.
- [ ] **[PERF] Optimize CSR Node Group Operations (`csr_node_group.cpp`, `csr_chunked_node_group.cpp`)**: Implement early skipping if no changes exist, optimize loops with batch appends, and vectorize length chunk processing.
- [ ] **[PERF] Improve Compression Routines (`compression.cpp`)**: Implement bytewise `memcmp` comparisons, dynamic compression selection, and integer bitpacking.
- [ ] **[PERF] Enhance Filter Push Down Optimizer (`filter_push_down_optimizer.cpp`)**: Expand the optimizer to rewrite expressions beyond just `left = right`.
- [ ] **[PERF] Improve Cardinality Estimation (`cardinality_estimator.cpp`)**: Implement HyperLogLog (HLL) to better estimate distinct keys.
- [ ] **[PERF] Add HNSW Batch Insertion Optimization (`hnsw_index.cpp`)**: Implement batch insertion optimizations and switch to scan instead of lookup.
- [ ] **[FEAT] Add Timezone Support for Arrow/NumPy (`arrow_type.cpp`, `numpy_type.cpp/.h`)**: Implement pure time types and add support for timestamps with timezones.
- [ ] **[FEAT] Support `binary_as_string` Parquet Copy Option (`parquet_reader.cpp`)**: Add logic to handle the `binary_as_string` option.
- [ ] **[FEAT] Add int128_t Function Bindings (`value.cpp`)**: Bind all supported functions for `int128_t` in the C API.
- [ ] **[FEAT] Support Decimals and Exponents in String Casting (`cast_string_non_nested_functions.h`)**: Add handling for decimals and exponent notation.
- [ ] **[FEAT] Allow Copy statement to be wrapped in manual transaction (`create_fts_index.cpp`)**: Remove FTS copy restriction.
- [ ] **[BUG] Fix `py_connection.cpp` ROLLBACK leak**: Remove ROLLBACK once database deletion after connection closure is guaranteed.

---

### ⚙️ 3. Medium-Term (Performance, Minor Features, & Code Cleanup)
*These items improve system performance, add minor features, and clean up technical debt without blocking current work.*

- [ ] **[PERF] Spill to disk in a different location for remote files (`buffer_manager.cpp`)**.
- [ ] **[PERF] Optimize index scans by grouping adjacent ones (`string_column.cpp`)**.
- [ ] **[PERF] Scan batches of adjacent values (`dictionary_column.cpp`)**.
- [ ] **[PERF] Re-use the chunk and shift the values (`node_batch_insert.cpp`)**.
- [ ] **[PERF] Flush `queuedTuples` if it reaches a certain size (`base_aggregate.cpp`)**.
- [ ] **[PERF] Use vertex compute for Betweenness Centrality (`betweenness_centrality.cpp` - x4 occurrences)**.
- [ ] **[PERF] Implement binary search for large numbers of entries (`column_chunk.h`)**.
- [ ] **[PERF] Avoid scanning if all that's necessary is to count results (`graph.h`)**.
- [ ] **[PERF] Optimize loops accessing version info (`node_group.cpp`)**.
- [ ] **[PERF] Optimize bitunpack offsets in Parquet (`parquet_column_reader.cpp`)**.
- [ ] **[FEAT] Add newline character option and string delimiter to CSV (`csv_reader_config.h`)**.
- [ ] **[FEAT] Set npy reader data type to ARRAY to simplify checks (`npy_reader.cpp`)**.
- [ ] **[FEAT] Make maximum JSON object size configurable (`json_scan.cpp`)**.
- [ ] **[FEAT] Support multiple paths merge in JSON (`json_merge_patch.test`)**.
- [ ] **[FEAT] Support INF in the database for JSON (`json_utils.test`)**.
- [ ] **[FEAT] Apply casting from int64->bool when copying from SQLite (`sqlite.test`)**.
- [ ] **[FEAT] Implement regex in testing framework for platform-dependent Postgres errors (`postgres.test`)**.
- [ ] **[REFACTOR] Rename `logical_operator_collector.h`**.
- [ ] **[REFACTOR] Merge `DataChunkState` with `SelectionVector` (`data_chunk_state.h`)**.
- [ ] **[REFACTOR] Unexport functions in `transaction.h`**.
- [ ] **[REFACTOR] Introduce an error query result class (`query_result.cpp`)**.
- [ ] **[REFACTOR] Move JSON printing logic into `QueryResult` interface (`json_printer.cpp`)**.
- [ ] **[REFACTOR] Fix datatype inference for `COPY (RETURN null) TO...` (`map_copy_to.cpp`)**.
- [ ] **[REFACTOR] Remove `types` parameter in `merge.cpp`**.
- [ ] **[REFACTOR] Remove hard-coded nbr and rel column ID 0/1 (`rel_batch_insert.cpp`)**.
- [ ] **[REFACTOR] Pass `fileName` from binder phase to processor (`export_db.cpp`)**.
- [ ] **[REFACTOR] Add better error messages for unsupported types in Rust API (`value.rs`)**.
- [ ] **[REFACTOR] Drastically improve the binding step in PyArrow (`pyarrow_scan.cpp`)**.
- [ ] **[REFACTOR] Review Node.js date difference handling (`node_util.cpp`)**.

---

### 🧹 4. Long-Term (Architectural Redesigns & Micro-Optimizations)
*These items require significant rewrites, affect deep architectural layers, or are minor optimizations/cleanups that can be addressed over time.*

#### Deep Architecture & System Design
- [ ] **[ARCH] Rework `DiskArray` to use the `memoryManager` (`disk_array.h`)**.
- [ ] **[ARCH] Split `CSRIndex` into two levels and serialize info to disk (`csr_node_group.h`)**.
- [ ] **[ARCH] Remove the shadow file entirely (`shadow_file.cpp`)**.
- [ ] **[ARCH] Add `ALWAYS_INSERTED` optimization to version info (`version_info.cpp`)**.
- [ ] **[ARCH] Find a better way to skip WAL logging for FTS (`table.h`)**.
- [ ] **[ARCH] Remove the API to add a single item in `storage_manager.cpp`**.
- [ ] **[ARCH] Decide if we should plan as a correlated subquery (`plan_subquery.cpp`)**.
- [ ] **[ARCH] Rethink SIP direction: put outer plan always on the build side? (`side_way_info_passing.h`)**.
- [ ] **[ARCH] Remove `limitNum` once recursive extend is pipelined (`logical_recursive_extend.h`)**.

#### Storage & Execution Engine Internals
- [ ] **[STORAGE] Add support for DiskArrays that don't grow in size (`disk_array.cpp`)**.
- [ ] **[STORAGE] Make datatype cached in chunk; let `getNumValues` read from metadata (`column_chunk_data.h`)**.
- [ ] **[STORAGE] Make the ALP exception function `const` (`column_chunk.h`)**.
- [ ] **[STORAGE] Allow segments to share a single datatype (`column_chunk.h`)**.
- [ ] **[STORAGE] Remove the `storage_info` workaround (`column_chunk.h`)**.
- [ ] **[STORAGE] Adapt offsets to current node group (`column.cpp`)**.
- [ ] **[STORAGE] Ensure all writes update `numValues` (`column.cpp`)**.
- [ ] **[STORAGE] Predict if splitting is needed based on compression type (`column.cpp`)**.
- [ ] **[STORAGE] Replace indices with `ValueVector` (`string_column.cpp`)**.
- [ ] **[STORAGE] Remove the necessity for the current struct handling (`struct_column.cpp`)**.
- [ ] **[STORAGE] Ensure child columns are split together as a group (`struct_column.cpp`)**.
- [ ] **[STORAGE] Move locked part of figuring out chunked group to `initScan` (`node_group.cpp`)**.
- [ ] **[STORAGE] Optimize local storage loop in `node_table.cpp`**.
- [ ] **[STORAGE] Remove the copy of `rowIndices` in `rel_table.cpp`**.
- [ ] **[STORAGE] Move `VectorUpdateInfo` into `UndoBuffer` (`update_info.cpp`)**.
- [ ] **[STORAGE] Ensure `rowsInVector` is sorted (`update_info.cpp`)**.
- [ ] **[STORAGE] Keep an additional insertion/deletion field to avoid extra work (`version_info.cpp`)**.
- [ ] **[STORAGE] Allow parallelization of `HashIndex` operations (`overflow_file.cpp`)**.
- [ ] **[STORAGE] Optimize slot splitting and reduce locking when frames are unpinned (`hash_index.cpp`)**.
- [ ] **[EXEC] Stop adding to the end of factorized table to prevent empty blocks (`factorized_table.cpp`)**.
- [ ] **[EXEC] Set up a rule for when to reset the factorized table (`factorized_table.cpp`)**.
- [ ] **[EXEC] Change function to not use `dataChunkPos` in `ColumnSchema` (`factorized_table.cpp`)**.
- [ ] **[EXEC] Merge hash aggregate functions into a single function (`hash_aggregate.cpp`)**.
- [ ] **[EXEC] Remove the assumption requiring keys to be stored in a specific order (`intersect.cpp`)**.
- [ ] **[EXEC] Directly feed `sharedState` to merger and dispatcher (`order_by_merge.cpp`)**.
- [ ] **[EXEC] Remove hacky use of `factorizedTable::lookup` function (`sort_state.cpp`)**.
- [ ] **[EXEC] Revisit printing order (src->dst vs left->right) in `path_property_probe.cpp`**.
- [ ] **[EXEC] Reference data vector instead of copying it (`insert_executor.cpp`)**.
- [ ] **[EXEC] Handle concurrency between COPY and other insertions (`rel_batch_insert.cpp`)**.

#### Planner & Optimizer Internals
- [ ] **[OPT] Check if SIP can be applied to `ScanNodeTable` & `RecursiveJoin` (`acc_hash_join_optimizer.cpp`)**.
- [ ] **[OPT] Review if passing semi mask is optimal (`acc_hash_join_optimizer.cpp`)**.
- [ ] **[OPT] Apply SIP from build to probe (`acc_hash_join_optimizer.cpp`)**.
- [ ] **[OPT] Correctly set cardinality in `factorization_rewriter.cpp`**.
- [ ] **[OPT] Replace current projection push down logic with a separate optimizer (`projection_push_down_optimizer.cpp`)**.
- [ ] **[OPT] Double check recent changes in `remove_unnecessary_join_optimizer.cpp`**.
- [ ] **[OPT] Remove the projection between ORDER BY and MULTIPLICITY REDUCER (`top_k_optimizer.cpp`)**.
- [ ] **[OPT] Calculate intersect cost so it will be picked in worst-case scenarios (`cost_model.cpp`)**.
- [ ] **[OPT] Provide an interface to append operator to `resultPlan` (`join_plan_solver.cpp`)**.
- [ ] **[OPT] Fix the requirement to flatten in `logical_hash_join.cpp`**.

#### Codebase Cleanup & Micro-Features
- [ ] **[CLEAN] Figure out if a specific flag can be set automatically by CMake (`CMakeLists.txt`)**.
- [ ] **[CLEAN] Remove augment group by (ensure sufficient testing first) (`bind_projection_clause.cpp`)**.
- [ ] **[CLEAN] Return a deep copy of the expression (`bind_function_expression.cpp`)**.
- [ ] **[CLEAN] Remove `l97-l100` after removing `propertyDataExprs` from node (`bind_property_expression.cpp`)**.
- [ ] **[CLEAN] Move logic to an expression rewriters (`expression_binder.h`)**.
- [ ] **[CLEAN] Avoid using extend direction for storage direction (`rel_group_catalog_entry.h`)**.
- [ ] **[CLEAN] Rework `data_chunk_collection.h` to use `ColumnChunk`**.
- [ ] **[CLEAN] Unify the design on how we calculate `totalSize` (`scan_file_function.h`)**.
- [ ] **[CLEAN] Find a better way to pass `semiMasks` to `QueryHNSWIndex` (`table_function.h`)**.
- [ ] **[CLEAN] Fix unsafe `reinterpret_cast` (`unary_function_executor.h`)**.
- [ ] **[CLEAN] Add a comment explaining the logic (`expression_mapper.h`)**.
- [ ] **[CLEAN] Remove `standalone_call.h`**.
- [ ] **[CLEAN] Remove vectors after `dataChunk` refactor (`map_dummy_scan.cpp`)**.
- [ ] **[CLEAN] Remove `val` suffix from all values in `Val` (`types.h`)**.
- [ ] **[CLEAN] Rename to `getValueRef` (`value_vector.h`)**.
- [ ] **[CLEAN] Merge `setDataVector` & `copyListEntryAndBufferMetaData` (`value_vector.h`)**.
- [ ] **[CLEAN] Refactor datatype constructor to follow C++ way (`gorgonzola.h`)**.
- [ ] **[CLEAN] Check `any` type in built-in function utils (`built_in_function_utils.cpp`)**.
- [ ] **[CLEAT] Create unified interface for table/aggregate/scalar matching (`built_in_function_utils.h`)**.
- [ ] **[CLEAN] Handle escape characters in cast from string (`cast_from_string_functions.cpp`)**.
- [ ] **[CLEAN] Handle remaining special cases in vector cast (`vector_cast_functions.cpp`)**.
- [ ] **[CLEAN] Unify binding of casting function with other scalar functions (`vector_cast_functions.cpp`)**.
- [ ] **[CLEAN] Allow selection size to be greater than default vector capacity (`vector_hash_functions.cpp`)**.
- [ ] **[CLEAN] Have evaluators resolve state for result vector (`vector_hash_functions.cpp`)**.
- [ ] **[CLEAN] Make output `pos` respect `resultSelVector` (`list_filter.cpp`)**.
- [ ] **[CLEAN] Uncomment test logic when testable (`show_sequences.cpp`)**.
- [ ] **[CLEAN] Add `isFinite` function in `value.h` (`int128_t.cpp`)**.
- [ ] **[CLEAN] Use `UINT8` to represent tag value (`types.cpp`)**.
- [ ] **[CLEAN] Remove `dataChunkPos` in `ColumnSchema` (`factorized_table.cpp`)**.
- [ ] **[CLEAN] Review functions used to store copy messages (`factorized_table_util.h`)**.
- [ ] **[CLEAN] Remove hard-coded nbr and rel column ID 0/1 (`rel_batch_insert.cpp`)**.
- [ ] **[CLEAN] Add an interface in factorized table for specific operations (`result_collector.cpp`)**.
- [ ] **[CLEAN] Add test case for header logic (`database_header.cpp`)**.
- [ ] **[CLEAN] Add the tests promised in the timestamp PR (`timestamp_t.cpp`)**.
- [ ] **[CLEAN] Automate test file upload if storage has changed (`http.test`)**.
- [ ] **[CLEAN] Handle nested struct in iceberg (`iceberg.test`)**.
- [ ] **[CLEAN] Fix `dict_width` being 0 in Parquet (`parquet_column_reader.cpp`)**.
- [ ] **[CLEAN] Complain/throw error if we run out of buffer in Parquet (`parquet_rle_bp_decoder.h`)**.
- [ ] **[CLEAN] Handle what happens if width is 0 in Parquet (`parquet_dbp_decoder.h`)**.
- [ ] **[CLEAN] Add statistics to indicate column characteristics in Parquet (`parquet_column_writer.h`)**.
- [ ] **[CLEAN] Check return value of derive type in Parquet (`parquet_reader.cpp`)**.
- [ ] **[CLEAN] Add prefetching support for remote file systems in Parquet (`parquet_reader.h`)**.
- [ ] **[CLEAN] Reduce the number of fields for performance reasons in CSV (`base_csv_reader.cpp`)**.
- [ ] **[CLEAN] Implement pure time type scanning in Arrow (`arrow_array_scan.cpp`)**.
- [ ] **[CLEAN] Refactor Arrow row batch into a function (`arrow_row_batch.cpp`)**.
- [ ] **[CLEAN] Double check `NpyReader` instantiation (`npy_reader.cpp`)**.
- [ ] **[CLEAN] Remove `edgeID` optimization if not needed (`bfs_graph.h`)**.
- [ ] **[CLEAN] Handle conflicts between predicate and custom `edgePropertyIndex` (`on_disk_graph.cpp`)**.
- [ ] **[CLEAN] Remove the need to provide nbr info at prepare stage (`on_disk_graph.cpp`)**.
- [ ] **[CLEAN] Make timeout/retry configurations exposed in HTTPFS (`httpfs.h`)**.
- [ ] **[CLEAN] When docs are created, validate the URL in the string (`provider.h`)**.
- [ ] **[CLEAN] Investigate if `VirtualAlloc(..., MEM_RESET, ...)` is faster on Windows (`vm_region.cpp`)**.

---

### ✅ Completed

- [x] ~~**[BUG] Fix `StringUtils::split()` empty-part bug (`fts_index.cpp`)**: Fix the bug where `StringUtils::split()` doesn't ignore empty parts, causing FTS indexing issues.~~
- [x] ~~**[EXEC] Review `setToUnflat` logic in restore/save state (`hash_join_probe.cpp`)**.~~
