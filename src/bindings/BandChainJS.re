module Client = {
  type t;
  type reference_data_t = {
    pair: string,
    rate: float,
  };

  [@bs.module "@bandprotocol/bandchain.js"] [@bs.new] external create: string => t = "Client";
  [@bs.send]
  external getReferenceData: (t, array(string)) => Js.Promise.t(array(reference_data_t)) =
    "getReferenceData";
};

module Obi = {
  type t;

  [@bs.module "@bandprotocol/bandchain.js"] [@bs.new] external create: string => t = "Obi";
  [@bs.send] external encodeInput: (t, 'a) => JsBuffer.t = "encodeInput";
  [@bs.send] external encodeOutput: (t, 'a) => JsBuffer.t = "encodeOutput";
  [@bs.send] external decodeInput: (t, JsBuffer.t) => 'a = "decodeInput";
  [@bs.send] external decodeOutput: (t, JsBuffer.t) => 'a = "decodeOutput";
};
