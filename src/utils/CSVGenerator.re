let getName = msg => {
  switch (msg) {
  | "/cosmos.bank.v1beta1.MsgSend" => "Send"
  | "receive" => "Not Found"
  | "/oracle.v1.MsgCreateDataSource" => "CreateDataSource"
  | "/oracle.v1.MsgEditDataSource" => "EditDataSource"
  | "/oracle.v1.MsgCreateOracleScript" => "CreateOracleScript"
  | "/oracle.v1.MsgEditOracleScript" => "EditOracleScript"
  | "/oracle.v1.MsgRequestData" => "Request"
  | "/oracle.v1.MsgReportData" => "Report"
  | "/cosmos.authz.v1beta1.MsgGrant" => "Grant"
  | "/cosmos.authz.v1beta1.MsgRevoke" => "Revoke"
  | "/cosmos.authz.v1beta1.MsgExec" => "Exec"
  | "/cosmos.staking.v1beta1.MsgCreateValidator" => "CreateValidator"
  | "/cosmos.staking.v1beta1.MsgEditValidator" => "EditValidator"
  | "/cosmos.staking.v1beta1.MsgDelegate" => "Delegate"
  | "/cosmos.staking.v1beta1.MsgUndelegate" => "Undelegate"
  | "/cosmos.staking.v1beta1.MsgBeginRedelegate" => "Redelegate"
  | "/cosmos.distribution.v1beta1.MsgWithdrawDelegatorReward" => "WithdrawReward"
  | "/cosmos.slashing.v1beta1.MsgUnjail" => "Unjail"
  | "/cosmos.distribution.v1beta1.MsgSetWithdrawAddress" => "SetWithdrawAddress"
  | "/cosmos.gov.v1beta1.MsgSubmitProposal" => "SubmitProposal"
  | "/cosmos.gov.v1beta1.MsgDeposit" => "Deposit"
  | "/cosmos.gov.v1beta1.MsgVote" => "Vote"
  | "/cosmos.gov.v1beta1.MsgVoteWeighted" => "VoteWeighted"
  | "/cosmos.distribution.v1beta1.MsgWithdrawValidatorCommission" => "WithdrawCommission"
  | "/cosmos.bank.v1beta1.MsgMultiSend" => "MultiSend"
  | "/oracle.v1.MsgActivate" => "Activate"
  | "/ibc.core.client.v1.MsgCreateClient" => "CreateClient"
  | "/ibc.core.client.v1.MsgUpdateClient" => "UpdateClient"
  | "/ibc.core.client.v1.MsgUpgradeClient" => "UpgradeClient"
  | "/ibc.core.client.v1.MsgSubmitClientMisbehaviour" => "SubmitClientMisbehaviour"
  | "/ibc.core.connection.v1.MsgConnectionOpenInit" => "ConnectionOpenInit"
  | "/ibc.core.connection.v1.MsgConnectionOpenTry" => "ConnectionOpenTry"
  | "/ibc.core.connection.v1.MsgConnectionOpenAck" => "ConnectionOpenAck"
  | "/ibc.core.connection.v1.MsgConnectionOpenConfirm" => "ConnectionOpenConfirm"
  | "/ibc.core.channel.v1.MsgChannelOpenInit" => "ChannelOpenInit"
  | "/ibc.core.channel.v1.MsgChannelOpenTry" => "ChannelOpenTry"
  | "/ibc.core.channel.v1.MsgChannelOpenAck" => "ChannelOpenAck"
  | "/ibc.core.channel.v1.MsgChannelOpenConfirm" => "ChannelOpenConfirm"
  | "/ibc.core.channel.v1.MsgChannelCloseInit" => "ChannelCloseInit"
  | "/ibc.core.channel.v1.MsgChannelCloseConfirm" => "ChannelCloseConfirm"
  | "timeout" => "Timeout"
  | "timeout_on_close" => "TimeoutOnClose"
  | "/ibc.core.channel.v1.MsgRecvPacket" => "RecvPacket"
  | "/ibc.core.channel.v1.MsgAcknowledgement" => "AcknowledgePacket"
  | "/ibc.applications.transfer.v1.MsgTransfer" => "Transfer"
  | _ => "Unknown"
  };
};
let create = (data: array(TxQueryByBlockTimestamp.t)) => {
  let keys =
    [|
      "TxHash",
      "BlockNo",
      "Success",
      "GasFee",
      "GasLimit",
      "GasUsed",
      "From",
      "UnixTimestamp",
      "DateTime",
      "Method",
      "Memo",
    |]
    |> Js.Array.joinWith(",");
  let result =
    data->Belt.Array.map(item => {
      let msgString =
        item.messages->Belt.List.toArray->Belt.Array.map(msg => getName(msg))
        |> Js.Array.joinWith(";");
      item.txHash
      ++ ","
      ++ (item.blockHeight |> ID.Block.toInt |> string_of_int)
      ++ ","
      ++ (item.success |> string_of_bool)
      ++ ","
      ++ (item.gasFee |> Coin.getBandAmountFromCoins |> Format.fPretty(~digits=2))
      ++ ","
      ++ (item.gasLimit |> string_of_int)
      ++ ","
      ++ (item.gasUsed |> string_of_int)
      ++ ","
      ++ item.from
      ++ ","
      ++ (item.timestamp |> MomentRe.Moment.toUnix |> string_of_int)
      ++ ","
      ++ (item.timestamp |> MomentRe.Moment.format(Config.timestampDisplayFormat))
      ++ ","
      ++ msgString
      ++ ","
      ++ item.memo;
    })
    |> Js.Array.joinWith("\n");
  keys ++ "\n" ++ result;
};
