type t =
  | Height(ID.Block.t)
  | Count(int)
  | Float(float)
  | Text(string)
  | Description(string)
  | Timestamp(MomentRe.Moment.t)
  | Fee(float)
  | DataSources(list(ID.DataSource.t))
  | OracleScript(ID.OracleScript.t, string)
  | TxHash(Hash.t, int)
  | Hash(Hash.t, Css.Types.Color.t)
  | Address(Address.t, int)
  | Fraction(int, int, bool)
  | FloatWithSuffix(float, string, int)
  | Validator(Address.t, string, string)
  | ValidatorsMini(array(ValidatorSub.Mini.t))
  | Validators(array(ValidatorSub.t))
  | Currency(float)
  | Loading(int);

module Styles = {
  open Css;

  let mainContainer = isLeft =>
    style([
      display(`flex),
      flexDirection(`column),
      alignItems(isLeft ? `flexStart : `flexEnd),
      minWidth(`zero),
    ]);
  let vFlex = style([display(`flex), alignItems(`center)]);
  let addressContainer = maxwidth_ => style([alignItems(`center), maxWidth(`px(maxwidth_))]);
  let datasourcesContainer = style([display(`flex), alignItems(`center), flexWrap(`wrap)]);
  let oracleScriptContainer = style([display(`flex), width(`px(240))]);
  let validatorsContainer = style([display(`flex), flexDirection(`column), flexWrap(`wrap)]);
  let sourceContainer =
    style([
      display(`inlineFlex),
      alignItems(`center),
      marginRight(`px(40)),
      marginTop(`px(13)),
    ]);
  let sourceIcon = style([width(`px(16)), marginRight(`px(8))]);
  let marginRightOnly = size => style([marginRight(`px(size))]);
};

[@react.component]
let make = (~info, ~header, ~isLeft=true) => {
  let infoSub = React.useContext(GlobalContext.context);
  <div className={Styles.mainContainer(isLeft)}>
    <Text
      value=header
      color=Colors.gray7
      size=Text.Sm
      weight=Text.Thin
      height={Text.Px(18)}
      spacing={Text.Em(0.03)}
    />
    {switch (info) {
     | Height(height) =>
       <div className=Styles.vFlex> <TypeID.Block id=height position=TypeID.Subtitle /> </div>
     | Float(value) =>
       <Text
         value={value |> Format.fPretty}
         size=Text.Lg
         weight=Text.Semibold
         spacing={Text.Em(0.02)}
         code=true
       />
     | FloatWithSuffix(value, suffix, digits) =>
       <Text
         value={(value |> Format.fPretty(~digits)) ++ suffix}
         size=Text.Lg
         weight=Text.Semibold
         spacing={Text.Em(0.02)}
         code=true
       />
     | Count(value) =>
       <Text
         value={value |> Format.iPretty}
         size=Text.Lg
         weight=Text.Semibold
         spacing={Text.Em(0.02)}
         code=true
       />
     | Text(text) =>
       <Text
         value=text
         size=Text.Lg
         weight=Text.Semibold
         code=true
         spacing={Text.Em(0.02)}
         nowrap=true
         ellipsis=true
       />
     | Description(text) =>
       <Text value=text size=Text.Lg weight=Text.Thin spacing={Text.Em(0.)} />
     | Timestamp(time) =>
       <div className=Styles.vFlex>
         <Text
           value={
             time
             |> MomentRe.Moment.format(Config.timestampDisplayFormat)
             |> String.uppercase_ascii
           }
           size=Text.Lg
           weight=Text.Semibold
           spacing={Text.Em(0.02)}
           code=true
         />
         <HSpacing size=Spacing.sm />
         <TimeAgos
           time
           prefix="("
           suffix=")"
           size=Text.Md
           weight=Text.Thin
           spacing={Text.Em(0.06)}
           color=Colors.gray7
         />
       </div>
     | Fee(fee) =>
       <div className=Styles.vFlex>
         <Text value={fee |> Format.fPretty} size=Text.Lg weight=Text.Bold code=true />
         <HSpacing size=Spacing.md />
         <Text value="BAND" size=Text.Lg weight=Text.Regular spacing={Text.Em(0.02)} code=true />
         <HSpacing size=Spacing.xs />
         <HSpacing size=Spacing.xs />
         {switch (infoSub) {
          | Data(info) =>
            let feeInUsd =
              info.financial.usdPrice *. fee |> Js.Float.toFixedWithPrecision(~digits=2);
            <Text
              value={j|(\$$feeInUsd)|j}
              size=Text.Lg
              weight=Text.Regular
              spacing={Text.Em(0.02)}
              code=true
            />;
          | _ => React.null
          }}
       </div>
     | DataSources(ids) =>
       switch (ids |> Belt_List.size) {
       | 0 => <Text value="TBD" size=Text.Lg spacing={Text.Em(0.06)} height={Text.Px(17)} />
       | _ =>
         <div className=Styles.datasourcesContainer>
           {ids
            ->Belt.List.map(id =>
                <React.Fragment key={id |> ID.DataSource.toString}>
                  <TypeID.DataSource id position=TypeID.Subtitle />
                  <HSpacing size=Spacing.sm />
                </React.Fragment>
              )
            ->Array.of_list
            ->React.array}
         </div>
       }
     | OracleScript(id, name) =>
       <div className=Styles.oracleScriptContainer>
         <TypeID.OracleScript id position=TypeID.Subtitle />
         <HSpacing size=Spacing.sm />
         <Text
           value=name
           size=Text.Lg
           weight=Text.Regular
           spacing={Text.Em(0.02)}
           code=true
           nowrap=true
           ellipsis=true
         />
       </div>
     | TxHash(txHash, width) => <TxLink txHash width size=Text.Lg />
     | Hash(hash, textColor) =>
       <Text
         value={hash |> Hash.toHex(~with0x=true, ~upper=true)}
         size=Text.Lg
         weight=Text.Semibold
         color=textColor
       />
     | Fraction(x, y, space) =>
       <Text
         value={(x |> Format.iPretty) ++ (space ? " / " : "/") ++ (y |> Format.iPretty)}
         size=Text.Lg
         weight=Text.Semibold
         spacing={Text.Em(0.02)}
         code=true
       />
     | Address(address, maxWidth) =>
       <div className={Styles.addressContainer(maxWidth)}>
         <AddressRender address position=AddressRender.Subtitle />
       </div>
     | Validator(address, moniker, identity) =>
       <ValidatorMonikerLink
         validatorAddress=address
         moniker
         size=Text.Lg
         identity
         width={`percent(100.)}
       />
     | ValidatorsMini(validators) =>
       <div className=Styles.validatorsContainer>
         {validators
          ->Belt_Array.map(validator =>
              <div key={validator.operatorAddress |> Address.toBech32} className=Styles.vFlex>
                <Text value={j|●|j} />
                <HSpacing size=Spacing.md />
                <ValidatorMonikerLink
                  validatorAddress={validator.operatorAddress}
                  moniker={validator.moniker}
                  size=Text.Lg
                />
              </div>
            )
          ->React.array}
       </div>
     | Validators(validators) =>
       <div className=Styles.datasourcesContainer>
         {validators
          ->Belt_Array.map(validator =>
              <div
                key={validator.operatorAddress |> Address.toBech32}
                className={Styles.marginRightOnly(10)}>
                <ValidatorMonikerLink
                  validatorAddress={validator.operatorAddress}
                  moniker={validator.moniker}
                  size=Text.Lg
                  underline=true
                />
              </div>
            )
          ->React.array}
       </div>
     | Currency(amount) =>
       <Text
         value={amount |> Format.fCurrency}
         size=Text.Lg
         weight=Text.Semibold
         spacing={Text.Em(0.02)}
         code=true
       />
     | Loading(width) => <LoadingCensorBar width height=15 />
     }}
  </div>;
};
