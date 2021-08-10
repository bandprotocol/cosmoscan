type t

@module("crypto") @val
external createHash: string => t = "createHash"

@send external update: (t, string) => t = "update"

@send external digest: (t, string) => string = "digest"
