type client_t

type reference_data_t = {
  pair: string,
  rate: float,
}

@module("@bandprotocol/bandchain.js") @new
external createClient: string => client_t = "Client"
@send
external getReferenceData: (client_t, array<string>) => Js.Promise.t<array<reference_data_t>> =
  "getReferenceData"
