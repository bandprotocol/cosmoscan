module Styles = {
  open Css;

  let successLogo = style([width(`px(20)), marginRight(`px(10))]);

  let notfoundContainer = (theme: Theme.t) =>
    style([
      width(`percent(100.)),
      minHeight(`px(450)),
      display(`flex),
      flexDirection(`column),
      alignItems(`center),
      paddingLeft(`px(50)),
      paddingRight(`px(50)),
      justifyContent(`center),
      backgroundColor(theme.secondaryBg),
      borderRadius(`px(8)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.2)))),
    ]);
  let notfoundLogo = style([width(`px(180)), marginRight(`px(10))]);
};

module TxNotFound = {
  [@react.component]
  let make = () => {
    let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);
    <Section>
      <div className=CssHelper.container>
        <VSpacing size=Spacing.lg />
        <div className={Styles.notfoundContainer(theme)}>
          <div className={CssHelper.flexBox()}>
            <img
              src={isDarkMode ? Images.noTxDark : Images.noTxLight}
              className=Styles.notfoundLogo
            />
          </div>
          <VSpacing size=Spacing.md />
          <Text
            value="Sorry, we are unable to retrieve information on this transaction hash."
            size=Text.Lg
          />
          <VSpacing size=Spacing.lg />
          <Text value="Note: Transactions usually take 5-10 seconds to appear." size=Text.Lg />
        </div>
      </div>
    </Section>;
  };
};

module TxSyncing = {
  [@react.component]
  let make = (~height, ~link) => {
    let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);
    <Section>
      <div className=CssHelper.container>
        <VSpacing size=Spacing.lg />
        <div className={Styles.notfoundContainer(theme)}>
          <div className={CssHelper.flexBox()}>
            <img
              src={isDarkMode ? Images.noTxDark : Images.noTxLight}
              className=Styles.notfoundLogo
            />
          </div>
          <VSpacing size=Spacing.md />
          <Text
            value={j|This transaction is available on block #B$height but our database is syncing now.|j}
            size=Text.Lg
          />
          <VSpacing size=Spacing.lg />
          <div className={CssHelper.flexBox()}>
            <Text value="You can check the transaction information" size=Text.Lg />
            <HSpacing size=Spacing.xs />
            <AbsoluteLink href=link>
              <Text value="here" size=Text.Lg underline=true />
            </AbsoluteLink>
          </div>
        </div>
      </div>
    </Section>;
  };
};

module TxNotFoundOrSyncing = {
  [@react.component]
  let make = (~txHash) => {
    let txHash' = txHash |> Hash.toHex;
    let path = {j|/txs/$txHash'|j};

    let decodeHeight = json => json |> JsonUtils.Decode.at(["height"], JsonUtils.Decode.string);
    let heightOpt = {
      let resultOpt = AxiosHooks.use(path);
      let%Opt result = resultOpt;

      Opt.ret(decodeHeight(result));
    };
    switch (heightOpt) {
    | Some(height) => <TxSyncing height link={Env.rpc ++ path} />
    | None => <TxNotFound />
    };
  };
};

[@react.component]
let make = (~txHash) => {
  let isMobile = Media.isMobile();
  let txSub = TxSub.get(txHash);

  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  switch (txSub) {
  | Loading
  | Data(_) =>
    <Section>
      <div className=CssHelper.container>
        <Row marginBottom=40 marginBottomSm=16>
          <Col>
            <Heading
              value="Transaction Details"
              size=Heading.H2
              marginBottom=32
              marginBottomSm=8
            />
            <div className={CssHelper.flexBox()}>
              {switch (txSub) {
               | Data(_) =>
                 isMobile
                   ? <Text
                       value={txHash |> Hash.toHex(~upper=true)}
                       size=Text.Lg
                       weight=Text.Bold
                       nowrap=false
                       breakAll=true
                       code=true
                       color={theme.textPrimary}
                     />
                   : <>
                       <Text
                         value={txHash |> Hash.toHex(~upper=true)}
                         size=Text.Xxl
                         nowrap=true
                         code=true
                         color={theme.textPrimary}
                       />
                       <HSpacing size=Spacing.sm />
                       <CopyRender width=15 message={txHash |> Hash.toHex(~upper=true)} />
                     </>
               | _ => <LoadingCensorBar width=270 height=15 />
               }}
            </div>
            <div className={Css.merge([CssHelper.flexBox(), CssHelper.mt(~size=16, ())])}>
              {switch (txSub) {
               | Data({success}) =>
                 <>
                   <img
                     src={success ? Images.success : Images.fail}
                     className=Styles.successLogo
                   />
                   <Text
                     value={success ? "Success" : "Failed"}
                     nowrap=true
                     size=Text.Sm
                     transform=Text.Uppercase
                     block=true
                     color={theme.textPrimary}
                   />
                 </>
               | _ =>
                 <>
                   <LoadingCensorBar width=20 height=20 radius=20 />
                   <HSpacing size=Spacing.sm />
                   <LoadingCensorBar width=60 height=15 />
                 </>
               }}
            </div>
          </Col>
        </Row>
        {switch (txSub) {
         | Data({success, errMsg}) when !success =>
           <Row> <Col> <TxError.Full msg=errMsg /> </Col> </Row>
         | _ => React.null
         }}
        <Row marginBottom=24>
          <Col>
            <InfoContainer>
              <Heading value="Information" size=Heading.H4 />
              <SeperatedLine mt=32 mb=24 />
              <Row marginBottom=24 alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Block"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (txSub) {
                   | Data({blockHeight}) =>
                     <TypeID.Block id=blockHeight position=TypeID.Subtitle />
                   | _ => <LoadingCensorBar width=75 height=15 />
                   }}
                </Col>
              </Row>
              <Row marginBottom=24 alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Sender"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (txSub) {
                   | Data({sender}) =>
                     <AddressRender address=sender position=AddressRender.Subtitle />
                   | _ => <LoadingCensorBar width=280 height=15 />
                   }}
                </Col>
              </Row>
              <Row marginBottom=24 alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Timestamp"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (txSub) {
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
                       <TimeAgos time=timestamp prefix="(" suffix=")" />
                     </div>
                   | _ => <LoadingCensorBar width=280 height=15 />
                   }}
                </Col>
              </Row>
              <Row alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Memo"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (txSub) {
                   | Data({memo}) =>
                     <Text value=memo weight=Text.Regular size=Text.Lg block=true />
                   | _ => <LoadingCensorBar width=280 height=15 />
                   }}
                </Col>
              </Row>
              <SeperatedLine mt=32 mb=24 />
              <Row marginBottom=24 alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Gas Used"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (txSub) {
                   | Data({gasUsed}) => <Text value={gasUsed |> Format.iPretty} size=Text.Lg />
                   | _ => <LoadingCensorBar width=75 height=15 />
                   }}
                </Col>
              </Row>
              <Row marginBottom=24 alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Gas Limit"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (txSub) {
                   | Data({gasLimit}) => <Text value={gasLimit |> Format.iPretty} size=Text.Lg />
                   | _ => <LoadingCensorBar width=75 height=15 />
                   }}
                </Col>
              </Row>
              <Row marginBottom=24 alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Gas Price (UBAND)"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (txSub) {
                   | Data({gasFee, gasLimit}) =>
                     <Text
                       value={
                         (gasFee |> Coin.getBandAmountFromCoins)
                         /. (gasLimit |> float_of_int)
                         *. 1e6
                         |> Format.fPretty
                       }
                       size=Text.Lg
                     />
                   | _ => <LoadingCensorBar width=75 height=15 />
                   }}
                </Col>
              </Row>
              <Row alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Fee (BAND)"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (txSub) {
                   | Data({gasFee}) =>
                     <Text
                       value={gasFee |> Coin.getBandAmountFromCoins |> Format.fPretty}
                       size=Text.Lg
                     />
                   | _ => <LoadingCensorBar width=75 height=15 />
                   }}
                </Col>
              </Row>
            </InfoContainer>
          </Col>
        </Row>
        <Row marginBottom=40 marginTop=40>
          <Col>
            {switch (txSub) {
             | Data({messages}) =>
               let msgCount = messages |> Belt.List.length;
               <div className={CssHelper.flexBox()}>
                 <Text value={msgCount |> string_of_int} size=Text.Xxl />
                 <HSpacing size=Spacing.md />
                 <Text value={msgCount > 1 ? "Messages" : "Message"} size=Text.Xxl />
               </div>;
             | _ => <LoadingCensorBar width=100 height=20 />
             }}
          </Col>
        </Row>
        {switch (txSub) {
         | Data({messages}) => <TxIndexPageTable messages />
         | _ => <TxIndexPageTable.Loading />
         }}
      </div>
    </Section>
  | _ => <TxNotFoundOrSyncing txHash />
  };
};
