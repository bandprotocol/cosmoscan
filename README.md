# Cosmoscan

## Installation

```
yarn install
```

## Download GraphQL Schema

```sh
<!-- For devnet(main branch) / testnet -->
npx get-graphql-schema https://graphql-lt4.bandchain.org/v1/graphql -j > graphql_schema.json

<!-- For mainnet -->
npx get-graphql-schema https://graphql-lm.bandchain.org/v1/graphql -j > graphql_schema.json
```

## Running App Development

In 2 separate tabs:

```sh
# First tab
yarn bsb -make-world -w -ws # ReasonML compiler

# Second tab
<!-- For devnet(main branch) / testnet -->
RPC_URL=http://rpc.laozi-testnet4.bandchain.org GRAPHQL_URL=wss://graphql-lt4.bandchain.org/v1/graphql LAMBDA_URL=https://asia-southeast1-testnet-instances.cloudfunctions.net/executer-cosmoscan GRPC=https://laozi-testnet4.bandchain.org/grpc-web FAUCET_URL=https://laozi-testnet4.bandchain.org/faucet yarn parcel index.html

<!-- For mainnet -->
RPC_URL=https://rpc.laozi1.bandchain.org GRAPHQL_URL=wss://graphql-lm.bandchain.org/v1/graphql LAMBDA_URL=https://asia-southeast1-testnet-instances.cloudfunctions.net/executer-cosmoscan GRPC=https://laozi1.bandchain.org/grpc-web yarn parcel index.html
```

Serve to [localhost:1234](localhost:1234)

## Build production

```sh
RPC_URL=https://laozi1.bandchain.org/api GRAPHQL_URL=wss://graphql-lm.bandchain.org/v1/graphql LAMBDA_URL=https://asia-southeast1-bandchain-293014.cloudfunctions.net/frontend-laozi-mainnet GRPC=https://laozi1.bandchain.org/grpc-web yarn build
```
