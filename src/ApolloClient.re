/* Create an InMemoryCache */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

/* Create a WS Link */
let webSocketLink =
  ApolloLinks.webSocketLink({
    uri: Env.graphql,
    options: {
      reconnect: true,
      connectionParams: None,
    },
  });

let client = ReasonApollo.createApolloClient(~link=webSocketLink, ~cache=inMemoryCache, ());

[@react.component]
let make = (~children) => {
  <ApolloHooks.Provider client> children </ApolloHooks.Provider>;
};
