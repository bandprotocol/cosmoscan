module Styles = {
  open Css;
  let mostRequestCard = (theme: Theme.t) =>
    style([
      backgroundColor(theme.secondaryBg),
      borderRadius(`px(12)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.2)))),
      padding3(~top=`px(24), ~h=`px(24), ~bottom=`px(16)),
      height(`calc((`sub, `percent(100.), `px(23)))),
      marginBottom(`px(24)),
    ]);
  let requestResponseBox = style([flexShrink(0.), flexGrow(0.), flexBasis(`percent(50.))]);
  let descriptionBox =
    style([minHeight(`px(36)), margin3(~top=`px(32), ~h=`zero, ~bottom=`px(16))]);
  let idBox = style([marginBottom(`px(4))]);
  let tbodyContainer = style([minHeight(`px(600))]);
  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);

  let oracleScriptLink = (theme: Theme.t) =>
    style([
      backgroundColor(theme.baseBlue),
      borderRadius(`px(8)),
      width(`px(32)),
      height(`px(32)),
      hover([backgroundColor(theme.darkBlue)]),
    ]);
};

type sort_by_t =
  | MostRequested
  | LatestUpdate;

let getName =
  fun
  | MostRequested => "Most Requested"
  | LatestUpdate => "Latest Update";

let defaultCompare = (a: OracleScriptSub.t, b: OracleScriptSub.t) =>
  if (a.timestamp !== b.timestamp) {
    compare(b.id |> ID.OracleScript.toInt, a.id |> ID.OracleScript.toInt);
  } else {
    compare(b.requestCount, a.requestCount);
  };

let sorting = (oracleSctipts: array(OracleScriptSub.t), sortedBy) => {
  oracleSctipts
  ->Belt.List.fromArray
  ->Belt.List.sort((a, b) => {
      let result = {
        switch (sortedBy) {
        | MostRequested => compare(b.requestCount, a.requestCount)
        | LatestUpdate => compare(b.timestamp, a.timestamp)
        };
      };
      if (result !== 0) {
        result;
      } else {
        defaultCompare(a, b);
      };
    })
  ->Belt.List.toArray;
};

module RenderMostRequestedCard = {
  [@react.component]
  let make =
      (
        ~reserveIndex,
        ~oracleScriptSub: ApolloHooks.Subscription.variant(OracleScriptSub.t),
        ~statsSub:
           ApolloHooks.Subscription.variant(array(OracleScriptSub.response_last_1_day_t)),
      ) => {
    let allSub = Sub.all2(oracleScriptSub, statsSub);
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Col
      key={
        switch (oracleScriptSub) {
        | Data({id}) => id |> ID.OracleScript.toString
        | _ => reserveIndex |> string_of_int
        }
      }
      col=Col.Four>
      <div
        className={Css.merge([
          Styles.mostRequestCard(theme),
          CssHelper.flexBox(~direction=`column, ~justify=`spaceBetween, ~align=`stretch, ()),
        ])}>
        <div
          className={CssHelper.flexBox(
            ~justify=`spaceBetween,
            ~align=`flexStart,
            ~wrap=`nowrap,
            (),
          )}>
          <div>
            <div className=Styles.idBox>
              {switch (oracleScriptSub) {
               | Data({id}) => <TypeID.OracleScript id position=TypeID.Title />
               | _ => <LoadingCensorBar width=40 height=15 />
               }}
            </div>
            {switch (oracleScriptSub) {
             | Data({name}) => <Heading size=Heading.H4 value=name weight=Heading.Thin />
             | _ => <LoadingCensorBar width=200 height=15 />
             }}
          </div>
          {switch (oracleScriptSub) {
           | Data({id}) =>
             <TypeID.OracleScriptLink id>
               <div
                 className={Css.merge([
                   Styles.oracleScriptLink(theme),
                   CssHelper.flexBox(~justify=`center, ()),
                 ])}>
                 <Icon name="far fa-arrow-right" color={theme.white} />
               </div>
             </TypeID.OracleScriptLink>
           | _ => <LoadingCensorBar width=32 height=32 radius=8 />
           }}
        </div>
        <div className=Styles.descriptionBox>
          {switch (oracleScriptSub) {
           | Data({description}) =>
             let text = Ellipsis.format(~text=description, ~limit=70, ());
             <Text value=text block=true />;
           | _ => <LoadingCensorBar width=250 height=15 />
           }}
        </div>
        <SeperatedLine />
        <div className={CssHelper.flexBox()}>
          <div className=Styles.requestResponseBox>
            <Heading
              size=Heading.H5
              value="Requests"
              marginBottom=8
              weight=Heading.Thin
              color={theme.textSecondary}
            />
            {switch (oracleScriptSub) {
             | Data({requestCount}) =>
               <Text value={requestCount |> Format.iPretty} block=true color={theme.textPrimary} />
             | _ => <LoadingCensorBar width=100 height=15 />
             }}
          </div>
          <div className=Styles.requestResponseBox>
            <Heading
              size=Heading.H5
              value="Response time"
              marginBottom=8
              weight=Heading.Thin
              color={theme.textSecondary}
            />
            {switch (allSub) {
             | Data(({id}, stats)) =>
               let resultOpt = stats->Belt.Array.getBy(stat => id == stat.id);
               switch (resultOpt) {
               | Some({responseTime}) =>
                 <Text
                   value={(responseTime |> Format.fPretty(~digits=2)) ++ " s"}
                   block=true
                   color={theme.textPrimary}
                 />
               | None => <Text value="TBD" />
               };
             | _ => <LoadingCensorBar width=100 height=15 />
             }}
          </div>
        </div>
      </div>
    </Col>;
  };
};

module RenderBody = {
  [@react.component]
  let make =
      (
        ~reserveIndex,
        ~oracleScriptSub: ApolloHooks.Subscription.variant(OracleScriptSub.t),
        ~statsSub:
           ApolloHooks.Subscription.variant(array(OracleScriptSub.response_last_1_day_t)),
      ) => {
    let allSub = Sub.all2(oracleScriptSub, statsSub);

    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <TBody
      key={
        switch (oracleScriptSub) {
        | Data({id}) => id |> ID.OracleScript.toString
        | _ => reserveIndex |> string_of_int
        }
      }>
      <Row alignItems=Row.Center>
        <Col col=Col.Six>
          {switch (oracleScriptSub) {
           | Data({id, name}) =>
             <div className={CssHelper.flexBox()}>
               <TypeID.OracleScript id />
               <HSpacing size=Spacing.sm />
               <Text value=name ellipsis=true color={theme.textPrimary} />
             </div>
           | _ => <LoadingCensorBar width=300 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          <div className={CssHelper.flexBox(~justify=`flexStart, ~align=`flexStart, ())}>
            {switch (allSub) {
             | Data(({id, requestCount}, stats)) =>
               let resultOpt = stats->Belt.Array.getBy(stat => id == stat.id);
               <>
                 <div>
                   <Text
                     value={requestCount |> Format.iPretty}
                     weight=Text.Medium
                     block=true
                     ellipsis=true
                   />
                 </div>
                 <HSpacing size={`px(2)} />
                 <div>
                   <Text
                     value={
                       switch (resultOpt) {
                       | Some({responseTime}) =>
                         "(" ++ (responseTime |> Format.fPretty(~digits=2)) ++ " s)"
                       | None => "(TBD)"
                       }
                     }
                     weight=Text.Medium
                     block=true
                   />
                 </div>
               </>;
             | _ => <LoadingCensorBar width=70 height=15 />
             }}
          </div>
        </Col>
        <Col col=Col.Two>
          <div className={CssHelper.flexBox(~justify=`flexStart, ~align=`flexStart, ())}>
            {switch (allSub) {
             | Data(({version}, _)) =>
               <div>
                 <Text
                   value={
                     switch (version) {
                     | Ok => "OK"
                     | Redeploy => "Need Redeployment"
                     | Nothing => ""
                     }
                   }
                   weight=Text.Medium
                   block=true
                 />
               </div>
             | _ => <LoadingCensorBar width=70 height=15 />
             }}
          </div>
        </Col>
        <Col col=Col.Two>
          <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
            {switch (oracleScriptSub) {
             | Data({timestamp: timestampOpt}) =>
               switch (timestampOpt) {
               | Some(timestamp') =>
                 <Timestamp
                   time=timestamp'
                   size=Text.Md
                   weight=Text.Regular
                   textAlign=Text.Right
                 />
               | None => <Text value="Genesis" />
               }
             | _ => <LoadingCensorBar width=80 height=15 />
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
        ~oracleScriptSub: ApolloHooks.Subscription.variant(OracleScriptSub.t),
        ~statsSub:
           ApolloHooks.Subscription.variant(array(OracleScriptSub.response_last_1_day_t)),
      ) => {
    switch (oracleScriptSub) {
    | Data({id, timestamp: timestampOpt, name, requestCount, version}) =>
      <MobileCard
        values=InfoMobileCard.[
          ("Oracle Script", OracleScript(id, name)),
          (
            "Request&\nResponse time",
            switch (statsSub) {
            | Data(stats) =>
              RequestResponse({
                requestCount,
                responseTime:
                  stats
                  ->Belt.Array.getBy(stat => id == stat.id)
                  ->Belt.Option.map(({responseTime}) => responseTime),
              })
            | _ => Loading(80)
            },
          ),
          (
            "Version",
            switch (version) {
            | Ok => Text("OK")
            | Redeploy => Text("Need Redeployment")
            | Nothing => Text("")
            },
          ),
          (
            "Timestamp",
            switch (timestampOpt) {
            | Some(timestamp') => Timestamp(timestamp')
            | None => Text("Genesis")
            },
          ),
        ]
        key={id |> ID.OracleScript.toString}
        idx={id |> ID.OracleScript.toString}
      />
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("Oracle Script", Loading(200)),
          ("Request&\nResponse time", Loading(80)),
          ("Version", Loading(80)),
          ("Timestamp", Loading(180)),
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
  let (page, setPage) = React.useState(_ => 1);
  let (searchTerm, setSearchTerm) = React.useState(_ => "");
  let (sortedBy, setSortedBy) = React.useState(_ => LatestUpdate);

  React.useEffect1(
    () => {
      if (searchTerm !== "") {
        setPage(_ => 1);
      };
      None;
    },
    [|searchTerm|],
  );

  let pageSize = 10;
  let mostRequestedPageSize = isMobile ? 3 : 6;
  let oracleScriptsCountSub = OracleScriptSub.count(~searchTerm, ());
  let oracleScriptsSub = OracleScriptSub.getList(~pageSize, ~page, ~searchTerm, ());

  let mostRequestedOracleScriptSub =
    OracleScriptSub.getList(~pageSize=mostRequestedPageSize, ~page=1, ~searchTerm="", ());
  let statsSub = OracleScriptSub.getResponseTimeList();

  let allSub = Sub.all2(oracleScriptsSub, oracleScriptsCountSub);

  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  <Section>
    <div className=CssHelper.container id="oraclescriptsSection">
      <div className=CssHelper.mobileSpacing>
        <Heading value="All Oracle Scripts" size=Heading.H2 marginBottom=40 marginBottomSm=24 />
        {switch (mostRequestedOracleScriptSub) {
         | Data(oracleScripts) when oracleScripts->Belt.Array.length === 0 => React.null
         | Data(oracleScripts) =>
           <>
             <Heading
               value="Most Requested"
               size=Heading.H3
               marginBottom=16
               color={theme.textSecondary}
             />
             <Row>
               {oracleScripts
                ->Belt_Array.mapWithIndex((i, e) =>
                    <RenderMostRequestedCard
                      key={e.id |> ID.OracleScript.toString}
                      reserveIndex=i
                      oracleScriptSub={Sub.resolve(e)}
                      statsSub
                    />
                  )
                ->React.array}
             </Row>
           </>
         | _ =>
           <>
             <Heading value="Most Requested" size=Heading.H4 marginBottom=16 />
             <Row>
               {Belt_Array.make(mostRequestedPageSize, ApolloHooks.Subscription.NoData)
                ->Belt_Array.mapWithIndex((i, e) =>
                    <RenderMostRequestedCard
                      key={i |> string_of_int}
                      reserveIndex=i
                      oracleScriptSub=NoData
                      statsSub=NoData
                    />
                  )
                ->React.array}
             </Row>
           </>
         }}
        <Row alignItems=Row.Center marginBottom=40 marginBottomSm=24>
          <Col>
            {switch (allSub) {
             | Data((_, oracleScriptsCount)) =>
               <Heading
                 value={(oracleScriptsCount |> Format.iPretty) ++ " In total"}
                 size=Heading.H3
               />
             | _ => <LoadingCensorBar width=65 height=21 />
             }}
          </Col>
        </Row>
        <Table>
          <Row alignItems=Row.Center marginTop=32 marginBottom=16>
            <Col col=Col.Six colSm=Col.Eight>
              <SearchInput placeholder="Search Oracle Script" onChange=setSearchTerm />
            </Col>
            <Col col=Col.Six colSm=Col.Four>
              <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
                <SortableDropdown
                  sortedBy
                  setSortedBy
                  sortList=[
                    (MostRequested, getName(MostRequested)),
                    (LatestUpdate, getName(LatestUpdate)),
                  ]
                />
              </div>
            </Col>
          </Row>
          {isMobile
             ? React.null
             : <THead>
                 <Row alignItems=Row.Center>
                   <Col col=Col.Six>
                     <Text
                       block=true
                       value="Oracle Script"
                       weight=Text.Semibold
                       transform=Text.Uppercase
                       size=Text.Sm
                     />
                   </Col>
                   <Col col=Col.Two>
                     <Text
                       block=true
                       value="Request & Response time"
                       weight=Text.Semibold
                       transform=Text.Uppercase
                       size=Text.Sm
                     />
                   </Col>
                   <Col col=Col.Two>
                     <Text
                       block=true
                       value="Version"
                       weight=Text.Semibold
                       transform=Text.Uppercase
                       size=Text.Sm
                     />
                   </Col>
                   <Col col=Col.Two>
                     <Text
                       block=true
                       value="Timestamp"
                       weight=Text.Semibold
                       transform=Text.Uppercase
                       size=Text.Sm
                       align=Text.Right
                     />
                   </Col>
                 </Row>
               </THead>}
          {switch (allSub) {
           | Data((oracleScripts, _)) when oracleScripts->Belt.Array.length === 0 =>
             <EmptyContainer>
               <img
                 src={isDarkMode ? Images.noOracleDark : Images.noOracleLight}
                 className=Styles.noDataImage
                 alt="No Oracle Script"
               />
               <Heading
                 size=Heading.H4
                 value="No Oracle Script"
                 align=Heading.Center
                 weight=Heading.Regular
                 color={theme.textSecondary}
               />
             </EmptyContainer>
           | Data((oracleScripts, oracleScriptsCount)) =>
             let pageCount = Page.getPageCount(oracleScriptsCount, pageSize);
             <>
               {oracleScripts
                ->sorting(sortedBy)
                ->Belt_Array.mapWithIndex((i, e) =>
                    isMobile
                      ? <RenderBodyMobile
                          key={e.id |> ID.OracleScript.toString}
                          reserveIndex=i
                          oracleScriptSub={Sub.resolve(e)}
                          statsSub
                        />
                      : <RenderBody
                          key={e.id |> ID.OracleScript.toString}
                          reserveIndex=i
                          oracleScriptSub={Sub.resolve(e)}
                          statsSub
                        />
                  )
                ->React.array}
               {isMobile
                  ? React.null
                  : <Pagination
                      currentPage=page
                      pageCount
                      onPageChange={newPage => setPage(_ => newPage)}
                    />}
             </>;
           | _ =>
             <div className=Styles.tbodyContainer>
               {Belt_Array.make(10, ApolloHooks.Subscription.NoData)
                ->Belt_Array.mapWithIndex((i, e) =>
                    isMobile
                      ? <RenderBodyMobile
                          key={i |> string_of_int}
                          reserveIndex=i
                          oracleScriptSub=NoData
                          statsSub=NoData
                        />
                      : <RenderBody
                          key={i |> string_of_int}
                          reserveIndex=i
                          oracleScriptSub=NoData
                          statsSub=NoData
                        />
                  )
                ->React.array}
             </div>
           }}
        </Table>
      </div>
    </div>
  </Section>;
};
