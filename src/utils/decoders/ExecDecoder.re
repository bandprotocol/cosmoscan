module RawDataReport = {
  type t = {
    externalDataID: int,
    data: JsBuffer.t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      externalDataID: json |> intWithDefault(field("external_id")),
      data: json |> bufferWithDefault(field("data")),
    };
};

module Report = {
  type t = {
    requestID: ID.Request.t,
    rawReports: list(RawDataReport.t),
    validator: Address.t,
  };

  let decode = json =>
    JsonUtils.Decode.{
      requestID:
        json
        |> either(
             at(["msg", "request_id"], ID.Request.fromJson),
             at(["request_id"], ID.Request.fromJson),
           ),
      rawReports:
        json
        |> either(
             at(["msg", "raw_reports"], list(RawDataReport.decode)),
             at(["raw_reports"], list(RawDataReport.decode)),
           ),
      validator:
        json
        |> either(at(["msg", "validator"], string), at(["validator"], string))
        |> Address.fromBech32
    };
};

type t =
  | ReportMsg(Report.t)
  | UnknownMsg;

let decode = json => {
  let msgType = json |> JsonUtils.Decode.(field("type", string));

  switch (msgType) {
  | "/oracle.v1.MsgReportData" => ReportMsg(json |> Report.decode)
  | _ => UnknownMsg
  };
};

let getName =
  fun
  | ReportMsg(_) => "Report"
  | UnknownMsg => "Unknown";
