@obj external makeProps: (~src: Js.Json.t, ~theme: string, ~style: 'a, unit) => _ = ""

@module("react-json-view")
external make: React.component<{
  "src": Js.Json.t,
  "theme": string,
  "style": 'a,
}> = "default"
