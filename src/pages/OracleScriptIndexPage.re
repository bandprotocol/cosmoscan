module Styles = {
  open Css;
  let titleSpacing = style([marginBottom(`px(26))]);
  let idCointainer = style([marginBottom(`px(16))]);
  let containerSpacingSm = style([Media.mobile([marginTop(`px(16))])]);

  let relatedDSContainer =
    style([
      selector("> div + div", [marginTop(`px(16))]),
      selector("> div > a", [marginRight(`px(8))]),
    ]);
};

module Content = {
  [@react.component]
  let make =
      (
        ~oracleScriptSub: ApolloHooks.Subscription.variant(BandScan.OracleScriptSub.t),
        ~oracleScriptID,
        ~hashtag,
      ) => {
    let statSub = OracleScriptSub.getResponseTime(oracleScriptID);

    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Section>
      <div className=CssHelper.container>
        <Heading value="Oracle Script" size=Heading.H2 marginBottom=40 marginBottomSm=24 />
        <Row marginBottom=40 marginBottomSm=16 alignItems=Row.Center>
          <Col col=Col.Six>
            <div className={Css.merge([CssHelper.flexBox(), Styles.idCointainer])}>
              {switch (oracleScriptSub) {
               | Data({id, name}) =>
                 <>
                   <TypeID.OracleScript id position=TypeID.Title />
                   <HSpacing size=Spacing.sm />
                   <Heading size=Heading.H3 value=name weight=Heading.Thin />
                 </>
               | _ => <LoadingCensorBar width=270 height=15 />
               }}
            </div>
          </Col>
          <Col col=Col.Three colSm=Col.Six>
            <InfoContainer>
              <Heading
                value="Requests"
                size=Heading.H4
                weight=Heading.Thin
                color={theme.textSecondary}
                marginBottom=26
              />
              {switch (oracleScriptSub) {
               | Data({requestCount}) =>
                 <Text
                   value={requestCount |> Format.iPretty}
                   size=Text.Xxxl
                   block=true
                   weight=Text.Bold
                   color={theme.textPrimary}
                 />
               | _ => <LoadingCensorBar width=100 height=15 />
               }}
            </InfoContainer>
          </Col>
          <Col col=Col.Three colSm=Col.Six>
            <InfoContainer>
              <div className={Css.merge([CssHelper.flexBox(), Styles.titleSpacing])}>
                <Heading
                  value="Response time"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
                <HSpacing size=Spacing.xs />
                <CTooltip
                  tooltipPlacementSm=CTooltip.BottomRight
                  tooltipText="The average time requests to this oracle script takes to resolve">
                  <Icon name="fal fa-info-circle" size=12 color={theme.textSecondary} />
                </CTooltip>
              </div>
              {switch (statSub) {
               | Data(statOpt) =>
                 <Text
                   value={
                     switch (statOpt) {
                     | Some({responseTime}) => responseTime |> Format.fPretty(~digits=2)
                     | None => "TBD"
                     }
                   }
                   size=Text.Xxxl
                   weight=Text.Bold
                   block=true
                   color={theme.textPrimary}
                 />
               | _ => <LoadingCensorBar width=100 height=15 />
               }}
            </InfoContainer>
          </Col>
        </Row>
        <Row marginBottom=24>
          <Col>
            <InfoContainer>
              <Heading value="Information" size=Heading.H4 />
              <SeperatedLine mt=32 mb=24 />
              <Row marginBottom=24 alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <div className={CssHelper.flexBox()}>
                    <Heading
                      value="Owner"
                      size=Heading.H4
                      weight=Heading.Thin
                      color={theme.textSecondary}
                    />
                    <HSpacing size=Spacing.xs />
                    <CTooltip tooltipText="The owner of the oracle script">
                      <Icon name="fal fa-info-circle" size=10 color={theme.textSecondary} />
                    </CTooltip>
                  </div>
                </Col>
                <Col col=Col.Eight>
                  {switch (oracleScriptSub) {
                   | Data({owner}) =>
                     <AddressRender address=owner position=AddressRender.Subtitle />
                   | _ => <LoadingCensorBar width=284 height=15 />
                   }}
                </Col>
              </Row>
              <Row marginBottom=24>
                <Col col=Col.Four mbSm=8>
                  <div className={Css.merge([CssHelper.flexBox(), Styles.containerSpacingSm])}>
                    <Heading
                      value="Data Sources"
                      size=Heading.H4
                      weight=Heading.Thin
                      color={theme.textSecondary}
                    />
                    <HSpacing size=Spacing.xs />
                    <CTooltip tooltipText="The data sources used in this oracle script">
                      <Icon name="fal fa-info-circle" size=10 color={theme.textSecondary} />
                    </CTooltip>
                  </div>
                </Col>
                <Col col=Col.Eight>
                  <div className=Styles.relatedDSContainer>
                    {switch (oracleScriptSub) {
                     | Data({relatedDataSources}) =>
                       relatedDataSources->Belt.List.size > 0
                         ? relatedDataSources
                           ->Belt.List.map(({dataSourceName, dataSourceID}) =>
                               <div
                                 key={dataSourceID |> ID.DataSource.toString}
                                 className={CssHelper.flexBox()}>
                                 <TypeID.DataSource id=dataSourceID position=TypeID.Subtitle />
                                 <Text
                                   value=dataSourceName
                                   size=Text.Lg
                                   block=true
                                   color={theme.textPrimary}
                                 />
                               </div>
                             )
                           ->Belt.List.toArray
                           ->React.array
                         : <Text value="TBD" />

                     | _ => <LoadingCensorBar width=284 height=15 />
                     }}
                  </div>
                </Col>
              </Row>
              <Row>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Description"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch (oracleScriptSub) {
                   | Data({description}) => <Text value=description size=Text.Lg block=true />
                   | _ => <LoadingCensorBar width=284 height=15 />
                   }}
                </Col>
              </Row>
            </InfoContainer>
          </Col>
        </Row>
        <Table>
          <Tab
            tabs=[|
              {
                name: "Requests",
                route:
                  oracleScriptID |> ID.OracleScript.getRouteWithTab(_, Route.OracleScriptRequests),
              },
              {
                name: "OWASM Code",
                route:
                  oracleScriptID |> ID.OracleScript.getRouteWithTab(_, Route.OracleScriptCode),
              },
              {
                name: "Bridge Code",
                route:
                  oracleScriptID
                  |> ID.OracleScript.getRouteWithTab(_, Route.OracleScriptBridgeCode),
              },
              {
                name: "Make New Request",
                route:
                  oracleScriptID |> ID.OracleScript.getRouteWithTab(_, Route.OracleScriptExecute),
              },
              // {
              //   name: "Revisions",
              //   route:
              //     oracleScriptID |> ID.OracleScript.getRouteWithTab(_, Route.OracleScriptRevisions),
              // },
            |]
            currentRoute={oracleScriptID |> ID.OracleScript.getRouteWithTab(_, hashtag)}>
            {switch (hashtag) {
             | OracleScriptExecute =>
               switch (oracleScriptSub) {
               | Data({schema}) => <OracleScriptExecute id=oracleScriptID schema />
               | _ => <LoadingCensorBar fullWidth=true height=400 />
               }

             | OracleScriptCode =>
               switch (oracleScriptSub) {
               | Data({sourceCodeURL}) => <OracleScriptCode url=sourceCodeURL />
               | _ => <LoadingCensorBar fullWidth=true height=400 />
               }

             | OracleScriptBridgeCode =>
               switch (oracleScriptSub) {
               | Data({schema}) => <OracleScriptBridgeCode schema />
               | _ => <LoadingCensorBar fullWidth=true height=400 />
               }
             | OracleScriptRequests => <OracleScriptRequestTable oracleScriptID />
             | OracleScriptRevisions => <OracleScriptRevisionTable id=oracleScriptID />
             }}
          </Tab>
        </Table>
      </div>
    </Section>;
  };
};

[@react.component]
let make = (~oracleScriptID, ~hashtag) => {
  let oracleScriptSub = OracleScriptSub.get(oracleScriptID);

  switch (oracleScriptSub) {
  | NoData => <NotFound />
  | _ => <Content oracleScriptSub oracleScriptID hashtag />
  };
};
