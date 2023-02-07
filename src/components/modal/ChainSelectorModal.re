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
      border(`px(1), `solid, active ? theme.neutral_600 : theme.neutral_100),
      borderRadius(`px(4)),
      cursor(`pointer),
      backgroundColor(theme.neutral_100),
      hover([border(`px(1), `solid, theme.neutral_600)]),
    ]);

  let backButton = (theme: Theme.t) =>
    style([
      border(`zero, `none, theme.neutral_600),
      hover([
        border(`zero, `none, theme.neutral_600),
        backgroundColor(`rgba((0, 0, 0, `num(0.0)))),
        color(theme.neutral_900),
        selector("> i", [color(theme.neutral_900)]),
      ]),
    ]);
};

[@react.component]
let make = (~targetChain) => {
  let (searchTerm, setSearchTerm) = React.useState(_ => "");
  let transferableChainsQuery = IBCConnectionQuery.getList();
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  let (_, dispatchModal) = React.useContext(ModalContext.context);

  let handleClick = chain => {
    ModalContext.CloseModal->dispatchModal;
    SubmitMsg.Send(None, chain)->SubmitTx->OpenModal->dispatchModal;
  };

  let backModal = () => {
    ModalContext.CloseModal->dispatchModal;
    SubmitMsg.Send(None, targetChain)->SubmitTx->OpenModal->dispatchModal;
  };

  React.useEffect1(
    () => {
      dispatchModal(DisableExit);

      Some(() => dispatchModal(EnableExit));
    },
    [|targetChain|],
  );

  <div className=Styles.container>
    <Button
      variant=Button.Outline
      px=0
      py=0
      onClick={_ => backModal()}
      style={Styles.backButton(theme)}>
      <Icon name="far fa-angle-left" color={theme.neutral_900} size=18 mr=5 />
      {"Back" |> React.string}
    </Button>
    <Heading value="Select Chain" size=Heading.H4 marginBottom=24 marginTop=24 />
    <SearchInput
      placeholder="Search Chain"
      onChange=setSearchTerm
      maxWidth=512
      inputStyle=SearchInput.Default
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
                  className={Styles.button(targetChain === IBCConnectionQuery.BAND, theme)}
                  onClick={_ => handleClick(IBCConnectionQuery.BAND)}>
                  <TargetChainInfo targetChain=IBCConnectionQuery.BAND />
                </div>
              : React.null}
           {transferableChains
            ->Belt.Array.keep(transferableChain => {
                switch (transferableChain) {
                | IBCConnectionQuery.IBC({name, chainID})
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
                  | IBCConnectionQuery.IBC({channel}) => channel
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
