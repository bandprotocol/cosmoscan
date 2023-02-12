open RequestSub

module RequestCountWithOffsetConfig = [%graphql
  {|
  query RequestCount($request_time: Int) {
    requests_aggregate(where: {request_time: {_gte: $request_time}}) {
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
      RequestCountWithOffsetConfig.definition,
      ~variables=RequestCountWithOffsetConfig.makeVariables(~request_time=timestamp, ()),
    );
  result
  |> Query.map(_, x => x##requests_aggregate##aggregate |> Belt_Option.getExn |> (y => y##count));
};
