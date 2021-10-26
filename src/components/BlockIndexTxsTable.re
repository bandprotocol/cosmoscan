module Styles = {
  open Css;

  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
};

module RenderBody = {
  [@react.component]
  let make = (~txSub: ApolloHooks.Subscription.variant(TxSub.t)) => {
    <TBody>
      <Row>
        <Col col=Col.Two>
          {switch (txSub) {
           | Data({txHash}) => <TxLink txHash width=140 />
           | _ => <LoadingCensorBar width=170 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          {switch (txSub) {
           | Data({gasFee}) =>
             <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
               <Text
                 block=true
                 code=true
                 spacing={Text.Em(0.02)}
                 value={gasFee->Coin.getBandAmountFromCoins->Format.fPretty}
                 weight=Text.Medium
               />
             </div>
           | _ => <LoadingCensorBar width=30 height=15 isRight=true />
           }}
        </Col>
        <Col col=Col.Eight>
          {switch (txSub) {
           | Data({messages, txHash, success, errMsg}) =>
             <TxMessages txHash messages success errMsg />
           | _ => <LoadingCensorBar width=530 height=15 />
           }}
        </Col>
      </Row>
    </TBody>;
  };
};

module RenderBodyMobile = {
  [@react.component]
  let make = (~reserveIndex, ~txSub: ApolloHooks.Subscription.variant(TxSub.t)) => {
    switch (txSub) {
    | Data({txHash, gasFee, success, messages, errMsg}) =>
      <MobileCard
        values=InfoMobileCard.[
          ("TX Hash", TxHash(txHash, 200)),
          ("Gas Fee\n(BAND)", Coin({value: gasFee, hasDenom: false})),
          ("Actions", Messages(txHash, messages, success, errMsg)),
        ]
        idx={txHash |> Hash.toHex}
        status=success
      />
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("TX Hash", Loading(200)),
          ("Gas Fee\n(BAND)", Loading(60)),
          ("Actions", Loading(230)),
        ]
        idx={reserveIndex |> string_of_int}
      />
    };
  };
};

[@react.component]
let make = (~txsSub: ApolloHooks.Subscription.variant(array(TxSub.t))) => {
  let isMobile = Media.isMobile();
  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  <>
    {isMobile
       ? React.null
       : <THead>
           <Row alignItems=Row.Center>
             <Col col=Col.Two>
               <Text
                 block=true
                 value="TX Hash"
                 size=Text.Sm
                 weight=Text.Semibold
                 transform=Text.Uppercase
               />
             </Col>
             <Col col=Col.Two>
               <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
                 <Text
                   block=true
                   value="Gas Fee (BAND)"
                   size=Text.Sm
                   weight=Text.Semibold
                   transform=Text.Uppercase
                 />
               </div>
             </Col>
             <Col col=Col.Eight>
               <Text
                 block=true
                 value="Actions"
                 size=Text.Sm
                 weight=Text.Semibold
                 transform=Text.Uppercase
               />
             </Col>
           </Row>
         </THead>}
    {switch (txsSub) {
     | Data(txs) =>
       txs->Belt.Array.size > 0
         ? txs
           ->Belt_Array.mapWithIndex((i, e) =>
               isMobile
                 ? <RenderBodyMobile
                     reserveIndex=i
                     txSub={Sub.resolve(e)}
                     key={e.txHash |> Hash.toHex}
                   />
                 : <RenderBody txSub={Sub.resolve(e)} key={e.txHash |> Hash.toHex} />
             )
           ->React.array
         : <EmptyContainer>
             <img
               src={isDarkMode ? Images.noTxDark : Images.noTxLight}
               className=Styles.noDataImage
             />
             <Heading
               size=Heading.H4
               value="No Transaction"
               align=Heading.Center
               weight=Heading.Regular
               color={theme.textSecondary}
             />
           </EmptyContainer>
     | _ =>
       Belt_Array.make(isMobile ? 1 : 10, ApolloHooks.Subscription.NoData)
       ->Belt_Array.mapWithIndex((i, noData) =>
           isMobile
             ? <RenderBodyMobile reserveIndex=i txSub=noData key={i |> string_of_int} />
             : <RenderBody txSub=noData key={i |> string_of_int} />
         )
       ->React.array
     }}
  </>;
};
