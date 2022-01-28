type block_t = {timestamp: MomentRe.Moment.t};
type t = {
  txHash: Hash.t,
  blockHeight: ID.Block.t,
  success: bool,
  gasFee: list(Coin.t),
  gasLimit: int,
  gasUsed: int,
  from: string,
  timestamp: MomentRe.Moment.t,
  messages: list(MsgDecoder.t),
  memo: string,
};

type internal_t = {
  txHash: Hash.t,
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

type account_transactions_t = {transaction: internal_t};

module TxQueryByBlockTimestampConfig = [%graphql
  {|
  query TxQueryBlockTimestamp($address: String!, $greater:timestamp, $less: timestamp) {
    account_transactions(limit: 350, order_by: {transaction_id: desc}, where: {account: {address: {_eq: $address}}, transaction: {block: {timestamp: {_gte: $greater, _lte: $less}}}}) @bsRecord {
      transaction @bsRecord {
          txHash: hash @bsDecoder(fn: "GraphQLParser.hash")
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
    msgList->Belt.List.map(success ? MsgDecoder.decodeAction : MsgDecoder.decodeFailAction);
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
         a##account_transactions->Belt_Array.map(each => toExternal(each.transaction))
       });
  (transactions, full.refetch);
};
