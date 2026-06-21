const { assert } = require("chai");

describe("Get version", function () {
  it("should get the version of the library", function () {
    assert.isString(gorgonzola.VERSION);
    assert.notEqual(gorgonzola.VERSION, "");
  });

  it("should get the storage version of the library", function () {
    assert.isNumber(gorgonzola.STORAGE_VERSION);
    assert.isAtLeast(gorgonzola.STORAGE_VERSION, 1);
  });
});
