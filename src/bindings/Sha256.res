@module("js-sha256") @scope("sha256") @val
external _digest: JsBuffer.t => array<int> = "array"
let digest = arr => arr->JsBuffer.from->_digest
let hexDigest = arr => arr->digest->JsBuffer.arrayToHex
