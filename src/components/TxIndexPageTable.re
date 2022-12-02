module Styles = {
  open Css;

  let topicContainer =
    style([
      display(`flex),
      justifyContent(`spaceBetween),
      width(`percent(100.)),
      lineHeight(`px(16)),
      alignItems(`center),
    ]);

  let failIcon = style([width(`px(16)), height(`px(16))]);
  let msgContainer = style([selector("> div + div", [marginTop(`px(24))])]);
  let jsonMode =
    style([display(`flex), alignItems(`center), cursor(`pointer), height(`px(30))]);
};

let renderUnknownMessage = () => {
  <Col col=Col.Six>
    <div className=Styles.topicContainer>
      <Text value="Unknown Message" size=Text.Sm transform=Text.Uppercase />
      <img alt="Fail Icon" src=Images.fail className=Styles.failIcon />
    </div>
  </Col>;
};

let renderBody = (msg: MsgDecoder.t) =>
  switch (msg.decoded) {
  | SendMsgSuccess(send)
  | SendMsgFail(send) => <IndexTokenMsg.SendMsg send />
  | DelegateMsgSuccess(delegation) => <IndexTokenMsg.DelegateMsg delegation />
  | DelegateMsgFail(delegation) => <IndexTokenMsg.DelegateFailMsg delegation />
  | UndelegateMsgSuccess(undelegation) => <IndexTokenMsg.UndelegateMsg undelegation />
  | UndelegateMsgFail(undelegation) => <IndexTokenMsg.UndelegateFailMsg undelegation />
  | RedelegateMsgSuccess(redelegation) => <IndexTokenMsg.RedelegateMsg redelegation />
  | RedelegateMsgFail(redelegation) => <IndexTokenMsg.RedelegateFailMsg redelegation />
  | WithdrawRewardMsgSuccess(withdrawal) => <IndexTokenMsg.WithdrawRewardMsg withdrawal />
  | WithdrawRewardMsgFail(withdrawal) => <IndexTokenMsg.WithdrawRewardFailMsg withdrawal />
  | WithdrawCommissionMsgSuccess(withdrawal) => <IndexTokenMsg.WithdrawComissionMsg withdrawal />
  | WithdrawCommissionMsgFail(withdrawal) => <IndexTokenMsg.WithdrawComissionFailMsg withdrawal />
  | MultiSendMsgSuccess(tx)
  | MultiSendMsgFail(tx) => <IndexTokenMsg.MultisendMsg tx />
  | CreateDataSourceMsgSuccess(dataSource) => <IndexDataMsg.CreateDataSourceMsg dataSource />
  | CreateDataSourceMsgFail(dataSource) => <IndexDataMsg.CreateDataSourceFailMsg dataSource />
  | EditDataSourceMsgSuccess(dataSource)
  | EditDataSourceMsgFail(dataSource) => <IndexDataMsg.EditDataSourceMsg dataSource />
  | CreateOracleScriptMsgSuccess(oracleScript) =>
    <IndexDataMsg.CreateOracleScriptMsg oracleScript />
  | CreateOracleScriptMsgFail(oracleScript) =>
    <IndexDataMsg.CreateOracleScriptFailMsg oracleScript />
  | EditOracleScriptMsgSuccess(oracleScript)
  | EditOracleScriptMsgFail(oracleScript) => <IndexDataMsg.EditOracleScriptMsg oracleScript />
  | RequestMsgSuccess(request) => <IndexDataMsg.RequestMsg request />
  | RequestMsgFail(request) => <IndexDataMsg.RequestFailMsg request />
  | ReportMsgSuccess(report)
  | ReportMsgFail(report) => <IndexDataMsg.ReportMsg report />
  | GrantMsg(address) => <IndexValidatorMsg.GrantMsg address />
  | RevokeMsg(address) => <IndexValidatorMsg.RevokeMsg address />
  | RevokeAllowanceMsg(address) => <IndexValidatorMsg.RevokeAllowanceMsg address />
  | GrantAllowanceMsg(address) => <IndexValidatorMsg.GrantAllowanceMsg address />
  | ExecMsgSuccess(address) => <IndexValidatorMsg.ExecSuccessMsg address />
  | ExecMsgFail(address) => <IndexValidatorMsg.ExecFailMsg address />
  | CreateValidatorMsgSuccess(validator)
  | CreateValidatorMsgFail(validator) => <IndexValidatorMsg.CreateValidatorMsg validator />
  | EditValidatorMsgSuccess(validator)
  | EditValidatorMsgFail(validator) => <IndexValidatorMsg.EditValidatorMsg validator />
  | UnjailMsgSuccess(unjail)
  | UnjailMsgFail(unjail) => <IndexValidatorMsg.UnjailMsg unjail />
  | SetWithdrawAddressMsgSuccess(set)
  | SetWithdrawAddressMsgFail(set) => <IndexValidatorMsg.SetWithdrawAddressMsg set />
  | SubmitProposalMsgSuccess(proposal) => <IndexProposalMsg.SubmitProposalMsg proposal />
  | SubmitProposalMsgFail(proposal) => <IndexProposalMsg.SubmitProposalFailMsg proposal />
  | DepositMsgSuccess(deposit) => <IndexProposalMsg.DepositMsg deposit />
  | DepositMsgFail(deposit) => <IndexProposalMsg.DepositFailMsg deposit />
  | VoteMsgSuccess(vote) => <IndexProposalMsg.VoteMsg vote />
  | VoteMsgFail(vote) => <IndexProposalMsg.VoteFailMsg vote />
  | VoteWeightedMsgSuccess(vote) => <IndexProposalMsg.VoteWeightedMsg vote />
  | VoteWeightedMsgFail(vote) => <IndexProposalMsg.VoteWeightedFailMsg vote />
  | ActivateMsgSuccess(activate)
  | ActivateMsgFail(activate) => <IndexValidatorMsg.ActivateMsg activate />
  | UnknownMsg => renderUnknownMessage()
  // IBC Msg
  | CreateClientMsg(client) => <IndexIBCClientMsg.CreateClient client />
  | UpdateClientMsg(client) => <IndexIBCClientMsg.UpdateClient client />
  | UpgradeClientMsg(client) => <IndexIBCClientMsg.UpgradeClient client />
  | SubmitClientMisbehaviourMsg(client) => <IndexIBCClientMsg.SubmitClientMisbehaviour client />
  | ConnectionOpenInitMsg(connection) => <IndexIBCConnectionMsg.ConnectionOpenInit connection />
  | ConnectionOpenTryMsg(connection) => <IndexIBCConnectionMsg.ConnectionOpenTry connection />
  | ConnectionOpenAckMsg(connection) => <IndexIBCConnectionMsg.ConnectionOpenAck connection />
  | ConnectionOpenConfirmMsg(connection) =>
    <IndexIBCConnectionMsg.ConnectionOpenConfirm connection />
  | ChannelOpenInitMsg(channel) => <IndexIBCChannelMsg.ChannelOpenInit channel />
  | ChannelOpenTryMsg(channel) => <IndexIBCChannelMsg.ChannelOpenTry channel />
  | ChannelOpenAckMsg(channel) => <IndexIBCChannelMsg.ChannelOpenAck channel />
  | ChannelOpenConfirmMsg(channel) => <IndexIBCChannelMsg.ChannelOpenConfirm channel />
  | ChannelCloseInitMsg(channel) => <IndexIBCChannelMsg.ChannelCloseInit channel />
  | ChannelCloseConfirmMsg(channel) => <IndexIBCChannelMsg.ChannelCloseConfirm channel />
  | AcknowledgePacketMsg(packet) => <IndexIBCPacketMsg.AcknowledgePacket packet />
  | RecvPacketMsgSuccess(packet) => <IndexIBCPacketMsg.RecvPacketSuccess packet />
  | RecvPacketMsgFail(packet) => <IndexIBCPacketMsg.RecvPacketFail packet />
  | TimeoutMsg(packet) => <IndexIBCPacketMsg.Timeout packet />
  | TimeoutOnCloseMsg(packet) => <IndexIBCPacketMsg.TimeoutOnClose packet />
  | TransferMsgSuccess(msg) => <IndexIBCTransferMsg.TransferSuccess msg />
  | TransferMsgFail(msg) => <IndexIBCTransferMsg.TransferFail msg />
  | _ => React.null
  };

module MsgDetailCard = {
  [@react.component]
  let make = (~msg: MsgDecoder.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    let badgeTheme = msg |> MsgDecoder.getBadgeTheme;
    let (showJson, setShowJson) = React.useState(_ => false);
    let toggle = () => setShowJson(prev => !prev);

    <InfoContainer>
      <div className={CssHelper.flexBox(~justify=`spaceBetween, ())}>
        <div className={CssHelper.flexBox()}>
          <IndexMsgIcon category={badgeTheme.category} />
          <HSpacing size=Spacing.sm />
          <Heading value={badgeTheme.name} size=Heading.H4 />
        </div>
        <div className=Styles.jsonMode onClick={_ => toggle()}>
          <Text value="JSON Mode" weight=Text.Semibold color={theme.textPrimary} />
          <Switch checked=showJson />
        </div>
      </div>
      {showJson
         ? <div className={CssHelper.mt(~size=32, ())}> <JsonViewer src={msg.raw} /> </div>
         : <> <SeperatedLine mt=32 mb=24 /> {renderBody(msg)} </>}
    </InfoContainer>;
  };
};

[@react.component]
let make = (~messages: list(MsgDecoder.t)) => {
  <div className=Styles.msgContainer>
    {messages
     ->Belt.List.mapWithIndex((index, msg) => {
         let badgeTheme = msg |> MsgDecoder.getBadgeTheme;
         <MsgDetailCard key={(index |> string_of_int) ++ badgeTheme.name} msg />;
       })
     ->Array.of_list
     ->React.array}
  </div>;
};

module Loading = {
  [@react.component]
  let make = () => {
    <InfoContainer>
      <div className={CssHelper.flexBox()}>
        <LoadingCensorBar width=24 height=24 radius=24 />
        <HSpacing size=Spacing.sm />
        <LoadingCensorBar width=75 height=15 />
        <SeperatedLine mt=32 mb=24 />
      </div>
      <Row>
        <Col col=Col.Six mb=24>
          <LoadingCensorBar width=75 height=15 mb=8 />
          <LoadingCensorBar width=150 height=15 />
        </Col>
        <Col col=Col.Six mb=24>
          <LoadingCensorBar width=75 height=15 mb=8 />
          <LoadingCensorBar width=150 height=15 />
        </Col>
        <Col col=Col.Six>
          <LoadingCensorBar width=75 height=15 mb=8 />
          <LoadingCensorBar width=150 height=15 />
        </Col>
      </Row>
    </InfoContainer>;
  };
};
