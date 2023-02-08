module Styles = {
  open Css;

  let container = style([paddingBottom(`px(24))]);

  let info = style([display(`flex), justifyContent(`spaceBetween)]);

  let validator =
    style([display(`flex), flexDirection(`column), alignItems(`flexEnd), width(`px(330))]);
};

[@react.component]
let make = (~address, ~validator, ~setMsgsOpt) => {
  let accountSub = AccountSub.get(address);
  let validatorInfoSub = ValidatorSub.get(validator);

  let allSub = Sub.all2(accountSub, validatorInfoSub);

  let (amount, setAmount) = React.useState(_ => EnhanceTxInput.empty);

  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  React.useEffect1(
    _ => {
      let msgsOpt = {
        let%Opt amountValue = amount.value;

        let coin = BandChainJS.Coin.create();
        coin->BandChainJS.Coin.setDenom("uband");
        coin->BandChainJS.Coin.setAmount(amountValue |> Js.Float.toString);
        Some([|TxCreator2.Delegate(validator, coin)|]);
      };
      setMsgsOpt(_ => msgsOpt);
      None;
    },
    [|amount|],
  );

  <>
    <div className=Styles.container>
      <Heading
        value="Delegate to"
        size=Heading.H5
        marginBottom=8
        align=Heading.Left
        weight=Heading.Regular
        color={theme.neutral_600}
      />
      {switch (allSub) {
       | Data((_, {moniker})) =>
         <div>
           <Text value=moniker ellipsis=true align=Text.Right />
           <Text value={"(" ++ validator->Address.toOperatorBech32 ++ ")"} code=true block=true />
         </div>
       | _ => <LoadingCensorBar width=300 height=34 />
       }}
    </div>
    <div className=Styles.container>
      <Heading
        value="Account Balance"
        size=Heading.H5
        marginBottom=8
        align=Heading.Left
        weight=Heading.Regular
        color={theme.neutral_600}
      />
      {switch (allSub) {
       | Data(({balance}, _)) =>
         <div>
           <Text
             value={balance |> Coin.getBandAmountFromCoins |> Format.fPretty(~digits=6)}
             code=true
           />
           <Text value=" BAND" />
         </div>
       | _ => <LoadingCensorBar width=150 height=18 />
       }}
    </div>
    {switch (allSub) {
     | Data(({balance}, _)) =>
       //  TODO: hard-coded tx fee
       let maxValInUband = (balance |> Coin.getUBandAmountFromCoins) -. 5000.;
       <EnhanceTxInput
         width=300
         inputData=amount
         setInputData=setAmount
         parse={Parse.getBandAmount(maxValInUband)}
         maxValue={maxValInUband /. 1e6 |> Js.Float.toString}
         msg="Amount"
         placeholder="0.000000"
         inputType="number"
         code=true
         autoFocus=true
         id="delegateAmountInput"
         maxWarningMsg=true
       />;
     | _ => <EnhanceTxInput.Loading msg="Amount" code=true useMax=true placeholder="0.000000" />
     }}
  </>;
};
