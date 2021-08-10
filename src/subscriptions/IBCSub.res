module OracleRequestAcknowledge = {
  type t = {requestID: option<int>}

  let decode = json => {
    open JsonUtils.Decode
    {requestID: json |> optional(at(list{"request_id"}, int))}
  }
}

module OracleResponseData = {
  type status_t =
    | Success
    | Fail
    | Unknown

  let getStatus = x =>
    switch x {
    | 1 => Success
    | 2 => Fail
    | _ => Unknown
    }

  type t = {
    requestID: int,
    resolveStatus: status_t,
  }

  let decode = json => {
    open JsonUtils.Decode
    {
      requestID: json |> at(list{"request_id"}, int),
      resolveStatus: json |> at(list{"resolve_status"}, int) |> getStatus,
    }
  }
}

type packet_type_t =
  | OracleRequest
  | OracleResponse
  | FungibleToken
  | Unknown

type packet_direction_t =
  | Incoming
  | Outgoing

type acknowledge_data_t =
  | Request(OracleRequestAcknowledge.t)
  | Empty

type data_t =
  | Response(OracleResponseData.t)
  | Empty

type packet_status_t =
  | Pending
  | Success
  | Fail

type acknowledgement_t = {
  data: acknowledge_data_t,
  reason: option<string>,
  status: packet_status_t,
}

let getPacketType = x =>
  switch x {
  | "oracle_request" => OracleRequest
  | "oracle_response" => OracleResponse
  | "fungible_token" => FungibleToken
  | _ => Unknown
  }

let getPacketTypeText = x =>
  switch x {
  | OracleRequest => "Oracle Request"
  | OracleResponse => "Oracle Response"
  | FungibleToken => "Fungible Token"
  | Unknown => "Unknown"
  }

let fromLabel = x =>
  switch x {
  | "Oracle Request" => "oracle_request"
  | "Oracle Response" => "oracle_response"
  | "Fungible Token" => "fungible_token"
  | _ => ""
  }

let getPacketStatus = x =>
  switch x {
  | "success" => Success
  | "pending" => Pending
  | "failure" => Fail
  | _ => raise(Not_found)
  }

type tx_t = {hash: Hash.t}

type connection_t = {counterPartyChainID: string}
type channel_t = {connection: connection_t}

type t = {
  srcChannel: string,
  srcPort: string,
  dstChannel: string,
  sequence: int,
  dstPort: string,
  packetType: packet_type_t,
  acknowledgement: option<Js.Json.t>,
  data: Js.Json.t,
  transaction: option<tx_t>,
  blockHeight: ID.Block.t,
  channel: option<channel_t>,
}

module Internal = {
  type t = {
    srcChannel: string,
    srcPort: string,
    sequence: int,
    dstChannel: string,
    dstPort: string,
    packetType: packet_type_t,
    acknowledgement: option<acknowledgement_t>,
    blockHeight: ID.Block.t,
    counterPartyChainID: string,
    txHash: option<Hash.t>,
    data: data_t,
  }

  let toExternal = ({
    srcChannel,
    srcPort,
    dstChannel,
    sequence,
    dstPort,
    packetType,
    acknowledgement,
    data,
    transaction,
    blockHeight,
    channel,
  }) => {
    srcChannel: srcChannel,
    srcPort: srcPort,
    sequence: sequence,
    dstChannel: dstChannel,
    dstPort: dstPort,
    packetType: packetType,
    blockHeight: blockHeight,
    counterPartyChainID: (channel |> Belt.Option.getExn).connection.counterPartyChainID,
    acknowledgement: %Opt({
      let ack = acknowledgement
      Some({
        open JsonUtils.Decode
        {
          data: switch packetType {
          | OracleRequest => Request(ack |> OracleRequestAcknowledge.decode)
          | OracleResponse
          | FungibleToken
          | _ =>
            Empty
          },
          reason: ack |> optional(at(list{"reason"}, string)),
          status: ack |> at(list{"status"}, string) |> getPacketStatus,
        }
      })
    }),
    txHash: %Opt({
      let tx = transaction
      Some(tx.hash)
    }),
    data: switch packetType {
    | OracleResponse => Response(data |> OracleResponseData.decode)
    | OracleRequest
    | FungibleToken
    | _ =>
      Empty
    },
  }
}

module IncomingPacketsWithoutSequenceConfig = %graphql(`
  subscription IncomingPackets($limit: Int!  $packetType: String!, $port: String!, $channel: String!, $chainID: String!) {
    incoming_packets(limit: $limit,order_by: {block_height: desc}, where: {type: {_ilike: $packetType}, dst_port: {_ilike: $port}, dst_channel: {_ilike: $channel}, channel:{connection: {counterparty_chain: {chain_id: {_ilike: $chainID}}}}}) @bsRecord{
        packetType: type @bsDecoder(fn: "getPacketType")
        srcPort: src_port
        srcChannel: src_channel
        sequence
        dstPort: dst_port
        dstChannel: dst_channel
        data
        acknowledgement
        transaction @bsRecord{
          hash @bsDecoder(fn: "GraphQLParser.hash")
        }
        blockHeight: block_height @bsDecoder(fn: "ID.Block.fromInt")
        channel @bsRecord{
          connection @bsRecord{
            counterPartyChainID: counterparty_chain_id
          }
        }
      }
    }
`)

module OutgoingPacketsWithoutSequenceConfig = %graphql(`
  subscription OutgoingPackets($limit: Int!  $packetType: String!, $port: String!, $channel: String!, $chainID: String!) {
    outgoing_packets(limit: $limit,order_by: {block_height: desc}, where: {type: {_ilike: $packetType}, src_port: {_ilike: $port}, src_channel: {_ilike: $channel}, channel:{connection: {counterparty_chain: {chain_id: {_ilike: $chainID}}}}}) @bsRecord{
        packetType: type @bsDecoder(fn: "getPacketType")
        srcPort: src_port
        srcChannel: src_channel
        sequence
        dstPort: dst_port
        dstChannel: dst_channel
        data
        acknowledgement
        transaction @bsRecord{
          hash @bsDecoder(fn: "GraphQLParser.hash")
        }
        blockHeight: block_height @bsDecoder(fn: "ID.Block.fromInt")
        channel @bsRecord{
          connection @bsRecord{
            counterPartyChainID: counterparty_chain_id
          }
        }
      }
    }
`)
module IncomingPacketsConfig = %graphql(`
  subscription IncomingPackets($limit: Int!  $packetType: String!, $port: String!, $channel: String!, $sequence: Int!, $chainID: String!) {
    incoming_packets(limit: $limit,order_by: {block_height: desc}, where: {type: {_ilike: $packetType}, sequence: {_eq: $sequence}, dst_port: {_ilike: $port}, dst_channel: {_ilike: $channel}, channel:{connection: {counterparty_chain: {chain_id: {_ilike: $chainID}}}}}) @bsRecord{
        packetType: type @bsDecoder(fn: "getPacketType")
        srcPort: src_port
        srcChannel: src_channel
        sequence
        dstPort: dst_port
        dstChannel: dst_channel
        data
        acknowledgement
        transaction @bsRecord{
          hash @bsDecoder(fn: "GraphQLParser.hash")
        }
        blockHeight: block_height @bsDecoder(fn: "ID.Block.fromInt")
        channel @bsRecord{
          connection @bsRecord{
            counterPartyChainID: counterparty_chain_id
          }
        }
      }
    }
`)

module OutgoingPacketsConfig = %graphql(`
  subscription OutgoingPackets($limit: Int!  $packetType: String!, $port: String!, $channel: String!, $sequence: Int!, $chainID: String!) {
    outgoing_packets(limit: $limit,order_by: {block_height: desc}, where: {type: {_ilike: $packetType}, sequence: {_eq: $sequence} ,src_port: {_ilike: $port}, src_channel: {_ilike: $channel}, channel:{connection: {counterparty_chain: {chain_id: {_ilike: $chainID}}}}}) @bsRecord{
        packetType: type @bsDecoder(fn: "getPacketType")
        srcPort: src_port
        srcChannel: src_channel
        sequence
        dstPort: dst_port
        dstChannel: dst_channel
        data
        acknowledgement
        transaction @bsRecord{
          hash @bsDecoder(fn: "GraphQLParser.hash")
        }
        blockHeight: block_height @bsDecoder(fn: "ID.Block.fromInt")
        channel @bsRecord{
          connection @bsRecord{
            counterPartyChainID: counterparty_chain_id
          }
        }
      }
    }
`)

let getList = (
  ~pageSize,
  ~direction,
  ~packetType,
  ~port,
  ~channel,
  ~sequence: option<int>,
  ~chainID,
  (),
) => {
  let result = switch direction {
  | Incoming =>
    switch sequence {
    | Some(sequence) =>
      let (result, _) = ApolloHooks.useSubscription(
        IncomingPacketsConfig.definition,
        ~variables=IncomingPacketsConfig.makeVariables(
          ~limit=pageSize,
          ~packetType=j`%$packetType%`,
          ~port=port !== "" ? port : "%%",
          ~channel=channel !== "" ? channel : "%%",
          ~chainID=chainID !== "" ? chainID : "%%",
          ~sequence,
          (),
        ),
      )
      result |> Sub.map(_, x => x["incoming_packets"]->Belt_Array.map(Internal.toExternal))
    | None =>
      let (result, _) = ApolloHooks.useSubscription(
        IncomingPacketsWithoutSequenceConfig.definition,
        ~variables=IncomingPacketsWithoutSequenceConfig.makeVariables(
          ~limit=pageSize,
          ~packetType=j`%$packetType%`,
          ~port=port !== "" ? port : "%%",
          ~channel=channel !== "" ? channel : "%%",
          ~chainID=chainID !== "" ? chainID : "%%",
          (),
        ),
      )
      result |> Sub.map(_, x => x["incoming_packets"]->Belt_Array.map(Internal.toExternal))
    }
  | Outgoing =>
    switch sequence {
    | Some(sequence) =>
      let (result, _) = ApolloHooks.useSubscription(
        OutgoingPacketsConfig.definition,
        ~variables=OutgoingPacketsConfig.makeVariables(
          ~limit=pageSize,
          ~packetType=j`%$packetType%`,
          ~port=port !== "" ? port : "%%",
          ~channel=channel !== "" ? channel : "%%",
          ~chainID=chainID !== "" ? chainID : "%%",
          ~sequence,
          (),
        ),
      )
      result |> Sub.map(_, x => x["outgoing_packets"]->Belt_Array.map(Internal.toExternal))
    | None =>
      let (result, _) = ApolloHooks.useSubscription(
        OutgoingPacketsWithoutSequenceConfig.definition,
        ~variables=OutgoingPacketsWithoutSequenceConfig.makeVariables(
          ~limit=pageSize,
          ~packetType=j`%$packetType%`,
          ~port=port !== "" ? port : "%%",
          ~channel=channel !== "" ? channel : "%%",
          ~chainID=chainID !== "" ? chainID : "%%",
          (),
        ),
      )
      result |> Sub.map(_, x => x["outgoing_packets"]->Belt_Array.map(Internal.toExternal))
    }
  }
  result
}
