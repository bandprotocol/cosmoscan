open TxSub

module TxCountWithOffsetConfig = [%graphql
  {|
  query TransactionsCount($greater: timestamp) {
    transactions_aggregate(where: {block: {timestamp: {_gt: $greater }}}) {
      aggregate {
        count
      }
    }
  }
|}
];

let countOffset = (~timestamp) => {
  let (result, _) = 
    ApolloHooks.useQuery(
      TxCountWithOffsetConfig.definition,
      ~variables=TxCountWithOffsetConfig.makeVariables(~greater=timestamp |> Js.Json.string, ()),
    );
  result
  |> Query.map(_, x => x##transactions_aggregate##aggregate |> Belt_Option.getExn |> (y => y##count));
};
