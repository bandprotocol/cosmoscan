module Styles = {
  open Css;

  let container = style([paddingBottom(`px(24))]);

  let warning = (theme: Theme.t) =>
    style([
      display(`flex),
      flexDirection(`column),
      padding2(~v=`px(16), ~h=`px(24)),
      backgroundColor(theme.neutral_100),
      borderRadius(`px(4)),
      marginBottom(`px(24)),
    ]);
};

[@react.component]
let make = (~address, ~validator, ~setMsgsOpt) => {
  let validatorInfoSub = ValidatorSub.get(validator);
  let delegationSub = DelegationSub.getStakeByValidator(address, validator);

  let allSub = Sub.all2(validatorInfoSub, delegationSub);

  let (amount, setAmount) = React.useState(_ => EnhanceTxInput.empty);

  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  React.useEffect1(
    _ => {
      let msgsOpt = {
        let%Opt amountValue = amount.value;

        let coin = BandChainJS.Coin.create();
        coin->BandChainJS.Coin.setDenom("uband");
        coin->BandChainJS.Coin.setAmount(amountValue |> Js.Float.toString);
        Some([|TxCreator2.Undelegate(validator, coin)|]);
      };
      setMsgsOpt(_ => msgsOpt);
      None;
    },
    [|amount|],
  );

  <>
    <div className={Styles.warning(theme)}>
      <Heading
        value="Please read before proceeding:"
        size=Heading.H5
        marginBottom=4
        align=Heading.Left
      />
      <VSpacing size=Spacing.xs />
      <Text
        value="1. Undelegated balance are locked for 21 days. After the unbonding period, the balance will automatically be added to your account"
      />
      <VSpacing size=Spacing.xs />
      <Text
        value="2. You can have a maximum of 7 pending unbonding transactions at any one time."
      />
    </div>
    <div className=Styles.container>
      <Heading
        value="Undelegate from"
        size=Heading.H5
        marginBottom=8
        align=Heading.Left
        weight=Heading.Regular
        color={theme.neutral_600}
      />
      {switch (allSub) {
       | Data(({moniker}, _)) =>
         <div>
           <Text value=moniker ellipsis=true align=Text.Right />
           <Text value={"(" ++ validator->Address.toOperatorBech32 ++ ")"} code=true block=true />
         </div>
       | _ => <LoadingCensorBar width=300 height=34 />
       }}
    </div>
    <div className=Styles.container>
      <Heading
        value="Current Stake"
        size=Heading.H5
        marginBottom=8
        align=Heading.Left
        weight=Heading.Regular
        color={theme.neutral_600}
      />
      {switch (allSub) {
       | Data((_, {amount: stakedAmount})) =>
         <div>
           <Text
             value={stakedAmount |> Coin.getBandAmountFromCoin |> Format.fPretty(~digits=6)}
             code=true
           />
           <Text value=" BAND" />
         </div>
       | _ => <LoadingCensorBar width=150 height=18 />
       }}
    </div>
    {switch (allSub) {
     | Data((_, {amount: stakedAmount})) =>
       let maxValInUband = stakedAmount |> Coin.getUBandAmountFromCoin;
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
         id="undelegateAmountInput"
       />;
     | _ => <EnhanceTxInput.Loading msg="Amount" code=true useMax=true placeholder="0.000000" />
     }}
  </>;
};
