module OracleRequestPacket = {
  type t = {
    requestID: ID.Request.t,
    oracleScriptID: ID.OracleScript.t,
    oracleScriptName: string,
    clientID: string,
    calldata: JsBuffer.t,
    askCount: int,
    minCount: int,
    feeLimit: string,
    executeGas: int,
    prepareGas: int,
    schema: string,
    requestKey: string,
    payer: Address.t,
  };
  let decode = json => {
    JsonUtils.Decode.{
      requestID: ID.Request.ID(json |> at(["id"], int)),
      oracleScriptID: ID.OracleScript.ID(json |> at(["decoded_data", "oracle_script_id"], int)),
      oracleScriptName: json |> at(["decoded_data", "oracle_script_name"], string),
      clientID: json |> at(["decoded_data", "client_id"], string),
      calldata: json |> bufferWithDefault(at(["decoded_data", "calldata"])),
      askCount: json |> at(["decoded_data", "ask_count"], int),
      minCount: json |> at(["decoded_data", "min_count"], int),
      feeLimit: json |> at(["decoded_data", "fee_limit"], string),
      executeGas: json |> at(["decoded_data", "execute_gas"], int),
      prepareGas: json |> at(["decoded_data", "prepare_gas"], int),
      schema: json |> at(["decoded_data", "oracle_script_schema"], string),
      requestKey: json |> at(["decoded_data", "request_key"], string),
      payer: json |> at(["decoded_data", "payer"], string) |> Address.fromBech32,
    };
  };
};

module FungibleTokenPacket = {
  type t = {
    amount: int,
    denom: string,
    receiver: string,
    sender: string,
  };

  let decode = json => {
    JsonUtils.Decode.{
      amount: json |> at(["amount"], int),
      denom: json |> at(["denom"], string),
      receiver: json |> at(["receiver"], string),
      sender: json |> at(["sender"], string),
    };
  };
};

type packet_t =
  | OracleRequestPacket(OracleRequestPacket.t)
  | FungibleTokenPacket(FungibleTokenPacket.t)
  | Unknown;

type t = {
  packetDetail: packet_t,
  packetType: string,
};

let getPacketTypeText =
  fun
  | "oracle_request" => "Oracle Request"
  | "oracle_response" => "Oracle Response"
  | "fungible_token" => "Fungible Token"
  | _ => "Unknown";

let decodeAction = json => {
  JsonUtils.Decode.{
    packetDetail: {
      switch (json |> at(["msg", "packet_type"], string)) {
      | "oracle_request" => OracleRequestPacket(json |> at(["msg"], OracleRequestPacket.decode))
      | "fungible_token" =>
        FungibleTokenPacket(json |> at(["msg", "decoded_data"], FungibleTokenPacket.decode))
      | _ => Unknown
      };
    },
    packetType: json |> at(["msg", "packet_type"], string) |> getPacketTypeText,
  };
};
