type pos =
  | Msg
  | TxIndex
  | Fee;

module Styles = {
  open Css;

  let container = style([display(`flex), alignItems(`center)]);
};

[@react.component]
let make = (~coins, ~pos=Msg) => {
  <div className=Styles.container>
    {switch (pos) {
     | TxIndex =>
       <Text
         value={coins |> Coin.getBandAmountFromCoins |> Format.fPretty}
         code=true
         block=true
         nowrap=true
         size=Text.Lg
       />
     | _ =>
       <Text
         value={coins |> Coin.getBandAmountFromCoins |> Format.fPretty}
         block=true
         nowrap=true
         code=true
       />
     }}
    <HSpacing size=Spacing.sm />
    {switch (pos) {
     | Msg => <Text value="BAND" weight=Text.Regular nowrap=true block=true />
     | TxIndex => <Text value="BAND" weight=Text.Regular nowrap=true block=true />
     | Fee => React.null
     }}
  </div>;
};
