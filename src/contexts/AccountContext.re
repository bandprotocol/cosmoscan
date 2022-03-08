type t = {
  address: Address.t,
  pubKey: PubKey.t,
  wallet: Wallet.t,
  chainID: string,
};

type send_request_t = {
  oracleScriptID: ID.OracleScript.t,
  calldata: JsBuffer.t,
  callback: Js.Promise.t(TxCreator2.response_t) => unit,
  askCount: string,
  minCount: string,
  clientID: string,
  feeLimit: string,
  prepareGas: string,
  executeGas: string,
};

type a =
  | Connect(Wallet.t, Address.t, PubKey.t, string)
  | Disconnect
  | SendRequest(send_request_t, BandChainJS.Client.t);

let reducer = state =>
  fun
  | Connect(wallet, address, pubKey, chainID) => Some({wallet, pubKey, address, chainID})
  | Disconnect => {
      switch (state) {
      | Some({wallet}) => wallet |> Wallet.disconnect
      | None => ()
      };
      None;
    }
  | SendRequest(
      {
        oracleScriptID,
        calldata,
        callback,
        askCount,
        minCount,
        clientID,
        feeLimit,
        prepareGas,
        executeGas,
      },
      client,
    ) =>
    switch (state) {
    | Some({address, wallet, pubKey, chainID}) =>
      let feeLimitCoin = BandChainJS.Coin.create();
      feeLimitCoin->BandChainJS.Coin.setAmount(feeLimit);
      feeLimitCoin->BandChainJS.Coin.setDenom("uband");

      let pubKeyHex = pubKey->PubKey.toHex;
      let wrappedPubKey = pubKeyHex->BandChainJS.PubKey.fromHex;

      callback(
        {
          let%Promise rawTx =
            TxCreator2.createRawTx(
              ~sender=address,
              ~msgs=[|
                Request({
                  osID: oracleScriptID,
                  calldata,
                  askCount: askCount |> int_of_string,
                  minCount: minCount |> int_of_string,
                  sender: address,
                  clientID,
                  feeLimitList: [|feeLimitCoin|],
                  prepareGas: {
                    switch (prepareGas) {
                    | "" => None
                    | _ => Some(prepareGas |> int_of_string)
                    };
                  },
                  executeGas: {
                    switch (executeGas) {
                    | "" => None
                    | _ => Some(executeGas |> int_of_string)
                    };
                  },
                }),
              |],
              ~chainID,
              ~gas=700000,
              ~feeAmount="0",
              ~memo="send via scan",
              ~client,
              (),
            );
          let jsonTxStr = rawTx->BandChainJS.Transaction.getSignMessage->JsBuffer.toUTF8;
          let%Promise signature = Wallet.sign(jsonTxStr, wallet);
          let signedTx = rawTx->BandChainJS.Transaction.getTxData(signature, wrappedPubKey, 127);
          TxCreator2.broadcast(client, signedTx);
        },
      );

      state;
    | None =>
      callback(Promise.ret(TxCreator2.Unknown));
      state;
    };

let context = React.createContext(ContextHelper.default);

[@react.component]
let make = (~children) => {
  let (state, dispatch) = React.useReducer(reducer, None);

  React.createElement(
    React.Context.provider(context),
    {"value": (state, dispatch), "children": children},
  );
};
