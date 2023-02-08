module Styles = {
  open Css;

  let proposerContainer = style([width(`fitContent)]);

  let pageContainer = (theme: Theme.t) =>
    style([
      paddingTop(`px(50)),
      minHeight(`px(450)),
      backgroundColor(theme.neutral_100),
      borderRadius(`px(4)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), rgba(0, 0, 0, `num(0.1)))),
    ]);

  let logo = style([width(`px(180)), marginRight(`px(10))]);
  let linkToHome = style([display(`flex), alignItems(`center), cursor(`pointer)]);
  let rightArrow = style([width(`px(20)), filter([`saturate(50.0), `brightness(70.0)])]);
};

let isIBCTx = (tx: TxSub.t) => {
  tx.messages->Belt.List.reduce(false, (acc, message) => acc || message.isIBC);
};

[@react.component]
let make = (~height) => {
  let isMobile = Media.isMobile();
  let blockSub = BlockSub.get(height);
  let latestBlockSub = BlockSub.getLatest();
  let txsSub = TxSub.getListByBlockHeight(height, ());
  let ibcTxsSub = {
    let%Sub txs = txsSub;
    Sub.resolve(txs->Belt.Array.keepMap(tx => isIBCTx(tx) ? Some(tx) : None));
  };
  let commonTxsSub = {
    let%Sub txs = txsSub;
    Sub.resolve(txs->Belt.Array.keepMap(tx => !isIBCTx(tx) ? Some(tx) : None));
  };

  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  switch (blockSub, latestBlockSub) {
  | (NoData, Data(latestBlock)) =>
    <Section>
      <div className=CssHelper.container>
        <VSpacing size=Spacing.xxl />
        <div
          className={Css.merge([
            Styles.pageContainer(theme),
            CssHelper.flexBox(~direction=`column, ~justify=`center, ()),
          ])}>
          <div className={CssHelper.flexBox()}>
            <img alt="Not Found" src=Images.notFoundBg className=Styles.logo />
          </div>
          <VSpacing size=Spacing.xxl />
          {height > latestBlock.height
             ? <Text
                 value={j|This block(#B$height) hasn't mined yet.|j}
                 size=Text.Lg
                 color={theme.neutral_600}
               />
             : <div className={CssHelper.flexBox(~justify=`center, ())}>
                 <Text
                   value="The database is syncing."
                   size=Text.Lg
                   color={theme.neutral_600}
                   block=true
                 />
                 <VSpacing size=Spacing.md />
                 <Text
                   value="Please waiting for the state up to date."
                   size=Text.Lg
                   color={theme.neutral_600}
                   block=true
                 />
               </div>}
          <VSpacing size=Spacing.lg />
          <Link className=Styles.linkToHome route=Route.HomePage>
            <Text
              value="Back to Homepage"
              weight=Text.Bold
              size=Text.Md
              color={theme.neutral_600}
            />
            <HSpacing size=Spacing.md />
            <img alt="Right Arrow Icon" src=Images.rightArrow className=Styles.rightArrow />
          </Link>
          <VSpacing size=Spacing.xxl />
        </div>
      </div>
    </Section>
  | _ =>
    <Section>
      <div className=CssHelper.container>
        <Row marginBottom=40 marginBottomSm=16>
          <Col>
            <Heading value="Block Details" size=Heading.H2 marginBottom=32 marginBottomSm=8 />
            {switch (blockSub) {
             | Data({height}) => <TypeID.Block id=height position=TypeID.Title />
             | _ => <LoadingCensorBar width=100 height=15 />
             }}
          </Col>
        </Row>
        <Row marginBottom=24>
          <Col>
            <InfoContainer>
              <Heading value="Information" size=Heading.H4 />
              <SeperatedLine mt=32 mb=24 />
              <Row marginBottom=24 alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Block Hash"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.neutral_600}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (blockSub) {
                   | Data({hash}) =>
                     <Text
                       value={hash |> Hash.toHex(~upper=true)}
                       code=true
                       block=true
                       size=Text.Lg
                       breakAll=true
                     />
                   | _ => <LoadingCensorBar width={isMobile ? 200 : 350} height=15 />
                   }}
                </Col>
              </Row>
              <Row marginBottom=24 alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Transaction"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.neutral_600}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (blockSub) {
                   | Data({txn}) => <Text value={txn |> string_of_int} size=Text.Lg />
                   | _ => <LoadingCensorBar width=40 height=15 />
                   }}
                </Col>
              </Row>
              <Row marginBottom=24 alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Timestamp"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.neutral_600}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (blockSub) {
                   | Data({timestamp}) =>
                     <div className={CssHelper.flexBox()}>
                       <Text
                         value={
                           timestamp
                           |> MomentRe.Moment.format(Config.timestampDisplayFormat)
                           |> String.uppercase_ascii
                         }
                         size=Text.Lg
                       />
                       <HSpacing size=Spacing.sm />
                       <TimeAgos
                         time=timestamp
                         prefix="("
                         suffix=")"
                         size=Text.Md
                         weight=Text.Thin
                       />
                     </div>
                   | _ => <LoadingCensorBar width=200 height=15 />
                   }}
                </Col>
              </Row>
              <Row alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Proposer"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.neutral_600}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (blockSub) {
                   | Data({validator: {operatorAddress, moniker, identity}}) =>
                     <div className=Styles.proposerContainer>
                       <ValidatorMonikerLink
                         validatorAddress=operatorAddress
                         moniker
                         identity
                         size=Text.Lg
                         avatarWidth=33
                       />
                     </div>
                   | _ => <LoadingCensorBar width=200 height=15 />
                   }}
                </Col>
              </Row>
            </InfoContainer>
          </Col>
        </Row>
        <ResolvedRequest blockSub />
        {switch (ibcTxsSub) {
         | Data(ibcTxs) when ibcTxs->Belt.Array.length !== 0 =>
           <Row marginBottom=24>
             <Col>
               <Table>
                 <Heading value="IBC Transactions" size=Heading.H4 marginBottom=16 marginTop=32 />
                 <Text value="This section contains only IBC-related transactions." />
                 <SeperatedLine mt=32 mb=0 />
                 <BlockIndexTxsTable txsSub=ibcTxsSub />
               </Table>
             </Col>
           </Row>
         | _ => React.null
         }}
        <Table>
          <Heading value="Transactions" size=Heading.H4 marginBottom=32 marginTop=32 />
          <SeperatedLine mt=32 mb=0 />
          <BlockIndexTxsTable txsSub=commonTxsSub />
        </Table>
      </div>
    </Section>
  };
};
