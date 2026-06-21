/**
 * This file is a customized loader for the gorgonzolajs.node native module.
 * It is used to load the native module with the correct flags on Linux so that
 * extension loading works correctly.
 * @module gorgonzola_native
 * @private
 */

const process = require("process");
const constants = require("constants");
const join = require("path").join;

const gorgonzolaNativeModule = { exports: {} };
const modulePath = join(__dirname, "gorgonzolajs.node");
if (process.platform === "linux") {
  process.dlopen(
    gorgonzolaNativeModule,
    modulePath,
    constants.RTLD_LAZY | constants.RTLD_GLOBAL
  );
} else {
  process.dlopen(gorgonzolaNativeModule, modulePath);
}

module.exports = gorgonzolaNativeModule.exports;
