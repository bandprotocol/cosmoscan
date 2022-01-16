type block_t = {timestamp: MomentRe.Moment.t};

type t = {
  id: int,
  txHash: Hash.t,
  blockHeight: ID.Block.t,
  success: bool,
  gasFee: list(Coin.t),
  gasLimit: int,
  gasUsed: int,
  sender: Address.t,
  timestamp: MomentRe.Moment.t,
  messages: list(MsgDecoder.t),
  memo: string,
  errMsg: string,
};

type internal_t = {
  id: int,
  txHash: Hash.t,
  blockHeight: ID.Block.t,
  success: bool,
  gasFee: list(Coin.t),
  gasLimit: int,
  gasUsed: int,
  sender: Address.t,
  block: block_t,
  messages: Js.Json.t,
  memo: string,
  errMsg: option(string),
};

type account_transaction_t = {transaction: internal_t};

module TxQueryByBlockTimestampConfig = [%graphql
  {|
  query TxQueryBlockTimestamp($address: String!, $greater:timestamp, $less: timestamp) {
    accounts_by_pk(address: $address) {
      account_transactions(order_by: {transaction_id: desc}, limit: 300, where: {transaction: {block: {timestamp: {_lte: $less, _gte: $greater}}}})  @bsRecord {
        transaction @bsRecord {
          id
          txHash: hash @bsDecoder(fn: "GraphQLParser.hash")
          blockHeight: block_height @bsDecoder(fn: "ID.Block.fromInt")
          success
          memo
          gasFee: gas_fee @bsDecoder(fn: "GraphQLParser.coins")
          gasLimit: gas_limit
          gasUsed: gas_used
          sender  @bsDecoder(fn: "Address.fromBech32")
          messages
          errMsg: err_msg
          block @bsRecord {
            timestamp  @bsDecoder(fn: "GraphQLParser.timestamp")
          }
        }
      }
    }
  }
|}
];

let toExternal =
    (
      {
        id,
        txHash,
        blockHeight,
        success,
        gasFee,
        gasLimit,
        gasUsed,
        sender,
        memo,
        block,
        messages,
        errMsg,
      },
    ) => {
  id,
  txHash,
  blockHeight,
  success,
  gasFee,
  gasLimit,
  gasUsed,
  sender,
  memo,
  timestamp: block.timestamp,
  messages: {
    let msg = messages |> Js.Json.decodeArray |> Belt.Option.getExn |> Belt.List.fromArray;
    msg->Belt.List.map(success ? MsgDecoder.decodeAction : MsgDecoder.decodeFailAction);
  },
  errMsg: errMsg->Belt.Option.getWithDefault(""),
};

let get = (address, dateStart, dateEnd, ()) => {
  let (resultQuery, _) =
    ApolloHooks.useQuery(
      TxQueryByBlockTimestampConfig.definition,
      ~variables=
        TxQueryByBlockTimestampConfig.makeVariables(
          ~address=address |> Address.toBech32,
          ~greater=dateStart |> Js.Json.string,
          ~less=dateEnd |> Js.Json.string,
          (),
        ),
    );
  let transactions =
    resultQuery
    |> Query.map(_, a => {
         switch (a##accounts_by_pk) {
         | Some(x') =>
           x'##account_transactions->Belt_Array.map(({transaction}) => transaction->toExternal)
         | None => [||]
         }
       });
  transactions;
};
