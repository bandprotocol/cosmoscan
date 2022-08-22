// let wsLink = ApolloLinks.webSocketLink(~uri=Env.graphql, ~reconnect=true, ());
let webSocketLinkT: ReasonApolloTypes.webSocketLinkT = {
  uri: Env.graphql,
  options: {
    reconnect: true,
    connectionParams: None,
  },
};
/* WebSocket client */
let webSocketLink = ApolloLinks.webSocketLink(webSocketLinkT);

let client =
  ReasonApollo.createApolloClient(
    ~link=webSocketLink,
    ~cache=ApolloInMemoryCache.createInMemoryCache(),
    (),
  );

[@react.component]
let make = (~children) => {
  <ApolloHooks.Provider client> children </ApolloHooks.Provider>;
};
