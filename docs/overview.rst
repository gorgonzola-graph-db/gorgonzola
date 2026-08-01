Overview & Features
===================

Gorgonzola is an embedded, serverless graph database engineered for analytical query speed and scalability.
It is a clean fork of `Kùzu <https://github.com/kuzudb/kuzu>`_—a project that has since been archived by its original authors—and carries the codebase forward with C/C++ API extensions and performance improvements in a unified, maintainable form.

Core Features
-------------

* **Property Graph Model** — Flexible node and relationship schema supporting the Cypher query language.
* **Embeddable & Serverless** — Link directly into your application binary with no separate server process required.
* **Full-Text Search** — Native FTS index for text-heavy workloads.
* **Vector Index** — Built-in HNSW index for approximate nearest-neighbor search.
* **Columnar Storage** — Disk-based columnar layout optimized for fast analytical scans.
* **CSR Join Indices** — Columnar Sparse Row-based adjacency list for rapid graph traversal.
* **Vectorized Query Engine** — Factorized, vectorized execution with novel join algorithms.
* **Multi-core Parallelism** — Parallel query execution across all available CPU cores.
* **ACID Transactions** — Full serializable transaction support.
* **WebAssembly Bindings** — Run Gorgonzola directly in the browser via WebAssembly (Wasm).

Improvements in Gorgonzola
--------------------------

Gorgonzola maintains API compatibility with Kùzu while incorporating several key improvements and extensions:

C/C++ API Extensions
~~~~~~~~~~~~~~~~~~~~

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Feature
     - Description
   * - ``uint128_t`` Support
     - Full 128-bit unsigned integer type alongside ``int128_t`` and ``uuid``, complete with arithmetic, comparison, and cast operators.
   * - ``gorgonzola_value_get_blob_with_size``
     - Null-byte-safe BLOB retrieval. Returns the complete binary buffer along with its byte length, resolving issues where binary data containing ``\0`` was truncated.

Cypher Language Enhancements
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* **Leading Zeros in Numeric Literals**: Numbers like ``00123`` are recognized as valid integer literals (previously rejected by the parser grammar).
* **Standardized Vector Functions**: Consistent function naming and argument ordering across all vector and array operations.
