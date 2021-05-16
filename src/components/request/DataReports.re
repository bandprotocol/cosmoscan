module Styles = {
  open Css;

  let reportCard = style([Media.mobile([padding2(~v=`px(24), ~h=`px(16))])]);

  let reportsTable = (theme: Theme.t) =>
    style([
      padding2(~v=`px(16), ~h=`px(24)),
      paddingBottom(`px(1)),
      marginTop(`px(24)),
      backgroundColor(theme.secondaryTableBg),
      transition(~duration=200, "all"),
      height(`auto),
      Media.mobile([padding(`zero), backgroundColor(Colors.white)]),
    ]);

  let mobileCard =
    style([backgroundColor(Colors.profileBG), boxShadow(`none), marginTop(`px(8))]);

  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
};

[@react.component]
let make = (~reports: array(RequestSub.report_t)) => {
  let isMobile = Media.isMobile();

  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  reports->Belt.Array.size > 0
    ? reports
      ->Belt.Array.mapWithIndex(
          (
            idx,
            {
              reportValidator: {operatorAddress, moniker, identity},
              transactionOpt,
              reportDetails,
            },
          ) => {
          <div key={operatorAddress |> Address.toOperatorBech32} className=Styles.reportCard>
            {idx == 0 ? React.null : <SeperatedLine mt=32 mb=24 />}
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Report by"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                <ValidatorMonikerLink
                  validatorAddress=operatorAddress
                  moniker
                  identity
                  width={`percent(100.)}
                  avatarWidth=20
                />
              </Col>
            </Row>
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="TX Hash"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                {switch (transactionOpt) {
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
                       <Col col=Col.Three> <Text value="EXTERNAL ID" weight=Text.Medium /> </Col>
                       <Col col=Col.Three> <Text value="EXIT CODE" weight=Text.Medium /> </Col>
                       <Col col=Col.Six> <Text value="VALUE" weight=Text.Medium /> </Col>
                     </Row>
                     <SeperatedLine mt=10 mb=15 />
                   </>}
              {reportDetails
               ->Belt.Array.map(({externalID, exitCode, data}) => {
                   isMobile
                     ? <MobileCard
                         values=InfoMobileCard.[
                           ("EXTERNAL ID", Text(externalID)),
                           ("EXIT CODE", Text(exitCode)),
                           ("VALUE", Text(data |> JsBuffer.toUTF8)),
                         ]
                         key={externalID ++ exitCode}
                         idx={externalID ++ exitCode}
                         styles=Styles.mobileCard
                       />
                     : <Row alignItems=Row.Start marginBottom=16 key=externalID>
                         <Col col=Col.Three> <Text value=externalID weight=Text.Medium /> </Col>
                         <Col col=Col.Three> <Text value=exitCode weight=Text.Medium /> </Col>
                         <Col col=Col.Six>
                           <Text
                             value={data |> JsBuffer.toUTF8}
                             weight=Text.Medium
                             breakAll=true
                           />
                         </Col>
                       </Row>
                 })
               ->React.array}
            </div>
          </div>
        })
      ->React.array
    : <EmptyContainer height={`px(250)} backgroundColor={theme.lightBlue}>
        <img src=Images.noSource className=Styles.noDataImage />
        <Heading
          size=Heading.H4
          value="No Report"
          align=Heading.Center
          weight=Heading.Regular
          color=Colors.bandBlue
        />
      </EmptyContainer>;
};
