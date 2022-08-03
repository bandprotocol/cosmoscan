module OracleRequestAcknowledge = {
  type t = {requestID: option(int)};

  let decode = json => JsonUtils.Decode.{requestID: json |> optional(at(["request_id"], int))};
};

module OracleResponseData = {
  type status_t =
    | Success
    | Fail
    | Unknown;

  let getStatus =
    fun
    | 1 => Success
    | 2 => Fail
    | _ => Unknown;

  type t = {
    requestID: int,
    resolveStatus: status_t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      requestID: json |> at(["request_id"], int),
      resolveStatus: json |> at(["resolve_status"], int) |> getStatus,
    };
};

type packet_type_t =
  | OracleRequest
  | OracleResponse
  | FungibleToken
  | Unknown;

type packet_direction_t =
  | Incoming
  | Outgoing;

type acknowledge_data_t =
  | Request(OracleRequestAcknowledge.t)
  | Empty;

type data_t =
  | Response(OracleResponseData.t)
  | Empty;

type packet_status_t =
  | Pending
  | Success
  | Fail;

type acknowledgement_t = {
  data: acknowledge_data_t,
  reason: option(string),
  status: packet_status_t,
};

let parsePacketTypeOpt =
  fun
  | Some("oracle_request") => OracleRequest
  | Some("oracle response") => OracleResponse
  | Some("fungible_token") => FungibleToken
  | _ => Unknown;

let getPacketTypeText =
  fun
  | OracleRequest => "Oracle Request"
  | OracleResponse => "Oracle Response"
  | FungibleToken => "Fungible Token"
  | Unknown => "Unknown";

let getPacketStatus =
  fun
  | "success" => Success
  | "pending" => Pending
  | "failure" => Fail
  | _ => raise(Not_found);

type tx_t = {hash: Hash.t};

type connection_t = {counterPartyChainID: string};
type channel_t = {connection: connection_t};

type t = {
  srcChannel: string,
  srcPort: string,
  sequence: int,
  dstChannel: string,
  dstPort: string,
  packetType: packet_type_t,
  acknowledgement: option(acknowledgement_t),
  blockHeight: ID.Block.t,
  counterPartyChainID: string,
  txHash: option(Hash.t),
  data: data_t,
};

type internal_t = {
  srcChannel: string,
  srcPort: string,
  dstChannel: string,
  sequence: int,
  dstPort: string,
  packetType: packet_type_t,
  acknowledgement: option(Js.Json.t),
  dataOpt: option(Js.Json.t),
  transaction: option(tx_t),
  blockHeight: ID.Block.t,
  channel: option(channel_t),
};

let toExternal =
    (
      {
        srcChannel,
        srcPort,
        dstChannel,
        sequence,
        dstPort,
        packetType,
        acknowledgement,
        dataOpt,
        transaction,
        blockHeight,
        channel,
      },
    ) => {
  srcChannel,
  srcPort,
  sequence,
  dstChannel,
  dstPort,
  packetType,
  blockHeight,
  counterPartyChainID: {
    (channel |> Belt.Option.getExn).connection.counterPartyChainID;
  },
  acknowledgement: {
    let%Opt ack = acknowledgement;
    Some(
      JsonUtils.Decode.{
        data:
          switch (packetType) {
          | OracleRequest => Request(ack |> OracleRequestAcknowledge.decode)
          | OracleResponse
          | FungibleToken
          | _ => Empty
          },
        reason: ack |> optional(at(["reason"], string)),
        status: ack |> at(["status"], string) |> getPacketStatus,
      },
    );
  },
  txHash: {
    let%Opt tx = transaction;
    Some(tx.hash);
  },
  data: {
    switch (packetType, dataOpt) {
    | (OracleResponse, Some(data)) => Response(data |> OracleResponseData.decode)
    | _ => Empty
    };
  },
};
module IncomingPacketsConfig = [%graphql
  {|
    query IncomingPackets($limit: Int!, $packetType: String!, $packetTypeIsNull: Boolean!, $port: String!, $channel: String!, $sequence: Int_comparison_exp, $chainID: String!) {
    incoming_packets(limit: $limit, order_by: {block_height: desc}, where: {type: {_is_null: $packetTypeIsNull, _ilike: $packetType}, sequence: $sequence, dst_port: {_ilike: $port}, dst_channel: {_ilike: $channel}, channel:{connection: {counterparty_chain: {chain_id: {_ilike: $chainID}}}}}) @bsRecord {
        packetType: type @bsDecoder(fn: "parsePacketTypeOpt")
        srcPort: src_port
        srcChannel: src_channel
        sequence
        dstPort: dst_port
        dstChannel: dst_channel
        dataOpt: data
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
|}
];

module OutgoingPacketsConfig = [%graphql
  {|
    query OutgoingPackets($limit: Int!, $packetType: String!, $packetTypeIsNull: Boolean!, $port: String!, $channel: String!, $sequence: Int_comparison_exp, $chainID: String!) {
    outgoing_packets(limit: $limit, order_by: {block_height: desc}, where: {type: {_is_null: $packetTypeIsNull, _ilike: $packetType}, sequence: $sequence ,src_port: {_ilike: $port}, src_channel: {_ilike: $channel}, channel:{connection: {counterparty_chain: {chain_id: {_ilike: $chainID}}}}}) @bsRecord {
        packetType: type @bsDecoder(fn: "parsePacketTypeOpt")
        srcPort: src_port
        srcChannel: src_channel
        sequence
        dstPort: dst_port
        dstChannel: dst_channel
        dataOpt: data
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
|}
];

let getList =
    (~pageSize, ~direction, ~packetType, ~port, ~channel, ~sequence: option(int), ~chainID, ()) => {
  let packetTypeKeyword = {
    switch (packetType) {
    | "Oracle Request" => Some("oracle_request")
    | "Oracle Response" => Some("oracle response")
    | "Fungible Token" => Some("fungible_token")
    | "Unknown"
    | "" => None
    | _ => raise(Not_found)
    };
  };
  let packetTypeIsNull = {
    switch (packetType) {
    | "Unknown" => Some(true)
    | _ => None
    };
  };

  let sequenceFilter = {
    switch (sequence) {
    | Some(_) => {
        "_eq": sequence,
        "_gt": None,
        "_gte": None,
        "_in": None,
        "_is_null": None,
        "_lt": None,
        "_lte": None,
        "_neq": None,
        "_nin": None,
      }
    | _ => {
        "_eq": None,
        "_gt": None,
        "_gte": None,
        "_in": None,
        "_is_null": None,
        "_lt": None,
        "_lte": None,
        "_neq": None,
        "_nin": None,
      }
    };
  };

  let result =
    switch (direction) {
    | Incoming =>
      let (result, _) =
        ApolloHooks.useQuery(
          IncomingPacketsConfig.definition,
          ~variables=
            IncomingPacketsConfig.makeVariables(
              ~limit=pageSize,
              ~packetType={
                switch (packetTypeKeyword) {
                | Some("oracle_request") => "oracle_request"
                | Some("oracle response") => "oracle response"
                | Some("fungible_token") => "fungible_token"
                | _ => "%%"
                };
              },
              ~packetTypeIsNull={
                switch (packetTypeIsNull) {
                | Some(true) => true
                | _ => false
                };
              },
              ~port={
                port !== "" ? port : "%%";
              },
              ~channel={
                channel !== "" ? channel : "%%";
              },
              ~chainID={
                chainID !== "" ? chainID : "%%";
              },
              ~sequence={
                sequenceFilter;
              },
              (),
            ),
          ~pollInterval=?Some(5000),
        );
      result |> Query.map(_, x => x##incoming_packets->Belt_Array.map(toExternal));
    | Outgoing =>
      let (result, _) =
        ApolloHooks.useQuery(
          OutgoingPacketsConfig.definition,
          ~variables=
            OutgoingPacketsConfig.makeVariables(
              ~limit=pageSize,
              ~packetType={
                switch (packetTypeKeyword) {
                | Some("oracle_request") => "oracle_request"
                | Some("oracle response") => "oracle response"
                | Some("fungible_token") => "fungible_token"
                | _ => "%%"
                };
              },
              ~packetTypeIsNull={
                switch (packetTypeIsNull) {
                | Some(true) => true
                | _ => false
                };
              },
              ~port={
                port !== "" ? port : "%%";
              },
              ~channel={
                channel !== "" ? channel : "%%";
              },
              ~chainID={
                chainID !== "" ? chainID : "%%";
              },
              ~sequence={
                sequenceFilter;
              },
              (),
            ),
          ~pollInterval=?Some(5000),
        );
      result |> Query.map(_, x => x##outgoing_packets->Belt_Array.map(toExternal));
    };
  result;
};
