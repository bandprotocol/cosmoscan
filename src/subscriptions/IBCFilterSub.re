type filter_channel_t = {
  port: string,
  channel: string,
};

type filter_connection_t = {channels: array(filter_channel_t)};

type filter_counterparty_t = {
  chainID: string,
  connections: array(filter_connection_t),
};

module CounterpartyConfig = [%graphql
  {|
  subscription Counterparty($chainID: String!) {
    counterparty_chains(where :{chain_id: {_ilike: $chainID}}) @bsRecord{
      chainID: chain_id
      connections @bsRecord{
        channels @bsRecord{
          channel
          port
        }
      }
    }
  }
|}
];

let getChainFilterList = () => {
  let (result, _) =
    ApolloHooks.useSubscription(
      CounterpartyConfig.definition,
      ~variables=CounterpartyConfig.makeVariables(~chainID="%%%%", ()),
    );
  result |> Sub.map(_, internal => internal##counterparty_chains);
};

let getFilterList = (~chainID, ()) => {
  let (result, _) =
    ApolloHooks.useSubscription(
      CounterpartyConfig.definition,
      ~variables=CounterpartyConfig.makeVariables(~chainID="%%" ++ chainID ++ "%%", ()),
    );

  result
  |> Sub.map(
       _,
       internal => {
         let portDict = Js.Dict.empty();
         let chainIDList = internal##counterparty_chains;

         let connectionsList =
           chainIDList->Belt.Array.reduce([||], (acc, {connections}) => {
             acc->Belt.Array.concat(connections)
           });

         let channelsList =
           connectionsList->Belt.Array.reduce([||], (acc, {channels}) => {
             acc->Belt.Array.concat(channels)
           });

         let keys = [|"oracle", "transfer"|];

         keys->Belt_Array.forEach(key => {
           let channelArray: array(string) =
             channelsList->Belt.Array.reduce([||], (acc, {port, channel}) =>
               port === key ? acc->Belt.Array.concat([|channel|]) : acc
             );
           portDict->Js.Dict.set(key, channelArray);
         });

         portDict;
       },
     );
};
