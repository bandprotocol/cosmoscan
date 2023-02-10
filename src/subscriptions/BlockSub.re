open ValidatorSub.Mini;

type aggregate_t = {count: int};

type transactions_aggregate_t = {aggregate: option(aggregate_t)};
type transaction_t = {id: option(int)};

type resolve_request_t = {
  id: ID.Request.t,
  isIBC: bool,
};

type internal_t = {
  height: ID.Block.t,
  hash: Hash.t,
  inflation: float,
  validator: ValidatorSub.Mini.t,
  timestamp: MomentRe.Moment.t,
  transactions_aggregate: transactions_aggregate_t,
};

type t = {
  height: ID.Block.t,
  hash: Hash.t,
  inflation: float,
  timestamp: MomentRe.Moment.t,
  validator: ValidatorSub.Mini.t,
  txn: int,
};

let toExternal = ({height, hash, inflation, timestamp, validator, transactions_aggregate}) => {
  height,
  hash,
  inflation,
  timestamp,
  validator,
  txn:
    switch (transactions_aggregate.aggregate) {
    | Some(aggregate) => aggregate.count
    | _ => 0
    },
};

type internal_block_t = {
  height: ID.Block.t,
  hash: Hash.t,
  inflation: float,
  validator: ValidatorSub.Mini.t,
  timestamp: MomentRe.Moment.t,
  transactions_aggregate: transactions_aggregate_t,
  requests: array(resolve_request_t),
};

type block_t = {
  height: ID.Block.t,
  hash: Hash.t,
  inflation: float,
  timestamp: MomentRe.Moment.t,
  validator: ValidatorSub.Mini.t,
  txn: int,
  requests: array(resolve_request_t),
};

type block_with_tx_t = {
  transactions: array(transaction_t),
};

let toExternalBlock =
    ({height, hash, inflation, timestamp, validator, transactions_aggregate, requests}) => {
  height,
  hash,
  inflation,
  timestamp,
  validator,
  txn:
    switch (transactions_aggregate.aggregate) {
    | Some(aggregate) => aggregate.count
    | _ => 0
    },
  requests,
};

module MultiConfig = [%graphql
  {|
  subscription Blocks($limit: Int!, $offset: Int!) {
    blocks(limit: $limit, offset: $offset, order_by: {height: desc}) @bsRecord {
      height @bsDecoder(fn: "ID.Block.fromInt")
      hash @bsDecoder(fn: "GraphQLParser.hash")
      inflation @bsDecoder(fn: "GraphQLParser.floatString")
      validator @bsRecord {
        consensusAddress: consensus_address
        operatorAddress: operator_address @bsDecoder(fn: "Address.fromBech32")
        moniker
        identity
      }
      timestamp @bsDecoder(fn: "GraphQLParser.timestamp")
      transactions_aggregate @bsRecord {
        aggregate @bsRecord {
          count
        }
      }
    }
  }
|}
];

module MultiConsensusAddressConfig = [%graphql
  {|
  subscription BlocksByConsensusAddress($limit: Int!, $offset: Int!, $address: String!) {
    blocks(limit: $limit, offset: $offset, order_by: {height: desc}, where: {proposer: {_eq: $address}}) @bsRecord {
      height @bsDecoder(fn: "ID.Block.fromInt")
      hash @bsDecoder(fn: "GraphQLParser.hash")
      inflation @bsDecoder(fn: "GraphQLParser.floatString")
      validator @bsRecord {
        consensusAddress: consensus_address
        operatorAddress: operator_address @bsDecoder(fn: "Address.fromBech32")
        moniker
        identity
      }
      timestamp @bsDecoder(fn: "GraphQLParser.timestamp")
      transactions_aggregate @bsRecord {
        aggregate @bsRecord {
          count
        }
      }
    }
  }
|}
];

module SingleConfig = [%graphql
  {|
  subscription Block($height: Int!) {
    blocks_by_pk(height: $height) @bsRecord {
      height @bsDecoder(fn: "ID.Block.fromInt")
      hash @bsDecoder(fn: "GraphQLParser.hash")
      inflation @bsDecoder(fn: "GraphQLParser.floatString")
      validator @bsRecord {
        consensusAddress: consensus_address
        operatorAddress: operator_address @bsDecoder(fn: "Address.fromBech32")
        moniker
        identity
      }
      timestamp @bsDecoder(fn: "GraphQLParser.timestamp")
      transactions_aggregate @bsRecord {
        aggregate @bsRecord {
          count
        }
      }
      requests(where: {resolve_status: {_neq: "Open"}}) @bsRecord {
        id @bsDecoder(fn: "ID.Request.fromInt")
        isIBC: is_ibc
      }
    }
  },
|}
];

module BlockCountConfig = [%graphql
  {|
  subscription BlocksCount {
    blocks_aggregate{
      aggregate{
        count
      }
    }
  }
|}
];

module PastDayBlockCountConfig = [%graphql
  {|
  subscription AvgDayBlocksCount($greater: timestamp, $less: timestamp) {
    blocks_aggregate(where: {timestamp: {_lte: $less, _gte: $greater}}) {
      aggregate{
        count
      }
    }
  }
|}
];

module BlockCountConsensusAddressConfig = [%graphql
  {|
  subscription BlocksCountByConsensusAddress($address: String!) {
    blocks_aggregate(where: {proposer: {_eq: $address}}) {
      aggregate{
        count
      }
    }
  }
|}
];

module BlockWithTxByTimestamp = [%graphql
  {|
  subscription BlockWithTxByTimestamp($greater: timestamp) {
    blocks(limit: 20, where: {timestamp: {_gte: $greater}}) {
      transactions {
        id
      }
    }
  }
|}
];

// utility func
let getFirstTxOfBlock = (blocks) => {
    Belt.Array.reduce(blocks,[||], (acc, block) => {
      Belt.Array.concat(acc, block##transactions)
    }
  )
}

let getFirstTxOfTheDay = (~timestamp) => {
  let (result, _) =
    ApolloHooks.useSubscription(
      BlockWithTxByTimestamp.definition,
      ~variables=BlockWithTxByTimestamp.makeVariables(~greater=timestamp |> Js.Json.string, ()),
    );
  result |> Sub.map(_, internal => internal##blocks -> getFirstTxOfBlock -> (txs => txs[0]##id));
};

let get = height => {
  let (result, _) =
    ApolloHooks.useSubscription(
      SingleConfig.definition,
      ~variables=SingleConfig.makeVariables(~height=height |> ID.Block.toInt, ()),
    );
  let%Sub x = result;
  switch (x##blocks_by_pk) {
  | Some(data) => Sub.resolve(data |> toExternalBlock)
  | None => NoData
  };
};

let getList = (~page, ~pageSize, ()) => {
  let offset = (page - 1) * pageSize;
  let (result, _) =
    ApolloHooks.useSubscription(
      MultiConfig.definition,
      ~variables=MultiConfig.makeVariables(~limit=pageSize, ~offset, ()),
    );
  result |> Sub.map(_, internal => internal##blocks->Belt_Array.map(toExternal));
};

let getListByConsensusAddress = (~address, ~page, ~pageSize, ()) => {
  let offset = (page - 1) * pageSize;
  let (result, _) =
    ApolloHooks.useSubscription(
      MultiConsensusAddressConfig.definition,
      ~variables=
        MultiConsensusAddressConfig.makeVariables(
          ~address=address |> Address.toHex,
          ~limit=pageSize,
          ~offset,
          (),
        ),
    );
  result |> Sub.map(_, internal => internal##blocks->Belt_Array.map(toExternal));
};

let getLatest = () => {
  let%Sub blocks = getList(~pageSize=1, ~page=1, ());
  switch (blocks->Belt_Array.get(0)) {
  | Some(latestBlock) => latestBlock |> Sub.resolve
  | None => NoData
  };
};

let count = () => {
  let (result, _) = ApolloHooks.useSubscription(BlockCountConfig.definition);
  result
  |> Sub.map(_, x => x##blocks_aggregate##aggregate |> Belt_Option.getExn |> (y => y##count));
};

let getAvgBlockTime = (greater, less) => {
  let (result, _) =
    ApolloHooks.useSubscription(
      PastDayBlockCountConfig.definition,
      ~variables=
        PastDayBlockCountConfig.makeVariables(
          ~greater=greater |> Js.Json.string,
          ~less=less |> Js.Json.string,
          (),
        ),
    );
  let blockCountSub =
    result
    |> Sub.map(_, x => x##blocks_aggregate##aggregate |> Belt_Option.getExn |> (y => y##count));
  let%Sub blockCount = blockCountSub;
  (24 * 60 * 60 |> float_of_int) /. (blockCount |> float_of_int) |> Sub.resolve;
};

let countByConsensusAddress = (~address, ()) => {
  let (result, _) =
    ApolloHooks.useSubscription(
      BlockCountConsensusAddressConfig.definition,
      ~variables=
        BlockCountConsensusAddressConfig.makeVariables(~address=address |> Address.toHex, ()),
    );
  result
  |> Sub.map(_, x => x##blocks_aggregate##aggregate |> Belt_Option.getExn |> (y => y##count));
};
