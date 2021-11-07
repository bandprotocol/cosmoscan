module Styles = {
  open Css;

  let tableWrapper =
    style([
      Media.mobile([padding2(~v=`px(16), ~h=`px(12)), margin2(~v=`zero, ~h=`px(-12))]),
    ]);
  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
};

module RenderBody = {
  [@react.component]
  let make = (~depositSub: ApolloHooks.Subscription.variant(DepositSub.t)) => {
    <TBody>
      <Row alignItems=Row.Center>
        <Col col=Col.Five>
          {switch (depositSub) {
           | Data({depositor}) => <AddressRender address=depositor />
           | _ => <LoadingCensorBar width=300 height=15 />
           }}
        </Col>
        <Col col=Col.Five>
          {switch (depositSub) {
           | Data({txHashOpt}) =>
             switch (txHashOpt) {
             | Some(txHash) => <TxLink txHash width=240 />
             | None => <Text value="Deposited on Wenchang" />
             }
           | _ => <LoadingCensorBar width=100 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
            {switch (depositSub) {
             | Data({amount}) =>
               <Text
                 block=true
                 value={amount |> Coin.getBandAmountFromCoins |> Format.fPretty(~digits=6)}
               />
             | _ => <LoadingCensorBar width=100 height=15 />
             }}
          </div>
        </Col>
      </Row>
    </TBody>;
  };
};

module RenderBodyMobile = {
  [@react.component]
  let make = (~reserveIndex, ~depositSub: ApolloHooks.Subscription.variant(DepositSub.t)) => {
    switch (depositSub) {
    | Data({depositor, txHashOpt, amount}) =>
      <MobileCard
        values=InfoMobileCard.[
          ("Depositor", Address(depositor, 200, `account)),
          (
            "TX Hash",
            switch (txHashOpt) {
            | Some(txHash) => TxHash(txHash, 200)
            | None => Text("Deposited on Wenchang")
            },
          ),
          ("Amount", Coin({value: amount, hasDenom: false})),
        ]
        key={depositor |> Address.toBech32}
        idx={depositor |> Address.toBech32}
      />
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("Depositor", Loading(200)),
          ("TX Hash", Loading(200)),
          ("Amount", Loading(80)),
        ]
        key={reserveIndex |> string_of_int}
        idx={reserveIndex |> string_of_int}
      />
    };
  };
};

[@react.component]
let make = (~proposalID) => {
  let (page, setPage) = React.useState(_ => 1);
  let pageSize = 5;
  let isMobile = Media.isMobile();

  let depositsSub = DepositSub.getList(proposalID, ~pageSize, ~page, ());
  let depositCountSub = DepositSub.count(proposalID);
  let allSub = Sub.all2(depositsSub, depositCountSub);

  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  <div className=Styles.tableWrapper>
    {isMobile
       ? <Row marginBottom=16>
           <Col>
             {switch (allSub) {
              | Data((_, depositCount)) =>
                <div className={CssHelper.flexBox()}>
                  <Text
                    block=true
                    value={depositCount |> string_of_int}
                    weight=Text.Semibold
                    size=Text.Sm
                    transform=Text.Uppercase
                  />
                  <HSpacing size=Spacing.xs />
                  <Text
                    block=true
                    value={depositCount > 1 ? "Depositors" : "Depositor"}
                    weight=Text.Semibold
                    size=Text.Sm
                    transform=Text.Uppercase
                  />
                </div>
              | _ => <LoadingCensorBar width=100 height=15 />
              }}
           </Col>
         </Row>
       : <THead>
           <Row alignItems=Row.Center>
             <Col col=Col.Five>
               {switch (allSub) {
                | Data((_, depositCount)) =>
                  <div className={CssHelper.flexBox()}>
                    <Text
                      block=true
                      value={depositCount |> string_of_int}
                      weight=Text.Semibold
                      size=Text.Sm
                      transform=Text.Uppercase
                    />
                    <HSpacing size=Spacing.xs />
                    <Text
                      block=true
                      value="Depositors"
                      weight=Text.Semibold
                      size=Text.Sm
                      transform=Text.Uppercase
                    />
                  </div>
                | _ => <LoadingCensorBar width=100 height=15 />
                }}
             </Col>
             <Col col=Col.Five>
               <Text
                 block=true
                 value="TX Hash"
                 weight=Text.Semibold
                 size=Text.Sm
                 transform=Text.Uppercase
               />
             </Col>
             <Col col=Col.Two>
               <Text
                 block=true
                 value="Amount"
                 weight=Text.Semibold
                 size=Text.Sm
                 transform=Text.Uppercase
                 align=Text.Right
               />
             </Col>
           </Row>
         </THead>}
    {switch (allSub) {
     | Data((delegators, depositCount)) =>
       let pageCount = Page.getPageCount(depositCount, pageSize);
       <>
         {depositCount > 0
            ? delegators
              ->Belt_Array.mapWithIndex((i, e) =>
                  isMobile
                    ? <RenderBodyMobile
                        reserveIndex=i
                        key={e.depositor |> Address.toBech32}
                        depositSub={Sub.resolve(e)}
                      />
                    : <RenderBody
                        key={e.depositor |> Address.toBech32}
                        depositSub={Sub.resolve(e)}
                      />
                )
              ->React.array
            : <EmptyContainer>
                <img
                  alt="No Depositors"
                  src={isDarkMode ? Images.noDelegatorDark : Images.noDelegatorLight}
                  className=Styles.noDataImage
                />
                <Heading
                  size=Heading.H4
                  value="No Depositors"
                  align=Heading.Center
                  weight=Heading.Regular
                  color={theme.textSecondary}
                />
              </EmptyContainer>}
         {isMobile
            ? React.null
            : <Pagination
                currentPage=page
                pageCount
                onPageChange={newPage => setPage(_ => newPage)}
              />}
       </>;
     | _ =>
       Belt_Array.make(pageSize, ApolloHooks.Subscription.NoData)
       ->Belt_Array.mapWithIndex((i, noData) =>
           isMobile
             ? <RenderBodyMobile reserveIndex=i key={i |> string_of_int} depositSub=noData />
             : <RenderBody key={i |> string_of_int} depositSub=noData />
         )
       ->React.array
     }}
  </div>;
};
