/**
 * @file gorgonzola.js is the internal wrapper for the WebAssembly module.
 */
const gorgonzola_wasm = require("../gorgonzola/gorgonzola_wasm.js");

class gorgonzola {
  constructor() {
    this._gorgonzola = null;
  }

  async init() {
    this._gorgonzola = await gorgonzola_wasm();
  }

  checkInit() {
    if (!this._gorgonzola) {
      throw new Error("The WebAssembly module is not initialized.");
    }
  }

  getVersion() {
    this.checkInit();
    return this._gorgonzola.getVersion();
  }

  getStorageVersion() {
    this.checkInit();
    return this._gorgonzola.getStorageVersion();
  }

  getFS() {
    this.checkInit();
    return this._gorgonzola.FS;
  }

  getWasmMemory() {
    this.checkInit();
    return this._gorgonzola.wasmMemory;
  }
}

const gorgonzolaInstance = new gorgonzola();
module.exports = gorgonzolaInstance;
