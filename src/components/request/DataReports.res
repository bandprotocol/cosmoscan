module Styles = {
  open Css

  let reportCard = style(list{Media.mobile(list{padding2(~v=#px(8), ~h=#zero)})})

  let reportsTable = (theme: Theme.t) =>
    style(list{
      padding2(~v=#px(16), ~h=#px(24)),
      paddingBottom(#px(1)),
      marginTop(#px(24)),
      backgroundColor(theme.secondaryTableBg),
      transition(~duration=200, "all"),
      height(#auto),
      Media.mobile(list{paddingTop(#zero), paddingBottom(#zero)}),
    })

  let mobileCard = style(list{boxShadow(#none), marginTop(#px(8))})

  let noDataImage = style(list{width(#auto), height(#px(70)), marginBottom(#px(16))})
}

@react.component
let make = (~reports: array<RequestSub.report_t>) => {
  let isMobile = Media.isMobile()
  let ({ThemeContext.theme: theme, isDarkMode}, _) = React.useContext(ThemeContext.context)

  reports->Belt.Array.size > 0
    ? reports
      ->Belt.Array.mapWithIndex((
        idx,
        {reportValidator: {operatorAddress, moniker, identity}, transactionOpt, reportDetails},
      ) =>
        <div key={operatorAddress |> Address.toOperatorBech32} className=Styles.reportCard>
          {idx == 0 ? React.null : <SeperatedLine mt=32 mb=24 />}
          <Row marginBottom=24 alignItems=Row.Center>
            <Col col=Col.Four mbSm=8>
              <Heading
                value="Report by" size=Heading.H4 weight=Heading.Thin color=theme.textSecondary
              />
            </Col>
            <Col col=Col.Eight>
              <ValidatorMonikerLink
                validatorAddress=operatorAddress
                moniker
                identity
                width=#percent(100.)
                avatarWidth=20
              />
            </Col>
          </Row>
          <Row marginBottom=24 alignItems=Row.Center>
            <Col col=Col.Four mbSm=8>
              <Heading
                value="TX Hash" size=Heading.H4 weight=Heading.Thin color=theme.textSecondary
              />
            </Col>
            <Col col=Col.Eight>
              {switch transactionOpt {
              | Some({hash}) => <TxLink txHash=hash width=280 />
              | None => <Text value="Genesis Transaction" />
              }}
            </Col>
          </Row>
          <div className={Styles.reportsTable(theme)}>
            {isMobile
              ? React.null
              : <>
                  <Row alignItems=Row.Center marginBottom=16>
                    <Col col=Col.Three>
                      <Text value="External ID" weight=Text.Medium transform=Text.Uppercase />
                    </Col>
                    <Col col=Col.Three>
                      <Text value="Exit Code" weight=Text.Medium transform=Text.Uppercase />
                    </Col>
                    <Col col=Col.Six>
                      <Text value="Value" weight=Text.Medium transform=Text.Uppercase />
                    </Col>
                  </Row>
                  <SeperatedLine mt=10 mb=15 />
                </>}
            {reportDetails
            ->Belt.Array.map(({externalID, exitCode, data}) =>
              isMobile
                ? <MobileCard
                    values={
                      open InfoMobileCard
                      list{
                        ("External ID", Text(externalID)),
                        ("Exit Code", Text(exitCode)),
                        ("Value", Text(data |> JsBuffer.toUTF8)),
                      }
                    }
                    key={externalID ++ exitCode}
                    idx={externalID ++ exitCode}
                    styles=Styles.mobileCard
                  />
                : <Row alignItems=Row.Start marginBottom=16 key=externalID>
                    <Col col=Col.Three> <Text value=externalID weight=Text.Medium /> </Col>
                    <Col col=Col.Three> <Text value=exitCode weight=Text.Medium /> </Col>
                    <Col col=Col.Six>
                      <Text value={data |> JsBuffer.toUTF8} weight=Text.Medium breakAll=true />
                    </Col>
                  </Row>
            )
            ->React.array}
          </div>
        </div>
      )
      ->React.array
    : <EmptyContainer>
        <img src={isDarkMode ? Images.noTxDark : Images.noTxLight} className=Styles.noDataImage />
        <Heading
          size=Heading.H4
          value="No Report"
          align=Heading.Center
          weight=Heading.Regular
          color=theme.textSecondary
        />
      </EmptyContainer>
}
