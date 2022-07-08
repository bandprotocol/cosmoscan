type validator_t = {commission: list(Coin.t)};

type connection_t = {counterparty_chain_id: string};

type interchain_account_t = {
  counterparty_address: Address.t,
  connection: connection_t,
};

type t = {
  balance: list(Coin.t),
  commission: list(Coin.t),
  counterpartyAddress: option(Address.t),
  counterpartyChainID: option(string),
};
type internal_t = {
  balance: list(Coin.t),
  validator: option(validator_t),
  interchainAccountOpt: option(interchain_account_t),
};

let toExternal = ({balance, validator, interchainAccountOpt}) => {
  {
    balance,
    commission:
      switch (validator) {
      | Some(validator') => validator'.commission
      | None => []
      },
    counterpartyAddress:
      interchainAccountOpt->Belt_Option.map(({counterparty_address}) => counterparty_address),
    counterpartyChainID:
      interchainAccountOpt->Belt.Option.map(({connection}) => connection.counterparty_chain_id),
  };
};

module SingleConfig = [%graphql
  {|
  subscription Account($address: String!) {
    accounts_by_pk(address: $address) @bsRecord {
      balance @bsDecoder(fn: "GraphQLParser.coins")
      validator @bsRecord {
        commission: accumulated_commission @bsDecoder(fn: "GraphQLParser.coins")
      }
      interchainAccountOpt: interchain_account @bsRecord {
        counterparty_address @bsDecoder(fn: "Address.fromHex")
        connection @bsRecord {
          counterparty_chain_id
        }
      }
    }
  }
  |}
];

let get = address => {
  let (result, _) =
    ApolloHooks.useSubscription(
      SingleConfig.definition,
      ~variables=SingleConfig.makeVariables(~address=address |> Address.toBech32, ()),
    );
  result
  |> Sub.map(_, x =>
       x##accounts_by_pk
       |> Belt_Option.mapWithDefault(
            _,
            {balance: [], commission: [], counterpartyAddress: None, counterpartyChainID: None},
            toExternal,
          )
     );
};
