let create = (data: array(TxQueryByBlockTimestamp.t)) => {
  let keys =
    [|
      "TxHash",
      "BlockNo",
      "Success",
      "GasFee",
      "GasLimit",
      "GasUsed",
      "From",
      "UnixTimestamp",
      "DateTime",
      "Method",
      "Memo",
    |]
    |> Js.Array.joinWith(",");
  let result =
    data->Belt.Array.map(item => {
      let msgString =
        item.messages
        ->Belt.List.toArray
        ->Belt.Array.map(msg => {
            let badgeTheme = msg |> MsgDecoder.getBadgeTheme;
            badgeTheme.name;
          })
        |> Js.Array.joinWith(";");

      let dataString =
        [|
          item.txHash,
          item.blockHeight |> ID.Block.toInt |> string_of_int,
          item.success |> string_of_bool,
          item.gasFee |> Coin.getBandAmountFromCoins |> Format.fPretty(~digits=2),
          item.gasLimit |> string_of_int,
          item.gasUsed |> string_of_int,
          item.from,
          item.timestamp |> MomentRe.Moment.toUnix |> string_of_int,
          item.timestamp |> MomentRe.Moment.format(Config.timestampDisplayFormat),
          msgString,
          item.memo,
        |]
        |> Js.Array.joinWith(",");
      dataString;
    })
    |> Js.Array.joinWith("\n");
  keys ++ "\n" ++ result;
};
