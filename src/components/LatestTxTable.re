module Styles = {
  open Css;

  let statusImg = style([width(`px(20)), marginTop(`px(-3))]);
  let allTxLink = (theme: Theme.t) =>
    style([
      backgroundColor(theme.baseBlue),
      borderRadius(`px(8)),
      width(`px(32)),
      height(`px(32)),
      hover([backgroundColor(theme.darkBlue)]),
    ]);
};

module RenderBody = {
  [@react.component]
  let make = (~txSub: ApolloHooks.Subscription.variant(TxSub.t)) => {
    <TBody>
      <Row alignItems=Row.Start>
        <Col col=Col.Two>
          {switch (txSub) {
           | Data({txHash}) => <TxLink txHash width=110 />
           | _ => <LoadingCensorBar width=60 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          {switch (txSub) {
           | Data({blockHeight}) => <TypeID.Block id=blockHeight />
           | _ => <LoadingCensorBar width=50 height=15 />
           }}
        </Col>
        <Col col=Col.One>
          <div className={CssHelper.flexBox(~justify=`center, ~align=`center, ())}>
            {switch (txSub) {
             | Data({success}) =>
               <img src={success ? Images.success : Images.fail} className=Styles.statusImg />
             | _ => <LoadingCensorBar width=20 height=20 radius=20 />
             }}
          </div>
        </Col>
        <Col col=Col.Seven>
          {switch (txSub) {
           | Data({messages, txHash, success, errMsg}) =>
             <TxMessages txHash messages success errMsg width=320 />
           | _ => <LoadingCensorBar width=320 height=15 />
           }}
        </Col>
      </Row>
    </TBody>;
  };
};

module RenderBodyMobile = {
  [@react.component]
  let make = (~reserveIndex, ~txSub: ApolloHooks.Subscription.variant(TxSub.t)) => {
    let isSmallMobile = Media.isSmallMobile();

    switch (txSub) {
    | Data({txHash, blockHeight, success, messages, errMsg}) =>
      <MobileCard
        values=InfoMobileCard.[
          ("Tx Hash", TxHash(txHash, isSmallMobile ? 170 : 200)),
          ("Block", Height(blockHeight)),
          ("Actions", Messages(txHash, messages, success, errMsg)),
        ]
        key={txHash |> Hash.toHex}
        idx={txHash |> Hash.toHex}
        status=success
      />
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("Tx Hash", Loading(isSmallMobile ? 170 : 200)),
          ("Block", Loading(70)),
          (
            "Actions",
            Loading(
              {
                isSmallMobile ? 160 : 230;
              },
            ),
          ),
        ]
        key={reserveIndex |> string_of_int}
        idx={reserveIndex |> string_of_int}
      />
    };
  };
};

[@react.component]
let make = () => {
  let isMobile = Media.isMobile();
  let txCount = 5;
  let txsSub = TxSub.getList(~page=1, ~pageSize=txCount, ());
  let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);

  <Table>
    <Row marginTop=30 marginBottom=25 marginTopSm=24 marginBottomSm=0>
      <Col col=Col.Six colSm=Col.Six>
        <Heading value="Latest Transactions" size=Heading.H4 />
        <VSpacing size={`px(4)} />
        {switch (txsSub) {
         | ApolloHooks.Subscription.Data(txs) =>
           <Text
             value={
               txs
               ->Belt.Array.get(0)
               ->Belt.Option.mapWithDefault(0, ({id}) => id - 2207294)
               ->Format.iPretty
             }
             size=Text.Lg
             weight=Text.Medium
           />
         | _ => <LoadingCensorBar width=90 height=18 />
         }}
      </Col>
      <Col col=Col.Six colSm=Col.Six>
        <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
          {isMobile ? React.null : <Heading value="More Transactions" size=Heading.H4 />}
          <HSpacing size=Spacing.md />
          <Link className={CssHelper.flexBox(~align=`center, ())} route=Route.TxHomePage>
            <div
              className={Css.merge([
                Styles.allTxLink(theme),
                CssHelper.flexBox(~justify=`center, ()),
              ])}>
              <Icon name="far fa-arrow-right" color={theme.white} />
            </div>
          </Link>
        </div>
      </Col>
    </Row>
    {isMobile
       ? React.null
       : <THead height=30>
           <Row alignItems=Row.Center>
             <Col col=Col.Two>
               <div className={CssHelper.flexBox()}>
                 <Text value="Tx Hash" size=Text.Sm transform=Text.Uppercase />
               </div>
             </Col>
             <Col col=Col.Two>
               <div className={CssHelper.flexBox()}>
                 <Text value="Block" size=Text.Sm transform=Text.Uppercase />
               </div>
             </Col>
             <Col col=Col.One>
               <div className={CssHelper.flexBox(~justify=`center, ~align=`center, ())}>
                 <Text value="Status" size=Text.Sm transform=Text.Uppercase />
               </div>
             </Col>
             <Col col=Col.Seven>
               <div className={CssHelper.flexBox()}>
                 <Text value="Actions" size=Text.Sm transform=Text.Uppercase />
               </div>
             </Col>
           </Row>
         </THead>}
    {switch (txsSub) {
     | Data(txs) =>
       txs
       ->Belt_Array.mapWithIndex((i, e) =>
           isMobile
             ? <RenderBodyMobile
                 key={e.txHash |> Hash.toHex}
                 reserveIndex=i
                 txSub={Sub.resolve(e)}
               />
             : <RenderBody key={e.txHash |> Hash.toHex} txSub={Sub.resolve(e)} />
         )
       ->React.array
     | _ =>
       Belt_Array.make(txCount, ApolloHooks.Subscription.NoData)
       ->Belt_Array.mapWithIndex((i, noData) =>
           isMobile
             ? <RenderBodyMobile key={string_of_int(i)} reserveIndex=i txSub=noData />
             : <RenderBody key={string_of_int(i)} txSub=noData />
         )
       ->React.array
     }}
  </Table>;
};
