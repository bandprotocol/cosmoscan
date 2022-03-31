module Client = {
  type t;

  type reference_data_t = {
    pair: string,
    rate: float,
  };

  [@bs.module "@bandprotocol/bandchain.js"] [@bs.new] external create: string => t = "Client";
  [@bs.send]
  external getReferenceData:
    (t, array(string), int, int) => Js.Promise.t(array(reference_data_t)) =
    "getReferenceData";
  [@bs.send] external sendTxBlockMode: (t, JsBuffer.t) => Js.Promise.t('a) = "sendTxBlockMode";
  [@bs.send] external sendTxSyncMode: (t, JsBuffer.t) => Js.Promise.t('a) = "sendTxSyncMode";
};

module PubKey = {
  type t;

  [@bs.module "@bandprotocol/bandchain.js"] [@bs.scope ("Wallet", "PublicKey")] [@bs.val]
  external fromHex: string => t = "fromHex";

  [@bs.send] external toHex: t => string = "toHex";
};

module Coin = {
  type t;

  [@bs.module "@bandprotocol/bandchain.js"] [@bs.new] external create: unit => t = "Coin";
  [@bs.send] external setDenom: (t, string) => unit = "setDenom";
  [@bs.send] external setAmount: (t, string) => unit = "setAmount";
};

module Fee = {
  type t;

  [@bs.module "@bandprotocol/bandchain.js"] [@bs.new] external create: unit => t = "Fee";
  [@bs.send] external setAmountList: (t, array(Coin.t)) => unit = "setAmountList";
  [@bs.send] external setGasLimit: (t, int) => unit = "setGasLimit";
};

type msg_t;
module MsgSend = {
  [@bs.module "@bandprotocol/bandchain.js"] [@bs.scope "Message"] [@bs.new]
  external create: (string, string, array(Coin.t)) => msg_t = "MsgSend";
};

module MsgDelegate = {
  [@bs.module "@bandprotocol/bandchain.js"] [@bs.scope "Message"] [@bs.new]
  external create: (string, string, Coin.t) => msg_t = "MsgDelegate";
};

module MsgUndelegate = {
  [@bs.module "@bandprotocol/bandchain.js"] [@bs.scope "Message"] [@bs.new]
  external create: (string, string, Coin.t) => msg_t = "MsgUndelegate";
};

module MsgRedelegate = {
  [@bs.module "@bandprotocol/bandchain.js"] [@bs.scope "Message"] [@bs.new]
  external create: (string, string, string, Coin.t) => msg_t = "MsgBeginRedelegate";
};

module MsgWithdrawReward = {
  [@bs.module "@bandprotocol/bandchain.js"] [@bs.scope "Message"] [@bs.new]
  external create: (string, string) => msg_t = "MsgWithdrawDelegatorReward";
};

module MsgVote = {
  [@bs.module "@bandprotocol/bandchain.js"] [@bs.scope "Message"] [@bs.new]
  external create: (int, string, int) => msg_t = "MsgVote";
};

module MsgRequest = {
  [@bs.module "@bandprotocol/bandchain.js"] [@bs.scope "Message"] [@bs.new]
  external create:
    (int, JsBuffer.t, int, int, string, string, array(Coin.t), option(int), option(int)) =>
    msg_t =
    "MsgRequestData";
};

module MsgTransfer = {
  [@bs.module "@bandprotocol/bandchain.js"] [@bs.scope "Message"] [@bs.new]
  external create: (string, string, string, string, Coin.t, float) => msg_t = "MsgTransfer";
};

module Transaction = {
  type t;

  [@bs.module "@bandprotocol/bandchain.js"] [@bs.new] external create: unit => t = "Transaction";
  [@bs.send] external withMessage: (t, msg_t) => unit = "withMessages";
  [@bs.send] external withChainId: (t, string) => unit = "withChainId";
  [@bs.send] external withSender: (t, Client.t, string) => Js.Promise.t(t) = "withSender";
  [@bs.send] external withAccountNum: (t, int) => unit = "withAccountNum";
  [@bs.send] external withSequence: (t, int) => unit = "withSequence";
  [@bs.send] external withFee: (t, Fee.t) => unit = "withFee";
  [@bs.send] external withMemo: (t, string) => unit = "withMemo";
  [@bs.send] external getSignDoc: (t, PubKey.t, int) => array(int) = "getSignDoc";
  [@bs.send] external getTxData: (t, JsBuffer.t, PubKey.t, int) => JsBuffer.t = "getTxData";
  [@bs.send] external getSignMessage: t => JsBuffer.t = "getSignMessage";
};
