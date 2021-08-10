type t

@module @new external _create: unit => t = "ripemd160"
@send external _update: (t, JsBuffer.t) => t = "update"
@send external _digest: (t, string) => string = "digest"
let hexDigest = data => _create()->_update(data->JsBuffer.from)->_digest("hex")
let digest = data => data->hexDigest->JsBuffer.hexToArray
