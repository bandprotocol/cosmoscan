module Styles = {
  open Css;

  let container = style([paddingBottom(`px(24))]);
};

[@react.component]
let make = (~validator, ~amount, ~setMsgsOpt) => {
  let validatorInfoSub = ValidatorSub.get(validator);

  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  React.useEffect0(_ => {
    let msgsOpt = {
      let%Opt amountValue = Some(amount);

      let coin = BandChainJS.Coin.create();
      coin->BandChainJS.Coin.setDenom("uband");
      coin->BandChainJS.Coin.setAmount(amountValue->Js.Math.floor_float->Js.Float.toString);
      Some([|TxCreator2.WithdrawReward(validator), TxCreator2.Delegate(validator, coin)|]);
    };
    setMsgsOpt(_ => msgsOpt);
    None;
  });

  <>
    <div className=Styles.container>
      <Heading
        value="Reinvest to"
        size=Heading.H5
        marginBottom=8
        align=Heading.Left
        weight=Heading.Regular
        color={theme.neutral_600}
      />
      {switch (validatorInfoSub) {
       | Data({moniker}) =>
         <div>
           <Text value=moniker ellipsis=true align=Text.Right />
           <Text value={"(" ++ validator->Address.toOperatorBech32 ++ ")"} code=true block=true />
         </div>
       | _ => <LoadingCensorBar width=300 height=34 />
       }}
    </div>
    <div className=Styles.container>
      <Heading
        value="Current Reward"
        size=Heading.H5
        marginBottom=8
        align=Heading.Left
        weight=Heading.Regular
        color={theme.neutral_600}
      />
      <div>
        <Text
          value={
            amount
            |> Coin.newUBANDFromAmount
            |> Coin.getBandAmountFromCoin
            |> Format.fPretty(~digits=6)
          }
          code=true
        />
        <Text value=" BAND" />
      </div>
    </div>
  </>;
};
