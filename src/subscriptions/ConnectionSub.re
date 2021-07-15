type state_t =
  | Uninitialized
  | Init
  | TryOpen
  | Open
  | Closed;

type channel_t = {
  port: string,
  counterpartyPort: string,
  channelID: string,
  counterpartyChannelID: string,
  state: state_t,
  order: string,
};

type t = {
  clientID: string,
  connectionID: string,
  counterpartyChainID: string,
  counterpartyClientID: string,
  counterpartyConnectionID: string,
  channels: array(channel_t),
};

exception NotMatch(string);
let parseState =
  fun
  | 0 => Uninitialized
  | 1 => Init
  | 2 => TryOpen
  | 3 => Open
  | 4 => Closed
  | x => raise(NotMatch({j|This $x doesn't match any state.|j}));

let parseOrder =
  fun
  | "0" => "None"
  | "1" => "Unordered"
  | "2" => "Ordered"
  | x => raise(NotMatch({j|This $x doesn't match any state.|j}));

module MultiConfig = [%graphql
  {|
  subscription Blocks($chainID: String!) {
    connections(where: {counterparty_chain: {chain_id: {_eq: $chainID}}}) @bsRecord {
      connectionID: connection_id
      clientID: client_id
      counterpartyClientID: counterparty_client_id
      counterpartyChainID: counterparty_chain_id
      counterpartyConnectionID: counterparty_connection_id
      channels @bsRecord {
          channelID: channel
          counterpartyPort: counterparty_port
          counterpartyChannelID: counterparty_channel
          order @bsDecoder(fn: "parseOrder")
          state @bsDecoder(fn: "parseState")
          port
      }
    }
  }
|}
];

let getList = (~chainID, ()) => {
  let (result, _) =
    ApolloHooks.useSubscription(
      MultiConfig.definition,
      ~variables=MultiConfig.makeVariables(~chainID, ()),
    );
  result |> Sub.map(_, internal => internal##connections);
};
