/**
 * @file index.js is the root file for the synchronous version of Gorgonzola
 * WebAssembly module. It exports the module's public interface.
 */
"use strict";

const GorgonzolaWasm = require("./gorgonzola.js");
const Database = require("./database.js");
const Connection = require("./connection.js");
const PreparedStatement = require("./prepared_statement.js");
const QueryResult = require("./query_result.js");

/**
 * The synchronous version of Gorgonzola WebAssembly module.
 * @module gorgonzola-wasm
 */
module.exports = {
  /**
   * Initialize the Gorgonzola WebAssembly module.
   * @memberof module:gorgonzola-wasm
   * @returns {Promise<void>} a promise that resolves when the module is 
   * initialized. The promise is rejected if the module fails to initialize.
   */
  init: () => {
    return GorgonzolaWasm.init();
  },

  /**
   * Get the version of the Gorgonzola WebAssembly module.
   * @memberof module:gorgonzola-wasm
   * @returns {String} the version of the Gorgonzola WebAssembly module.
   */
  getVersion: () => {
    return GorgonzolaWasm.getVersion();
  },

  /**
   * Get the storage version of the Gorgonzola WebAssembly module.
   * @memberof module:gorgonzola-wasm
   * @returns {BigInt} the storage version of the Gorgonzola WebAssembly module.
   */
  getStorageVersion: () => {
    return GorgonzolaWasm.getStorageVersion();
  },
  
  /**
   * Get the standard emscripten filesystem module (FS). Please refer to the 
   * emscripten documentation for more information.
   * @memberof module:gorgonzola-wasm
   * @returns {Object} the standard emscripten filesystem module (FS).
   */
  getFS: () => {
    return GorgonzolaWasm.getFS();
  },

  /**
   * Get the WebAssembly memory. Please refer to the emscripten documentation 
   * for more information.
   * @memberof module:gorgonzola-wasm
   * @returns {Object} the WebAssembly memory object.
   */
  getWasmMemory: () => {
    return GorgonzolaWasm.getWasmMemory();
  },

  Database,
  Connection,
  PreparedStatement,
  QueryResult,
};
