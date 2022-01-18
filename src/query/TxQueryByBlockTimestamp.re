type block_t = {timestamp: MomentRe.Moment.t};
type t = {
  txHash: Hash.t,
  blockHeight: ID.Block.t,
  success: bool,
  gasFee: list(Coin.t),
  gasLimit: int,
  gasUsed: int,
  bandAddress: Address.t,
  timestamp: MomentRe.Moment.t,
  messages: list(string),
  memo: string,
};

type internal_t = {
  txHash: Hash.t,
  blockHeight: ID.Block.t,
  success: bool,
  gasFee: list(Coin.t),
  gasLimit: int,
  gasUsed: int,
  bandAddress: Address.t,
  block: block_t,
  messages: Js.Json.t,
  memo: string,
};

type account_transaction_t = {transaction: internal_t};

module TxQueryByBlockTimestampConfig = [%graphql
  {|
  query TxQueryBlockTimestamp($address: String!, $greater:timestamp, $less: timestamp) {
    accounts_by_pk(address: $address) {
      account_transactions(order_by: {transaction_id: desc}, limit: 300, where: {transaction: {block: {timestamp: {_lte: $less, _gte: $greater}}}})  @bsRecord {
        transaction @bsRecord {
          txHash: hash @bsDecoder(fn: "GraphQLParser.hash")
          blockHeight: block_height @bsDecoder(fn: "ID.Block.fromInt")
          success
          memo
          gasFee: gas_fee @bsDecoder(fn: "GraphQLParser.coins")
          gasLimit: gas_limit
          gasUsed: gas_used
          bandAddress: sender  @bsDecoder(fn: "Address.fromBech32")
          messages
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
        txHash,
        blockHeight,
        success,
        gasFee,
        gasLimit,
        gasUsed,
        bandAddress,
        memo,
        block,
        messages,
      },
    ) => {
  txHash,
  blockHeight,
  success,
  gasFee,
  gasLimit,
  gasUsed,
  bandAddress,
  memo,
  timestamp: block.timestamp,
  messages: {
    let msgList = messages |> Js.Json.decodeArray |> Belt.Option.getExn |> Belt.List.fromArray;
    // msgList->Belt.List.map(success ? MsgDecoder.decodeAction : MsgDecoder.decodeFailAction);
    msgList->Belt.List.map(msg => msg |> JsonUtils.Decode.(field("type", string)));
  },
};

let get = (address, dateStart, dateEnd, ()) => {
  let (resultQuery, full) =
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
  (transactions, full.fetchMore);
};
