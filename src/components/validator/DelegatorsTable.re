module Styles = {
  open Css;

  let tableWrapper =
    style([Media.mobile([padding2(~v=`px(16), ~h=`zero)])]);
  let noDataImage =
    style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
};

module RenderBody = {
  [@react.component]
  let make =
      (
        ~delegatorSub: ApolloHooks.Subscription.variant(DelegationSub.stake_t),
      ) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
    <TBody>
      <Row alignItems=Row.Center minHeight={`px(30)}>
        <Col col=Col.Six>
          {switch (delegatorSub) {
           | Data({delegatorAddress}) =>
             <AddressRender address=delegatorAddress />
           | _ => <LoadingCensorBar width=300 height=15 />
           }}
        </Col>
        <Col col=Col.Four>
          {switch (delegatorSub) {
           | Data({sharePercentage}) =>
             <Text
               block=true
               value={sharePercentage |> Format.fPretty}
               color={theme.textPrimary}
             />
           | _ => <LoadingCensorBar width=100 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
            {switch (delegatorSub) {
             | Data({amount}) =>
               <Text
                 block=true
                 value={amount |> Coin.getBandAmountFromCoin |> Format.fPretty}
                 color={theme.textPrimary}
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
  let make =
      (
        ~reserveIndex,
        ~delegatorSub:
           ApolloHooks.Subscription.variant(DelegationSub.stake_t),
      ) => {
    switch (delegatorSub) {
    | Data({amount, sharePercentage, delegatorAddress}) =>
      <MobileCard
        values=InfoMobileCard.[
          ("Delegator", Address(delegatorAddress, 149, `account)),
          ("Shares (%)", Float(sharePercentage, Some(4))),
          ("Amount\n(BAND)", Coin({value: [amount], hasDenom: false})),
        ]
        key={delegatorAddress |> Address.toBech32}
        idx={delegatorAddress |> Address.toBech32}
      />
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("Delegator", Loading(150)),
          ("Shares (%)", Loading(60)),
          ("Amount\n(BAND)", Loading(80)),
        ]
        key={reserveIndex |> string_of_int}
        idx={reserveIndex |> string_of_int}
      />
    };
  };
};

[@react.component]
let make = (~address) => {
  let (page, setPage) = React.useState(_ => 1);
  let pageSize = 10;

  let delegatorsSub =
    DelegationSub.getDelegatorsByValidator(address, ~pageSize, ~page, ());
  let delegatorCountSub = DelegationSub.getDelegatorCountByValidator(address);

  let allSub = Sub.all2(delegatorsSub, delegatorCountSub);

  let isMobile = Media.isMobile();
  let (ThemeContext.{theme, isDarkMode}, _) =
    React.useContext(ThemeContext.context);

  <div className=Styles.tableWrapper>
    {isMobile
       ? <Row marginBottom=16>
           <Col>
             {switch (allSub) {
              | Data((_, delegatorCount)) =>
                <div className={CssHelper.flexBox()}>
                  <Text
                    block=true
                    value={delegatorCount |> Format.iPretty}
                    weight=Text.Semibold
                    size=Text.Sm
                  />
                  <HSpacing size=Spacing.xs />
                  <Text
                    block=true
                    value="Delegators"
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
             <Col col=Col.Six>
               {switch (allSub) {
                | Data((_, delegatorCount)) =>
                  <div className={CssHelper.flexBox()}>
                    <Text
                      block=true
                      value={delegatorCount |> Format.iPretty}
                      weight=Text.Semibold
                      transform=Text.Uppercase
                      size=Text.Sm
                    />
                    <HSpacing size=Spacing.xs />
                    <Text
                      block=true
                      value="Delegators"
                      weight=Text.Semibold
                      transform=Text.Uppercase
                      size=Text.Sm
                    />
                  </div>
                | _ => <LoadingCensorBar width=100 height=15 />
                }}
             </Col>
             <Col col=Col.Four>
               <Text
                 block=true
                 value="Share(%)"
                 weight=Text.Semibold
                 transform=Text.Uppercase
                 size=Text.Sm
               />
             </Col>
             <Col col=Col.Two>
               <Text
                 block=true
                 value="Amount"
                 weight=Text.Semibold
                 transform=Text.Uppercase
                 size=Text.Sm
                 align=Text.Right
               />
             </Col>
           </Row>
         </THead>}
    {switch (allSub) {
     | Data((delegators, delegatorCount)) =>
       let pageCount = Page.getPageCount(delegatorCount, pageSize);
       <>
         {delegatorCount > 0
            ? delegators
              ->Belt_Array.mapWithIndex((i, e) =>
                  isMobile
                    ? <RenderBodyMobile
                        key={e.delegatorAddress |> Address.toBech32}
                        reserveIndex=i
                        delegatorSub={Sub.resolve(e)}
                      />
                    : <RenderBody
                        key={e.delegatorAddress |> Address.toBech32}
                        delegatorSub={Sub.resolve(e)}
                      />
                )
              ->React.array
            : <EmptyContainer>
                <img
                  alt="No Delegators"
                  src={isDarkMode ? Images.noDataDark : Images.noDataLight}
                  className=Styles.noDataImage
                />
                <Heading
                  size=Heading.H4
                  value="No Delegators"
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
             ? <RenderBodyMobile
                 key={string_of_int(i)}
                 reserveIndex=i
                 delegatorSub=noData
               />
             : <RenderBody key={string_of_int(i)} delegatorSub=noData />
         )
       ->React.array
     }}
  </div>;
};
