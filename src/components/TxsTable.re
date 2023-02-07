module Styles = {
  open Css;

  let statusImg = style([width(`px(20)), marginTop(`px(-3))]);
  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
};

module RenderBody = {
  [@react.component]
  let make =
      (
        ~txSub: ApolloHooks.Subscription.variant(TxSub.t),
        ~msgTransform: MsgDecoder.t => MsgDecoder.t,
      ) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <TBody>
      <Row alignItems=Row.Start>
        <Col col=Col.Three>
          {switch (txSub) {
           | Data({txHash}) => <TxLink txHash width=200 />
           | _ => <LoadingCensorBar width=200 height=15 />
           }}
        </Col>
        <Col col=Col.One>
          {switch (txSub) {
           | Data({blockHeight}) => <TypeID.Block id=blockHeight />
           | _ => <LoadingCensorBar width=65 height=15 />
           }}
        </Col>
        <Col col=Col.One>
          <div className={CssHelper.flexBox(~justify=`center, ())}>
            {switch (txSub) {
             | Data({success}) =>
               <img
                 alt="Status Icon"
                 src={success ? Images.success : Images.fail}
                 className=Styles.statusImg
               />
             | _ => <LoadingCensorBar width=20 height=20 radius=20 />
             }}
          </div>
        </Col>
        <Col col=Col.Two>
          {switch (txSub) {
           | Data({gasFee}) =>
             <Text
               block=true
               value={gasFee->Coin.getBandAmountFromCoins->Format.fPretty}
               weight=Text.Semibold
               color={theme.neutral_900}
             />
           | _ => <LoadingCensorBar width=65 height=15 />
           }}
        </Col>
        <Col col=Col.Five>
          {switch (txSub) {
           | Data({messages, txHash, success, errMsg}) =>
             <div>
               <TxMessages
                 txHash
                 messages={messages->Belt_List.map(msgTransform)}
                 success
                 errMsg
               />
             </div>
           | _ => <> <LoadingCensorBar width=400 height=15 /> </>
           }}
        </Col>
      </Row>
    </TBody>;
  };
};

module RenderBodyMobile = {
  [@react.component]
  let make =
      (
        ~reserveIndex,
        ~txSub: ApolloHooks.Subscription.variant(TxSub.t),
        ~msgTransform: MsgDecoder.t => MsgDecoder.t,
      ) => {
    let isSmallMobile = Media.isSmallMobile();

    switch (txSub) {
    | Data({txHash, blockHeight, gasFee, success, messages, errMsg}) =>
      let msgTransform = messages->Belt_List.map(msgTransform);
      <MobileCard
        values=InfoMobileCard.[
          ("TX Hash", TxHash(txHash, isSmallMobile ? 170 : 200)),
          ("Block", Height(blockHeight)),
          ("Gas Fee\n(BAND)", Coin({value: gasFee, hasDenom: false})),
          ("Actions", Messages(txHash, msgTransform, success, errMsg)),
        ]
        key={txHash |> Hash.toHex}
        idx={txHash |> Hash.toHex}
        status=success
      />;
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("TX Hash", Loading(isSmallMobile ? 170 : 200)),
          ("Block", Loading(70)),
          ("Gas Fee\n(BAND)", Loading(70)),
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
let make =
    (
      ~txsSub: ApolloHooks.Subscription.variant(array(TxSub.t)),
      ~msgTransform: MsgDecoder.t => MsgDecoder.t=x => x,
    ) => {
  let isMobile = Media.isMobile();

  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);
  <>
    {switch (txsSub) {
     | Data(txs) =>
       txs->Belt.Array.size > 0
         ? txs
           ->Belt_Array.mapWithIndex((i, e) =>
               isMobile
                 ? <RenderBodyMobile
                     key={e.txHash |> Hash.toHex}
                     reserveIndex=i
                     txSub={Sub.resolve(e)}
                     msgTransform
                   />
                 : <RenderBody key={e.txHash |> Hash.toHex} txSub={Sub.resolve(e)} msgTransform />
             )
           ->React.array
         : <EmptyContainer>
             <img
               alt="No Transaction"
               src={isDarkMode ? Images.noTxDark : Images.noTxLight}
               className=Styles.noDataImage
             />
             <Heading
               size=Heading.H4
               value="No Transaction"
               align=Heading.Center
               weight=Heading.Regular
               color={theme.neutral_600}
             />
           </EmptyContainer>
     | _ =>
       Belt_Array.make(10, ApolloHooks.Subscription.NoData)
       ->Belt_Array.mapWithIndex((i, noData) =>
           isMobile
             ? <RenderBodyMobile
                 key={string_of_int(i)}
                 reserveIndex=i
                 txSub=noData
                 msgTransform
               />
             : <RenderBody key={string_of_int(i)} txSub=noData msgTransform />
         )
       ->React.array
     }}
  </>;
};
