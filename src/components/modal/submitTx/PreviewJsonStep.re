module Styles = {
  open Css;
  let container =
    style([
      width(`px(468)),
      minHeight(`px(500)),
      padding3(~top=`px(32), ~h=`px(24), ~bottom=`px(24)),
      borderRadius(`px(4)),
    ]);

  let resultContainer = style([minHeight(`px(400)), width(`percent(100.))]);

  let btn = style([width(`percent(100.))]);

  let jsonDisplay = (theme: Theme.t) =>
    style([
      resize(`none),
      fontSize(`px(12)),
      color(theme.textPrimary),
      backgroundColor(theme.contrastBg),
      border(`px(1), `solid, theme.tableRowBorderColor),
      borderRadius(`px(4)),
      width(`percent(100.)),
      height(`px(300)),
      overflowY(`scroll),
      marginBottom(`px(16)),
      fontFamilies([
        `custom("IBM Plex Mono"),
        `custom("cousine"),
        `custom("sfmono-regular"),
        `custom("Consolas"),
        `custom("Menlo"),
        `custom("liberation mono"),
        `custom("ubuntu mono"),
        `custom("Courier"),
        `monospace,
      ]),
    ]);

  let resultIcon = style([width(`px(48)), marginBottom(`px(16))]);

  let txhashContainer = style([cursor(`pointer)]);
};

type state_t =
  | Nothing
  | Signing
  | Broadcasting
  | Success(Hash.t)
  | Error(string);

[@react.component]
let make = (~rawTx, ~onBack, ~account: AccountContext.t) => {
  let (_, dispatchModal) = React.useContext(ModalContext.context);
  let (state, setState) = React.useState(_ => Nothing);
  let jsonTxStr = rawTx->BandChainJS.Transaction.getSignMessage->JsBuffer.toUTF8;

  let client = React.useContext(ClientContext.context);
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <div className=Styles.container>
    <Heading value="Confirm Transactions" size=Heading.H4 marginBottom=24 />
    {switch (state) {
     | Nothing =>
       <div>
         <div className={CssHelper.mb(~size=16, ())}>
           <Text
             value="Please verify the transaction details below before proceeding"
             size=Text.Lg
           />
         </div>
         <textarea
           className={Styles.jsonDisplay(theme)}
           disabled=true
           defaultValue={jsonTxStr |> Js.Json.parseExn |> TxCreator2.stringifyWithSpaces}
         />
         <div id="broadcastButtonContainer">
           <Button
             py=10
             style={Css.merge([Styles.btn, CssHelper.mb(~size=16, ())])}
             onClick={_ => {
               dispatchModal(DisableExit);
               setState(_ => Signing);
               let _ =
                 Wallet.sign(jsonTxStr, account.wallet)
                 |> Js.Promise.then_(signature => {
                      setState(_ => Broadcasting);
                      let pubKeyHex = account.pubKey->PubKey.toHex;
                      let pubKey = pubKeyHex->BandChainJS.PubKey.fromHex;
                      let txRawBytes =
                        rawTx->BandChainJS.Transaction.getTxData(signature, pubKey, 127);

                      ignore(
                        TxCreator2.broadcast(client, txRawBytes)
                        |> Js.Promise.then_(res => {
                             switch (res) {
                             | TxCreator2.Tx(txResponse) =>
                               txResponse.success
                                 ? {
                                   setState(_ => Success(txResponse.txHash));
                                 }
                                 : {
                                   Js.Console.error(txResponse);
                                   setState(_ => Error(txResponse.code |> TxResError.parse));
                                 };
                               dispatchModal(EnableExit);
                               Js.Promise.resolve();
                             | _ =>
                               setState(_ => Error("Fail to broadcast"));
                               dispatchModal(EnableExit);
                               Js.Promise.resolve();
                             }
                           })
                        |> Js.Promise.catch(err => {
                             switch (Js.Json.stringifyAny(err)) {
                             | Some(errorValue) => setState(_ => Error(errorValue))
                             | None => setState(_ => Error("Can not stringify error"))
                             };
                             dispatchModal(EnableExit);
                             Js.Promise.resolve();
                           }),
                      );

                      Promise.ret();
                    })
                 |> Js.Promise.catch(_ => {
                      setState(_ => Error("Failed to sign message"));
                      dispatchModal(EnableExit);
                      Promise.ret();
                    });
               ();
             }}>
             {"Broadcast" |> React.string}
           </Button>
         </div>
         <Button py=10 style=Styles.btn onClick=onBack> {"Back" |> React.string} </Button>
       </div>
     | Success(txHash) =>
       <div
         className={Css.merge([
           CssHelper.flexBox(~direction=`column, ~justify=`center, ()),
           Styles.resultContainer,
         ])}>
         <img alt="Success Icon" src=Images.success className=Styles.resultIcon />
         <div id="successMsgContainer" className={CssHelper.mb(~size=16, ())}>
           <Text
             value="Broadcast transaction success"
             size=Text.Lg
             block=true
             align=Text.Center
             color={theme.textPrimary}
           />
         </div>
         <Link className=Styles.txhashContainer route={Route.TxIndexPage(txHash)}>
           <Button py=8 px=13 variant=Button.Outline onClick={_ => {dispatchModal(CloseModal)}}>
             {"View Details" |> React.string}
           </Button>
         </Link>
       </div>
     | Signing =>
       <div
         className={Css.merge([
           CssHelper.flexBox(~direction=`column, ~justify=`center, ()),
           Styles.resultContainer,
         ])}>
         <div className={CssHelper.mb(~size=16, ())}>
           <Icon name="fad fa-spinner-third fa-spin" size=48 />
         </div>
         <Text
           value="Waiting for signing transaction"
           size=Text.Lg
           block=true
           align=Text.Center
           color={theme.textPrimary}
         />
       </div>
     | Broadcasting =>
       <div
         className={Css.merge([
           CssHelper.flexBox(~direction=`column, ~justify=`center, ()),
           Styles.resultContainer,
         ])}>
         <div className={CssHelper.mb(~size=16, ())}>
           <Icon name="fad fa-spinner-third fa-spin" size=48 />
         </div>
         <Text
           value="Waiting for broadcasting transaction"
           size=Text.Lg
           block=true
           align=Text.Center
           color={theme.textPrimary}
         />
       </div>
     | Error(err) =>
       <div
         className={Css.merge([
           CssHelper.flexBox(~direction=`column, ~justify=`center, ()),
           Styles.resultContainer,
         ])}>
         <img alt="Fail Icon" src=Images.fail className=Styles.resultIcon />
         <div className={CssHelper.mb()}>
           <Text
             value="Broadcast transaction fail"
             size=Text.Lg
             block=true
             align=Text.Center
             color={theme.textPrimary}
           />
         </div>
         <Text value=err color={theme.failColor} align=Text.Center breakAll=true />
       </div>
     }}
  </div>;
};
