let getNameOpt = str =>
  switch (str->Address.toBech32) {
  | "band1w4ml3tmn7t00603m2euqfj9ndm8h8q2rp4enfj" => Some("Binance")
  | _ => None
  };
