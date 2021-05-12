module Styles = {
  open Css;

  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
};

type sort_by_t =
  | MostRequested
  | LatestUpdate;

let getName =
  fun
  | MostRequested => "Most Requested"
  | LatestUpdate => "Latest Update";

let defaultCompare = (a: DataSourceSub.t, b: DataSourceSub.t) =>
  if (a.timestamp != b.timestamp) {
    compare(b.id |> ID.DataSource.toInt, a.id |> ID.DataSource.toInt);
  } else {
    compare(b.requestCount, a.requestCount);
  };

let sorting = (dataSources: array(DataSourceSub.t), sortedBy) => {
  dataSources
  ->Belt.List.fromArray
  ->Belt.List.sort((a, b) => {
      let result = {
        switch (sortedBy) {
        | MostRequested => compare(b.requestCount, a.requestCount)
        | LatestUpdate => compare(b.timestamp, a.timestamp)
        };
      };
      if (result != 0) {
        result;
      } else {
        defaultCompare(a, b);
      };
    })
  ->Belt.List.toArray;
};

module RenderBody = {
  [@react.component]
  let make = (~reserveIndex, ~dataSourcesSub: ApolloHooks.Subscription.variant(DataSourceSub.t)) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <TBody
      key={
        switch (dataSourcesSub) {
        | Data({id}) => id |> ID.DataSource.toString
        | _ => reserveIndex |> string_of_int
        }
      }>
      <Row alignItems=Row.Center>
        <Col col=Col.Five>
          {switch (dataSourcesSub) {
           | Data({id, name}) =>
             <div className={CssHelper.flexBox()}>
               <TypeID.DataSource id />
               <HSpacing size=Spacing.sm />
               <Text value=name ellipsis=true color={theme.textPrimary} />
             </div>
           | _ => <LoadingCensorBar width=300 height=15 />
           }}
        </Col>
        <Col col=Col.Four>
          {switch (dataSourcesSub) {
           | Data({description}) => <Text value=description block=true />
           | _ => <LoadingCensorBar width=270 height=15 />
           }}
        </Col>
        <Col col=Col.One>
          {switch (dataSourcesSub) {
           | Data({requestCount}) =>
             <div>
               <Text
                 value={requestCount |> Format.iPretty}
                 weight=Text.Medium
                 block=true
                 ellipsis=true
               />
             </div>
           | _ => <LoadingCensorBar width=70 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
            {switch (dataSourcesSub) {
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
  let make = (~reserveIndex, ~dataSourcesSub: ApolloHooks.Subscription.variant(DataSourceSub.t)) => {
    switch (dataSourcesSub) {
    | Data({id, timestamp: timestampOpt, description, name, requestCount}) =>
      <MobileCard
        values=InfoMobileCard.[
          ("Data Source", DataSource(id, name)),
          ("Description", Text(description)),
          ("Requests", Count(requestCount)),
          (
            "Timestamp",
            switch (timestampOpt) {
            | Some(timestamp') => Timestamp(timestamp')
            | None => Text("Genesis")
            },
          ),
        ]
        key={id |> ID.DataSource.toString}
        idx={id |> ID.DataSource.toString}
      />
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("Data Source", Loading(70)),
          ("Description", Loading(136)),
          ("Requests", Loading(20)),
          ("Timestamp", Loading(166)),
        ]
        key={reserveIndex |> string_of_int}
        idx={reserveIndex |> string_of_int}
      />
    };
  };
};

[@react.component]
let make = () => {
  let (page, setPage) = React.useState(_ => 1);
  let (searchTerm, setSearchTerm) = React.useState(_ => "");
  let (sortedBy, setSortedBy) = React.useState(_ => LatestUpdate);
  let pageSize = 10;
  let isMobile = Media.isMobile();

  let dataSourcesCountSub = DataSourceSub.count(~searchTerm, ());
  let dataSourcesSub = DataSourceSub.getList(~pageSize, ~page, ~searchTerm, ());

  let allSub = Sub.all2(dataSourcesSub, dataSourcesCountSub);

  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  React.useEffect1(
    () => {
      if (searchTerm != "") {
        setPage(_ => 1);
      };
      None;
    },
    [|searchTerm|],
  );

  <Section>
    <div className=CssHelper.container id="datasourcesSection">
      <div className=CssHelper.mobileSpacing>
        <Row alignItems=Row.Center marginBottom=40 marginBottomSm=24>
          <Col col=Col.Twelve>
            <Heading value="All Data Sources" size=Heading.H2 marginBottom=16 marginBottomSm=8 />
            {switch (allSub) {
             | Data((_, dataSourcesCount)) =>
               <Heading
                 value={(dataSourcesCount |> Format.iPretty) ++ " In total"}
                 size=Heading.H3
                 weight=Heading.Thin
                 color={theme.textSecondary}
               />
             | _ => <LoadingCensorBar width=65 height=21 />
             }}
          </Col>
        </Row>
        <Table>
          <Row alignItems=Row.Center marginTop=32 marginBottom=16>
            <Col col=Col.Six colSm=Col.Eight>
              <SearchInput placeholder="Search Data Source" onChange=setSearchTerm />
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
                   <Col col=Col.Five>
                     <div className=TElement.Styles.hashContainer>
                       <Text
                         block=true
                         value="Data Source"
                         transform=Text.Uppercase
                         size=Text.Sm
                         weight=Text.Semibold
                       />
                     </div>
                   </Col>
                   <Col col=Col.Four>
                     <Text
                       block=true
                       value="Description"
                       transform=Text.Uppercase
                       size=Text.Sm
                       weight=Text.Semibold
                     />
                   </Col>
                   <Col col=Col.One>
                     <Text
                       block=true
                       value="Requests"
                       transform=Text.Uppercase
                       size=Text.Sm
                       weight=Text.Semibold
                     />
                   </Col>
                   <Col col=Col.Two>
                     <Text
                       block=true
                       value="Timestamp"
                       transform=Text.Uppercase
                       size=Text.Sm
                       weight=Text.Semibold
                       align=Text.Right
                     />
                   </Col>
                 </Row>
               </THead>}
          {switch (allSub) {
           | Data((dataSources, dataSourcesCount)) =>
             let pageCount = Page.getPageCount(dataSourcesCount, pageSize);
             <>
               {dataSources->Belt.Array.length > 0
                  ? dataSources
                    ->sorting(sortedBy)
                    ->Belt_Array.mapWithIndex((i, e) =>
                        isMobile
                          ? <RenderBodyMobile
                              key={e.id |> ID.DataSource.toString}
                              reserveIndex=i
                              dataSourcesSub={Sub.resolve(e)}
                            />
                          : <RenderBody
                              key={e.id |> ID.DataSource.toString}
                              reserveIndex=i
                              dataSourcesSub={Sub.resolve(e)}
                            />
                      )
                    ->React.array
                  : <EmptyContainer>
                      <img src=Images.noSource className=Styles.noDataImage />
                      <Heading
                        size=Heading.H4
                        value="No Data Source"
                        align=Heading.Center
                        weight=Heading.Regular
                        color=Colors.bandBlue
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
             Belt_Array.make(10, ApolloHooks.Subscription.NoData)
             ->Belt_Array.mapWithIndex((i, noData) =>
                 isMobile
                   ? <RenderBodyMobile
                       key={i |> string_of_int}
                       reserveIndex=i
                       dataSourcesSub=noData
                     />
                   : <RenderBody key={i |> string_of_int} reserveIndex=i dataSourcesSub=noData />
               )
             ->React.array
           }}
        </Table>
      </div>
    </div>
  </Section>;
};
