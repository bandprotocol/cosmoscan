@deriving(abstract)
type decoded_t = {
  prefix: string,
  words: array<int>,
}

@module("bech32") @val external fromWords: array<int> => array<int> = "fromWords"
@module("bech32") @val external toWords: array<int> => array<int> = "toWords"

@module("bech32") @val external decode: string => decoded_t = "decode"
@module("bech32") @val external encode: (string, array<int>) => string = "encode"

let decodeOpt = str =>
  switch str->decode {
  | result => Some(result)
  | exception _ => None
  }
