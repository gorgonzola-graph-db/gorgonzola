# Codebase TODOs
```
CMakeLists.txt:    # TODO (bmwinger): Figure out if this can be set automatically by cmake,
modules/dataset/long-string-multiple-files/vpersonstringaj.csv:2qWwyewK1TODOc
modules/gorgonzola-api-langs/java_api/src/jni/gorgonzola_java.cpp:            // TODO: Need to review this for overflow
modules/gorgonzola-api-langs/nodejs_api/src_cpp/node_util.cpp:        // TODO: By default, Node.js returns the difference in milliseconds between two dates, so we
modules/gorgonzola-api-langs/python_api/pyproject.toml:  "TD002", # Missing author in TODO
modules/gorgonzola-api-langs/python_api/pyproject.toml:  "TD003", # Missing issue link on the line following TODO
modules/gorgonzola-api-langs/python_api/src_cpp/include/numpy/numpy_type.h:// TODO(Ziyi): Support more timestamp types, object and category(enum) type.
modules/gorgonzola-api-langs/python_api/src_cpp/numpy/numpy_type.cpp:    // TODO(Ziyi): timestamp with timezone is currently not supported.
modules/gorgonzola-api-langs/python_api/src_cpp/py_connection.cpp:            // TODO(Chang): remove ROLLBACK once we can guarantee database is deleted after conn
modules/gorgonzola-api-langs/python_api/src_cpp/pyarrow/pyarrow_scan.cpp:    // TODO: This binding step could use some drastic improvements.
modules/gorgonzola-api-langs/python_api/test/test_database.py:    # TODO: Enable this test on Windows when the read-only mode is implemented.
modules/gorgonzola-api-langs/python_api/test/test_database.py:        # TODO: Enable this test on Windows when the read-only mode is implemented.
modules/gorgonzola-api-langs/python_api/test/test_database.py:    # TODO: Enable this test on Windows when the read-only mode is implemented.
modules/gorgonzola-api-langs/python_api/test/test_issue.py:# TODO(Maxwell): check if we should change getCastCost() for the following test
modules/gorgonzola-api-langs/python_api/test/test_parameter.py:    # TODO(maxwell): fixme. The following case should be executed successfully.
modules/gorgonzola-api-langs/python_api/test/test_torch_geometric_remote_backend.py:# TODO: FIX-ME. Re-enable the tests when StorageDriver is fixed.
modules/gorgonzola-api-langs/rust_api/src/connection.rs:    // TODO(bmwinger): Instead of having a Value enum in the results, perhaps QueryResult, and thus query
modules/gorgonzola-api-langs/rust_api/src/query_result.rs:                // TODO: Return result instead of unwrapping?
modules/gorgonzola-api-langs/rust_api/src/value.rs:    // TODO: Enforce type of contents
modules/gorgonzola-api-langs/rust_api/src/value.rs:            // TODO(bmwinger): Better error message for types which are unsupported
modules/gorgonzola-api-langs/rust_api/src/value.rs:                // TODO: Test equivalence to value constructed inside a a query
modules/gorgonzola-db-extensions/algo/src/function/betweenness_centrality.cpp:    // TODO: Use vertex compute for this.
modules/gorgonzola-db-extensions/algo/src/function/betweenness_centrality.cpp:    // TODO: Use vertex compute for this.
modules/gorgonzola-db-extensions/algo/src/function/betweenness_centrality.cpp:// TODO: Use function from https://github.com/kuzudb/kuzu/pull/5916
modules/gorgonzola-db-extensions/fts/src/function/create_fts_index.cpp:    // TODO(Ziyi): Copy statement can't be wrapped in manual transaction, so we can't wrap all
modules/gorgonzola-db-extensions/fts/src/index/fts_index.cpp:        // TODO(Ziyi): StringUtils::split() has a bug which doesn't ignore empty parts even
modules/gorgonzola-db-extensions/httpfs/src/httpfs.cpp:    // TODO(Chang): Windows CI is missing some certificates, which causes tests to fail. Enable the
modules/gorgonzola-db-extensions/httpfs/src/include/httpfs.h:    // TODO(Ziyi): Make them configurable.
modules/gorgonzola-db-extensions/httpfs/test/test_files/http.test:#TODO(Ziyi): figure out a way to automate upload if storage has changed.
modules/gorgonzola-db-extensions/iceberg/test/test_files/iceberg.test:# TODO(Sterling): DUCKDB didn't handle nested struct in iceberg yet
modules/gorgonzola-db-extensions/json/src/functions/table_functions/json_scan.cpp:            // TODO(Ziyi): We should make the maximum object size as a configurable option.
modules/gorgonzola-db-extensions/json/src/functions/table_functions/json_scan.cpp:    // TODO(Ziyi): fix this.
modules/gorgonzola-db-extensions/json/test/json_merge_patch.test:#(TODO): we may need to support multiple paths merge
modules/gorgonzola-db-extensions/json/test/json_utils.test:#TODO(sterling): the INF may be need to be supported by database.
modules/gorgonzola-db-extensions/llm/src/include/providers/provider.h:    // TODO(Tanvir) When docs are created validate the url in the string
modules/gorgonzola-db-extensions/postgres/test/test_files/postgres.test:# TODO(Ziyi): the error message is dependent on platforms. We have to implement regex in testing framework to handle this.
modules/gorgonzola-db-extensions/sqlite/test/test_files/sqlite.test:# TODO(Xiyang): There is no BOOL,DATE type in sqlite, so we should apply a casting from int64->bool when doing copy from.
modules/gorgonzola-db-extensions/vector/src/index/hnsw_index.cpp:    // TODO(Guodong): Perhaps should switch to scan instead of lookup here.
modules/gorgonzola-db-extensions/vector/src/index/hnsw_index.cpp:    // TODO(Guodong): Perhaps should switch to scan instead of lookup here.
modules/gorgonzola-db-extensions/vector/src/index/hnsw_index.cpp:    // TODO(Guodong): Should add the optimization for batch insertions.
modules/gorgonzola-db-extensions/vector/test/test_files/transaction.test:# TODO(Guodong): Uncomment this when we support CREATE_VECTOR_INDEX in the manual transaction mode.
modules/scripts/generate-cpp-docs/c/Doxyfile:# The GENERATE_TODOLIST tag can be used to enable (YES) or disable (NO) the todo
modules/scripts/generate-cpp-docs/c/Doxyfile:GENERATE_TODOLIST      = YES
modules/scripts/generate-cpp-docs/cpp/Doxyfile:# The GENERATE_TODOLIST tag can be used to enable (YES) or disable (NO) the todo
modules/scripts/generate-cpp-docs/cpp/Doxyfile:GENERATE_TODOLIST      = YES
modules/third_party/antlr4_runtime/src/ANTLRErrorStrategy.h:  /// TODO: what to do about lexers
modules/third_party/antlr4_runtime/src/ANTLRFileStream.h:  // TODO: this class needs testing.
modules/third_party/antlr4_runtime/src/Lexer.cpp:  // TODO: Do we lose character or line position information?
modules/third_party/antlr4_runtime/src/Parser.cpp:  // TODO: useful in parser?
modules/third_party/antlr4_runtime/src/atn/ATNConfig.h:     * don't ever decrement. TODO: make it a boolean then</p>
modules/third_party/antlr4_runtime/src/atn/ATNConfigSet.h:    // TODO: these fields make me pretty uncomfortable but nice to pack up info together, saves recomputation
modules/third_party/antlr4_runtime/src/atn/ATNConfigSet.h:    // TODO: can we track conflicts as they are added to save scanning configs later?
modules/third_party/antlr4_runtime/src/atn/ATNDeserializationOptions.h:  // TODO: Is this useful? If so we should mark it as explicit, otherwise remove it.
modules/third_party/antlr4_runtime/src/atn/AtomTransition.h:  /// TODO: make all transitions sets? no, should remove set edges.
modules/third_party/antlr4_runtime/src/atn/AtomTransition.h:    /// TODO: rename this to label
modules/third_party/antlr4_runtime/src/atn/LexerATNSimulator.cpp:        // TODO: if the entry rule is invoked recursively, some
modules/third_party/antlr4_runtime/src/atn/ParserATNSimulator.cpp:        configs->dipsIntoOuterContext = true; // TODO: can remove? only care when we add to set per middle of this method
modules/third_party/antlr4_runtime/src/atn/ParserATNSimulator.h:    /* TODO: If we are doing predicates, there is no point in pursuing
modules/third_party/antlr4_runtime/src/atn/PredicateTransition.h:  /// TODO: this is old comment:
modules/third_party/antlr4_runtime/src/atn/PredictionContext.cpp:  // TODO: track whether this is possible above during merge sort for speed
modules/third_party/antlr4_runtime/src/atn/ProfilingATNSimulator.cpp:      // TODO: does not handle delayed errors per getSynValidOrSemInvalidAltThatFinishedDecisionEntryRule()
modules/third_party/antlr4_runtime/src/tree/xpath/XPath.h:    // TODO: check for invalid token/rule names, bad syntax
modules/third_party/brotli/c/common/platform.h:/* TODO(eustas): add appropriate icc/sunpro/arm/ibm/ti checks. */
modules/third_party/brotli/c/common/platform.h:/* TODO(eustas): detect ARMv6T2 and enable this code for it. */
modules/third_party/brotli/c/common/shared_dictionary.c:   TODO(lode): link to RFC for shared brotli once published. */
modules/third_party/brotli/c/common/shared_dictionary.c:  /* TODO(eustas): explicitly initialize all the fields? */
modules/third_party/brotli/c/dec/decode.c:  /* TODO(eustas): avoid allocation for single uncompressed block. */
modules/third_party/brotli/c/dec/state.h:  /* TODO(eustas): +2 bytes padding */
modules/third_party/brotli/c/dec/state.h:  /* TODO(eustas): +3 bytes padding */
modules/third_party/brotli/c/dec/state.h:  /* TODO(eustas): +4 bytes padding */
modules/third_party/brotli/c/dec/state.h:  /* TODO(eustas): +12 bits padding */
modules/third_party/brotli/c/enc/block_splitter.c:    /* TODO(eustas): reuse for distances? */
modules/third_party/brotli/c/enc/block_splitter_inc.h:  /* TODO(eustas): move to arena? */
modules/third_party/brotli/c/enc/block_splitter_inc.h:      /* TODO(eustas): should we give a block-switch discount here? */
modules/third_party/brotli/c/enc/brotli_bit_stream.c:  /* TODO(eustas): fix me */
modules/third_party/brotli/c/enc/brotli_bit_stream.c:  if (num_types > 1) {  /* TODO(eustas): else? could StoreBlockSwitch occur? */
modules/third_party/brotli/c/enc/brotli_bit_stream.c:/* TODO(eustas): pull alloc/dealloc to caller? */
modules/third_party/brotli/c/enc/brotli_bit_stream.c:  /* TODO(eustas): merge bits and depth? */
modules/third_party/brotli/c/enc/cluster_inc.h:  /* TODO(eustas): by using idea of "cycle-sort" we can avoid allocation of
modules/third_party/brotli/c/enc/cluster_inc.h:  /* TODO(eustas): move to "persistent" arena? */
modules/third_party/brotli/c/enc/compound_dictionary.c:  /* TODO(eustas): apply custom "store" order. */
modules/third_party/brotli/c/enc/compress_fragment.c:  /* TODO(eustas): do only once on initialization. */
modules/third_party/brotli/c/enc/compress_fragment.c:    /* TODO(eustas): could/should full-length machinery be avoided? */
modules/third_party/brotli/c/enc/compress_fragment_two_pass.c:  /* TODO(eustas): initialize once. */
modules/third_party/brotli/c/enc/compress_fragment_two_pass.c:  /* TODO(eustas): is that necessary? */
modules/third_party/brotli/c/enc/compress_fragment_two_pass.c:  /* TODO(eustas): is that necessary? */
modules/third_party/brotli/c/enc/compress_fragment_two_pass.h:/* TODO(eustas): turn to macro. */
modules/third_party/brotli/c/enc/encode.c:  /* TODO(eustas): Validate/clamp parameters here. */
modules/third_party/brotli/c/enc/encode.c:/* TODO(eustas): move to compress_fragment.c? */
modules/third_party/brotli/c/enc/encode.c:  /* TODO(eustas): find more precise minimal block overhead. */
modules/third_party/brotli/c/enc/encode.c:        /* TODO(eustas): pull to higher level and reuse. */
modules/third_party/brotli/c/enc/encode.c:    /* TODO(eustas): Postpone decision until next block arrives? */
modules/third_party/brotli/c/enc/encode.c:    /* TODO(eustas): check that parameters are sane. */
modules/third_party/brotli/c/enc/encoder_dict.c:  /* TODO(lode): consider adding optional user-supplied frequency_map to use
modules/third_party/brotli/c/enc/encoder_dict.c:     TODO(lode): consider a simple flexible uint8_t[10] instead of the uint64_t
modules/third_party/brotli/c/enc/encoder_dict.c:/* TODO(eustas): make sure that tooling will warn user if not all the cutoff
modules/third_party/brotli/c/enc/hash_composite_inc.h:  /* TODO(lode): Initialize of the hashers is deferred to Prepare (and params
modules/third_party/brotli/c/enc/hash_longest_match_quickly_inc.h:  /* TODO: compare 4 bytes at once (and set the minimum best len to 4) */
modules/third_party/brotli/c/enc/histogram.c:    /* TODO(eustas): unwrap iterator blocks. */
modules/third_party/brotli/c/enc/memory.c:  /* TODO(eustas): fine-tune for "many slots" case */
modules/third_party/brotli/c/enc/params.h:  /* TODO(eustas): rename to BrotliShared... */
modules/third_party/brotli/c/enc/quality.h:    /* TODO(eustas): often previous setting (H6) is faster and denser; consider
modules/third_party/brotli/c/enc/static_dict.c:        /* TODO(lode): check for dist overflow */
modules/third_party/brotli/c/tools/brotli.c:  /* TODO: cast name to lower case? */
modules/third_party/brotli/c/tools/brotli.c:   TODO(eustas): this is a "best effort" implementation; honest cross-platform
modules/third_party/cppjieba/deps/limonp/include/limonp/LocalVector.hpp:  LocalVector(const_iterator  begin, const_iterator end) { // TODO: make it faster
modules/third_party/cppjieba/deps/limonp/include/limonp/LocalVector.hpp:  LocalVector(size_t size, const T& t) { // TODO: make it faster
modules/third_party/cppjieba/include/cppjieba/Jieba.hpp:    //TODO
modules/third_party/cppjieba/include/cppjieba/MixSegment.hpp:      // TODO
modules/third_party/cppjieba/include/cppjieba/PreFilter.hpp:  //TODO use WordRange instead of Range
modules/third_party/cppjieba/include/cppjieba/Trie.hpp:  size_t nextPos; // TODO
modules/third_party/httplib/httplib.h:    // TODO: 'Accept-Encoding' has br, not br;q=0
modules/third_party/httplib/httplib.h:    // TODO: 'Accept-Encoding' has gzip, not gzip;q=0
modules/third_party/httplib/httplib.h:    // TODO: check if Content-Length is set
modules/third_party/httplib/httplib.h:    // TODO: Increase FD_SETSIZE statically (libzmq), dynamically (MySQL).
modules/third_party/httplib/httplib.h:            // TODO: refactoring
modules/third_party/httplib/httplib.h:        // TODO this requires a not-entirely-obvious chain of calls to be correct
modules/third_party/httplib/httplib.h:        // TODO: Brotli support
modules/third_party/httplib/httplib.h:        // TODO: Brotli support
modules/third_party/miniz/miniz.cpp:    /* TODO: Better sanity check archive_size and the # of actual remaining bytes */
modules/third_party/miniz/miniz.cpp:    /* TODO: parse local header extra data when local_header_comp_size is 0xFFFFFFFF! (big_descriptor.zip) */
modules/third_party/miniz/miniz.cpp:    /* TODO: We could add a flag that lets the user start writing immediately AFTER the existing central dir - this would be safer. */
modules/third_party/miniz/miniz.cpp:    /* TODO: We could easily maintain the sorted central directory offsets. */
modules/third_party/miniz/miniz.cpp:/* TODO: pArchive_name is a terrible name here! */
modules/third_party/miniz/miniz.cpp:/* TODO: This func is now pretty freakin complex due to zip64, split it up? */
modules/third_party/miniz/miniz.cpp:    /* TODO: We don't support central dir's >= MZ_UINT32_MAX bytes right now (+32 fudge factor in case we need to add more extra data) */
modules/third_party/miniz/miniz.cpp:        /* TODO: Our zip64 support still has some 32-bit limits that may not be worth fixing. */
modules/third_party/miniz/miniz.cpp:        /* TODO: Support central dirs >= 32-bits in size */
modules/third_party/miniz/miniz.cpp:        MZ_WRITE_LE16(hdr + MZ_ZIP64_ECDH_VERSION_MADE_BY_OFS, 0x031E); /* TODO: always Unix */
modules/third_party/miniz/miniz.hpp:/* TODO: Work around "error: include file 'sys\utime.h' when compiling with tcc on Linux */
modules/third_party/miniz/miniz.hpp:/* TODO */
modules/third_party/miniz/miniz.hpp:/* TODO: Perhaps add an option to leave the existing central dir in place in case the add dies? We could then truncate the file (so the old central dir would be at the end) if something goes wrong. */
modules/third_party/nlohmann_json/json.hpp:            get();  // TODO(niels): may we ignore N here?
modules/third_party/pybind11/CMakeLists.txt:# TODO: cmake 3.20 adds the cmake_path() function, which obsoletes this snippet
modules/third_party/pybind11/include/pybind11/detail/common.h:/// Compatibility macros for Python 2 / Python 3 versions TODO: remove
modules/third_party/pybind11/include/pybind11/detail/type_caster_base.h:        // TODO: is this still true for pure Python 3.6?
modules/third_party/pybind11/include/pybind11/eigen/matrix.h:          // TODO: when Eigen bug #747 is fixed, remove the tests for non-negativity.
modules/third_party/pybind11/include/pybind11/eigen/tensor.h:    // TODO: Move to std::optional once std::optional has more support
modules/third_party/pybind11/include/pybind11/pybind11.h:    using module_def = PyModuleDef; // TODO: Can this be removed (it was needed only for Python 2)?
modules/third_party/pybind11/include/pybind11/pybind11.h:        // TODO: Should be reinterpret_steal for Python 3, but Python also steals it again when
modules/third_party/pybind11/include/pybind11/pybind11.h:                        // TODO consolidate the erasure code in pybind11_meta_dealloc() in class.h
modules/third_party/pybind11/include/pybind11/pybind11.h:    // TODO: state captures only the types of Extra, not the values
modules/third_party/pybind11/include/pybind11/pytypes.h:    // TODO PYBIND11_DEPRECATED(
modules/third_party/pybind11/include/pybind11/pytypes.h:// TODO: After the deprecated constructors are removed, this macro can be simplified by
modules/third_party/pybind11/tools/test-pybind11GuessPythonExtSuffix.cmake:# TODO: I'm not sure if this is the right SOABI for PyPy debug builds
modules/third_party/re2/onepass.cpp:                // TODO(rsc): Ignoring kInstAltMatch optimization.
modules/third_party/re2/prefilter_tree.cpp:    // TODO(junyer): Use std::unordered_set<Prefilter*> instead?
modules/third_party/re2/prefilter_tree.cpp:            // this trigger. TODO(vsri): Adjust the threshold appropriately,
modules/third_party/re2/re2.cpp:            // TODO: Should we indicate what the error was?
modules/third_party/roaring_bitmap/roaring.c:            CAST_run(result));  // TODO make lazy
modules/third_party/roaring_bitmap/roaring.c:    // TODO: other cases being lazy, esp. when we know inplace not likely
modules/third_party/roaring_bitmap/roaring.c:// TODO: this could be accelerated, possibly, by using binarySearch4 as above.
modules/third_party/roaring_bitmap/roaring.c:    // TODO: This can likely be faster.
modules/third_party/roaring_bitmap/roaring.c:// TODO: consider keeping `art_t *art` in the iterator.
modules/third_party/roaring_bitmap/roaring.c:    // TODO: could unroll for performance, see bitset_set_list
modules/third_party/roaring_bitmap/roaring.c:/* TODO: consider whether worthwhile to make an asm version */
modules/third_party/roaring_bitmap/roaring.c:	bool iamfirst = true;// TODO: rework so that this is not necessary yet still readable
modules/third_party/roaring_bitmap/roaring.c:	bool iamfirst = true;// TODO: rework so that this is not necessary yet still readable
modules/third_party/roaring_bitmap/roaring.c:// TODO: use the fast lower bound, also
modules/third_party/roaring_bitmap/roaring.c:// TODO: split into run-  array-  and bitset-  subfunctions for sanity;
modules/third_party/roaring_bitmap/roaring.c: * TODO: Util.unsignedDifference does not use advanceUntil.  Is it cheaper
modules/third_party/roaring_bitmap/roaring.c:// TODO: make simplified and optimized negation code across
modules/third_party/roaring_bitmap/roaring.c:    // TODO : try a real inplace version
modules/third_party/roaring_bitmap/roaring.c:    // TODO maybe consider density-based estimate
modules/third_party/roaring_bitmap/roaring.c:    // TODO: efficient special case when range is 0 to 65535 inclusive
modules/third_party/roaring_bitmap/roaring.c:    // TODO: see whether the "2*" is spurious
modules/third_party/roaring_bitmap/roaring.c:    // TODO could optimize for full run containers.
modules/third_party/roaring_bitmap/roaring.c: * TODO: write actual inplace routine if efficiency warrants it
modules/third_party/roaring_bitmap/roaring.c:    // TODO: this could be a lot more efficient
modules/third_party/roaring_bitmap/roaring.c:    // TODO: this could be a lot more efficient
modules/third_party/roaring_bitmap/roaring.c:    // TODO: this could be a lot more efficient, could use SIMD optimizations
modules/third_party/roaring_bitmap/roaring.c:// TODO: write smart_append_exclusive version to match the overloaded 1 param
modules/third_party/roaring_bitmap/roaring.c:            // TODO : perhaps some of the copy_on_write should be based on
modules/third_party/roaring_bitmap/roaring.c:            (uint16_t)i);  // TODO: this introduces extra cloning!
modules/third_party/roaring_bitmap/roaring.c:// TODO: Copy on write.
modules/third_party/roaring_bitmap/roaring.c:// TODO: Error on failed allocation.
modules/third_party/roaring_bitmap/roaring.h: * TODO: decide whether r1 == r2 ok
modules/third_party/roaring_bitmap/roaring.h: * TODO: consider implementing:
modules/third_party/simsimd/include/dot.h:    // TODO: Consider using `_mm512_fmaddsub` and `_mm512_fcmadd_pch`
modules/third_party/simsimd/include/dot.h:    // TODO: Optimize this with tree-like reductions
modules/third_party/simsimd/include/dot.h:    // TODO: Consider using `_mm512_fmaddsub` and `_mm512_fcmadd_pch`
modules/third_party/simsimd/include/spatial.h:    if (n < 32) { // TODO: Avoid eaarly i16 upcast to step through 64 values at a time
modules/third_party/simsimd/lib.c:#if !defined(SIMSIMD_TARGET_SIERRA) && (defined(__linux__)) && 0 // TODO: Add target spec to GCC & Clang
modules/third_party/snappy/snappy-sinksource.cc:  // TODO: Switch to [[maybe_unused]] when we can assume C++17.
modules/third_party/snappy/snappy-sinksource.cc:  // TODO: Switch to [[maybe_unused]] when we can assume C++17.
modules/third_party/snappy/snappy-sinksource.cc:  // TODO: Switch to [[maybe_unused]] when we can assume C++17.
modules/third_party/snappy/snappy-sinksource.cc:  // TODO: Switch to [[maybe_unused]] when we can assume C++17.
modules/third_party/snappy/snappy.cc:  // TODO: Remove pragma when LLVM is aware this
modules/third_party/snappy/snappy.cc:  // TODO: remove annotation and macro defs once MSan is fixed.
modules/third_party/snappy/snappy.cc:        // TODO: Ideally we should memset, move back once the
modules/third_party/snappy/snappy.cc:      // TODO: Replace with loop with trip count hint.
modules/third_party/snappy/snappy.cc:      // TODO: Remove pragma when when cold regions don't get
modules/third_party/snappy/snappy.cc:    // TODO: Replace with loop with trip count hint.
modules/third_party/snappy/snappy.cc:  // TODO: Remove pragma when when cold regions don't get vectorized
modules/third_party/snappy/snappy.cc:  // TODO: Switch to [[maybe_unused]] when we can assume C++17.
modules/third_party/snappy/snappy.cc:  // TODO: Switch to [[maybe_unused]] when we can assume C++17.
modules/third_party/snappy/snappy.cc:  // TODO: Investigate wider copies on other platforms.
modules/third_party/snappy/snappy.cc:  // TODO: Switch to [[maybe_unused]] when we can assume C++17.
modules/third_party/snappy/snappy.cc:  // TODO clang misses the fact that the (c & 3) already correctly
modules/third_party/snappy/snappy.cc:  // TODO
modules/third_party/snappy/snappy.cc:  // TODO Clang is "optimizing" zero-extension (a totally free
modules/third_party/snappy/snappy.cc:  // TODO: Once the array is recognized as a register, improve the
modules/third_party/snappy/snappy.cc:// TODO This function probably does not need to be inlined, as it
modules/third_party/snappy/snappy.cc:    // TODO: Switch to [[maybe_unused]] when we can assume C++17.
modules/third_party/snappy/snappy.cc:    // TODO: Switch to [[maybe_unused]] when we can assume C++17.
modules/third_party/snappy/snappy.cc:    // TODO: Switch to [[maybe_unused]] when we can assume C++17.
modules/third_party/snappy/snappy.cc:  // TODO Replace this with a properly optimized path. This is not
modules/third_party/snappy/snappy.cc:    // TODO: Switch to [[maybe_unused]] when we can assume C++17.
modules/third_party/snappy/snappy.cc:  // TODO: Remove pragma when LLVM is aware this
modules/third_party/snappy/snappy.cc:      // TODO: remove annotation and macro defs once MSan is fixed.
modules/third_party/snappy/snappy.cc:    // TODO: Simplify this code when the compiler reliably produces
modules/third_party/snappy/snappy.cc:    // TODO: Replace with loop trip count hint.
modules/third_party/snappy/snappy.cc:  // TODO: Remove pragma when when cold regions don't get vectorized
modules/third_party/taywee_args/include/args.hxx:                                //TODO: support -abcVALUE where a and b take no value
modules/third_party/thrift/TToString.h:// TODO: replace the computations below with std::numeric_limits::max_digits10 once C++11
modules/third_party/thrift/protocol/TProtocol.h:  // TODO: remove these two calls, they are for backwards
modules/third_party/thrift/transport/TBufferTransports.h:      // refetch rBase_ from memory.  TODO(dreiss): Verify this.
modules/third_party/thrift/transport/TBufferTransports.h:    // TODO(dreiss): Investigate NULL-ing this if !owner.
modules/third_party/thrift/transport/TBufferTransports.h:  // TODO(dreiss): Make bufPtr const.
modules/third_party/thrift/transport/TTransport.h:   * TODO(dreiss): Is there any transport that could borrow but fail to
modules/third_party/yyjson/README.md:# TODO for v1.0
modules/third_party/zstd/compress/zstd_compress.cpp:            /* TODO: avoid memset? */
modules/third_party/zstd/compress/zstd_compress.cpp:            /* TODO: avoid memset? */
modules/third_party/zstd/compress/zstd_compress_superblock.cpp:    BYTE hufDesBuffer[500]; /* TODO give name to this value */
modules/third_party/zstd/compress/zstd_compress_superblock.cpp:    BYTE fseTablesBuffer[500]; /* TODO give name to this value */
modules/third_party/zstd/compress/zstd_compress_superblock.cpp:    (void)bmi2; /* TODO bmi2... */
modules/third_party/zstd/compress/zstd_compress_superblock.cpp:    /* TODO bmi2 */
modules/third_party/zstd/compress/zstd_ldm.cpp:         * TODO: * Test the chunk size.
modules/third_party/zstd/include/zstd/compress/zstd_cwksp.h:            /* TODO: cleaner, compiler warning friendly way to do this??? */
src/binder/bind/bind_import_database.cpp:    // TODO(Ziyi): This is a temporary workaround because our parser requires input cypher queries
src/binder/bind/bind_projection_clause.cpp:            // TODO(Xiyang): we can remove augment group by. But make sure we test sufficient
src/binder/bind_expression/bind_function_expression.cpp:            // TODO(Xiyang): We should return a deep copy otherwise the same expression might
src/binder/bind_expression/bind_property_expression.cpp:    // TODO(Xiyang): we should be able to remove l97-l100 after removing propertyDataExprs from node
src/binder/binder.cpp:    // TODO(Xiyang): assert name not in scope.
src/binder/binder.cpp:    // Note to Xiyang: I don't think the TODO still stands here. I tried adding the assertion, but
src/binder/binder.cpp:    // it failed a few tests. You may want to revisit this TODO.
src/c_api/value.cpp:// TODO: bind all int128_t supported functions
src/common/arrow/arrow_array_scan.cpp:            // TODO pure time type
src/common/arrow/arrow_row_batch.cpp:    // TODO(Guodong): make this as a function.
src/common/arrow/arrow_type.cpp:            // TODO implement pure time type
src/common/arrow/arrow_type.cpp:            // TODO maxwell: timezone support
src/common/types/int128_t.cpp:    // TODO: Maybe need to add func isFinite in value.h to see if every type is finite.
src/common/types/timestamp_t.cpp:// TODO(Kebing): will add the tests in the timestamp PR
src/common/types/types.cpp:    // TODO(Ziy): Use UINT8 to represent tag value.
src/function/base_lower_upper_operation.cpp:                // TODO(Xiyang): We shouldn't allow invalid UTF-8 to enter a string column.
src/function/built_in_function_utils.cpp:    // TODO(Jiamin): should check any type
src/function/cast_from_string_functions.cpp:    // TODO: escape char
src/function/export/export_parquet_function.cpp:    // TODO(Ziyi): We should let factorizedTable::append return the numTuples appended.
src/function/gds/algo/betweenness_centrality.cpp:    // TODO: Use vertex compute for this.
src/function/gds/algo/betweenness_centrality.cpp:    // TODO: Use vertex compute for this.
src/function/gds/algo/betweenness_centrality.cpp:// TODO: Use function from https://github.com/gorgonzoladb/gorgonzola/pull/5916
src/function/json/table_functions/json_scan.cpp:            // TODO(Ziyi): We should make the maximum object size as a configurable option.
src/function/json/table_functions/json_scan.cpp:    // TODO(Ziyi): fix this.
src/function/list/list_filter.cpp:            // TODO(Royi) make the output pos respect resultSelVector
src/function/table/show_sequences.cpp:    // TODO: uncomment this when we can test it
src/function/vector_cast_functions.cpp:    // TODO(Jiamin): there are still other special cases
src/function/vector_cast_functions.cpp:    // TODO(Xiyang): Can we unify the binding of casting function with other scalar functions?
src/function/vector_hash_functions.cpp:    // TODO(Ziyi): Allow selection size to be greater than default vector capacity, so we don't have
src/function/vector_hash_functions.cpp:    // TODO(Ziyi): evaluators should resolve the state for result vector.
src/graph/on_disk_graph.cpp:    // TODO(bmwinger): If there are both a predicate and a custom edgePropertyIndex, they will
src/graph/on_disk_graph.cpp:// TODO(Xiyang): since now we need to provide nbr info at prepare stage. It no longer make sense to
src/include/binder/expression_binder.h:    // TODO(Xiyang): move to an expression rewriter
src/include/c_api/gorgonzola.h:// TODO(Chang): Refactor the datatype constructor to follow the cpp way of creating dataTypes.
src/include/catalog/catalog_entry/rel_group_catalog_entry.h:    // TODO(Guodong): Avoid using extend direction for storage direction
src/include/common/copier_config/csv_reader_config.h:    // TODO(Xiyang): Add newline character option and delimiter can be a string.
src/include/common/copier_config/csv_reader_config.h:    // TODO: COPY FROM and COPY TO should support transform special options, like '\'.
src/include/common/data_chunk/data_chunk_collection.h:// TODO(Guodong): Should rework this to use ColumnChunk.
src/include/common/data_chunk/data_chunk_state.h:    // TODO: We should get rid of `fStateType` and merge DataChunkState with SelectionVector.
src/include/common/types/types.h:    // TODO(Royi) remove this when float compression no longer relies on this or ColumnChunkData
src/include/common/types/value/value.h:        // TODO(Ziyi): Should we remove the val suffix from all values in Val? Looks redundant.
src/include/common/vector/value_vector.h:    // TODO(Guodong): Rename this to getValueRef
src/include/common/vector/value_vector.h:    // TODO(Xiyang): try to merge setDataVector & copyListEntryAndBufferMetaData
src/include/function/built_in_function_utils.h:    // TODO(Ziyi): We should have a unified interface for matching table, aggregate and scalar
src/include/function/built_in_function_utils.h:    // TODO(Xiyang): move casting cost related functions to binder.
src/include/function/cast/cast_function_bind_data.h:    // TODO(Mahn): the following field should be removed once we refactor fixed list.
src/include/function/cast/functions/cast_string_non_nested_functions.h:    // TODO(Kebing): handle decimals
src/include/function/cast/functions/cast_string_non_nested_functions.h:// TODO(Kebing): support exponent + decimal
src/include/function/function.h:    // TODO: the following two fields should be moved to FunctionLocalState.
src/include/function/gds/bfs_graph.h:// TODO(Xiyang): optimize if edgeID is not needed.
src/include/function/table/scan_file_function.h:    uint64_t totalSize; // TODO(Mattias): I think we should unify the design on how we calculate the
src/include/function/table/table_function.h:    // TODO(Guodong): This is not a good way to pass semiMasks to QueryHNSWIndex function.
src/include/function/unary_function_executor.h:        // TODO(Ziyi): the reinterpret_cast is not safe since we don't always pass
src/include/graph/graph.h:            // TODO(bmwinger): avoid scanning if all that's necessary is to count the results
src/include/main/storage_driver.h:    // TODO: Should merge following two functions into a single one.
src/include/optimizer/logical_operator_collector.h:// TODO(Xiyang): Rename me.
src/include/planner/join_order/cardinality_estimator.h:    // TODO(Guodong): Extend this to cover rel tables.
src/include/planner/operator/extend/logical_recursive_extend.h:    common::offset_t limitNum; // TODO: remove this once recursive extend is pipelined.
src/include/planner/operator/logical_hash_join.h:    // TODO(Guodong): when the build side has only flat payloads, we should consider getting rid of
src/include/planner/operator/logical_operator.h:    // TODO: remove this function once planner do not share operator across plans
src/include/planner/operator/sip/side_way_info_passing.h:    // TODO(Xiyang/Guodong): Temp hack to allow vector index search to pass semi mask.
src/include/planner/operator/sip/side_way_info_passing.h: * TODO(Xiyang): it worth thinking if we should simply put outer plan always on the build side.
src/include/processor/expression_mapper.h:    // TODO: comment
src/include/processor/operator/intersect/intersect.h:    // TODO(Xiyang): payload is not an accurate name for intersect.
src/include/processor/operator/order_by/order_by.h:        // TODO(Ziyi): we always call lookup function on the first factorizedTable in sharedState
src/include/processor/operator/partitioner.h:    // TODO: For now, RelBatchInsert will guarantee all data are inside one data chunk. Should be
src/include/processor/operator/persistent/reader/csv/driver.h:// TODO(Keenan): Split up this file.
src/include/processor/operator/persistent/reader/parquet/parquet_dbp_decoder.h:                    // TODO what happens if width is 0?
src/include/processor/operator/persistent/reader/parquet/parquet_reader.h:    // TODO(Ziyi): We currently only support reading from local file system, thus the prefetch
src/include/processor/operator/persistent/reader/parquet/parquet_rle_bp_decoder.h:        // TODO complain if we run out of buffer
src/include/processor/operator/persistent/writer/parquet/column_writer.h:    // TODO(Ziyi): We currently don't have statistics to indicate whether a column
src/include/processor/operator/standalone_call.h:    // TODO: we should remove this.
src/include/processor/result/factorized_table.h:    // TODO(Guodong): Unify these two interfaces along with `readUnflatCol`.
src/include/processor/result/factorized_table.h:    // TODO(Guodong): Unify these two `readUnflatCol()` with a (possibly templated) copy executor.
src/include/processor/result/factorized_table_schema.h:// TODO(Guodong/Ziyi): Move these typedef to common and unify them with the ones without `ft_`.
src/include/processor/result/factorized_table_util.h:    // TODO(Ziyi): These two functions are used to store the copy message in a factorizedTable
src/include/storage/buffer_manager/page_state.h:        // TODO(Keenan / Guodong): Track down this rare bug and re-enable the assert. Ref #2289.
src/include/storage/buffer_manager/page_state.h:        // TODO(Keenan / Guodong): Track down this rare bug and re-enable the assert. Ref #2289.
src/include/storage/disk_array.h:// TODO(bmwinger): this should use the memoryManager
src/include/storage/disk_array.h:        // TODO(bmwinger): Instead of pinning the page and updating in-place, it might be better to
src/include/storage/index/hash_index_header.h:    // TODO(bmwinger): Make use of this in the on-disk hash index
src/include/storage/index/in_mem_hash_index.h:    // TODO: might be more efficient to use a vector for each slot since this is now only needed
src/include/storage/local_storage/local_table.h:    // TODO(Guodong): Revisit this interface. We don't need to pass in Table here, instead should
src/include/storage/shadow_file.h:    // TODO(Guodong): Remove originalFile param.
src/include/storage/table/column.h:    // TODO(Guodong): This function should be removed through rewriting INTERNAL_ID as STRUCT.
src/include/storage/table/column_chunk.h:    // TODO(bmwinger): try binary search (might only make a difference for a very large number
src/include/storage/table/column_chunk.h:    // TODO(bmwinger): the above function should be const and only isn't because of ALP exception
src/include/storage/table/column_chunk.h:    // TODO(bmwinger): Segments could probably share a single datatype
src/include/storage/table/column_chunk.h:    // TODO(bmwinger): This is not ideal; it's just a workaround for storage_info
src/include/storage/table/column_chunk.h:    // TODO(Guodong): This field should be removed. Ideally it shouldn't be cached anywhere in
src/include/storage/table/column_chunk_data.h:// TODO(bmwinger): Hide access to variables.
src/include/storage/table/column_chunk_data.h:    // TODO(Guodong): In general, this is not a good interface. Instead of passing in
src/include/storage/table/column_chunk_data.h:    // TODO(Guodong): Alternatively, we can let `getNumValues` read from metadata when ON_DISK.
src/include/storage/table/column_chunk_data.h:        // TODO(bmwinger): this should use the inMemoryStats to avoid scanning the data, however not
src/include/storage/table/column_chunk_data.h:    // TODO(bmwinger): This was added to work around the issue of complex nested types having a
src/include/storage/table/column_chunk_data.h:    // TODO(Guodong): Should make InternalIDChunkData has no NULL.
src/include/storage/table/column_chunk_metadata.h:    // TODO(Guodong): Delete copy constructor.
src/include/storage/table/csr_node_group.h:    // TODO(Guodong): Should seperate `isSequential` and `rowIndices` to two different data
src/include/storage/table/csr_node_group.h:    row_idx_vec_t rowIndices; // TODO(Guodong): Should optimze the vector to a more space-efficient
src/include/storage/table/csr_node_group.h:// TODO(Guodong): Split CSRIndex into two levels: one level per csr leaf region, another per node
src/include/storage/table/csr_node_group.h:// TODO(Guodong): Serialize the info to disk. This should be a config per node group.
src/include/storage/table/node_group.h:    // TODO(Guodong): These should be merged together with `lookup`.
src/include/storage/table/node_group.h:    // TODO(Guodong): Remove this field.
src/include/storage/table/node_table.h:    // TODO(Guodong): This should be merged together with `lookup`.
src/include/storage/table/rel_table.h:    // TODO(Guodong): Copy of rowIndices here is only to simplify the implementation. We can always
src/include/storage/table/table.h:    // TODO(Guodong): Remove this when we have a better way to skip WAL logging for FTS.
src/include/storage/table/table.h:    // TODO(Guodong): Remove this when we have a better way to skip WAL logging for FTS.
src/include/transaction/transaction.h:// TODO(bmwinger): These shouldn't need to be exported
src/main/query_result.cpp:    // TODO(Xiyang): consider introduce error query result class.
src/main/settings.cpp:    // TODO: This is a temporary solution to make tests of multiple write transactions easier.
src/optimizer/acc_hash_join_optimizer.cpp:    // TODO(Xiyang): Check if a semi mask can/need to be applied to ScanNodeTable, RecursiveJoin &
src/optimizer/acc_hash_join_optimizer.cpp:    // TODO(Xiyang): this may not be the best solution. Most of the time we will pass a semi mask
src/optimizer/acc_hash_join_optimizer.cpp:// TODO(Xiyang): we don't apply SIP from build to probe.
src/optimizer/factorization_rewriter.cpp:    // TODO(Royi) correctly set the cardinality here
src/optimizer/filter_push_down_optimizer.cpp:        // TODO(Xiyang): this can only rewrite left = right, we should also be able to do
src/optimizer/filter_push_down_optimizer.cpp:    // TODO(Xiyang): fold parameter expression in binder.
src/optimizer/projection_push_down_optimizer.cpp:        // TODO(Xiyang): replace this with a separate optimizer.
src/optimizer/remove_unnecessary_join_optimizer.cpp:    // TODO(Xiyang): Double check on these changes here.
src/optimizer/top_k_optimizer.cpp:// TODO(Xiyang): we should probably remove the projection between ORDER BY and MULTIPLICITY REDUCER
src/planner/join_order/cardinality_estimator.cpp:    // TODO(Royi) we can use HLL to better estimate the number of distinct keys here
src/planner/join_order/cost_model.cpp:    // TODO(Xiyang): think of how to calculate intersect cost such that it will be picked in worst
src/planner/join_order/join_plan_solver.cpp:    // TODO(Xiyang): provide an interface to append operator to resultPlan.
src/planner/operator/logical_hash_join.cpp:        return true; // TODO(Guodong): fix this. We shouldn't require flatten.
src/planner/plan/plan_join_order.cpp:        // TODO(Xiyang): Fixme according to the description above.
src/planner/plan/plan_port_db.cpp:    // TODO(Ziyi): Shouldn't these be done in Binder?
src/planner/plan/plan_subquery.cpp:        // TODO(Xiyang): there is a question regarding if we want to plan as a correlated subquery
src/processor/map/map_copy_to.cpp:    // TODO(Xiyang): Query: COPY (RETURN null) TO '/tmp/1.parquet', the datatype of the first
src/processor/map/map_dummy_scan.cpp:    // TODO(Ziyi): remove vectors when we have done the refactor of dataChunk.
src/processor/operator/aggregate/base_aggregate.cpp:                // TODO(bmwinger): if the queuedTuples has at least a certain size (benchmark to see
src/processor/operator/aggregate/hash_aggregate.cpp:        // TODO(bmwinger): ideally these can be merged into a single function.
src/processor/operator/flatten.cpp:        dataChunkState->setToUnflat(); // TODO(Xiyang): this should be part of restore/save
src/processor/operator/hash_join/hash_join_probe.cpp:        // TODO(Guodong): we have potential bugs here because all keys' states should be restored.
src/processor/operator/hash_join/hash_join_probe.cpp:        // TODO(Xiyang): We have a bug in LEFT JOIN which should not discard NULL keys. To be more
src/processor/operator/hash_join/join_hash_table.cpp:        // TODO(Xiyang): I can no longer recall why I set to un-filtered but this is probably wrong.
src/processor/operator/index_lookup.cpp:// TODO(Guodong): Add short path for unfiltered case.
src/processor/operator/index_lookup.cpp:// TODO(Guodong): Add short path for unfiltered case.
src/processor/operator/intersect/intersect.cpp:            // TODO(Guodong): Remove this assumption so that keys can be stored in any order. Change
src/processor/operator/order_by/order_by_merge.cpp:    // TODO(Ziyi): directly feed sharedState to merger and dispatcher.
src/processor/operator/order_by/sort_state.cpp:        // TODO(Ziyi): This is a hacky way of using factorizedTable::lookup function,
src/processor/operator/path_property_probe.cpp:// TODO(Xiyang): revisit me. Instead of printing in src->dst order. Maybe left->right order make
src/processor/operator/persistent/insert_executor.cpp:// TODO(Guodong/Xiyang): think we can reference data vector instead of copy.
src/processor/operator/persistent/merge.cpp:    // TODO: remove types
src/processor/operator/persistent/node_batch_insert.cpp:    // TODO(bmwinger): Can probably re-use the chunk and shift the values
src/processor/operator/persistent/reader/csv/base_csv_reader.cpp:// TODO(Royi) for performance reasons we may want to reduce the number of fields here since each
src/processor/operator/persistent/reader/npy/npy_reader.cpp:    // TODO(Guodong): Set npy reader data type to ARRAY, so we can simplify checks here.
src/processor/operator/persistent/reader/npy/npy_reader.cpp:    auto reader = NpyReader(fileScanInfo.filePaths[fileIdx]); // TODO: double check
src/processor/operator/persistent/reader/parquet/column_reader.cpp:    // TODO this can be optimized, for example we dont actually have to bitunpack offsets
src/processor/operator/persistent/reader/parquet/column_reader.cpp:            // TODO keep this in the state
src/processor/operator/persistent/reader/parquet/column_reader.cpp:        // TODO somehow dict_width can be 0 ?
src/processor/operator/persistent/reader/parquet/parquet_reader.cpp:        // TODO check return value of derive type or should we only do this on read()
src/processor/operator/persistent/reader/parquet/parquet_reader.cpp:            // TODO(Ziyi): Support parquet copy option(binary_as_string).
src/processor/operator/persistent/rel_batch_insert.cpp:    // TODO(Guodong): Get rid of the hard-coded nbr and rel column ID 0/1.
src/processor/operator/persistent/rel_batch_insert.cpp:        // TODO(Guodong): We need to handle the concurrency between COPY and other insertions
src/processor/operator/result_collector.cpp:        // TODO(Ziyi): add an interface in factorized table
src/processor/operator/simple/export_db.cpp:    // TODO(Ziyi): We should pass fileName from binder phase to here.
src/processor/operator/simple/export_db.cpp:        // TODO(Ziyi): We should pass fileName from binder phase to here.
src/processor/operator/simple/import_db.cpp:    // TODO(Guodong): this is special for "Import database". Should refactor after we support
src/processor/result/factorized_table.cpp:        // TODO: It always adds to the end, so this will leave empty blocks in the middle if it's
src/processor/result/factorized_table.cpp:        // TODO(Xiyang/Ziyi): we should set up a rule about when to reset. Should it be in operator?
src/processor/result/factorized_table.cpp:// TODO(Guodong): change this function to not use dataChunkPos in ColumnSchema.
src/storage/buffer_manager/buffer_manager.cpp:    // TODO(bmwinger): It may be better to spill to disk in a different location for remote file
src/storage/buffer_manager/vm_region.cpp:    // TODO: VirtualAlloc(..., MEM_RESET, ...) may be faster
src/storage/compression/compression.cpp:        // TODO(bmwinger): This could be optimized. We could do bytewise comparison with memcmp,
src/storage/compression/compression.cpp:    // TODO(bmwinger): this is hacky; we need a better system for dynamically choosing between
src/storage/compression/compression.cpp:    // TODO(bmwinger): Optimize, e.g. using an integer bitpacking function
src/storage/compression/compression.cpp:    // TODO(bmwinger): Optimize, e.g. using an integer bitpacking function
src/storage/compression/compression.cpp:        // TODO(bmwinger): concept for supported storagevalue types
src/storage/database_header.cpp:        // TODO(Guodong): Add a test case for this.
src/storage/disk_array.cpp:    // TODO: We are currently supporting only DiskArrays that can grow in size and not
src/storage/index/hash_index.cpp:                // TODO(Guodong/Ben): FIX-ME. We should vacuum the index during checkpoint.
src/storage/index/hash_index.cpp:    // TODO: Ideally we can split slots at the same time that we insert new ones
src/storage/index/hash_index.cpp:    // TODO: one pass would also reduce locking when frames are unpinned,
src/storage/index/hash_index.cpp:    // TODO: Use a separate random access iterator and one that's sequential for adding new overflow
src/storage/index/hash_index.cpp:    // TODO: Unify implementations to make sure this matches the size used by the disk array
src/storage/index/hash_index.cpp:    // TODO(Guodong): Fix this assertion statement which doesn't count the entries in
src/storage/index/hash_index.cpp:    // TODO: Should eventually be moved into the disk array when the disk array can
src/storage/index/in_mem_hash_index.cpp:    // TODO(bmwinger): If we split slots backwards instead of forwards we would need to reserve just
src/storage/overflow_file.cpp:    // TODO(bmwinger): Ideally this could be done separately and in parallel by each HashIndex
src/storage/shadow_file.cpp:    // TODO(Guodong): We should remove shadow file here. This requires changes:
src/storage/storage_manager.cpp:// TODO(Guodong): This API is added since storageManager doesn't provide an API to add a single
src/storage/table/chunked_node_group.cpp:            // TODO(bmwinger): This should be removed when splitting works predictively instead of
src/storage/table/column.cpp:// TODO: Do we need to adapt the offsets to this current node group?
src/storage/table/column.cpp:    // TODO(bmwinger): either have all writes update numValues, or have writeSegment update it
src/storage/table/column.cpp:    // TODO(bmwinger): for simple compression types, we can predict whether or not we will need to
src/storage/table/column_chunk.cpp:        // TODO: Replace with a function that modifies the existing stats in-place?
src/storage/table/column_chunk_data.cpp:    // TODO(Guodong): Better let NullChunkData also support `append` a
src/storage/table/column_chunk_data.cpp:    // TODO: FIX-ME. enableCompression.
src/storage/table/csr_chunked_node_group.cpp:    // TODO(Guodong): I think we can simplify the check here by getting rid of some of the
src/storage/table/csr_chunked_node_group.cpp:    // TODO(Guodong): I think we can simplify the check here by getting rid of some of the
src/storage/table/csr_chunked_node_group.cpp:    // TODO(bmwinger): maybe there's a way of also vectorizing this for the length chunk, E.g. a
src/storage/table/csr_chunked_node_group.cpp:    // TODO(bmwinger): should be able to vectorize this somewhat
src/storage/table/csr_chunked_node_group.cpp:    // TODO(Guodong): Rework to reuse ChunkedNodeGroup::deserialize().
src/storage/table/csr_chunked_node_group.cpp:    // TODO(Guodong): I think we can simplify the check here by getting rid of some of the
src/storage/table/csr_chunked_node_group.cpp:    // TODO(Guodong): I think we can simplify the check here by getting rid of some of the
src/storage/table/csr_node_group.cpp:    // TODO(Guodong): Should skip early here if no changes in the node group, so we avoid scanning
src/storage/table/csr_node_group.cpp:    // TODO(Guodong): Find max node offset in the node group.
src/storage/table/csr_node_group.cpp:    // TODO(Guodong): Optimize the for loop away by appending in batch
src/storage/table/csr_node_group.cpp:            // TODO(Guodong): Optimize here. if no deletions and has sequential rows, scan in
src/storage/table/csr_node_group.cpp:                // TODO(Guodong): Should skip deleted rows here.
src/storage/table/csr_node_group.cpp:    // TODO(Guodong): Use `finalizeCheckpoint`.
src/storage/table/dictionary_column.cpp:    // TODO(bmwinger): scan batches of adjacent values.
src/storage/table/node_group.cpp:    // TODO(Guodong): Move the locked part of figuring out the chunked group to initScan.
src/storage/table/node_group.cpp:            // TODO(Guodong): Optimize the for loop here to directly acess the version info.
src/storage/table/node_group_collection.cpp:            // TODO(Guodong): Further optimize on this. Should directly figure out startRowIdx to
src/storage/table/node_table.cpp:            // TODO(Guodong): Assume local storage is small here. Should optimize the loop away by
src/storage/table/rel_table.cpp:        // TODO(Royi/Guodong) remove this once delete_() supports unflat vectors
src/storage/table/string_column.cpp:    // TODO: Replace indices with ValueVector to avoid maintaining `scan` interface from
src/storage/table/string_column.cpp:            // TODO(bmwinger): optimize index scans by grouping them when adjacent
src/storage/table/struct_column.cpp:    // TODO(bmwinger): eventually this shouldn't be necessary
src/storage/table/struct_column.cpp:    // TODO(bmwinger): child columns are now handled as a group so they get split together
src/storage/table/update_info.cpp:    // TODO(Guodong): This will be optimized by moving VectorUpdateInfo into UndoBuffer.
src/storage/table/update_info.cpp:            // TODO(Guodong): Ideally we should make sure vecUpdateInfo.rowsInVector is sorted to
src/storage/table/version_info.cpp:    // TODO(Guodong): ALWAYS_INSERTED is not added for now, but it may be useful as an optimization
src/storage/table/version_info.cpp:    // TODO: Keep an additional same insertion/deletion field as an optimization to avoid the need
tools/shell/embedded_shell.cpp:// TODO(Ziyi): Move the printing logic under each printer and each printer should expose a unified
tools/shell/include/printer/printer.h:// TODO(Ziyi): Refactor printers to separate files.
tools/shell/printer/json_printer.cpp:// TODO(Ziyi): I'm inclined to move this as a QueryResult interface.
```
