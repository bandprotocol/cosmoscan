module Styles = {
  open Css;

  let container = style([paddingBottom(`px(24))]);

  let validator =
    style([display(`flex), flexDirection(`column), alignItems(`flexEnd), width(`px(330))]);
};

[@react.component]
let make = (~address, ~validator, ~setMsgsOpt) => {
  let validatorInfoSub = ValidatorSub.get(validator);
  let delegationSub = DelegationSub.getStakeByValidator(address, validator);

  let allSub = Sub.all2(validatorInfoSub, delegationSub);

  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  React.useEffect1(
    _ => {
      let msgsOpt = {
        Some([|TxCreator2.WithdrawReward(validator)|]);
      };
      setMsgsOpt(_ => msgsOpt);
      None;
    },
    [|validator|],
  );

  <>
    <div className=Styles.container>
      <Heading
        value="Withdraw Delegation Rewards"
        size=Heading.H5
        marginBottom=8
        align=Heading.Left
        weight=Heading.Regular
        color={theme.neutral_600}
      />
      <VSpacing size=Spacing.sm />
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
        value="Current Reward"
        size=Heading.H5
        marginBottom=8
        align=Heading.Left
        weight=Heading.Regular
        color={theme.neutral_600}
      />
      <VSpacing size=Spacing.sm />
      {switch (allSub) {
       | Data((_, {reward})) =>
         <div>
           <NumberCountup
             value={reward |> Coin.getBandAmountFromCoin}
             weight=Text.Thin
             spacing={Text.Em(0.)}
             size=Text.Md
           />
           <Text value=" BAND" />
         </div>
       | _ => <LoadingCensorBar width=150 height=18 />
       }}
    </div>
  </>;
};
