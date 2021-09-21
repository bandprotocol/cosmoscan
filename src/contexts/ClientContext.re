type t = {client: BandChainJS.Client.t};

let context = React.createContext(ContextHelper.default);

[@react.component]
let make = (~children) => {
  let client = BandChainJS.Client.create(Env.grpc);

  React.createElement(React.Context.provider(context), {"value": client, "children": children});
};
