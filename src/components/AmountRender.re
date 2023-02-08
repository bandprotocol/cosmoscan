type pos =
  | Msg
  | TxIndex
  | Fee;

module Styles = {
  open Css;

  let container = style([display(`flex), alignItems(`center)]);
};

[@react.component]
let make = (~coins, ~pos=Msg, ~color=?) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  let textColor = color->Belt.Option.getWithDefault(theme.neutral_600);

  <div className=Styles.container>
    {switch (pos) {
     | TxIndex =>
       <Text
         value={coins |> Coin.getBandAmountFromCoins |> Format.fPretty}
         code=true
         block=true
         nowrap=true
         size=Text.Lg
         color=textColor
       />
     | _ =>
       <Text
         value={coins |> Coin.getBandAmountFromCoins |> Format.fPretty}
         block=true
         nowrap=true
         code=true
         color=textColor
       />
     }}
    <HSpacing size=Spacing.sm />
    {switch (pos) {
     | Msg => <Text value="BAND" weight=Text.Regular color=textColor nowrap=true block=true />
     | TxIndex => <Text value="BAND" weight=Text.Regular color=textColor nowrap=true block=true />
     | Fee => React.null
     }}
  </div>;
};
