type t

@module("@cosmostation/cosmosjs") external network: (string, string) => t = "network"

@send external setPath: (t, string) => unit = "setPath"

@send external setBech32MainPrefix: (t, string) => unit = "setBech32MainPrefix"

@send external getAddress: (t, string) => string = "getAddress"

@send external getECPairPriv: (t, string) => JsBuffer.t = "getECPairPriv"
