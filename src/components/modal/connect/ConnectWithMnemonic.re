module Styles = {
  open Css;

  let container =
    style([
      display(`flex),
      flexDirection(`column),
      width(`percent(100.)),
      padding2(~v=`zero, ~h=`px(18)),
    ]);

  let inputBar = (theme: Theme.t, isDarkMode) =>
    style([
      width(`percent(100.)),
      height(`px(37)),
      paddingLeft(`px(9)),
      borderRadius(`px(6)),
      border(`px(1), `solid, isDarkMode ? theme.neutral_400 : theme.neutral_200),
      backgroundColor(isDarkMode ? theme.neutral_300 : theme.neutral_000),
      outlineStyle(`none),
      color(theme.neutral_900),
    ]);

  let connectBtn = style([width(`percent(100.)), height(`px(37))]);
};

[@react.component]
let make = (~chainID) => {
  let (_, dispatchAccount) = React.useContext(AccountContext.context);
  let (_, dispatchModal) = React.useContext(ModalContext.context);
  let (mnemonic, setMnemonic) = React.useState(_ => "");
  let (errMsg, setErrMsg) = React.useState(_ => "");

  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  let createMnemonic = () =>
    if (mnemonic->Js.String.trim == "") {
      setErrMsg(_ => "Invalid mnemonic");
    } else {
      let wallet = Wallet.createFromMnemonic(mnemonic);
      let _ =
        wallet->Wallet.getAddressAndPubKey
        |> Js.Promise.then_(((address, pubKey)) => {
             dispatchAccount(Connect(wallet, address, pubKey, chainID));
             dispatchModal(CloseModal);
             Promise.ret();
           })
        |> Js.Promise.catch(err => {
             Js.Console.log(err);
             setErrMsg(_ => "An error occurred.");
             Promise.ret();
           });
      ();
    };

  <div className=Styles.container>
    <VSpacing size=Spacing.xl />
    <Heading value="Enter Your Mnemonic" size=Heading.H5 />
    <VSpacing size=Spacing.md />
    <input
      id="mnemonicInput"
      autoFocus=true
      value=mnemonic
      className={Styles.inputBar(theme, isDarkMode)}
      onChange={event => setMnemonic(ReactEvent.Form.target(event)##value)}
      onKeyDown={event =>
        switch (ReactEvent.Keyboard.key(event)) {
        | "Enter" =>
          createMnemonic();
          ReactEvent.Keyboard.preventDefault(event);
        | _ => ()
        }
      }
    />
    <VSpacing size=Spacing.xl />
    <div id="mnemonicConnectButton" className={CssHelper.flexBox(~justify=`flexEnd, ())}>
      <Button px=20 py=8 onClick={_ => createMnemonic()} style=Styles.connectBtn>
        {"Connect" |> React.string}
      </Button>
    </div>
    <VSpacing size=Spacing.lg />
    <Text value=errMsg color={theme.failColor} />
  </div>;
};
