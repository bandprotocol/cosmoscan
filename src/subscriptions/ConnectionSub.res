type state_t =
  | Uninitialized
  | Init
  | TryOpen
  | Open
  | Closed

type channel_t = {
  port: string,
  counterpartyPort: string,
  channelID: string,
  counterpartyChannelID: string,
  state: state_t,
  order: string,
}

type t = {
  clientID: string,
  connectionID: string,
  counterpartyChainID: string,
  counterpartyClientID: string,
  counterpartyConnectionID: string,
  channels: array<channel_t>,
}

exception NotMatch(string)
let parseState = x =>
  switch x {
  | 0 => Uninitialized
  | 1 => Init
  | 2 => TryOpen
  | 3 => Open
  | 4 => Closed
  | x => raise(NotMatch(j`This $x doesn't match any state.`))
  }

let parseOrder = x =>
  switch x {
  | "0" => "None"
  | "1" => "Unordered"
  | "2" => "Ordered"
  | x => raise(NotMatch(j`This $x doesn't match any state.`))
  }

module MultiConfig = %graphql(`
  subscription Connections($limit: Int!, $offset: Int!, $chainID: String!, $connectionID: String!) {
    connections(offset: $offset, limit: $limit, where: {counterparty_chain: {chain_id: {_ilike: $chainID}}, connection_id: {_ilike: $connectionID}}) @bsRecord {
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
`)

module ConnectionCountConfig = %graphql(`
    subscription ConnectionCount($chainID: String!, $connectionID: String!) {
       connections_aggregate(where: {counterparty_chain: {chain_id: {_ilike: $chainID}}, connection_id: {_ilike: $connectionID}}) {
        aggregate {
          count @bsDecoder(fn: "Belt_Option.getExn")
        }
      }
    }
  `)

let getList = (~counterpartyChainID, ~connectionID, ~page, ~pageSize, ()) => {
  let offset = (page - 1) * pageSize
  let (result, _) = ApolloHooks.useSubscription(
    MultiConfig.definition,
    ~variables=MultiConfig.makeVariables(
      ~chainID=counterpartyChainID !== "" ? counterpartyChainID : "%%",
      ~connectionID=j`%$connectionID%`,
      ~limit=pageSize,
      ~offset,
      (),
    ),
  )
  result |> Sub.map(_, internal => internal["connections"])
}

let getCount = (~counterpartyChainID, ~connectionID, ()) => {
  let (result, _) = ApolloHooks.useSubscription(
    ConnectionCountConfig.definition,
    ~variables=ConnectionCountConfig.makeVariables(
      ~chainID=counterpartyChainID !== "" ? counterpartyChainID : "%%",
      ~connectionID=j`%$connectionID%`,
      (),
    ),
  )
  result |> Sub.map(_, x =>
    x["connections_aggregate"]["aggregate"] |> Belt_Option.getExn |> (y => y["count"])
  )
}
