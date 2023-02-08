module Styles = {
  open Css;

  let noDataImage =
    style([
      width(`auto),
      height(`px(40)),
      marginBottom(`px(16)),
      Media.mobile([marginBottom(`px(8))]),
    ]);
  let container =
    style([
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.2)))),
    ]);

  let allRequestLink = (theme: Theme.t) =>
    style([
      backgroundColor(theme.primary_600),
      borderRadius(`px(8)),
      width(`px(32)),
      height(`px(32)),
      hover([backgroundColor(theme.darkBlue)]),
    ]);
};

module RenderBody = {
  [@react.component]
  let make = (~requestSub: ApolloHooks.Subscription.variant(RequestSub.t)) => {
    <TBody>
      <Row alignItems=Row.Center>
        <Col col=Col.Three>
          {switch (requestSub) {
           | Data({id}) => <TypeID.Request id />
           | _ => <LoadingCensorBar width=60 height=15 />
           }}
        </Col>
        <Col col=Col.Six>
          {switch (requestSub) {
           | Data({oracleScript: {oracleScriptID, name}}) =>
             <div className={CssHelper.flexBox(~wrap=`nowrap, ())}>
               <TypeID.OracleScript id=oracleScriptID />
               <HSpacing size=Spacing.sm />
               <Text value=name ellipsis=true />
             </div>
           | _ => <LoadingCensorBar width=150 height=15 />
           }}
        </Col>
        <Col col=Col.Three>
          <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
            {switch (requestSub) {
             | Data({resolveStatus, requestedValidators, reports}) =>
               let reportedCount = reports->Array.length;
               let requestedCount = requestedValidators->Array.length;

               <div className={CssHelper.flexBox()}>
                 <Text value={j|$reportedCount of $requestedCount|j} />
                 <HSpacing size=Spacing.sm />
                 <RequestStatus resolveStatus />
               </div>;
             | _ => <LoadingCensorBar width=70 height=15 />
             }}
          </div>
        </Col>
      </Row>
    </TBody>;
  };
};

module RenderBodyMobile = {
  [@react.component]
  let make = (~reserveIndex, ~requestSub: ApolloHooks.Subscription.variant(RequestSub.t)) => {
    switch (requestSub) {
    | Data({
        id,
        oracleScript: {oracleScriptID, name},
        resolveStatus,
        requestedValidators,
        reports,
      }) =>
      let reportedCount = reports->Array.length;
      let requestedCount = requestedValidators->Array.length;
      <MobileCard
        values=InfoMobileCard.[
          ("Request ID", RequestID(id)),
          ("Oracle Script", OracleScript(oracleScriptID, name)),
          ("Report Status", Text({j|$reportedCount of $requestedCount|j})),
        ]
        key={id |> ID.Request.toString}
        idx={id |> ID.Request.toString}
        requestStatus=resolveStatus
      />;
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("Request ID", Loading(60)),
          ("Oracle Script", Loading(150)),
          ("Report Status", Loading(60)),
        ]
        key={reserveIndex |> string_of_int}
        idx={reserveIndex |> string_of_int}
      />
    };
  };
};

[@react.component]
let make = (~latest5RequestSub: Sub.t(array(RequestSub.t))) => {
  let isMobile = Media.isMobile();
  let (ThemeContext.{theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  <Table>
    <Row marginTop=30 marginBottom=25 marginTopSm=24 marginBottomSm=0 alignItems=Row.Center>
      <Col col=Col.Six colSm=Col.Six>
        {isMobile
           ? <>
               <Heading value="Total Request" size=Heading.H4 />
               <VSpacing size={`px(4)} />
               {switch (latest5RequestSub) {
                | ApolloHooks.Subscription.Data(requests) =>
                  <Text
                    value={
                      requests
                      ->Belt.Array.get(0)
                      ->Belt.Option.mapWithDefault(0, ({id}) => id |> ID.Request.toInt)
                      ->Format.iPretty
                    }
                    size=Text.Lg
                    weight=Text.Medium
                  />
                | _ => <LoadingCensorBar width=90 height=18 />
                }}
             </>
           : <Heading value="Latest Requests" size=Heading.H4 />}
      </Col>
      <Col col=Col.Six colSm=Col.Six>
        <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
          {isMobile ? React.null : <Heading value="More Requests" size=Heading.H4 />}
          <HSpacing size=Spacing.md />
          <Link className={CssHelper.flexBox(~align=`center, ())} route=Route.RequestHomePage>
            <div
              className={Css.merge([
                Styles.allRequestLink(theme),
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
             <Col col=Col.Three>
               <Text
                 block=true
                 value="Request ID"
                 size=Text.Sm
                 weight=Text.Semibold
                 transform=Text.Uppercase
               />
             </Col>
             <Col col=Col.Six>
               <Text
                 block=true
                 value="Oracle Script"
                 size=Text.Sm
                 weight=Text.Semibold
                 transform=Text.Uppercase
               />
             </Col>
             <Col col=Col.Three>
               <Text
                 block=true
                 value="Report Status"
                 size=Text.Sm
                 weight=Text.Semibold
                 align=Text.Right
                 transform=Text.Uppercase
               />
             </Col>
           </Row>
         </THead>}
    {switch (latest5RequestSub) {
     | Data(requests) when requests->Belt.Array.length === 0 =>
       <EmptyContainer height={`calc((`sub, `percent(100.), `px(86)))} boxShadow=true>
         <img
           src={isDarkMode ? Images.noDataDark : Images.noDataLight}
           className=Styles.noDataImage
           alt="No Request"
         />
         <Heading size=Heading.H4 value="No Request" align=Heading.Center weight=Heading.Regular />
       </EmptyContainer>
     | Data(requests) =>
       requests
       ->Belt_Array.mapWithIndex((i, e) =>
           isMobile
             ? <RenderBodyMobile
                 key={e.id |> ID.Request.toString}
                 reserveIndex=i
                 requestSub={Sub.resolve(e)}
               />
             : <RenderBody key={e.id |> ID.Request.toString} requestSub={Sub.resolve(e)} />
         )
       ->React.array
     | _ =>
       Belt_Array.make(5, ApolloHooks.Subscription.NoData)
       ->Belt_Array.mapWithIndex((i, noData) =>
           isMobile
             ? <RenderBodyMobile key={i |> string_of_int} reserveIndex=i requestSub=noData />
             : <RenderBody key={i |> string_of_int} requestSub=noData />
         )
       ->React.array
     }}
  </Table>;
};
