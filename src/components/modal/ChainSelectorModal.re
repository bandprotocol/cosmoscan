module Styles = {
  open Css;

  let container =
    style([
      flexDirection(`column),
      width(`px(468)),
      minHeight(`px(300)),
      height(`auto),
      padding(`px(32)),
      borderRadius(`px(5)),
      justifyContent(`flexStart),
    ]);

  let selectorPanel =
    style([
      height(`px(320)),
      overflow(`auto),
      marginTop(`px(16)),
      selector("> div + div", [marginTop(`px(16))]),
    ]);

  let button = (active, theme: Theme.t) =>
    style([
      border(`px(1), `solid, active ? theme.textSecondary : theme.loadingBaseColor),
      borderRadius(`px(4)),
      cursor(`pointer),
      backgroundColor(theme.inputContrastColor),
      hover([border(`px(1), `solid, theme.textSecondary)]),
    ]);
};

[@react.component]
let make = (~targetChain) => {
  let (searchTerm, setSearchTerm) = React.useState(_ => "");
  let transferableChainsQuery = IBCQuery.getTransferConnections();
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  let (_, dispatchModal) = React.useContext(ModalContext.context);

  let handleClick = chain => {
    ModalContext.CloseModal->dispatchModal;
    SubmitMsg.Send(None, chain)->SubmitTx->OpenModal->dispatchModal;
  };

  React.useEffect1(
    () => {
      dispatchModal(DisableExit);

      Some(() => dispatchModal(EnableExit));
    },
    [|targetChain|],
  );

  <div className=Styles.container>
    <Heading value="Select Chain" size=Heading.H4 marginBottom=24 />
    <SearchInput
      placeholder="Search Chain"
      onChange=setSearchTerm
      maxWidth=512
      inputStyle="default"
    />
    <div className=Styles.selectorPanel>
      {switch (transferableChainsQuery) {
       | Data(transferableChains) =>
         <>
           {searchTerm
            |> Js.String.length <= 0
            || Js.String.includes(searchTerm |> Js.String.toLocaleLowerCase, "band")
              ? <div
                  key="band"
                  className={Styles.button(targetChain === IBCQuery.BAND, theme)}
                  onClick={_ => handleClick(IBCQuery.BAND)}>
                  <TargetChainInfo targetChain=IBCQuery.BAND />
                </div>
              : React.null}
           {transferableChains
            ->Belt.Array.keep(transferableChain => {
                switch (transferableChain) {
                | IBCQuery.IBC({name, chainID})
                    when
                      Js.String.includes(searchTerm, name |> Js.String.toLowerCase)
                      || Js.String.includes(searchTerm, chainID |> Js.String.toLowerCase)
                      || searchTerm
                      |> Js.String.length <= 0 =>
                  true
                | _ => false
                }
              })
            ->Belt.Array.map(transferableChain => {
                let keyString =
                  switch (transferableChain) {
                  | IBCQuery.IBC({channel}) => channel
                  | BAND => "band"
                  };

                let isEqual = {
                  switch (targetChain, transferableChain) {
                  | (
                      IBC({channel: targetChannel, chainID: targetChainID}),
                      IBC({channel, chainID}),
                    )
                      when targetChannel === channel && targetChainID === chainID =>
                    true
                  | _ => false
                  };
                };

                <div
                  key=keyString
                  className={Styles.button(isEqual, theme)}
                  onClick={_ => handleClick(transferableChain)}>
                  <TargetChainInfo targetChain=transferableChain />
                </div>;
              })
            ->React.array}
         </>
       | _ => <LoadingCensorBar.CircleSpin height=180 />
       }}
    </div>
  </div>;
};
