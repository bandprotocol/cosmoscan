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

module Mini = {
  type block_t = {timestamp: MomentRe.Moment.t};
  type t = {
    hash: Hash.t,
    blockHeight: ID.Block.t,
    block: block_t,
    gasFee: list(Coin.t),
  };
};

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

module SingleConfig = [%graphql
  {|
  subscription Transaction($tx_hash: bytea!) {
    transactions_by_pk(hash: $tx_hash) @bsRecord {
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
        timestamp @bsDecoder(fn: "GraphQLParser.timestamp")
      }
    }
  },
|}
];

module MultiConfig = [%graphql
  {|
  subscription Transactions($limit: Int!, $offset: Int!) {
    transactions(offset: $offset, limit: $limit, order_by: {id: desc}) @bsRecord {
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
|}
];

module MultiByHeightConfig = [%graphql
  {|
  subscription TransactionsByHeight($height: Int!, $limit: Int!, $offset: Int!) {
    transactions(where: {block_height: {_eq: $height}}, offset: $offset, limit: $limit, order_by: {id: desc}) @bsRecord {
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
|}
];

module MultiBySenderConfig = [%graphql
  {|
  subscription TransactionsBySender($sender: String!, $limit: Int!, $offset: Int!) {
    accounts_by_pk(address: $sender) {
      account_transactions(offset: $offset, limit: $limit, order_by: {transaction_id: desc}) @bsRecord{
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

module TxCountConfig = [%graphql
  {|
  subscription TransactionsCount {
    transactions_aggregate {
      aggregate {
        count @bsDecoder(fn: "Belt_Option.getExn")
      }
    }
  }
|}
];

module TxCountBySenderConfig = [%graphql
  {|
  subscription TransactionsCountBySender($sender: String!) {
    accounts_by_pk(address: $sender) {
      account_transactions_aggregate {
        aggregate {
          count @bsDecoder(fn: "Belt_Option.getExn")
        }
      }
    }
  }
|}
];

let get = txHash => {
  let (result, _) =
    ApolloHooks.useSubscription(
      SingleConfig.definition,
      ~variables=
        SingleConfig.makeVariables(
          ~tx_hash=txHash |> Hash.toHex |> (x => "\\x" ++ x) |> Js.Json.string,
          (),
        ),
    );
  let%Sub x = result;
  switch (x##transactions_by_pk) {
  | Some(data) => Sub.resolve(data |> toExternal)
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
  result |> Sub.map(_, x => x##transactions->Belt_Array.map(toExternal));
};

let getListBySender = (sender, ~page, ~pageSize, ()) => {
  let offset = (page - 1) * pageSize;
  let (result, _) =
    ApolloHooks.useSubscription(
      MultiBySenderConfig.definition,
      ~variables=
        MultiBySenderConfig.makeVariables(
          ~sender=sender |> Address.toBech32,
          ~limit=pageSize,
          ~offset,
          (),
        ),
    );
  result
  |> Sub.map(_, x => {
       switch (x##accounts_by_pk) {
       | Some(x') =>
         x'##account_transactions->Belt_Array.map(({transaction}) => transaction->toExternal)
       | None => [||]
       }
     });
};

let getListByBlockHeight = (height, ~page, ~pageSize, ()) => {
  let offset = (page - 1) * pageSize;
  let (result, _) =
    ApolloHooks.useSubscription(
      MultiByHeightConfig.definition,
      ~variables=
        MultiByHeightConfig.makeVariables(
          ~height=height |> ID.Block.toInt,
          ~limit=pageSize,
          ~offset,
          (),
        ),
    );
  result |> Sub.map(_, x => x##transactions->Belt_Array.map(toExternal));
};

let count = () => {
  let (result, _) = ApolloHooks.useSubscription(TxCountConfig.definition);
  result
  |> Sub.map(_, x => x##transactions_aggregate##aggregate |> Belt_Option.getExn |> (y => y##count));
};

let countBySender = sender => {
  let (result, _) =
    ApolloHooks.useSubscription(
      TxCountBySenderConfig.definition,
      ~variables=TxCountBySenderConfig.makeVariables(~sender=sender |> Address.toBech32, ()),
    );
  result
  |> Sub.map(_, a => {
       switch (a##accounts_by_pk) {
       | Some(account) =>
         account##account_transactions_aggregate##aggregate
         |> Belt_Option.getExn
         |> (y => y##count)
       | None => 0
       }
     });
};
