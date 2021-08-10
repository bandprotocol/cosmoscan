module Styles = {
  open Css

  let container = style(list{paddingBottom(#px(24))})
}

@react.component
let make = (~validator, ~amount, ~setMsgsOpt) => {
  let validatorInfoSub = ValidatorSub.get(validator)

  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

  React.useEffect0(_ => {
    let msgsOpt = %Opt({
      let amountValue = Some(amount)
      Some([
        TxCreator.WithdrawReward(validator),
        TxCreator.Delegate(
          validator,
          {amount: amountValue->Js.Math.floor_float->Js.Float.toString, denom: "uband"},
        ),
      ])
    })
    setMsgsOpt(_ => msgsOpt)
    None
  })

  <>
    <div className=Styles.container>
      <Heading
        value="Reinvest to"
        size=Heading.H5
        marginBottom=8
        align=Heading.Left
        weight=Heading.Regular
        color=theme.textSecondary
      />
      {switch validatorInfoSub {
      | Data({moniker}) =>
        <div>
          <Text value=moniker ellipsis=true align=Text.Right />
          <Text value={"(" ++ (validator->Address.toOperatorBech32 ++ ")")} code=true block=true />
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
        color=theme.textSecondary
      />
      <div>
        <Text
          value={amount
          |> Coin.newUBANDFromAmount
          |> Coin.getBandAmountFromCoin
          |> Format.fPretty(~digits=6)}
          code=true
        />
        <Text value=" BAND" />
      </div>
    </div>
  </>
}
