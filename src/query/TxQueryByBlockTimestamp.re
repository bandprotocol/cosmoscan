type block_t = {timestamp: MomentRe.Moment.t};
type t = {
  txHash: string,
  blockHeight: ID.Block.t,
  success: bool,
  gasFee: list(Coin.t),
  gasLimit: int,
  gasUsed: int,
  from: string,
  timestamp: MomentRe.Moment.t,
  messages: list(string),
  memo: string,
};

type internal_t = {
  txHash: string,
  blockHeight: ID.Block.t,
  success: bool,
  gasFee: list(Coin.t),
  gasLimit: int,
  gasUsed: int,
  from: string,
  block: block_t,
  messages: Js.Json.t,
  memo: string,
};

type account_transaction_t = {transaction: internal_t};

module TxQueryByBlockTimestampConfig = [%graphql
  {|
  query TxQueryBlockTimestamp($address: String!, $greater:timestamp, $less: timestamp) {
    accounts_by_pk(address: $address) {
      account_transactions(order_by: {transaction_id: desc}, limit: 350, where: {transaction: {block: {timestamp: {_lte: $less, _gte: $greater}}}})  @bsRecord {
        transaction @bsRecord {
          txHash: hash @bsDecoder(fn: "GraphQLParser.string")
          blockHeight: block_height @bsDecoder(fn: "ID.Block.fromInt")
          success
          memo
          gasFee: gas_fee @bsDecoder(fn: "GraphQLParser.coins")
          gasLimit: gas_limit
          gasUsed: gas_used
          from: sender
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
    ({txHash, blockHeight, success, gasFee, gasLimit, gasUsed, from, memo, block, messages}) => {
  txHash,
  blockHeight,
  success,
  gasFee,
  gasLimit,
  gasUsed,
  from,
  memo,
  timestamp: block.timestamp,
  messages: {
    let msgList = messages |> Js.Json.decodeArray |> Belt.Option.getExn |> Belt.List.fromArray;
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
  (transactions, full.refetch);
};
