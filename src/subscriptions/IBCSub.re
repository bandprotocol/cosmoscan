module OracleRequestPacket = {
  type t = {
    id: ID.Request.t,
    oracleScriptID: ID.OracleScript.t,
    oracleScriptName: string,
    clientID: string,
    askCount: int,
    minCount: int,
  };
};

module OracleResponsePacket = {
  type status_t =
    | Success
    | Fail;

  type t = {
    requestID: ID.Request.t,
    oracleScriptID: ID.OracleScript.t,
    oracleScriptName: string,
    status: status_t,
  };
};

type packet_type_t =
  | OracleRequest
  | OracleResponse
  | Transfer
  | Others;

type packet_direction_t =
  | Incoming
  | Outgoing;

type packet_t =
  | Unknown
  | OracleRequestPacket(OracleRequestPacket.t)
  | OracleResponsePacket(OracleResponsePacket.t);

type acknowledge_data_t =
  | Request(ID.Request.t)
  | Transfer(string)
  | Empty;

type acknowledgement_t = {
  data: acknowledge_data_t,
  reason: option(string),
  success: bool,
};

type t = {
  srcChannel: string,
  srcPort: string,
  dstChannel: string,
  sequence: int,
  dstPort: string,
  direction: packet_direction_t,
  packetType: packet_type_t,
  blockHeight: ID.Block.t,
  acknowledgement: option(acknowledgement_t),
  packet: packet_t,
};

module Internal = {
  type t = {
    srcChannel: string,
    srcPort: string,
    sequence: int,
    dstChannel: string,
    dstPort: string,
    blockHeight: ID.Block.t,
    packetType: packet_type_t,
    packetDetail: Js.Json.t,
    acknowledgement: option(acknowledgement_t),
    isIncoming: bool,
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
          isIncoming,
          blockHeight,
          packetDetail,
          acknowledgement,
        },
      ) => {
    srcChannel,
    srcPort,
    sequence,
    dstChannel,
    dstPort,
    direction: isIncoming ? Incoming : Outgoing,
    blockHeight,
    acknowledgement,
    packetType,
    packet:
      switch (packetType) {
      | OracleRequest =>
        OracleRequestPacket(
          JsonUtils.Decode.{
            id: ID.Request.ID(packetDetail |> at(["request_id"], string) |> int_of_string),
            oracleScriptID:
              ID.OracleScript.ID(
                packetDetail |> at(["oracle_script_id"], string) |> int_of_string,
              ),
            oracleScriptName: packetDetail |> at(["oracle_script_name"], string),
            clientID: packetDetail |> at(["client_id"], string),
            // calldata: packetDetail |> at(["calldata"], string) |> JsBuffer.fromHex,
            askCount: packetDetail |> at(["ask_count"], string) |> int_of_string,
            minCount: packetDetail |> at(["min_count"], string) |> int_of_string,
          },
        )
      | OracleResponse =>
        let status =
          packetDetail
          |> JsonUtils.Decode.at(["resolve_status"], JsonUtils.Decode.string) == "Success"
            ? OracleResponsePacket.Success : OracleResponsePacket.Fail;
        OracleResponsePacket(
          JsonUtils.Decode.{
            requestID:
              ID.Request.ID(packetDetail |> at(["request_id"], string) |> int_of_string),
            oracleScriptID: ID.OracleScript.ID(packetDetail |> at(["oracle_script_id"], int)),
            oracleScriptName: packetDetail |> at(["oracle_script_name"], string),
            status,
          },
        );
      | _ => Unknown
      },
  };
};

let getType =
  fun
  | "oracle request" => OracleRequest
  | "oracle response" => OracleResponse
  | "transfer" => Transfer
  | _ => Others;

let getList = (~page=1, ~pageSize=10, ()): ApolloHooks.Subscription.variant(array(t)) => {
  let result = [|
    {
      Internal.srcChannel: "channel-0",
      srcPort: "consuming",
      dstChannel: "channel-0",
      dstPort: "oracle",
      sequence: 2,
      packetType: OracleRequest,
      isIncoming: true,
      blockHeight: ID.Block.ID(214388),
      acknowledgement: Some({data: Request(ID.Request.ID(81801)), success: true, reason: None}),
      packetDetail: {
        let dict = Js.Dict.empty();
        Js.Dict.set(dict, "oracle_script_id", Js.Json.string("32"));
        Js.Dict.set(dict, "oracle_script_name", Js.Json.string("Desmos Themis"));
        Js.Dict.set(dict, "oracle_script_schema", Js.Json.string(""));
        Js.Dict.set(dict, "request_id", Js.Json.string("76189"));
        Js.Dict.set(
          dict,
          "client_id",
          Js.Json.string("desmos13yp2fq3tslq6mmtq4628q38xzj75ethzela9uu"),
        );
        Js.Dict.set(dict, "min_count", Js.Json.string("6"));
        Js.Dict.set(dict, "ask_count", Js.Json.string("10"));

        Js.Json.object_(dict);
      },
    },
    {
      Internal.srcChannel: "channel-0",
      srcPort: "consuming",
      dstChannel: "channel-0",
      dstPort: "oracle",
      sequence: 2,
      packetType: OracleRequest,
      isIncoming: true,
      blockHeight: ID.Block.ID(214388),
      acknowledgement:
        Some({
          data: Request(ID.Request.ID(81801)),
          success: false,
          reason: Some("require: 1uband, max: 0uband: not enough fee"),
        }),
      packetDetail: {
        let dict = Js.Dict.empty();
        Js.Dict.set(dict, "oracle_script_id", Js.Json.string("32"));
        Js.Dict.set(dict, "oracle_script_name", Js.Json.string("Desmos Themis"));
        Js.Dict.set(dict, "oracle_script_schema", Js.Json.string(""));
        Js.Dict.set(dict, "request_id", Js.Json.string("76189"));
        Js.Dict.set(
          dict,
          "client_id",
          Js.Json.string("desmos13yp2fq3tslq6mmtq4628q38xzj75ethzela9uu"),
        );
        Js.Dict.set(dict, "min_count", Js.Json.string("6"));
        Js.Dict.set(dict, "ask_count", Js.Json.string("10"));

        Js.Json.object_(dict);
      },
    },
  |];

  result |> Belt_Array.map(_, packet => Internal.toExternal(packet)) |> Sub.resolve;
};
