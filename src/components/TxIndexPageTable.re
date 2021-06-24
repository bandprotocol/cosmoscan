module Styles = {
  open Css;

  let addressContainer = width_ => style([width(`px(width_))]);

  let badgeContainer = style([display(`block), marginTop(`px(-4))]);

  let badge = color =>
    style([
      display(`inlineFlex),
      padding2(~v=`px(5), ~h=`px(8)),
      backgroundColor(color),
      borderRadius(`px(50)),
    ]);

  let hFlex = style([display(`flex), alignItems(`center)]);

  let topicContainer =
    style([
      display(`flex),
      justifyContent(`spaceBetween),
      width(`percent(100.)),
      lineHeight(`px(16)),
      alignItems(`center),
    ]);

  let detailContainer = style([display(`flex), maxWidth(`px(360)), justifyContent(`flexEnd)]);

  let hashContainer =
    style([
      display(`flex),
      maxWidth(`px(350)),
      justifyContent(`flexEnd),
      wordBreak(`breakAll),
    ]);

  let firstCol = 0.45;
  let secondCol = 0.50;
  let thirdCol = 1.20;

  let failIcon = style([width(`px(16)), height(`px(16))]);

  let failedMessageDetails =
    style([
      display(`flex),
      width(`px(120)),
      alignItems(`center),
      justifyContent(`spaceBetween),
    ]);

  let separatorLine =
    style([
      borderStyle(`none),
      backgroundColor(Colors.gray9),
      height(`px(1)),
      margin2(~v=`px(10), ~h=`auto),
    ]);

  let infoHeader =
    style([borderBottom(`px(1), `solid, Colors.gray9), paddingBottom(`px(16))]);

  let msgContainer = style([selector("> div + div", [marginTop(`px(24))])]);
};

let renderUnknownMessage = () => {
  <Col col=Col.Six>
    <div className=Styles.topicContainer>
      <Text value="Unknown Message" size=Text.Sm transform=Text.Uppercase />
      <img src=Images.fail className=Styles.failIcon />
    </div>
  </Col>;
};

let renderBody = (msg: MsgDecoder.t) =>
  switch (msg) {
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
  | AddReporterMsgSuccess(address) => <IndexValidatorMsg.AddReporterMsg address />
  | AddReporterMsgFail(address) => <IndexValidatorMsg.AddReporterFailMsg address />
  | RemoveReporterMsgSuccess(address) => <IndexValidatorMsg.RemoveReporterMsg address />
  | RemoveReporterMsgFail(address) => <IndexValidatorMsg.RemoveReporterFailMsg address />
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
  | ActivateMsgSuccess(activate)
  | ActivateMsgFail(activate) => <IndexValidatorMsg.ActivateMsg activate />
  | UnknownMsg => renderUnknownMessage()
  //TODO: Re-visit IBC Msg
  | CreateClientMsg(client) => <IndexIBCClientMsg.CreateClient client />
  | UpdateClientMsg(client) => <IndexIBCClientMsg.UpdateClient client />
  | UpgradeClientMsg(client) => <IndexIBCClientMsg.UpgradeClient client />
  | SubmitClientMisbehaviourMsg(client) => <IndexIBCClientMsg.SubmitClientMisbehaviour client />
  | ConnectionOpenInitMsg(connection) => <IndexIBCConnectionMsg.ConnectionOpenInit connection />
  | ConnectionOpenTryMsg(connection) => <IndexIBCConnectionMsg.ConnectionOpenTry connection />
  | ConnectionOpenAckMsg(connection) => <IndexIBCConnectionMsg.ConnectionOpenAck connection />
  | ConnectionOpenConfirmMsg(connection) =>
    <IndexIBCConnectionMsg.ConnectionOpenConfirm connection />
  | ChannelOpenInitMsg(_)
  | ChannelOpenTryMsg(_)
  | ChannelOpenAckMsg(_)
  | ChannelOpenConfirmMsg(_)
  | ChannelCloseInitMsg(_)
  | ChannelCloseConfirmMsg(_)
  | PacketMsg(_)
  | AcknowledgementMsg(_)
  | TimeoutMsg(_)
  | _ => React.null
  };

[@react.component]
let make = (~messages: list(MsgDecoder.t)) => {
  <div className=Styles.msgContainer>
    {messages
     ->Belt.List.mapWithIndex((index, msg) => {
         let theme = msg |> MsgDecoder.getBadgeTheme;
         <InfoContainer key={(index |> string_of_int) ++ theme.name}>
           <div className={CssHelper.flexBox()}>
             <IndexMsgIcon category={theme.category} />
             <HSpacing size=Spacing.sm />
             <Heading value={theme.name} size=Heading.H4 />
           </div>
           <SeperatedLine mt=32 mb=24 />
           {renderBody(msg)}
         </InfoContainer>;
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
