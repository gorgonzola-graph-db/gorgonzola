const fs = require("fs");
const path = require("path");
const process = require("process");

const GORGONZOLA_WASM_INDEX_PATH = path.join(__dirname, "node_modules", "gorgonzola-wasm", "index.js");
const GORGONZOLA_WASM_WORKER_PATH = path.join(__dirname, "node_modules", "gorgonzola-wasm", "gorgonzola_wasm_worker.js");
const DESTINATION_PATH = path.join(__dirname, "public");

if (!fs.existsSync(GORGONZOLA_WASM_INDEX_PATH) || !fs.existsSync(GORGONZOLA_WASM_WORKER_PATH)) {
    console.log("Gorgonzola WebAssembly module not found. Please run `npm i` to install the dependencies.");
    process.exit(1);
}

console.log("Copying Gorgonzola WebAssembly module to public directory...");
console.log(`Copying ${GORGONZOLA_WASM_INDEX_PATH} to ${DESTINATION_PATH}...`);
fs.copyFileSync(GORGONZOLA_WASM_INDEX_PATH, path.join(DESTINATION_PATH, "index.js"));
console.log(`Copying ${GORGONZOLA_WASM_WORKER_PATH} to ${DESTINATION_PATH}...`);
fs.copyFileSync(GORGONZOLA_WASM_WORKER_PATH, path.join(DESTINATION_PATH, "gorgonzola_wasm_worker.js"));
console.log("Done.");
