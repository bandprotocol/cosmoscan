module Styles = {
  open Css;

  let tableWrapper = style([Media.mobile([padding2(~v=`px(16), ~h=`zero)])]);
  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);

  // DataSource Table
  let dataSourceTable = (show, theme: Theme.t) => {
    style([
      padding2(~v=show ? `px(16) : `zero, ~h=`px(24)),
      marginTop(show ? `px(24) : `zero),
      backgroundColor(theme.tableRowBorderColor),
      transition(~duration=200, "all"),
      height(show ? `auto : `zero),
      opacity(show ? 1. : 0.),
      selector("> div + div", [paddingTop(`px(16))]),
    ]);
  };
  let toggle = style([cursor(`pointer)]);
};

module DataSourceItem = {
  [@react.component]
  let make = (~dataSource: ReportSub.ValidatorReport.report_details_t) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Two>
        <Text block=true value={dataSource.externalID} color={theme.textPrimary} />
      </Col>
      <Col col=Col.Three>
        <div className={CssHelper.flexBox(~wrap=`nowrap, ())}>
          <TypeID.DataSource
            id={
                 let rawRequest = dataSource.rawRequest |> Belt_Option.getExn;
                 rawRequest.dataSource.dataSourceID;
               }
          />
          <HSpacing size=Spacing.sm />
          <Text
            value={
                    let rawRequest = dataSource.rawRequest |> Belt_Option.getExn;
                    rawRequest.dataSource.dataSourceName;
                  }
            ellipsis=true
          />
        </div>
      </Col>
      <Col col=Col.Two>
        <Text
          block=true
          value={
                  let rawRequest = dataSource.rawRequest |> Belt_Option.getExn;
                  rawRequest.calldata |> JsBuffer.toUTF8;
                }
          color={theme.textPrimary}
        />
      </Col>
      <Col col=Col.Two>
        <Text block=true value={dataSource.exitCode} color={theme.textPrimary} />
      </Col>
      <Col col=Col.Three>
        <Text
          block=true
          value={dataSource.data |> JsBuffer.toUTF8}
          align=Text.Right
          color={theme.textPrimary}
          ellipsis=true
        />
      </Col>
    </Row>;
  };
};

module RenderBody = {
  [@react.component]
  let make = (~reportsSub: ApolloHooks.Subscription.variant(ReportSub.ValidatorReport.t)) => {
    let (show, setShow) = React.useState(_ => false);
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);

    <TBody>
      <Row alignItems=Row.Center minHeight={`px(30)}>
        <Col col=Col.Three>
          {switch (reportsSub) {
           | Data({request: {id}}) => <TypeID.Request id />
           | _ => <LoadingCensorBar width=135 height=15 />
           }}
        </Col>
        <Col col=Col.Four>
          {switch (reportsSub) {
           | Data({request: {oracleScript: {oracleScriptID, name}}}) =>
             <div className={CssHelper.flexBox()}>
               <TypeID.OracleScript id=oracleScriptID />
               <HSpacing size=Spacing.sm />
               <Text value=name ellipsis=true />
             </div>
           | _ => <LoadingCensorBar width=270 height=15 />
           }}
        </Col>
        <Col col=Col.Three>
          {switch (reportsSub) {
           | Data({txHash}) =>
             //  switch (txHash) {
             //  | Some(txHash') => <TxLink txHash=txHash' width=140 />
             //  | None => <Text value="Syncing" />
             //  }
             <TxLink txHash width=140 />
           | _ => <LoadingCensorBar width=170 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          <div
            onClick={_ => setShow(prev => !prev)}
            className={Css.merge([CssHelper.flexBox(~justify=`flexEnd, ()), Styles.toggle])}>
            {switch (reportsSub) {
             | Data(_) =>
               <>
                 <Text
                   block=true
                   value={show ? "Hide Report" : "Show Report"}
                   weight=Text.Semibold
                   color={theme.textPrimary}
                 />
                 <HSpacing size=Spacing.xs />
                 <Icon
                   name={show ? "fas fa-caret-up" : "fas fa-caret-down"}
                   color={theme.textSecondary}
                 />
               </>
             | _ => <LoadingCensorBar width=100 height=15 />
             }}
          </div>
        </Col>
      </Row>
      <div className={Styles.dataSourceTable(show, theme)}>
        <Row>
          <Col col=Col.Two>
            <Text
              block=true
              value="External ID"
              weight=Text.Semibold
              transform=Text.Uppercase
              size=Text.Sm
            />
          </Col>
          <Col col=Col.Three>
            <Text
              block=true
              value="Data Source"
              weight=Text.Semibold
              transform=Text.Uppercase
              size=Text.Sm
            />
          </Col>
          <Col col=Col.Two>
            <Text
              block=true
              value="Param"
              weight=Text.Semibold
              transform=Text.Uppercase
              size=Text.Sm
            />
          </Col>
          <Col col=Col.Two>
            <Text
              block=true
              value="Exit Code"
              weight=Text.Semibold
              transform=Text.Uppercase
              size=Text.Sm
            />
          </Col>
          <Col col=Col.Three>
            <Text
              block=true
              value="Value"
              weight=Text.Semibold
              align=Text.Right
              transform=Text.Uppercase
              size=Text.Sm
            />
          </Col>
        </Row>
        {switch (reportsSub) {
         | Data({reportDetails}) =>
           reportDetails
           ->Belt_Array.mapWithIndex((i, reportDetail) =>
               <DataSourceItem
                 key={(i |> string_of_int) ++ reportDetail.externalID}
                 dataSource=reportDetail
               />
             )
           ->React.array
         | _ => <LoadingCensorBar width=170 height=50 />
         }}
      </div>
    </TBody>;
  };
};

module RenderBodyMobile = {
  [@react.component]
  let make =
      (~reserveIndex, ~reportsSub: ApolloHooks.Subscription.variant(ReportSub.ValidatorReport.t)) => {
    let isSmallMobile = Media.isSmallMobile();
    switch (reportsSub) {
    | Data({txHash, request: {id, oracleScript: {oracleScriptID, name}}, reportDetails}) =>
      <MobileCard
        values=InfoMobileCard.[
          ("Request ID", RequestID(id)),
          ("Oracle Script", OracleScript(oracleScriptID, name)),
          (
            "TX Hash",
            TxHash(txHash, isSmallMobile ? 170 : 200),
            // switch (txHash) {
            // | Some(txHash') => TxHash(txHash', isSmallMobile ? 170 : 200)
            // | None => Text("Syncing")
            // },
          ),
        ]
        key={id |> ID.Request.toString}
        idx={id |> ID.Request.toString}
        panels={
          reportDetails
          ->Belt_Array.map(({externalID, exitCode, data, rawRequest: rawRequestOpt}) => {
              let ReportSub.ValidatorReport.{
                    dataSource: {dataSourceID, dataSourceName},
                    calldata,
                  } =
                rawRequestOpt->Belt.Option.getExn;
              InfoMobileCard.[
                ("External ID", Text(externalID)),
                ("Data Source", DataSource(dataSourceID, dataSourceName)),
                ("Param", Text(calldata |> JsBuffer.toUTF8)),
                ("Exit Code", Text(exitCode)),
                ("Value", Text(data |> JsBuffer.toUTF8)),
              ];
            })
          ->Belt.List.fromArray
        }
      />
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("Request ID", Loading(70)),
          ("Oracle Script", Loading(136)),
          ("TX Hash", Loading(isSmallMobile ? 170 : 200)),
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
  let pageSize = 5;

  let reportsSub =
    ReportSub.ValidatorReport.getListByValidator(
      ~page,
      ~pageSize,
      ~validator={
        address |> Address.toOperatorBech32;
      },
    );
  let reportsCountSub = ReportSub.ValidatorReport.count(address);
  let allSub = Sub.all2(reportsSub, reportsCountSub);

  let isMobile = Media.isMobile();
  let (ThemeContext.{theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  <div className=Styles.tableWrapper>
    {isMobile
       ? <Row marginBottom=16>
           <Col>
             {switch (allSub) {
              | Data((_, reportsCount)) =>
                <div className={CssHelper.flexBox()}>
                  <Text
                    block=true
                    value={reportsCount |> Format.iPretty}
                    weight=Text.Semibold
                    transform=Text.Uppercase
                    size=Text.Sm
                  />
                  <HSpacing size=Spacing.xs />
                  <Text
                    block=true
                    value="Requests"
                    weight=Text.Semibold
                    transform=Text.Uppercase
                    size=Text.Sm
                  />
                </div>
              | _ => <LoadingCensorBar width=100 height=15 />
              }}
           </Col>
         </Row>
       : <THead>
           <Row alignItems=Row.Center>
             <Col col=Col.Three>
               {switch (allSub) {
                | Data((_, reportsCount)) =>
                  <div className={CssHelper.flexBox()}>
                    <Text
                      block=true
                      value={reportsCount |> Format.iPretty}
                      weight=Text.Semibold
                      transform=Text.Uppercase
                      size=Text.Sm
                    />
                    <HSpacing size=Spacing.xs />
                    <Text
                      block=true
                      value="Oracle Reports"
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
                 value="Oracle Script"
                 weight=Text.Semibold
                 transform=Text.Uppercase
                 size=Text.Sm
               />
             </Col>
             <Col col=Col.Five>
               <Text
                 block=true
                 value="TX Hash"
                 weight=Text.Semibold
                 transform=Text.Uppercase
                 size=Text.Sm
               />
             </Col>
           </Row>
         </THead>}
    {switch (allSub) {
     | Data((reports, reportsCount)) =>
       let pageCount = Page.getPageCount(reportsCount, pageSize);
       <>
         {reportsCount > 0
            ? reports
              ->Belt_Array.mapWithIndex((i, e) =>
                  isMobile
                    ? <RenderBodyMobile
                        key={(i |> string_of_int) ++ (e.request.id |> ID.Request.toString)}
                        reserveIndex=i
                        reportsSub={Sub.resolve(e)}
                      />
                    : <RenderBody
                        key={(i |> string_of_int) ++ (e.request.id |> ID.Request.toString)}
                        reportsSub={Sub.resolve(e)}
                      />
                )
              ->React.array
            : <EmptyContainer>
                <img
                  alt="No Report"
                  src={isDarkMode ? Images.noDataDark : Images.noDataLight}
                  className=Styles.noDataImage
                />
                <Heading
                  size=Heading.H4
                  value="No Report"
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
             ? <RenderBodyMobile key={i |> string_of_int} reserveIndex=i reportsSub=noData />
             : <RenderBody key={i |> string_of_int} reportsSub=noData />
         )
       ->React.array
     }}
  </div>;
};
