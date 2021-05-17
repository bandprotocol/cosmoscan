module Styles = {
  open Css;

  let tableWrapper = style([Media.mobile([padding2(~v=`px(16), ~h=`zero)])]);
  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
};

module RenderBody = {
  [@react.component]
  let make = (~reporterSub: ApolloHooks.Subscription.variant(Address.t)) => {
    <TBody>
      <Row alignItems=Row.Center minHeight={`px(30)}>
        <Col>
          {switch (reporterSub) {
           | Data(address) => <AddressRender address />
           | _ => <LoadingCensorBar width=300 height=15 />
           }}
        </Col>
      </Row>
    </TBody>;
  };
};

module RenderBodyMobile = {
  [@react.component]
  let make = (~reserveIndex, ~reporterSub: ApolloHooks.Subscription.variant(Address.t)) => {
    switch (reporterSub) {
    | Data(address) =>
      <MobileCard
        values=InfoMobileCard.[("Reporter", Address(address, 200, `account))]
        key={address |> Address.toBech32}
        idx={address |> Address.toBech32}
      />
    | _ =>
      <MobileCard
        values=InfoMobileCard.[("Reporter", Loading(150))]
        key={reserveIndex |> string_of_int}
        idx={reserveIndex |> string_of_int}
      />
    };
  };
};

[@react.component]
let make = (~address) => {
  let (page, setPage) = React.useState(_ => 1);
  let pageSize = 5;
  let isMobile = Media.isMobile();
  let (ThemeContext.{theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  let reportersSub = ReporterSub.getList(~operatorAddress=address, ~pageSize, ~page, ());
  let reporterCountSub = ReporterSub.count(address);
  let allSub = Sub.all2(reportersSub, reporterCountSub);

  <div className=Styles.tableWrapper>
    {isMobile
       ? <Row marginBottom=16>
           <Col>
             {switch (allSub) {
              | Data((_, reporterCount)) =>
                <div className={CssHelper.flexBox()}>
                  <Text
                    block=true
                    value={reporterCount |> string_of_int}
                    weight=Text.Semibold
                    transform=Text.Uppercase
                  />
                  <HSpacing size=Spacing.xs />
                  <Text
                    block=true
                    value="Reporters"
                    weight=Text.Semibold
                    transform=Text.Uppercase
                  />
                </div>
              | _ => <LoadingCensorBar width=100 height=15 />
              }}
           </Col>
         </Row>
       : <THead>
           <Row alignItems=Row.Center>
             <Col>
               {switch (allSub) {
                | Data((_, reporterCount)) =>
                  <div className={CssHelper.flexBox()}>
                    <Text
                      block=true
                      value={reporterCount |> string_of_int}
                      weight=Text.Semibold
                      transform=Text.Uppercase
                    />
                    <HSpacing size=Spacing.xs />
                    <Text
                      block=true
                      value="Reporters"
                      weight=Text.Semibold
                      transform=Text.Uppercase
                    />
                  </div>
                | _ => <LoadingCensorBar width=100 height=15 />
                }}
             </Col>
           </Row>
         </THead>}
    {switch (allSub) {
     | Data((reporters, reporterCount)) =>
       let pageCount = Page.getPageCount(reporterCount, pageSize);
       <>
         {reporterCount > 0
            ? reporters
              ->Belt_Array.mapWithIndex((i, e) =>
                  isMobile
                    ? <RenderBodyMobile
                        key={e |> Address.toBech32}
                        reserveIndex=i
                        reporterSub={Sub.resolve(e)}
                      />
                    : <RenderBody key={e |> Address.toBech32} reporterSub={Sub.resolve(e)} />
                )
              ->React.array
            : <EmptyContainer>
                <img
                  src={isDarkMode ? Images.noDataDark : Images.noDataLight}
                  className=Styles.noDataImage
                />
                <Heading
                  size=Heading.H4
                  value="No Reporter"
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
             ? <RenderBodyMobile key={string_of_int(i)} reserveIndex=i reporterSub=noData />
             : <RenderBody key={string_of_int(i)} reporterSub=noData />
         )
       ->React.array
     }}
  </div>;
};
