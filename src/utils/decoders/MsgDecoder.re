type badge_t =
  | SendBadge
  | ReceiveBadge
  | CreateDataSourceBadge
  | EditDataSourceBadge
  | CreateOracleScriptBadge
  | EditOracleScriptBadge
  | RequestBadge
  | ReportBadge
  | GrantBadge
  | RevokeBadge
  | ExecBadge
  | CreateValidatorBadge
  | EditValidatorBadge
  | DelegateBadge
  | UndelegateBadge
  | RedelegateBadge
  | WithdrawRewardBadge
  | UnjailBadge
  | SetWithdrawAddressBadge
  | SubmitProposalBadge
  | DepositBadge
  | VoteBadge
  | WithdrawCommissionBadge
  | MultiSendBadge
  | ActivateBadge
  | CreateClientBadge
  | UpdateClientBadge
  | UpgradeClientBadge
  | SubmitClientMisbehaviourBadge
  | ConnectionOpenInitBadge
  | ConnectionOpenTryBadge
  | ConnectionOpenAckBadge
  | ConnectionOpenConfirmBadge
  | ChannelOpenInitBadge
  | ChannelOpenTryBadge
  | ChannelOpenAckBadge
  | ChannelOpenConfirmBadge
  | ChannelCloseInitBadge
  | ChannelCloseConfirmBadge
  | AcknowledgePacketBadge
  | RecvPacketBadge
  | TimeoutBadge
  | TimeoutOnCloseBadge
  | TransferBadge
  | UnknownBadge;

type msg_cat_t =
  | TokenMsg
  | ValidatorMsg
  | ProposalMsg
  | DataMsg
  | IBCClientMsg
  | IBCConnectionMsg
  | IBCChannelMsg
  | IBCPacketMsg
  | IBCTransferMsg
  | UnknownMsg;

let getBadgeVariantFromString = badge => {
  switch (badge) {
  | "/cosmos.bank.v1beta1.MsgSend" => SendBadge
  | "receive" => raise(Not_found)
  | "/oracle.v1.MsgCreateDataSource" => CreateDataSourceBadge
  | "/oracle.v1.MsgEditDataSource" => EditDataSourceBadge
  | "/oracle.v1.MsgCreateOracleScript" => CreateOracleScriptBadge
  | "/oracle.v1.MsgEditOracleScript" => EditOracleScriptBadge
  | "/oracle.v1.MsgRequestData" => RequestBadge
  | "/oracle.v1.MsgReportData" => ReportBadge
  | "/cosmos.authz.v1beta1.MsgGrant" => GrantBadge
  | "/cosmos.authz.v1beta1.MsgRevoke" => RevokeBadge
  | "/cosmos.authz.v1beta1.MsgExec" => ExecBadge
  | "/cosmos.staking.v1beta1.MsgCreateValidator" => CreateValidatorBadge
  | "/cosmos.staking.v1beta1.MsgEditValidator" => EditValidatorBadge
  | "/cosmos.staking.v1beta1.MsgDelegate" => DelegateBadge
  | "/cosmos.staking.v1beta1.MsgUndelegate" => UndelegateBadge
  | "/cosmos.staking.v1beta1.MsgBeginRedelegate" => RedelegateBadge
  | "/cosmos.distribution.v1beta1.MsgWithdrawDelegatorReward" => WithdrawRewardBadge
  | "/cosmos.slashing.v1beta1.MsgUnjail" => UnjailBadge
  | "/cosmos.distribution.v1beta1.MsgSetWithdrawAddress" => SetWithdrawAddressBadge
  | "submit_proposal" => SubmitProposalBadge
  | "deposit" => DepositBadge
  | "vote" => VoteBadge
  | "/cosmos.distribution.v1beta1.MsgWithdrawValidatorCommission" => WithdrawCommissionBadge
  | "/cosmos.bank.v1beta1.MsgMultiSend" => MultiSendBadge
  | "/oracle.v1.MsgActivate" => ActivateBadge
  | "/ibc.core.client.v1.MsgCreateClient" => CreateClientBadge
  | "/ibc.core.client.v1.MsgUpdateClient" => UpdateClientBadge
  | "/ibc.core.client.v1.MsgUpgradeClient" => UpgradeClientBadge
  | "/ibc.core.client.v1.MsgSubmitClientMisbehaviour" => SubmitClientMisbehaviourBadge
  | "/ibc.core.connection.v1.MsgConnectionOpenInit" => ConnectionOpenInitBadge
  | "/ibc.core.connection.v1.MsgConnectionOpenTry" => ConnectionOpenTryBadge
  | "/ibc.core.connection.v1.MsgConnectionOpenAck" => ConnectionOpenAckBadge
  | "/ibc.core.connection.v1.MsgConnectionOpenConfirm" => ConnectionOpenConfirmBadge
  | "/ibc.core.channel.v1.MsgChannelOpenInit" => ChannelOpenInitBadge
  | "/ibc.core.channel.v1.MsgChannelOpenTry" => ChannelOpenTryBadge
  | "/ibc.core.channel.v1.MsgChannelOpenAck" => ChannelOpenAckBadge
  | "/ibc.core.channel.v1.MsgChannelOpenConfirm" => ChannelOpenConfirmBadge
  | "/ibc.core.channel.v1.MsgChannelCloseInit" => ChannelCloseInitBadge
  | "/ibc.core.channel.v1.MsgChannelCloseConfirm" => ChannelCloseConfirmBadge
  | "timeout" => TimeoutBadge
  | "timeout_on_close" => TimeoutOnCloseBadge
  | "/ibc.core.channel.v1.MsgRecvPacket" => RecvPacketBadge
  | "/ibc.core.channel.v1.MsgAcknowledgement" => AcknowledgePacketBadge
  | "transfer" => TransferBadge
  | _ => UnknownBadge
  };
};

// for handling the empty value
let getDefaultValue = value => value |> Belt.Option.getWithDefault(_, "");

module Send = {
  type t = {
    fromAddress: Address.t,
    toAddress: Address.t,
    amount: list(Coin.t),
  };

  let decode = json =>
    JsonUtils.Decode.{
      fromAddress: json |> at(["msg", "from_address"], string) |> Address.fromBech32,
      toAddress: json |> at(["msg", "to_address"], string) |> Address.fromBech32,
      amount: json |> at(["msg", "amount"], list(Coin.decodeCoin)),
    };
};

module Receive = {
  type t = {
    fromAddress: Address.t,
    toAddress: Address.t,
    amount: list(Coin.t),
  };
};

module CreateDataSource = {
  type success_t = {
    id: ID.DataSource.t,
    owner: Address.t,
    name: string,
    executable: JsBuffer.t,
    treasury: Address.t,
    fee: list(Coin.t),
    sender: Address.t,
  };

  type fail_t = {
    owner: Address.t,
    name: string,
    executable: JsBuffer.t,
    treasury: Address.t,
    fee: list(Coin.t),
    sender: Address.t,
  };

  let decodeSuccess = json =>
    JsonUtils.Decode.{
      id: json |> at(["msg", "id"], ID.DataSource.fromJson),
      owner: json |> at(["msg", "owner"], string) |> Address.fromBech32,
      name: json |> at(["msg", "name"], string),
      executable: json |> at(["msg", "executable"], string) |> JsBuffer.fromBase64,
      treasury: json |> at(["msg", "treasury"], string) |> Address.fromBech32,
      fee: json |> at(["msg", "fee"], list(Coin.decodeCoin)),
      sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
    };

  let decodeFail = json =>
    JsonUtils.Decode.{
      owner: json |> at(["msg", "owner"], string) |> Address.fromBech32,
      name: json |> at(["msg", "name"], string),
      executable: json |> at(["msg", "executable"], string) |> JsBuffer.fromBase64,
      treasury: json |> at(["msg", "treasury"], string) |> Address.fromBech32,
      fee: json |> at(["msg", "fee"], list(Coin.decodeCoin)),
      sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
    };
};

module EditDataSource = {
  type t = {
    id: ID.DataSource.t,
    owner: Address.t,
    name: string,
    executable: JsBuffer.t,
    treasury: Address.t,
    fee: list(Coin.t),
    sender: Address.t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      id: json |> at(["msg", "data_source_id"], ID.DataSource.fromJson),
      owner: json |> at(["msg", "owner"], string) |> Address.fromBech32,
      name: json |> at(["msg", "name"], string),
      executable: json |> at(["msg", "executable"], string) |> JsBuffer.fromBase64,
      treasury: json |> at(["msg", "treasury"], string) |> Address.fromBech32,
      fee: json |> at(["msg", "fee"], list(Coin.decodeCoin)),
      sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
    };
};

module CreateOracleScript = {
  type success_t = {
    id: ID.OracleScript.t,
    owner: Address.t,
    name: string,
    code: JsBuffer.t,
    sender: Address.t,
  };

  type fail_t = {
    owner: Address.t,
    name: string,
    code: JsBuffer.t,
    sender: Address.t,
  };

  let decodeSuccess = json =>
    JsonUtils.Decode.{
      id: json |> at(["msg", "id"], ID.OracleScript.fromJson),
      owner: json |> at(["msg", "owner"], string) |> Address.fromBech32,
      name: json |> at(["msg", "name"], string),
      code: json |> at(["msg", "code"], string) |> JsBuffer.fromBase64,
      sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
    };

  let decodeFail = json =>
    JsonUtils.Decode.{
      owner: json |> at(["msg", "owner"], string) |> Address.fromBech32,
      name: json |> at(["msg", "name"], string),
      code: json |> at(["msg", "code"], string) |> JsBuffer.fromBase64,
      sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
    };
};

module EditOracleScript = {
  type t = {
    id: ID.OracleScript.t,
    owner: Address.t,
    name: string,
    code: JsBuffer.t,
    sender: Address.t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      id: json |> at(["msg", "oracle_script_id"], ID.OracleScript.fromJson),
      owner: json |> at(["msg", "owner"], string) |> Address.fromBech32,
      name: json |> at(["msg", "name"], string),
      code: json |> at(["msg", "code"], string) |> JsBuffer.fromBase64,
      sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
    };
};

module Request = {
  type success_t = {
    id: ID.Request.t,
    oracleScriptID: ID.OracleScript.t,
    oracleScriptName: string,
    calldata: JsBuffer.t,
    askCount: int,
    minCount: int,
    prepareGas: int,
    executeGas: int,
    feeLimit: list(Coin.t),
    schema: string,
    sender: Address.t,
  };

  type fail_t = {
    oracleScriptID: ID.OracleScript.t,
    calldata: JsBuffer.t,
    askCount: int,
    minCount: int,
    prepareGas: int,
    executeGas: int,
    feeLimit: list(Coin.t),
    sender: Address.t,
  };

  let decodeSuccess = json => {
    JsonUtils.Decode.{
      id: json |> at(["msg", "id"], ID.Request.fromJson),
      oracleScriptID: json |> at(["msg", "oracle_script_id"], ID.OracleScript.fromJson),
      oracleScriptName: json |> at(["msg", "name"], string),
      calldata: json |> bufferWithDefault(at(["msg", "calldata"])),
      askCount: json |> at(["msg", "ask_count"], int),
      minCount: json |> at(["msg", "min_count"], int),
      prepareGas: json |> at(["msg", "prepare_gas"], int),
      executeGas: json |> at(["msg", "execute_gas"], int),
      feeLimit: json |> at(["msg", "fee_limit"], list(Coin.decodeCoin)),
      schema: json |> at(["msg", "schema"], string),
      sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
    };
  };

  let decodeFail = json => {
    JsonUtils.Decode.{
      oracleScriptID: json |> at(["msg", "oracle_script_id"], ID.OracleScript.fromJson),
      calldata: json |> bufferWithDefault(at(["msg", "calldata"])),
      askCount: json |> at(["msg", "ask_count"], int),
      minCount: json |> at(["msg", "min_count"], int),
      prepareGas: json |> at(["msg", "prepare_gas"], int),
      executeGas: json |> at(["msg", "execute_gas"], int),
      feeLimit: json |> at(["msg", "fee_limit"], list(Coin.decodeCoin)),
      sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
    };
  };
};

module RawDataReport = {
  type t = {
    externalDataID: int,
    exitCode: int,
    data: JsBuffer.t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      externalDataID: json |> intWithDefault(field("external_id")),
      exitCode: json |> intWithDefault(field("exit_code")),
      data: json |> bufferWithDefault(field("data")),
    };
};

module Report = {
  type t = {
    requestID: ID.Request.t,
    rawReports: list(RawDataReport.t),
    validator: Address.t,
    reporter: Address.t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      requestID: json |> at(["msg", "request_id"], ID.Request.fromJson),
      rawReports: json |> at(["msg", "raw_reports"], list(RawDataReport.decode)),
      validator: json |> at(["msg", "validator"], string) |> Address.fromBech32,
      reporter: json |> at(["msg", "reporter"], string) |> Address.fromBech32,
    };
};
module Grant = {
  type t = {
    validator: Address.t,
    reporter: Address.t,
    url: string,
    expiration: MomentRe.Moment.t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      validator: json |> at(["msg", "granter"], string) |> Address.fromBech32,
      reporter: json |> at(["msg", "grantee"], string) |> Address.fromBech32,
      url: json |> at(["msg", "url"], string),
      expiration: json |> at(["msg", "grant", "expiration"], GraphQLParser.timestamp),
    };
};

module Revoke = {
  type t = {
    validator: Address.t,
    reporter: Address.t,
    msgTypeUrl: string,
  };

  let decode = json =>
    JsonUtils.Decode.{
      validator: json |> at(["msg", "granter"], string) |> Address.fromBech32,
      reporter: json |> at(["msg", "grantee"], string) |> Address.fromBech32,
      msgTypeUrl: json |> at(["msg", "msg_type_url"], string),
    };
};

module CreateValidator = {
  type t = {
    moniker: string,
    identity: string,
    website: string,
    details: string,
    commissionRate: float,
    commissionMaxRate: float,
    commissionMaxChange: float,
    delegatorAddress: Address.t,
    validatorAddress: Address.t,
    minSelfDelegation: Coin.t,
    selfDelegation: Coin.t,
  };
  let decode = json =>
    JsonUtils.Decode.{
      moniker: json |> at(["msg", "description", "moniker"], string),
      identity: json |> at(["msg", "description", "identity"], string),
      website: json |> at(["msg", "description", "website"], string),
      details: json |> at(["msg", "description", "details"], string),
      commissionRate: json |> at(["msg", "commission", "rate"], floatstr),
      commissionMaxRate: json |> at(["msg", "commission", "max_rate"], floatstr),
      commissionMaxChange: json |> at(["msg", "commission", "max_change_rate"], floatstr),
      delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
      validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
      minSelfDelegation:
        json |> at(["msg", "min_self_delegation"], floatstr) |> Coin.newUBANDFromAmount,
      selfDelegation: json |> at(["msg", "value"], Coin.decodeCoin),
    };
};

module EditValidator = {
  type t = {
    moniker: string,
    identity: string,
    website: string,
    details: string,
    commissionRate: option(float),
    sender: Address.t,
    minSelfDelegation: option(Coin.t),
  };

  let decode = json => {
    JsonUtils.Decode.{
      moniker: json |> at(["msg", "description", "moniker"], string),
      identity: json |> at(["msg", "description", "identity"], string),
      website: json |> at(["msg", "description", "website"], string),
      details: json |> at(["msg", "description", "details"], string),
      commissionRate: json |> optional(at(["msg", "commission_rate"], floatstr)),
      sender: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
      minSelfDelegation:
        json
        |> optional(at(["msg", "min_self_delegation"], floatstr))
        |> Belt.Option.map(_, Coin.newUBANDFromAmount),
    };
  };
};

module CreateClient = {
  type t = {signer: Address.t};

  let decode = json => {
    JsonUtils.Decode.{signer: json |> at(["msg", "signer"], string) |> Address.fromBech32};
  };
};

module UpdateClient = {
  type t = {
    signer: Address.t,
    clientID: string,
  };
  let decode = json => {
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      clientID: json |> at(["msg", "client_id"], string),
    };
  };
};

module UpgradeClient = {
  type t = {
    signer: Address.t,
    clientID: string,
  };
  let decode = json => {
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      clientID: json |> at(["msg", "client_id"], string),
    };
  };
};

module SubmitClientMisbehaviour = {
  type t = {
    signer: Address.t,
    clientID: string,
  };
  let decode = json => {
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      clientID: json |> at(["msg", "client_id"], string),
    };
  };
};

module Height = {
  type t = {
    revisionHeight: int,
    revisionNumber: int,
  };

  let decode = json =>
    JsonUtils.Decode.{
      revisionHeight: json |> field("revision_height", int),
      revisionNumber: json |> field("revision_number", int),
    };
};

module ConnectionCounterParty = {
  type t = {
    clientID: string,
    connectionID: string,
  };

  let decode = json =>
    JsonUtils.Decode.{
      clientID: json |> field("client_id", string),
      connectionID: json |> optional(field("connection_id", string)) |> getDefaultValue,
    };
};

module ConnectionOpenInit = {
  type t = {
    signer: Address.t,
    clientID: string,
    delayPeriod: int,
    counterparty: ConnectionCounterParty.t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      clientID: json |> at(["msg", "client_id"], string),
      delayPeriod: json |> at(["msg", "delay_period"], int),
      counterparty: json |> at(["msg", "counterparty"], ConnectionCounterParty.decode),
    };
};

module ConnectionOpenTry = {
  type t = {
    signer: Address.t,
    clientID: string,
    previousConnectionID: string,
    delayPeriod: int,
    counterparty: ConnectionCounterParty.t,
    consensusHeight: Height.t,
    proofHeight: Height.t,
  };
  let decode = json =>
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      clientID: json |> at(["msg", "client_id"], string),
      previousConnectionID: json |> at(["msg", "previous_connection_id"], string),
      delayPeriod: json |> at(["msg", "delay_period"], int),
      counterparty: json |> at(["msg", "counterparty"], ConnectionCounterParty.decode),
      consensusHeight: json |> at(["msg", "consensus_height"], Height.decode),
      proofHeight: json |> at(["msg", "proof_height"], Height.decode),
    };
};

module ConnectionOpenAck = {
  type t = {
    signer: Address.t,
    connectionID: string,
    counterpartyConnectionID: string,
    consensusHeight: Height.t,
    proofHeight: Height.t,
  };
  let decode = json =>
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      connectionID: json |> at(["msg", "connection_id"], string),
      counterpartyConnectionID: json |> at(["msg", "counterparty_connection_id"], string),
      consensusHeight: json |> at(["msg", "consensus_height"], Height.decode),
      proofHeight: json |> at(["msg", "proof_height"], Height.decode),
    };
};

module ConnectionOpenConfirm = {
  type t = {
    signer: Address.t,
    connectionID: string,
    proofHeight: Height.t,
  };
  let decode = json =>
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      connectionID: json |> at(["msg", "connection_id"], string),
      proofHeight: json |> at(["msg", "proof_height"], Height.decode),
    };
};
module ChannelCounterParty = {
  type t = {
    portID: string,
    channelID: string,
  };

  let decode = json =>
    JsonUtils.Decode.{
      portID: json |> field("port_id", string),
      channelID: json |> optional(field("channel_id", string)) |> getDefaultValue,
    };
};

let getStateText =
  fun
  | 0 => "Uninitialized"
  | 1 => "Init"
  | 2 => "Try Open"
  | 3 => "Open"
  | 4 => "Closed"
  | _ => "Unknown";

let getOrderText =
  fun
  | 0 => "None"
  | 1 => "Unordered"
  | 2 => "Ordered"
  | _ => "Unknown";

module Channel = {
  type t = {
    state: string,
    ordering: string,
    counterparty: ChannelCounterParty.t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      state: json |> field("state", int) |> getStateText,
      ordering: json |> field("ordering", int) |> getOrderText,
      counterparty: json |> field("counterparty", ChannelCounterParty.decode),
    };
};

module ChannelOpenInit = {
  type t = {
    signer: Address.t,
    portID: string,
    channel: Channel.t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      portID: json |> at(["msg", "port_id"], string),
      channel: json |> at(["msg", "channel"], Channel.decode),
    };
};

module ChannelOpenTry = {
  type t = {
    signer: Address.t,
    portID: string,
    channel: Channel.t,
    proofHeight: Height.t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      portID: json |> at(["msg", "port_id"], string),
      channel: json |> at(["msg", "channel"], Channel.decode),
      proofHeight: json |> at(["msg", "proof_height"], Height.decode),
    };
};

module ChannelOpenAck = {
  type t = {
    signer: Address.t,
    portID: string,
    channelID: string,
    counterpartyChannelID: string,
    proofHeight: Height.t,
  };
  let decode = json => {
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      portID: json |> at(["msg", "port_id"], string),
      channelID: json |> at(["msg", "channel_id"], string),
      counterpartyChannelID: json |> at(["msg", "counterparty_channel_id"], string),
      proofHeight: json |> at(["msg", "proof_height"], Height.decode),
    };
  };
};

module ChannelOpenConfirm = {
  type t = {
    signer: Address.t,
    portID: string,
    channelID: string,
    proofHeight: Height.t,
  };
  let decode = json => {
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      portID: json |> at(["msg", "port_id"], string),
      channelID: json |> at(["msg", "channel_id"], string),
      proofHeight: json |> at(["msg", "proof_height"], Height.decode),
    };
  };
};

module ChannelCloseInit = {
  type t = {
    signer: Address.t,
    portID: string,
    channelID: string,
  };

  let decode = json => {
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      portID: json |> at(["msg", "port_id"], string),
      channelID: json |> at(["msg", "channel_id"], string),
    };
  };
};

module ChannelCloseConfirm = {
  type t = {
    signer: Address.t,
    portID: string,
    channelID: string,
    proofHeight: Height.t,
  };

  let decode = json => {
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      portID: json |> at(["msg", "port_id"], string),
      channelID: json |> at(["msg", "channel_id"], string),
      proofHeight: json |> at(["msg", "proof_height"], Height.decode),
    };
  };
};

module Packet = {
  type t = {
    sequence: int,
    sourcePort: string,
    sourceChannel: string,
    destinationPort: string,
    destinationChannel: string,
    timeoutHeight: int,
    timeoutTimestamp: MomentRe.Moment.t,
    data: string,
  };

  let decode = json => {
    JsonUtils.Decode.{
      sequence: json |> field("sequence", int),
      sourcePort: json |> field("source_port", string),
      sourceChannel: json |> field("source_channel", string),
      destinationPort: json |> field("destination_port", string),
      destinationChannel: json |> field("destination_channel", string),
      timeoutHeight: json |> at(["timeout_height", "revision_height"], int),
      timeoutTimestamp: json |> at(["timeout_timestamp"], GraphQLParser.timeNS),
      data: json |> field("data", string),
    };
  };
};

module RecvPacket = {
  type success_t = {
    signer: Address.t,
    packet: Packet.t,
    proofHeight: Height.t,
    packetData: option(PacketDecoder.t),
  };

  type fail_t = {
    signer: Address.t,
    packet: Packet.t,
    proofHeight: Height.t,
  };

  let decodeSuccess = json => {
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      packet: json |> at(["msg", "packet"], Packet.decode),
      proofHeight: json |> at(["msg", "proof_height"], Height.decode),
      packetData: json |> optional(PacketDecoder.decodeAction),
    };
  };

  let decodeFail = json => {
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      packet: json |> at(["msg", "packet"], Packet.decode),
      proofHeight: json |> at(["msg", "proof_height"], Height.decode),
    };
  };
};

module AcknowledgePacket = {
  type t = {
    signer: Address.t,
    packet: Packet.t,
    proofHeight: Height.t,
  };

  let decode = json => {
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      packet: json |> at(["msg", "packet"], Packet.decode),
      proofHeight: json |> at(["msg", "proof_height"], Height.decode),
    };
  };
};

module Timeout = {
  type t = {
    signer: Address.t,
    packet: Packet.t,
    proofHeight: Height.t,
  };

  let decode = json => {
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      packet: json |> at(["msg", "packet"], Packet.decode),
      proofHeight: json |> at(["msg", "proof_height"], Height.decode),
    };
  };
};

module TimeoutOnClose = {
  type t = {
    signer: Address.t,
    packet: Packet.t,
    proofHeight: Height.t,
  };

  let decode = json => {
    JsonUtils.Decode.{
      signer: json |> at(["msg", "signer"], string) |> Address.fromBech32,
      packet: json |> at(["msg", "packet"], Packet.decode),
      proofHeight: json |> at(["msg", "proof_height"], Height.decode),
    };
  };
};

module Transfer = {
  type t = {
    sender: Address.t,
    receiver: string,
    sourcePort: string,
    sourceChannel: string,
    token: Coin.t,
    timeoutHeight: Height.t,
    timeoutTimestamp: MomentRe.Moment.t,
  };

  let decode = json => {
    JsonUtils.Decode.{
      sender: json |> at(["msg", "sender"], string) |> Address.fromBech32,
      receiver: json |> at(["msg", "receiver"], string),
      sourcePort: json |> at(["msg", "source_port"], string),
      sourceChannel: json |> at(["msg", "source_channel"], string),
      token: json |> at(["msg", "token"], Coin.decodeCoin),
      timeoutHeight: json |> at(["msg", "timeout_height"], Height.decode),
      timeoutTimestamp: json |> at(["msg", "timeout_timestamp"], GraphQLParser.timeNS),
    };
  };
};

module Delegate = {
  type success_t = {
    validatorAddress: Address.t,
    delegatorAddress: Address.t,
    amount: Coin.t,
    moniker: string,
    identity: string,
  };
  type fail_t = {
    validatorAddress: Address.t,
    delegatorAddress: Address.t,
    amount: Coin.t,
  };

  let decodeSuccess = json => {
    JsonUtils.Decode.{
      delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
      validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
      amount: json |> at(["msg", "amount"], Coin.decodeCoin),
      moniker: json |> at(["msg", "moniker"], string),
      identity: json |> at(["msg", "identity"], string),
    };
  };

  let decodeFail = json => {
    JsonUtils.Decode.{
      delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
      validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
      amount: json |> at(["msg", "amount"], Coin.decodeCoin),
    };
  };
};

module Undelegate = {
  type success_t = {
    validatorAddress: Address.t,
    delegatorAddress: Address.t,
    amount: Coin.t,
    moniker: string,
    identity: string,
  };
  type fail_t = {
    validatorAddress: Address.t,
    delegatorAddress: Address.t,
    amount: Coin.t,
  };

  let decodeSuccess = json => {
    JsonUtils.Decode.{
      delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
      validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
      amount: json |> at(["msg", "amount"], Coin.decodeCoin),
      moniker: json |> at(["msg", "moniker"], string),
      identity: json |> at(["msg", "identity"], string),
    };
  };

  let decodeFail = json => {
    JsonUtils.Decode.{
      delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
      validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
      amount: json |> at(["msg", "amount"], Coin.decodeCoin),
    };
  };
};

module Redelegate = {
  type success_t = {
    validatorSourceAddress: Address.t,
    validatorDestinationAddress: Address.t,
    delegatorAddress: Address.t,
    amount: Coin.t,
    monikerSource: string,
    monikerDestination: string,
    identitySource: string,
    identityDestination: string,
  };

  type fail_t = {
    validatorSourceAddress: Address.t,
    validatorDestinationAddress: Address.t,
    delegatorAddress: Address.t,
    amount: Coin.t,
  };
  let decodeSuccess = json => {
    JsonUtils.Decode.{
      validatorSourceAddress:
        json |> at(["msg", "validator_src_address"], string) |> Address.fromBech32,
      validatorDestinationAddress:
        json |> at(["msg", "validator_dst_address"], string) |> Address.fromBech32,
      delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
      amount: json |> at(["msg", "amount"], Coin.decodeCoin),
      monikerSource: json |> at(["msg", "val_src_moniker"], string),
      monikerDestination: json |> at(["msg", "val_dst_moniker"], string),
      identitySource: json |> at(["msg", "val_src_identity"], string),
      identityDestination: json |> at(["msg", "val_dst_identity"], string),
    };
  };

  let decodeFail = json => {
    JsonUtils.Decode.{
      validatorSourceAddress:
        json |> at(["msg", "validator_src_address"], string) |> Address.fromBech32,
      validatorDestinationAddress:
        json |> at(["msg", "validator_dst_address"], string) |> Address.fromBech32,
      delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
      amount: json |> at(["msg", "amount"], Coin.decodeCoin),
    };
  };
};

module WithdrawReward = {
  type success_t = {
    validatorAddress: Address.t,
    delegatorAddress: Address.t,
    amount: list(Coin.t),
    moniker: string,
    identity: string,
  };
  type fail_t = {
    validatorAddress: Address.t,
    delegatorAddress: Address.t,
  };

  let decodeSuccess = json => {
    JsonUtils.Decode.{
      validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
      delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
      amount: json |> at(["msg", "reward_amount"], string) |> GraphQLParser.coins,
      moniker: json |> at(["msg", "moniker"], string),
      identity: json |> at(["msg", "identity"], string),
    };
  };

  let decodeFail = json => {
    JsonUtils.Decode.{
      validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
      delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
    };
  };
};

module Unjail = {
  type t = {address: Address.t};

  let decode = json => {
    JsonUtils.Decode.{
      address: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
    };
  };
};

module SetWithdrawAddress = {
  type t = {
    delegatorAddress: Address.t,
    withdrawAddress: Address.t,
  };
  let decode = json => {
    JsonUtils.Decode.{
      delegatorAddress: json |> at(["msg", "delegator_address"], string) |> Address.fromBech32,
      withdrawAddress: json |> at(["msg", "withdraw_address"], string) |> Address.fromBech32,
    };
  };
};

module SubmitProposal = {
  type success_t = {
    proposer: Address.t,
    title: string,
    description: string,
    initialDeposit: list(Coin.t),
    proposalID: ID.Proposal.t,
  };

  type fail_t = {
    proposer: Address.t,
    title: string,
    description: string,
    initialDeposit: list(Coin.t),
  };

  let decodeSuccess = json => {
    JsonUtils.Decode.{
      proposer: json |> at(["msg", "proposer"], string) |> Address.fromBech32,
      title: json |> at(["msg", "content", "title"], string),
      description: json |> at(["msg", "content", "description"], string),
      initialDeposit: json |> at(["msg", "initial_deposit"], list(Coin.decodeCoin)),
      proposalID: json |> at(["msg", "proposal_id"], ID.Proposal.fromJson),
    };
  };

  let decodeFail = json => {
    JsonUtils.Decode.{
      proposer: json |> at(["msg", "proposer"], string) |> Address.fromBech32,
      title: json |> at(["msg", "content", "title"], string),
      description: json |> at(["msg", "content", "description"], string),
      initialDeposit: json |> at(["msg", "initial_deposit"], list(Coin.decodeCoin)),
    };
  };
};

module Deposit = {
  type success_t = {
    depositor: Address.t,
    proposalID: ID.Proposal.t,
    amount: list(Coin.t),
    title: string,
  };

  type fail_t = {
    depositor: Address.t,
    proposalID: ID.Proposal.t,
    amount: list(Coin.t),
  };

  let decodeSuccess = json => {
    JsonUtils.Decode.{
      depositor: json |> at(["msg", "depositor"], string) |> Address.fromBech32,
      proposalID: json |> at(["msg", "proposal_id"], ID.Proposal.fromJson),
      amount: json |> at(["msg", "amount"], list(Coin.decodeCoin)),
      title: json |> at(["msg", "title"], string),
    };
  };

  let decodeFail = json => {
    JsonUtils.Decode.{
      depositor: json |> at(["msg", "depositor"], string) |> Address.fromBech32,
      proposalID: json |> at(["msg", "proposal_id"], ID.Proposal.fromJson),
      amount: json |> at(["msg", "amount"], list(Coin.decodeCoin)),
    };
  };
};
module Vote = {
  exception ParseVoteNotMatch;
  let parse =
    fun
    | 0 => "Unspecified"
    | 1 => "Yes"
    | 2 => "Abstain"
    | 3 => "No"
    | 4 => "NoWithVeto"
    | _ => raise(ParseVoteNotMatch);

  type success_t = {
    voterAddress: Address.t,
    proposalID: ID.Proposal.t,
    option: string,
    title: string,
  };

  type fail_t = {
    voterAddress: Address.t,
    proposalID: ID.Proposal.t,
    option: string,
  };

  let decodeSuccess = json => {
    JsonUtils.Decode.{
      voterAddress: json |> at(["msg", "voter"], string) |> Address.fromBech32,
      proposalID: json |> at(["msg", "proposal_id"], ID.Proposal.fromJson),
      option: json |> at(["msg", "option"], int) |> parse,
      title: json |> at(["msg", "title"], string),
    };
  };

  let decodeFail = json => {
    JsonUtils.Decode.{
      voterAddress: json |> at(["msg", "voter"], string) |> Address.fromBech32,
      proposalID: json |> at(["msg", "proposal_id"], ID.Proposal.fromJson),
      option: json |> at(["msg", "option"], string),
    };
  };
};
module WithdrawCommission = {
  type success_t = {
    validatorAddress: Address.t,
    amount: list(Coin.t),
    moniker: string,
    identity: string,
  };
  type fail_t = {validatorAddress: Address.t};

  let decodeSuccess = json => {
    JsonUtils.Decode.{
      validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
      amount: json |> at(["msg", "commission_amount"], string) |> GraphQLParser.coins,
      moniker: json |> at(["msg", "moniker"], string),
      identity: json |> at(["msg", "identity"], string),
    };
  };

  let decodeFail = json => {
    JsonUtils.Decode.{
      validatorAddress: json |> at(["msg", "validator_address"], string) |> Address.fromBech32,
    };
  };
};

module MultiSend = {
  type send_tx_t = {
    address: Address.t,
    coins: list(Coin.t),
  };
  type t = {
    inputs: list(send_tx_t),
    outputs: list(send_tx_t),
  };
  let decodeSendTx = json => {
    JsonUtils.Decode.{
      address: json |> field("address", string) |> Address.fromBech32,
      coins: json |> field("coins", list(Coin.decodeCoin)),
    };
  };
  let decode = json => {
    JsonUtils.Decode.{
      inputs: json |> at(["msg", "inputs"], list(decodeSendTx)),
      outputs: json |> at(["msg", "outputs"], list(decodeSendTx)),
    };
  };
};

module Activate = {
  type t = {validatorAddress: Address.t};

  let decode = json => {
    JsonUtils.Decode.{
      validatorAddress: json |> at(["msg", "validator"], string) |> Address.fromBech32,
    };
  };
};

module Exec = {
  type success_t = {
    grantee: Address.t,
    msgs: list(ExecDecoder.t),
  };

  type fail_t = {grantee: Address.t};

  let decodeSuccess = json => {
    JsonUtils.Decode.{
      grantee: json |> at(["msg", "grantee"], string) |> Address.fromBech32,
      msgs: json |> at(["msg", "msgs"], list(ExecDecoder.decode)),
    };
  };

  let decodeFail = json => {
    JsonUtils.Decode.{grantee: json |> at(["msg", "grantee"], string) |> Address.fromBech32};
  };
};

type decoded_t =
  | SendMsgSuccess(Send.t)
  | SendMsgFail(Send.t)
  | ReceiveMsg(Receive.t)
  | CreateDataSourceMsgSuccess(CreateDataSource.success_t)
  | CreateDataSourceMsgFail(CreateDataSource.fail_t)
  | EditDataSourceMsgSuccess(EditDataSource.t)
  | EditDataSourceMsgFail(EditDataSource.t)
  | CreateOracleScriptMsgSuccess(CreateOracleScript.success_t)
  | CreateOracleScriptMsgFail(CreateOracleScript.fail_t)
  | EditOracleScriptMsgSuccess(EditOracleScript.t)
  | EditOracleScriptMsgFail(EditOracleScript.t)
  | RequestMsgSuccess(Request.success_t)
  | RequestMsgFail(Request.fail_t)
  | ReportMsgSuccess(Report.t)
  | ReportMsgFail(Report.t)
  | GrantMsg(Grant.t)
  | RevokeMsg(Revoke.t)
  | ExecMsgSuccess(Exec.success_t)
  | ExecMsgFail(Exec.fail_t)
  | CreateValidatorMsgSuccess(CreateValidator.t)
  | CreateValidatorMsgFail(CreateValidator.t)
  | EditValidatorMsgSuccess(EditValidator.t)
  | EditValidatorMsgFail(EditValidator.t)
  | DelegateMsgSuccess(Delegate.success_t)
  | DelegateMsgFail(Delegate.fail_t)
  | UndelegateMsgSuccess(Undelegate.success_t)
  | UndelegateMsgFail(Undelegate.fail_t)
  | RedelegateMsgSuccess(Redelegate.success_t)
  | RedelegateMsgFail(Redelegate.fail_t)
  | WithdrawRewardMsgSuccess(WithdrawReward.success_t)
  | WithdrawRewardMsgFail(WithdrawReward.fail_t)
  | UnjailMsgSuccess(Unjail.t)
  | UnjailMsgFail(Unjail.t)
  | SetWithdrawAddressMsgSuccess(SetWithdrawAddress.t)
  | SetWithdrawAddressMsgFail(SetWithdrawAddress.t)
  | SubmitProposalMsgSuccess(SubmitProposal.success_t)
  | SubmitProposalMsgFail(SubmitProposal.fail_t)
  | DepositMsgSuccess(Deposit.success_t)
  | DepositMsgFail(Deposit.fail_t)
  | VoteMsgSuccess(Vote.success_t)
  | VoteMsgFail(Vote.fail_t)
  | WithdrawCommissionMsgSuccess(WithdrawCommission.success_t)
  | WithdrawCommissionMsgFail(WithdrawCommission.fail_t)
  | MultiSendMsgSuccess(MultiSend.t)
  | MultiSendMsgFail(MultiSend.t)
  | ActivateMsgSuccess(Activate.t)
  | ActivateMsgFail(Activate.t)
  // IBC
  | CreateClientMsg(CreateClient.t)
  | UpdateClientMsg(UpdateClient.t)
  | UpgradeClientMsg(UpgradeClient.t)
  | SubmitClientMisbehaviourMsg(SubmitClientMisbehaviour.t)
  | ConnectionOpenInitMsg(ConnectionOpenInit.t)
  | ConnectionOpenTryMsg(ConnectionOpenTry.t)
  | ConnectionOpenAckMsg(ConnectionOpenAck.t)
  | ConnectionOpenConfirmMsg(ConnectionOpenConfirm.t)
  | ChannelOpenInitMsg(ChannelOpenInit.t)
  | ChannelOpenTryMsg(ChannelOpenTry.t)
  | ChannelOpenAckMsg(ChannelOpenAck.t)
  | ChannelOpenConfirmMsg(ChannelOpenConfirm.t)
  | ChannelCloseInitMsg(ChannelCloseInit.t)
  | ChannelCloseConfirmMsg(ChannelCloseConfirm.t)
  | AcknowledgePacketMsg(AcknowledgePacket.t)
  | RecvPacketMsgSuccess(RecvPacket.success_t)
  | RecvPacketMsgFail(RecvPacket.fail_t)
  | TimeoutMsg(Timeout.t)
  | TimeoutOnCloseMsg(TimeoutOnClose.t)
  | TransferMsg(Transfer.t)
  | UnknownMsg;

type t = {
  raw: Js.Json.t,
  decoded: decoded_t,
  isIBC: bool,
};

let isIBC =
  fun
  | SendMsgSuccess(_)
  | SendMsgFail(_)
  | ReceiveMsg(_)
  | CreateDataSourceMsgSuccess(_)
  | CreateDataSourceMsgFail(_)
  | EditDataSourceMsgSuccess(_)
  | EditDataSourceMsgFail(_)
  | CreateOracleScriptMsgSuccess(_)
  | CreateOracleScriptMsgFail(_)
  | EditOracleScriptMsgSuccess(_)
  | EditOracleScriptMsgFail(_)
  | RequestMsgSuccess(_)
  | RequestMsgFail(_)
  | ReportMsgSuccess(_)
  | ReportMsgFail(_)
  | GrantMsg(_)
  | RevokeMsg(_)
  | ExecMsgSuccess(_)
  | ExecMsgFail(_)
  | CreateValidatorMsgSuccess(_)
  | CreateValidatorMsgFail(_)
  | EditValidatorMsgSuccess(_)
  | EditValidatorMsgFail(_)
  | DelegateMsgSuccess(_)
  | DelegateMsgFail(_)
  | UndelegateMsgSuccess(_)
  | UndelegateMsgFail(_)
  | RedelegateMsgSuccess(_)
  | RedelegateMsgFail(_)
  | WithdrawRewardMsgSuccess(_)
  | WithdrawRewardMsgFail(_)
  | UnjailMsgSuccess(_)
  | UnjailMsgFail(_)
  | SetWithdrawAddressMsgSuccess(_)
  | SetWithdrawAddressMsgFail(_)
  | SubmitProposalMsgSuccess(_)
  | SubmitProposalMsgFail(_)
  | DepositMsgSuccess(_)
  | DepositMsgFail(_)
  | VoteMsgSuccess(_)
  | VoteMsgFail(_)
  | WithdrawCommissionMsgSuccess(_)
  | WithdrawCommissionMsgFail(_)
  | MultiSendMsgSuccess(_)
  | MultiSendMsgFail(_)
  | ActivateMsgSuccess(_)
  | ActivateMsgFail(_)
  | UnknownMsg => false
  // IBC
  | CreateClientMsg(_)
  | UpdateClientMsg(_)
  | UpgradeClientMsg(_)
  | SubmitClientMisbehaviourMsg(_)
  | ConnectionOpenInitMsg(_)
  | ConnectionOpenTryMsg(_)
  | ConnectionOpenAckMsg(_)
  | ConnectionOpenConfirmMsg(_)
  | ChannelOpenInitMsg(_)
  | ChannelOpenTryMsg(_)
  | ChannelOpenAckMsg(_)
  | ChannelOpenConfirmMsg(_)
  | ChannelCloseInitMsg(_)
  | ChannelCloseConfirmMsg(_)
  | AcknowledgePacketMsg(_)
  | RecvPacketMsgSuccess(_)
  | RecvPacketMsgFail(_)
  | TimeoutMsg(_)
  | TimeoutOnCloseMsg(_)
  | TransferMsg(_) => true;

let getCreator = msg => {
  switch (msg.decoded) {
  | ReceiveMsg(receive) => receive.toAddress // receiver
  | SendMsgSuccess(send)
  | SendMsgFail(send) => send.fromAddress
  | CreateDataSourceMsgSuccess(dataSource) => dataSource.sender
  | CreateDataSourceMsgFail(dataSource) => dataSource.sender
  | EditDataSourceMsgSuccess(dataSource) => dataSource.sender
  | EditDataSourceMsgFail(dataSource) => dataSource.sender
  | CreateOracleScriptMsgSuccess(oracleScript) => oracleScript.sender
  | CreateOracleScriptMsgFail(oracleScript) => oracleScript.sender
  | EditOracleScriptMsgSuccess(oracleScript) => oracleScript.sender
  | EditOracleScriptMsgFail(oracleScript) => oracleScript.sender
  | RequestMsgSuccess(request) => request.sender
  | RequestMsgFail(request) => request.sender
  | ReportMsgSuccess(report)
  | ReportMsgFail(report) => report.reporter
  | GrantMsg(address) => address.validator
  | RevokeMsg(address) => address.validator
  | ExecMsgSuccess(address) => address.grantee
  | ExecMsgFail(address) => address.grantee
  | CreateValidatorMsgSuccess(validator)
  | CreateValidatorMsgFail(validator) => validator.delegatorAddress
  | EditValidatorMsgSuccess(validator)
  | EditValidatorMsgFail(validator) => validator.sender
  | DelegateMsgSuccess(delegation) => delegation.delegatorAddress
  | DelegateMsgFail(delegation) => delegation.delegatorAddress
  | UndelegateMsgSuccess(delegation) => delegation.delegatorAddress
  | UndelegateMsgFail(delegation) => delegation.delegatorAddress
  | RedelegateMsgSuccess(delegation) => delegation.delegatorAddress
  | RedelegateMsgFail(delegation) => delegation.delegatorAddress
  | WithdrawRewardMsgSuccess(withdrawal) => withdrawal.delegatorAddress
  | WithdrawRewardMsgFail(withdrawal) => withdrawal.delegatorAddress
  | UnjailMsgSuccess(validator) => validator.address
  | UnjailMsgFail(validator) => validator.address
  | SetWithdrawAddressMsgSuccess(set)
  | SetWithdrawAddressMsgFail(set) => set.delegatorAddress
  | SubmitProposalMsgSuccess(proposal) => proposal.proposer
  | SubmitProposalMsgFail(proposal) => proposal.proposer
  | DepositMsgSuccess(deposit) => deposit.depositor
  | DepositMsgFail(deposit) => deposit.depositor
  | VoteMsgSuccess(vote) => vote.voterAddress
  | VoteMsgFail(vote) => vote.voterAddress
  | WithdrawCommissionMsgSuccess(withdrawal) => withdrawal.validatorAddress
  | WithdrawCommissionMsgFail(withdrawal) => withdrawal.validatorAddress
  | MultiSendMsgSuccess(tx)
  | MultiSendMsgFail(tx) =>
    let firstInput = tx.inputs |> Belt_List.getExn(_, 0);
    firstInput.address;
  | ActivateMsgSuccess(activator)
  | ActivateMsgFail(activator) => activator.validatorAddress
  //IBC
  | CreateClientMsg(client) => client.signer
  | UpdateClientMsg(client) => client.signer
  | UpgradeClientMsg(client) => client.signer
  | SubmitClientMisbehaviourMsg(client) => client.signer
  | ConnectionOpenInitMsg(connection) => connection.signer
  | ConnectionOpenTryMsg(connection) => connection.signer
  | ConnectionOpenAckMsg(connection) => connection.signer
  | ConnectionOpenConfirmMsg(connection) => connection.signer
  | ChannelOpenInitMsg(channel) => channel.signer
  | ChannelOpenTryMsg(channel) => channel.signer
  | ChannelOpenAckMsg(channel) => channel.signer
  | ChannelOpenConfirmMsg(channel) => channel.signer
  | ChannelCloseInitMsg(channel) => channel.signer
  | ChannelCloseConfirmMsg(channel) => channel.signer
  | RecvPacketMsgSuccess(packet) => packet.signer
  | RecvPacketMsgFail(packet) => packet.signer
  | AcknowledgePacketMsg(packet) => packet.signer
  | TimeoutMsg(timeout) => timeout.signer
  | TimeoutOnCloseMsg(timeout) => timeout.signer
  | TransferMsg(message) => message.sender
  | _ => "" |> Address.fromHex
  };
};

type badge_theme_t = {
  name: string,
  category: msg_cat_t,
};

let getBadge = badgeVariant => {
  switch (badgeVariant) {
  | SendBadge => {name: "Send", category: TokenMsg}
  | ReceiveBadge => {name: "Receive", category: TokenMsg}
  | CreateDataSourceBadge => {name: "Create Data Source", category: DataMsg}
  | EditDataSourceBadge => {name: "Edit Data Source", category: DataMsg}
  | CreateOracleScriptBadge => {name: "Create Oracle Script", category: DataMsg}
  | EditOracleScriptBadge => {name: "Edit Oracle Script", category: DataMsg}
  | RequestBadge => {name: "Request", category: DataMsg}
  | ReportBadge => {name: "Report", category: DataMsg}
  | GrantBadge => {name: "Grant", category: ValidatorMsg}
  | RevokeBadge => {name: "Revoke", category: ValidatorMsg}
  | ExecBadge => {name: "Exec", category: ValidatorMsg}
  | CreateValidatorBadge => {name: "Create Validator", category: ValidatorMsg}
  | EditValidatorBadge => {name: "Edit Validator", category: ValidatorMsg}
  | DelegateBadge => {name: "Delegate", category: TokenMsg}
  | UndelegateBadge => {name: "Undelegate", category: TokenMsg}
  | RedelegateBadge => {name: "Redelegate", category: TokenMsg}
  | VoteBadge => {name: "Vote", category: ProposalMsg}
  | WithdrawRewardBadge => {name: "Withdraw Reward", category: TokenMsg}
  | UnjailBadge => {name: "Unjail", category: ValidatorMsg}
  | SetWithdrawAddressBadge => {name: "Set Withdraw Address", category: ValidatorMsg}
  | SubmitProposalBadge => {name: "Submit Proposal", category: ProposalMsg}
  | DepositBadge => {name: "Deposit", category: ProposalMsg}
  | WithdrawCommissionBadge => {name: "Withdraw Commission", category: TokenMsg}
  | MultiSendBadge => {name: "Multi Send", category: TokenMsg}
  | ActivateBadge => {name: "Activate", category: ValidatorMsg}
  | UnknownBadge => {name: "Unknown", category: TokenMsg}
  //IBC
  | CreateClientBadge => {name: "Create Client", category: IBCClientMsg}
  | UpdateClientBadge => {name: "Update Client", category: IBCClientMsg}
  | UpgradeClientBadge => {name: "Upgrade Client", category: IBCClientMsg}
  | SubmitClientMisbehaviourBadge => {name: "Submit Client Misbehaviour", category: IBCClientMsg}
  | ConnectionOpenInitBadge => {name: "Connection Open Init", category: IBCConnectionMsg}
  | ConnectionOpenTryBadge => {name: "Connection Open Try", category: IBCConnectionMsg}
  | ConnectionOpenAckBadge => {name: "Connection Open Ack", category: IBCConnectionMsg}
  | ConnectionOpenConfirmBadge => {name: "Connection Open Confirm", category: IBCConnectionMsg}
  | ChannelOpenInitBadge => {name: "Channel Open Init", category: IBCChannelMsg}
  | ChannelOpenTryBadge => {name: "Channel Open Try", category: IBCChannelMsg}
  | ChannelOpenAckBadge => {name: "Channel Open Ack", category: IBCChannelMsg}
  | ChannelOpenConfirmBadge => {name: "Channel Open Confirm", category: IBCChannelMsg}
  | ChannelCloseInitBadge => {name: "Channel Close Init", category: IBCChannelMsg}
  | ChannelCloseConfirmBadge => {name: "Channel Close Confirm", category: IBCPacketMsg}
  | RecvPacketBadge => {name: "Recv Packet", category: IBCPacketMsg}
  | AcknowledgePacketBadge => {name: "Acknowledge Packet", category: IBCPacketMsg}
  | TimeoutBadge => {name: "Timeout", category: IBCPacketMsg}
  | TimeoutOnCloseBadge => {name: "Timeout", category: IBCPacketMsg}
  | TransferBadge => {name: "Transfer", category: IBCTransferMsg}
  };
};

let getBadgeTheme = msg => {
  switch (msg.decoded) {
  | SendMsgSuccess(_)
  | SendMsgFail(_) => getBadge(SendBadge)
  | ReceiveMsg(_) => getBadge(ReceiveBadge)
  | CreateDataSourceMsgSuccess(_)
  | CreateDataSourceMsgFail(_) => getBadge(CreateDataSourceBadge)
  | EditDataSourceMsgSuccess(_)
  | EditDataSourceMsgFail(_) => getBadge(EditDataSourceBadge)
  | CreateOracleScriptMsgSuccess(_)
  | CreateOracleScriptMsgFail(_) => getBadge(CreateOracleScriptBadge)
  | EditOracleScriptMsgSuccess(_)
  | EditOracleScriptMsgFail(_) => getBadge(EditOracleScriptBadge)
  | RequestMsgSuccess(_)
  | RequestMsgFail(_) => getBadge(RequestBadge)
  | ReportMsgSuccess(_)
  | ReportMsgFail(_) => getBadge(ReportBadge)
  | ExecMsgSuccess(_)
  | ExecMsgFail(_) => getBadge(ExecBadge)
  | GrantMsg(_) => getBadge(GrantBadge)
  | RevokeMsg(_) => getBadge(RevokeBadge)
  | CreateValidatorMsgSuccess(_)
  | CreateValidatorMsgFail(_) => getBadge(CreateValidatorBadge)
  | EditValidatorMsgSuccess(_)
  | EditValidatorMsgFail(_) => getBadge(EditValidatorBadge)
  | DelegateMsgSuccess(_)
  | DelegateMsgFail(_) => getBadge(DelegateBadge)
  | UndelegateMsgSuccess(_)
  | UndelegateMsgFail(_) => getBadge(UndelegateBadge)
  | RedelegateMsgSuccess(_)
  | RedelegateMsgFail(_) => getBadge(RedelegateBadge)
  | VoteMsgSuccess(_)
  | VoteMsgFail(_) => getBadge(VoteBadge)
  | WithdrawRewardMsgSuccess(_)
  | WithdrawRewardMsgFail(_) => getBadge(WithdrawRewardBadge)
  | UnjailMsgSuccess(_)
  | UnjailMsgFail(_) => getBadge(UnjailBadge)
  | SetWithdrawAddressMsgSuccess(_)
  | SetWithdrawAddressMsgFail(_) => getBadge(SetWithdrawAddressBadge)
  | SubmitProposalMsgSuccess(_)
  | SubmitProposalMsgFail(_) => getBadge(SubmitProposalBadge)
  | DepositMsgSuccess(_)
  | DepositMsgFail(_) => getBadge(DepositBadge)
  | WithdrawCommissionMsgSuccess(_)
  | WithdrawCommissionMsgFail(_) => getBadge(WithdrawCommissionBadge)
  | MultiSendMsgSuccess(_) => getBadge(MultiSendBadge)
  | MultiSendMsgFail(_) => getBadge(MultiSendBadge)
  | ActivateMsgSuccess(_) => getBadge(ActivateBadge)
  | ActivateMsgFail(_) => getBadge(ActivateBadge)
  | UnknownMsg => getBadge(UnknownBadge)
  //IBC
  | CreateClientMsg(_) => getBadge(CreateClientBadge)
  | UpdateClientMsg(_) => getBadge(UpdateClientBadge)
  | UpgradeClientMsg(_) => getBadge(UpgradeClientBadge)
  | SubmitClientMisbehaviourMsg(_) => getBadge(SubmitClientMisbehaviourBadge)
  | ConnectionOpenInitMsg(_) => getBadge(ConnectionOpenInitBadge)
  | ConnectionOpenTryMsg(_) => getBadge(ConnectionOpenTryBadge)
  | ConnectionOpenAckMsg(_) => getBadge(ConnectionOpenAckBadge)
  | ConnectionOpenConfirmMsg(_) => getBadge(ConnectionOpenConfirmBadge)
  | ChannelOpenInitMsg(_) => getBadge(ChannelOpenInitBadge)
  | ChannelOpenTryMsg(_) => getBadge(ChannelOpenTryBadge)
  | ChannelOpenAckMsg(_) => getBadge(ChannelOpenAckBadge)
  | ChannelOpenConfirmMsg(_) => getBadge(ChannelOpenConfirmBadge)
  | ChannelCloseInitMsg(_) => getBadge(ChannelCloseInitBadge)
  | ChannelCloseConfirmMsg(_) => getBadge(ChannelCloseConfirmBadge)
  | RecvPacketMsgSuccess(_)
  | RecvPacketMsgFail(_) => getBadge(RecvPacketBadge)
  | AcknowledgePacketMsg(_) => getBadge(AcknowledgePacketBadge)
  | TimeoutMsg(_) => getBadge(TimeoutBadge)
  | TimeoutOnCloseMsg(_) => getBadge(TimeoutOnCloseBadge)
  | TransferMsg(_) => getBadge(TransferBadge)
  };
};

let decodeAction = json => {
  let decoded =
    JsonUtils.Decode.(
      switch (json |> field("type", string) |> getBadgeVariantFromString) {
      | SendBadge => SendMsgSuccess(json |> Send.decode)
      | ReceiveBadge => raise(Not_found)
      | CreateDataSourceBadge =>
        CreateDataSourceMsgSuccess(json |> CreateDataSource.decodeSuccess)
      | EditDataSourceBadge => EditDataSourceMsgSuccess(json |> EditDataSource.decode)
      | CreateOracleScriptBadge =>
        CreateOracleScriptMsgSuccess(json |> CreateOracleScript.decodeSuccess)
      | EditOracleScriptBadge => EditOracleScriptMsgSuccess(json |> EditOracleScript.decode)
      | RequestBadge => RequestMsgSuccess(json |> Request.decodeSuccess)
      | ReportBadge => ReportMsgSuccess(json |> Report.decode)
      | GrantBadge => GrantMsg(json |> Grant.decode)
      | RevokeBadge => RevokeMsg(json |> Revoke.decode)
      | ExecBadge => ExecMsgSuccess(json |> Exec.decodeSuccess)
      | CreateValidatorBadge => CreateValidatorMsgSuccess(json |> CreateValidator.decode)
      | EditValidatorBadge => EditValidatorMsgSuccess(json |> EditValidator.decode)
      | DelegateBadge => DelegateMsgSuccess(json |> Delegate.decodeSuccess)
      | UndelegateBadge => UndelegateMsgSuccess(json |> Undelegate.decodeSuccess)
      | RedelegateBadge => RedelegateMsgSuccess(json |> Redelegate.decodeSuccess)
      | WithdrawRewardBadge => WithdrawRewardMsgSuccess(json |> WithdrawReward.decodeSuccess)
      | UnjailBadge => UnjailMsgSuccess(json |> Unjail.decode)
      | SetWithdrawAddressBadge => SetWithdrawAddressMsgSuccess(json |> SetWithdrawAddress.decode)
      | SubmitProposalBadge => SubmitProposalMsgSuccess(json |> SubmitProposal.decodeSuccess)
      | DepositBadge => DepositMsgSuccess(json |> Deposit.decodeSuccess)
      | VoteBadge => VoteMsgSuccess(json |> Vote.decodeSuccess)
      | WithdrawCommissionBadge =>
        WithdrawCommissionMsgSuccess(json |> WithdrawCommission.decodeSuccess)
      | MultiSendBadge => MultiSendMsgSuccess(json |> MultiSend.decode)
      | ActivateBadge => ActivateMsgSuccess(json |> Activate.decode)
      | UnknownBadge => UnknownMsg
      //IBC
      | CreateClientBadge => CreateClientMsg(json |> CreateClient.decode)
      | UpdateClientBadge => UpdateClientMsg(json |> UpdateClient.decode)
      | UpgradeClientBadge => UpgradeClientMsg(json |> UpgradeClient.decode)
      | SubmitClientMisbehaviourBadge =>
        SubmitClientMisbehaviourMsg(json |> SubmitClientMisbehaviour.decode)
      | ConnectionOpenInitBadge => ConnectionOpenInitMsg(json |> ConnectionOpenInit.decode)
      | ConnectionOpenTryBadge => ConnectionOpenTryMsg(json |> ConnectionOpenTry.decode)
      | ConnectionOpenAckBadge => ConnectionOpenAckMsg(json |> ConnectionOpenAck.decode)
      | ConnectionOpenConfirmBadge =>
        ConnectionOpenConfirmMsg(json |> ConnectionOpenConfirm.decode)
      | ChannelOpenInitBadge => ChannelOpenInitMsg(json |> ChannelOpenInit.decode)
      | ChannelOpenTryBadge => ChannelOpenTryMsg(json |> ChannelOpenTry.decode)
      | ChannelOpenAckBadge => ChannelOpenAckMsg(json |> ChannelOpenAck.decode)
      | ChannelOpenConfirmBadge => ChannelOpenConfirmMsg(json |> ChannelOpenConfirm.decode)
      | ChannelCloseInitBadge => ChannelCloseInitMsg(json |> ChannelCloseInit.decode)
      | ChannelCloseConfirmBadge => ChannelCloseConfirmMsg(json |> ChannelCloseConfirm.decode)
      | RecvPacketBadge => RecvPacketMsgSuccess(json |> RecvPacket.decodeSuccess)
      | AcknowledgePacketBadge => AcknowledgePacketMsg(json |> AcknowledgePacket.decode)
      | TimeoutBadge => TimeoutMsg(json |> Timeout.decode)
      | TimeoutOnCloseBadge => TimeoutOnCloseMsg(json |> TimeoutOnClose.decode)
      | TransferBadge => TransferMsg(json |> Transfer.decode)
      }
    );
  {raw: json, decoded, isIBC: decoded |> isIBC};
};

let decodeFailAction = json => {
  let decoded =
    JsonUtils.Decode.(
      switch (json |> field("type", string) |> getBadgeVariantFromString) {
      | SendBadge => SendMsgFail(json |> Send.decode)
      | ReceiveBadge => raise(Not_found)
      | CreateDataSourceBadge => CreateDataSourceMsgFail(json |> CreateDataSource.decodeFail)
      | EditDataSourceBadge => EditDataSourceMsgFail(json |> EditDataSource.decode)
      | CreateOracleScriptBadge =>
        CreateOracleScriptMsgFail(json |> CreateOracleScript.decodeFail)
      | EditOracleScriptBadge => EditOracleScriptMsgFail(json |> EditOracleScript.decode)
      | RequestBadge => RequestMsgFail(json |> Request.decodeFail)
      | ReportBadge => ReportMsgFail(json |> Report.decode)
      | GrantBadge => GrantMsg(json |> Grant.decode)
      | RevokeBadge => RevokeMsg(json |> Revoke.decode)
      | ExecBadge => ExecMsgFail(json |> Exec.decodeFail)
      | CreateValidatorBadge => CreateValidatorMsgFail(json |> CreateValidator.decode)
      | EditValidatorBadge => EditValidatorMsgFail(json |> EditValidator.decode)
      | DelegateBadge => DelegateMsgFail(json |> Delegate.decodeFail)
      | UndelegateBadge => UndelegateMsgFail(json |> Undelegate.decodeFail)
      | RedelegateBadge => RedelegateMsgFail(json |> Redelegate.decodeFail)
      | WithdrawRewardBadge => WithdrawRewardMsgFail(json |> WithdrawReward.decodeFail)
      | UnjailBadge => UnjailMsgFail(json |> Unjail.decode)
      | SetWithdrawAddressBadge => SetWithdrawAddressMsgFail(json |> SetWithdrawAddress.decode)
      | SubmitProposalBadge => SubmitProposalMsgFail(json |> SubmitProposal.decodeFail)
      | DepositBadge => DepositMsgFail(json |> Deposit.decodeFail)
      | VoteBadge => VoteMsgFail(json |> Vote.decodeFail)
      | WithdrawCommissionBadge =>
        WithdrawCommissionMsgFail(json |> WithdrawCommission.decodeFail)
      | MultiSendBadge => MultiSendMsgFail(json |> MultiSend.decode)
      | ActivateBadge => ActivateMsgFail(json |> Activate.decode)
      | UnknownBadge => UnknownMsg
      //IBC
      | CreateClientBadge => CreateClientMsg(json |> CreateClient.decode)
      | UpdateClientBadge => UpdateClientMsg(json |> UpdateClient.decode)
      | UpgradeClientBadge => UpgradeClientMsg(json |> UpgradeClient.decode)
      | SubmitClientMisbehaviourBadge =>
        SubmitClientMisbehaviourMsg(json |> SubmitClientMisbehaviour.decode)
      | ConnectionOpenInitBadge => ConnectionOpenInitMsg(json |> ConnectionOpenInit.decode)
      | ConnectionOpenTryBadge => ConnectionOpenTryMsg(json |> ConnectionOpenTry.decode)
      | ConnectionOpenAckBadge => ConnectionOpenAckMsg(json |> ConnectionOpenAck.decode)
      | ConnectionOpenConfirmBadge =>
        ConnectionOpenConfirmMsg(json |> ConnectionOpenConfirm.decode)
      | ChannelOpenInitBadge => ChannelOpenInitMsg(json |> ChannelOpenInit.decode)
      | ChannelOpenTryBadge => ChannelOpenTryMsg(json |> ChannelOpenTry.decode)
      | ChannelOpenAckBadge => ChannelOpenAckMsg(json |> ChannelOpenAck.decode)
      | ChannelOpenConfirmBadge => ChannelOpenConfirmMsg(json |> ChannelOpenConfirm.decode)
      | ChannelCloseInitBadge => ChannelCloseInitMsg(json |> ChannelCloseInit.decode)
      | ChannelCloseConfirmBadge => ChannelCloseConfirmMsg(json |> ChannelCloseConfirm.decode)
      | RecvPacketBadge => RecvPacketMsgFail(json |> RecvPacket.decodeFail)
      | AcknowledgePacketBadge => AcknowledgePacketMsg(json |> AcknowledgePacket.decode)
      | TimeoutBadge => TimeoutMsg(json |> Timeout.decode)
      | TimeoutOnCloseBadge => TimeoutOnCloseMsg(json |> TimeoutOnClose.decode)
      | TransferBadge => TransferMsg(json |> Transfer.decode)
      }
    );
  {raw: json, decoded, isIBC: decoded |> isIBC};
};
