module Styles = {
  open Css
  let infoHeader = style(list{borderBottom(#px(1), #solid, Colors.gray9), paddingBottom(#px(16))})
}

module Content = {
  @react.component
  let make = (
    ~dataSourceSub: ApolloHooks.Subscription.variant<BandScan.DataSourceSub.t>,
    ~dataSourceID,
    ~hashtag,
  ) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

    <Section>
      <div className=CssHelper.container>
        <Row marginBottom=40 marginBottomSm=16>
          <Col>
            <Heading value="Data Source" size=Heading.H2 marginBottom=32 marginBottomSm=8 />
            {switch dataSourceSub {
            | Data({id, name}) =>
              <div className={CssHelper.flexBox()}>
                <TypeID.DataSource id position=TypeID.Title />
                <HSpacing size=Spacing.sm />
                <Heading size=Heading.H3 value=name weight=Heading.Thin />
              </div>
            | _ => <LoadingCensorBar width=270 height=15 />
            }}
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
                      value="Owner" size=Heading.H4 weight=Heading.Thin color=theme.textSecondary
                    />
                    <HSpacing size=Spacing.xs />
                    <CTooltip tooltipText="The owner of the data source">
                      <Icon name="fal fa-info-circle" size=10 color=theme.textSecondary />
                    </CTooltip>
                  </div>
                </Col>
                <Col col=Col.Eight>
                  {switch dataSourceSub {
                  | Data({owner}) => <AddressRender address=owner position=AddressRender.Subtitle />
                  | _ => <LoadingCensorBar width=284 height=15 />
                  }}
                </Col>
              </Row>
              <Row marginBottom=24 alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Treasury" size=Heading.H4 weight=Heading.Thin color=theme.textSecondary
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch dataSourceSub {
                  | Data({treasury}) =>
                    <AddressRender address=treasury position=AddressRender.Subtitle />
                  | _ => <LoadingCensorBar width=284 height=15 />
                  }}
                </Col>
              </Row>
              <Row marginBottom=24 alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Fee" size=Heading.H4 weight=Heading.Thin color=theme.textSecondary
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch dataSourceSub {
                  | Data({fee}) => <AmountRender coins=fee pos=AmountRender.TxIndex />
                  | _ => <LoadingCensorBar width=284 height=15 />
                  }}
                </Col>
              </Row>
              <Row alignItems=Row.Center>
                <Col col=Col.Four mbSm=8>
                  <Heading
                    value="Description"
                    size=Heading.H4
                    weight=Heading.Thin
                    color=theme.textSecondary
                  />
                </Col>
                <Col col=Col.Eight>
                  {switch dataSourceSub {
                  | Data({description}) => <Text size=Text.Lg value=description />
                  | _ => <LoadingCensorBar width=284 height=15 />
                  }}
                </Col>
              </Row>
            </InfoContainer>
          </Col>
        </Row>
        <Table>
          <Tab.Route
            tabs=[
              {
                name: "Requests",
                route: dataSourceID |> ID.DataSource.getRouteWithTab(_, Route.DataSourceRequests),
              },
              {
                name: "Code",
                route: dataSourceID |> ID.DataSource.getRouteWithTab(_, Route.DataSourceCode),
              },
              {
                name: "Test Execution",
                route: dataSourceID |> ID.DataSource.getRouteWithTab(_, Route.DataSourceExecute),
              },
              // {
              //   name: "Revisions",
              //   route: dataSourceID |> ID.DataSource.getRouteWithTab(_, Route.DataSourceRevisions),
              // },
            ]
            currentRoute={dataSourceID |> ID.DataSource.getRouteWithTab(_, hashtag)}>
            {switch hashtag {
            | DataSourceExecute =>
              switch dataSourceSub {
              | Data({executable}) => <DataSourceExecute executable />
              | _ => <LoadingCensorBar.CircleSpin height=400 />
              }
            | DataSourceCode =>
              switch dataSourceSub {
              | Data({executable}) => <DataSourceCode executable />
              | _ => <LoadingCensorBar.CircleSpin height=300 />
              }
            | DataSourceRequests => <DataSourceRequestTable dataSourceID />
            | DataSourceRevisions => <DataSourceRevisionTable id=dataSourceID />
            }}
          </Tab.Route>
        </Table>
      </div>
    </Section>
  }
}

@react.component
let make = (~dataSourceID, ~hashtag) => {
  let dataSourceSub = DataSourceSub.get(dataSourceID)

  switch dataSourceSub {
  | NoData => <NotFound />
  | _ => <Content dataSourceSub dataSourceID hashtag />
  }
}
