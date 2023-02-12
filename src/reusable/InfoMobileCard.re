type coin_amount_t = {
  value: list(Coin.t),
  hasDenom: bool,
};

type request_count_t = {
  reportedValidators: int,
  minimumValidators: int,
  requestValidators: int,
};

type request_response_t = {
  requestCount: int,
  responseTime: option(float),
};

type t =
  | Address(Address.t, int, [ | `account | `validator])
  | Height(ID.Block.t)
  | Coin(coin_amount_t)
  | Count(int)
  | DataSource(ID.DataSource.t, string)
  | OracleScript(ID.OracleScript.t, string)
  | RequestID(ID.Request.t)
  | RequestResponse(request_response_t)
  | ProgressBar(request_count_t)
  | Float(float, option(int))
  | KVTableReport(list(string), list(MsgDecoder.RawDataReport.t))
  | KVTableRequest(option(array(Obi.field_key_value_t)))
  | CopyButton(JsBuffer.t)
  | Percentage(float, option(int))
  | Timestamp(MomentRe.Moment.t)
  | TxHash(Hash.t, int)
  | BlockHash(Hash.t)
  | Validator(Address.t, string, string)
  | Messages(Hash.t, list(MsgDecoder.t), bool, string)
  | MsgBadgeGroup(Hash.t, list(MsgDecoder.t))
  | PubKey(PubKey.t)
  | Badge(MsgDecoder.badge_theme_t)
  | VotingPower(Coin.t, float)
  | Uptime(option(float))
  | Loading(int)
  | Text(string)
  | Status(bool)
  | Nothing;

module Styles = {
  open Css;
  let vFlex = style([display(`flex), alignItems(`center)]);
  let addressContainer = w => {
    style([width(`px(w))]);
  };
  let badge = color =>
    style([
      display(`inlineFlex),
      padding2(~v=`px(5), ~h=`px(10)),
      backgroundColor(color),
      borderRadius(`px(15)),
    ]);
  let logo = style([width(`px(20))]);
};

[@react.component]
let make = (~info) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  switch (info) {
  | Address(address, width, accountType) =>
    <div className={Styles.addressContainer(width)}>
      <AddressRender address position=AddressRender.Text clickable=true accountType />
    </div>
  | Height(height) =>
    <div className=Styles.vFlex> <TypeID.Block id=height position=TypeID.Text /> </div>
  | Coin({value, hasDenom}) =>
    <AmountRender coins=value pos={hasDenom ? AmountRender.TxIndex : Fee} />
  | Count(value) => <Text value={value |> Format.iPretty} size=Text.Md />
  | DataSource(id, name) =>
    <div className=Styles.vFlex>
      <TypeID.DataSource id />
      <HSpacing size=Spacing.sm />
      <Text value=name ellipsis=true />
    </div>
  | OracleScript(id, name) =>
    <div className=Styles.vFlex>
      <TypeID.OracleScript id />
      <HSpacing size=Spacing.sm />
      <Text value=name ellipsis=true />
    </div>
  | RequestID(id) => <TypeID.Request id />
  | RequestResponse({requestCount, responseTime: responseTimeOpt}) =>
    <div className={CssHelper.flexBox()}>
      <Text value={requestCount |> Format.iPretty} block=true ellipsis=true />
      <HSpacing size=Spacing.sm />
      <Text
        value={
          switch (responseTimeOpt) {
          | Some(responseTime') => "(" ++ (responseTime' |> Format.fPretty(~digits=2)) ++ "s)"
          | None => "(TBD)"
          }
        }
        block=true
      />
    </div>
  | ProgressBar({reportedValidators, minimumValidators, requestValidators}) =>
    <ProgressBar reportedValidators minimumValidators requestValidators />
  | Float(value, digits) => <Text value={value |> Format.fPretty(~digits?)} />
  | KVTableReport(heading, rawReports) =>
    <KVTable
      headers=heading
      rows={
        rawReports
        |> Belt_List.map(_, rawReport =>
             [
               KVTable.Value(rawReport.externalDataID |> string_of_int),
               KVTable.Value(rawReport.exitCode |> string_of_int),
               KVTable.Value(rawReport.data |> JsBuffer.toUTF8),
             ]
           )
      }
    />
  | KVTableRequest(calldataKVsOpt) =>
    switch (calldataKVsOpt) {
    | Some(calldataKVs) =>
      <KVTable
        rows={
          calldataKVs
          ->Belt_Array.map(({fieldName, fieldValue}) =>
              [KVTable.Value(fieldName), KVTable.Value(fieldValue)]
            )
          ->Belt_List.fromArray
        }
      />
    | None =>
      <Text
        value="Could not decode calldata."
        spacing={Text.Em(0.02)}
        nowrap=true
        ellipsis=true
        block=true
      />
    }
  | CopyButton(calldata) =>
    <CopyButton
      data={calldata |> JsBuffer.toHex(~with0x=false)}
      title="Copy as bytes"
      width=125
    />
  | Percentage(value, digits) => <Text value={value |> Format.fPercent(~digits?)} />
  | Text(text) =>
    <Text value=text spacing={Text.Em(0.02)} nowrap=true ellipsis=true block=true />
  | Timestamp(time) => <Timestamp time size=Text.Md weight=Text.Regular />
  | Validator(address, moniker, identity) =>
    <ValidatorMonikerLink
      validatorAddress=address
      moniker
      size=Text.Md
      identity
      width={`px(230)}
    />
  | PubKey(publicKey) => <PubKeyRender alignLeft=true pubKey=publicKey display=`block />
  | TxHash(txHash, width) => <TxLink txHash width />
  | BlockHash(hash) =>
    <Text
      value={hash |> Hash.toHex(~upper=true)}
      weight=Text.Medium
      block=true
      code=true
      ellipsis=true
      color={theme.neutral_900}
    />
  | Messages(txHash, messages, success, errMsg) => <TxMessages txHash messages success errMsg />
  | MsgBadgeGroup(txHash, messages) => <MsgBadgeGroup txHash messages />
  | Badge({name, category}) => <MsgBadge name />
  | VotingPower(tokens, votingPercent) =>
    <div className=Styles.vFlex>
      <Text
        value={tokens |> Coin.getBandAmountFromCoin |> Format.fPretty(~digits=0)}
        block=true
      />
      <HSpacing size=Spacing.sm />
      <Text
        value={"(" ++ (votingPercent |> Format.fPercent(~digits=2)) ++ ")"}
        weight=Text.Thin
        block=true
      />
    </div>
  | Status(status) => <StatusIcon status />

  // Special case for uptime to have loading state inside.
  | Uptime(uptimeOpt) =>
    switch (uptimeOpt) {
    | Some(uptime) =>
      <div className=Styles.vFlex>
        <Text value={uptime |> Format.fPercent(~digits=2)} spacing={Text.Em(0.02)} nowrap=true />
        <HSpacing size=Spacing.lg />
        <ProgressBar.Uptime percent=uptime />
      </div>
    | None => <Text value="N/A" nowrap=true />
    }
  | Loading(width) => <LoadingCensorBar width height=21 />
  | Nothing => React.null
  };
};
