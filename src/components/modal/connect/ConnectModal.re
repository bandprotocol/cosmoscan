module Styles = {
  open Css;

  let container =
    style([
      display(`flex),
      justifyContent(`center),
      position(`relative),
      width(`px(800)),
      height(`px(520)),
    ]);

  let innerContainer = style([display(`flex), flexDirection(`column), width(`percent(100.))]);

  let loginSelectionContainer =
    style([padding2(~v=`zero, ~h=`px(24)), height(`percent(100.))]);

  let modalTitle = (theme: Theme.t) =>
    style([
      display(`flex),
      justifyContent(`center),
      flexDirection(`column),
      alignItems(`center),
      paddingTop(`px(30)),
      borderBottom(`px(1), `solid, theme.tableRowBorderColor),
    ]);

  let row = style([height(`percent(100.))]);
  let rowContainer = style([margin2(~v=`zero, ~h=`px(12)), height(`percent(100.))]);

  let header = (active, theme: Theme.t) =>
    style([
      display(`flex),
      flexDirection(`row),
      alignSelf(`center),
      alignItems(`center),
      padding2(~v=`zero, ~h=`px(20)),
      fontSize(`px(14)),
      fontWeight(active ? `bold : `normal),
      color(active ? theme.textPrimary : theme.textSecondary),
    ]);

  let loginList = active =>
    style([
      display(`flex),
      width(`percent(100.)),
      height(`px(50)),
      borderRadius(`px(8)),
      border(`px(2), `solid, active ? Theme.baseBlue : `transparent),
      cursor(`pointer),
      overflow(`hidden),
    ]);

  let loginSelectionBackground = (theme: Theme.t) => style([background(theme.contrastBg)]);

  let ledgerIcon = style([height(`px(28)), width(`px(28)), transform(translateY(`px(3)))]);
  let ledgerImageContainer = active =>
    style([opacity(active ? 1.0 : 0.5), marginRight(`px(15))]);
};

type login_method_t =
  | Mnemonic
  | LedgerWithCosmos
  | LedgerWithBandChain;

let toLoginMethodString = method => {
  switch (method) {
  | Mnemonic => "Mnemonic Phrase"
  | LedgerWithCosmos => "Ledger - Cosmos"
  | LedgerWithBandChain => "Ledger - Band (beta)"
  };
};

module LoginMethod = {
  [@react.component]
  let make = (~name, ~active, ~onClick) => {
    let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

    <div className={Styles.loginList(active)} onClick>
      <div className={Styles.header(active, theme)}>
        {switch (name) {
         | LedgerWithCosmos =>
           <div className={Styles.ledgerImageContainer(active)}>
             <img
               src={isDarkMode ? Images.ledgerCosmosDarkIcon : Images.ledgerCosmosLightIcon}
               className=Styles.ledgerIcon
             />
           </div>
         | LedgerWithBandChain =>
           <div className={Styles.ledgerImageContainer(active)}>
             <img
               src={isDarkMode ? Images.ledgerBandChainDarkIcon : Images.ledgerBandChainLightIcon}
               className=Styles.ledgerIcon
             />
           </div>
         | _ => <div />
         }}
        {name |> toLoginMethodString |> React.string}
      </div>
    </div>;
  };
};

[@react.component]
let make = (~chainID) => {
  let (loginMethod, setLoginMethod) = React.useState(_ => LedgerWithCosmos);
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <div className=Styles.container>
    <div className=Styles.innerContainer>
      <div className={Styles.modalTitle(theme)}>
        <Heading value="Connect with your wallet" size=Heading.H4 />
        {chainID == "band-guanyu-mainnet"
           ? <>
               <VSpacing size=Spacing.md />
               <div className={CssHelper.flexBox()}>
                 <Text value="Please check that you are visiting" size=Text.Lg />
                 <HSpacing size=Spacing.sm />
                 <Text
                   value="https://www.cosmoscan.io"
                   size=Text.Lg
                   weight=Text.Medium
                   color={theme.textPrimary}
                 />
               </div>
             </>
           : <VSpacing size=Spacing.sm />}
        <VSpacing size=Spacing.xl />
      </div>
      <div className=Styles.rowContainer>
        <Row style=Styles.row>
          <Col col=Col.Five style={Styles.loginSelectionBackground(theme)}>
            <div className=Styles.loginSelectionContainer>
              <VSpacing size=Spacing.xl />
              <Heading size=Heading.H5 value="Select your connection method" />
              <VSpacing size=Spacing.md />
              {[|LedgerWithCosmos, LedgerWithBandChain|]
               ->Belt_Array.map(method =>
                   <React.Fragment key={method |> toLoginMethodString}>
                     <VSpacing size=Spacing.lg />
                     <LoginMethod
                       name=method
                       active={loginMethod == method}
                       onClick={_ => setLoginMethod(_ => method)}
                     />
                   </React.Fragment>
                 )
               ->React.array}
            </div>
          </Col>
          <Col col=Col.Seven>
            {switch (loginMethod) {
             | Mnemonic => <ConnectWithMnemonic chainID />
             | LedgerWithCosmos => <ConnectWithLedger chainID ledgerApp=Ledger.Cosmos />
             | LedgerWithBandChain => <ConnectWithLedger chainID ledgerApp=Ledger.BandChain />
             }}
          </Col>
        </Row>
      </div>
    </div>
  </div>;
};
