module Styles = {
  open Css;
  let card = style([height(`percent(100.)), padding2(~v=`px(24), ~h=`px(32))]);

  let link = style([fontSize(`px(14))]);

  let bondedTokenContainer = style([height(`percent(100.))]);
  let avatarContainer =
    style([
      position(`relative),
      marginRight(`px(24)),
      Media.mobile([marginRight(`zero), marginBottom(`px(16))]),
    ]);
  let rankContainer =
    style([
      backgroundColor(Theme.baseBlue),
      borderRadius(`percent(50.)),
      position(`absolute),
      right(`zero),
      bottom(`zero),
      width(`px(26)),
      height(`px(26)),
    ]);

  // Oracle Status
  let oracleStatusBox = (isActive, theme: Theme.t) => {
    style([
      backgroundColor(isActive ? theme.successColor : theme.failColor),
      borderRadius(`px(50)),
      padding2(~v=`px(2), ~h=`px(10)),
    ]);
  };

  let customContainer = style([height(`percent(100.))])
};

module UptimePercentage = {
  [@react.component]
  let make = (~consensusAddress) => {
    let uptimeSub = ValidatorSub.getUptime(consensusAddress);
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
    <>
      {switch (uptimeSub) {
       | Data(uptime) =>
         switch (uptime) {
         | Some(uptime) =>
           <Text
             value={uptime |> Format.fPercent(~digits=2)}
             size=Text.Xxxl
             block=true
             color={theme.textPrimary}
             transform=Text.Uppercase
             weight=Text.Semibold
           />
         | None => <Text value="N/A" size=Text.Xxxl block=true />
         }
       | _ => <LoadingCensorBar width=100 height=24 />
       }}
    </>;
  };
};

[@react.component]
let make = (~address, ~hashtag: Route.validator_tab_t) => {
  let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
  let isMobile = Media.isMobile();
  let validatorSub = ValidatorSub.get(address);
  let bondedTokenCountSub = ValidatorSub.getTotalBondedAmount();
  let oracleReportsCountSub = ReportSub.ValidatorReport.count(address);
  // for finding validator rank
  let validatorsSub = ValidatorSub.getList(~isActive=true, ());

  let isMobile = Media.isMobile();

  let allSub = Sub.all3(validatorSub, validatorsSub, bondedTokenCountSub);

  <Section>
    <div className=CssHelper.container>
      <Heading value="Validator Details" size=Heading.H2 marginBottom=40 marginBottomSm=24 />
      <Row marginBottom=40 marginBottomSm=16 alignItems=Row.Center>
        <Col col=Col.Nine>
          <div
            className={Css.merge([
              CssHelper.flexBox(),
              CssHelper.flexBoxSm(~direction=`column, ()),
            ])}>
            <div className=Styles.avatarContainer>
              {switch (allSub) {
               | Data(({identity, moniker}, validators, _)) =>
                 let rankOpt =
                   validators
                   ->Belt.Array.keepMap(({moniker: m, rank}) =>
                       moniker === m ? Some(rank) : None
                     )
                   ->Belt.Array.get(0);
                 <>
                   <Avatar moniker identity width=100 widthSm=80 />
                   {switch (rankOpt) {
                    | Some(rank) =>
                      <div
                        className={Css.merge([
                          Styles.rankContainer,
                          CssHelper.flexBox(~justify=`center, ()),
                        ])}>
                        <Text value={rank |> string_of_int} color={theme.white} />
                      </div>
                    | None => React.null
                    }}
                 </>;
               | _ => <LoadingCensorBar width=100 height=100 radius=100 />
               }}
            </div>
            {switch (allSub) {
             | Data(({moniker}, _, _)) =>
               <Heading
                 size=Heading.H3
                 value=moniker
                 marginBottomSm=8
                 align={isMobile ? Heading.Center : Heading.Left}
               />
             | _ => <LoadingCensorBar width=270 height=20 />
             }}
          </div>
        </Col>
        <Col col=Col.Three>
          <div
            className={Css.merge([
              CssHelper.flexBox(~justify=`flexEnd, ()),
              CssHelper.flexBoxSm(~justify=`center, ()),
            ])}>
            {switch (allSub) {
             | Data(({isActive}, _, _)) =>
               <div className={CssHelper.flexBox()}>
                 <div className={CssHelper.flexBox(~justify=`center, ())}>
                   <img
                     src={isActive ? Images.activeValidatorLogo : Images.inactiveValidatorLogo}
                   />
                 </div>
                 <HSpacing size=Spacing.sm />
                 <Text value={isActive ? "Active" : "Inactive"} color={theme.textSecondary} />
               </div>
             | _ => <LoadingCensorBar width=60 height=20 />
             }}
            <HSpacing size=Spacing.md />
            {switch (allSub) {
             | Data(({oracleStatus}, _, _)) =>
               <div
                 className={Css.merge([
                   CssHelper.flexBox(~justify=`center, ()),
                   Styles.oracleStatusBox(oracleStatus, theme),
                 ])}>
                 <Text value="Oracle" color={theme.white} />
                 <HSpacing size=Spacing.sm />
                 <Icon
                   name={oracleStatus ? "fas fa-check" : "fal fa-times"}
                   color={theme.white}
                   size=10
                 />
               </div>
             | _ => <LoadingCensorBar width=75 height=20 />
             }}
          </div>
        </Col>
      </Row>
      // Validator Highlight Info
      <Row marginBottom=24 marginBottomSm=16>
        <Col col=Col.Three colSm=Col.Six mbSm=16>
          <InfoContainer style=Styles.card>
            <Heading
              value="Voting power"
              size=Heading.H4
              marginBottom=27
              color={theme.textSecondary}
              weight=Heading.Thin
            />
            <div className={CssHelper.mb(~size=4, ())}>
              {switch (allSub) {
               | Data(({votingPower}, _, {amount})) =>
                 <Text
                   value={votingPower *. 100. /. amount |> Format.fPercent(~digits=2)}
                   size=Text.Xxxl
                   align=Text.Left
                   block=true
                   color={theme.textPrimary}
                   transform=Text.Uppercase
                   weight=Text.Semibold
                 />
               | _ => <LoadingCensorBar width=100 height=24 />
               }}
            </div>
            {switch (allSub) {
             | Data(({votingPower}, _, _)) =>
               <>
                 <Text
                   value={(votingPower /. 1e6 |> Format.fPretty(~digits=0)) ++ " Band"}
                   size=Text.Lg
                   block=true
                 />
               </>
             | _ => <LoadingCensorBar width=80 height=14 />
             }}
          </InfoContainer>
        </Col>
        <Col col=Col.Three colSm=Col.Six mbSm=16>
          <InfoContainer style=Styles.card>
            <Heading
              value="Commission"
              size=Heading.H4
              marginBottom=27
              color={theme.textSecondary}
              weight=Heading.Thin
            />
            {switch (allSub) {
             | Data(({commission}, _, _)) =>
               <Text
                 value={commission |> Format.fPercent(~digits=2)}
                 size=Text.Xxxl
                 align=Text.Left
                 block=true
                 color={theme.textPrimary}
                 transform=Text.Uppercase
                 weight=Text.Semibold
               />
             | _ => <LoadingCensorBar width=100 height=24 />
             }}
          </InfoContainer>
        </Col>
        <Col col=Col.Three colSm=Col.Six mbSm=16>
          <InfoContainer style=Styles.card>
            <Heading
              value="Uptime"
              size=Heading.H4
              marginBottom=27
              color={theme.textSecondary}
              weight=Heading.Thin
            />
            {switch (allSub) {
             | Data(({consensusAddress}, _, _)) => <UptimePercentage consensusAddress />
             | _ => <LoadingCensorBar width=100 height=24 />
             }}
          </InfoContainer>
        </Col>
        <Col col=Col.Three colSm=Col.Six mbSm=16>
          <InfoContainer style=Styles.card>
            <div className={Css.merge([CssHelper.flexBox(), CssHelper.mb(~size=27, ())])}>
              <Heading
                value="Oracle Reports"
                size=Heading.H4
                color={theme.textSecondary}
                weight=Heading.Thin
              />
              <HSpacing size=Spacing.xs />
              <CTooltip
                tooltipPlacementSm=CTooltip.BottomRight
                tooltipText="The number of reports this validator has submitted to date">
                <Icon name="fal fa-info-circle" size=12 color={theme.textSecondary} />
              </CTooltip>
            </div>
            {switch (oracleReportsCountSub) {
             | Data(oracleReportsCount) =>
               <Text
                 value={oracleReportsCount |> Format.iPretty}
                 size=Text.Xxxl
                 block=true
                 color={theme.textPrimary}
                 transform=Text.Uppercase
                 weight=Text.Semibold
               />
             | _ => <LoadingCensorBar width=100 height=24 />
             }}
          </InfoContainer>
        </Col>
      </Row>
      // Validator Information
      <Row marginBottom=24>
        <Col>
          <InfoContainer>
            <Heading value="Information" size=Heading.H4 />
            <SeperatedLine mt=32 mb=24 />
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <div className={CssHelper.flexBox()}>
                  <Heading
                    value="Operator Address"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                  <HSpacing size={`px(4)} />
                  <CTooltip tooltipText="The address used to show the validator's entity status">
                    <Icon name="fal fa-info-circle" size=10 color={theme.textSecondary} />
                  </CTooltip>
                </div>
              </Col>
              <Col col=Col.Eight>
                {switch (allSub) {
                 | Data(({operatorAddress}, _, _)) =>
                   <AddressRender
                     address=operatorAddress
                     position=AddressRender.Subtitle
                     accountType=`validator
                     clickable=false
                     wordBreak=true
                   />
                 | _ => <LoadingCensorBar width=260 height=15 />
                 }}
              </Col>
            </Row>
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <div className={CssHelper.flexBox()}>
                  <Heading
                    value="Address"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                  <HSpacing size=Spacing.xs />
                  <CTooltip tooltipText="The validator entity's unique address">
                    <Icon name="fal fa-info-circle" size=10 />
                  </CTooltip>
                </div>
              </Col>
              <Col col=Col.Eight>
                {switch (allSub) {
                 | Data(({operatorAddress}, _, _)) =>
                   <AddressRender address=operatorAddress position=AddressRender.Subtitle />
                 | _ => <LoadingCensorBar width=260 height=15 />
                 }}
              </Col>
            </Row>
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <div className={CssHelper.flexBox()}>
                  <Heading
                    value="Commission Max Change"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                  <HSpacing size=Spacing.xs />
                  <CTooltip
                    tooltipText="The maximum increment by which the validator can increase their commission rate">
                    <Icon name="fal fa-info-circle" size=10 color={theme.textSecondary} />
                  </CTooltip>
                </div>
              </Col>
              <Col col=Col.Eight>
                {switch (allSub) {
                 | Data(({commissionMaxChange}, _, _)) =>
                   <Text value={commissionMaxChange |> Format.fPercent(~digits=2)} size=Text.Lg />
                 | _ => <LoadingCensorBar width=260 height=15 />
                 }}
              </Col>
            </Row>
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <div className={CssHelper.flexBox()}>
                  <Heading
                    value="Commission Max Rate"
                    size=Heading.H4
                    weight=Heading.Thin
                    color={theme.textSecondary}
                  />
                  <HSpacing size=Spacing.xs />
                  <CTooltip
                    tooltipText="Highest possible commission rate that the validator can set">
                    <Icon name="fal fa-info-circle" size=10 color={theme.textSecondary} />
                  </CTooltip>
                </div>
              </Col>
              <Col col=Col.Eight>
                {switch (allSub) {
                 | Data(({commissionMaxRate}, _, _)) =>
                   <Text value={commissionMaxRate |> Format.fPercent(~digits=2)} size=Text.Lg />
                 | _ => <LoadingCensorBar width=260 height=15 />
                 }}
              </Col>
            </Row>
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Website"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                {switch (allSub) {
                 | Data(({website}, _, _)) =>
                   <AbsoluteLink href=website className=Styles.link>
                     {website |> React.string}
                   </AbsoluteLink>
                 | _ => <LoadingCensorBar width=260 height=15 />
                 }}
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
                {switch (allSub) {
                 | Data(({details}, _, _)) => <p> <Text value=details size=Text.Lg /> </p>
                 | _ => <LoadingCensorBar width=260 height=15 />
                 }}
              </Col>
            </Row>
          </InfoContainer>
        </Col>
      </Row>
      // Bondded token & staking section
      <Row marginBottom=24>
        <Col col=Col.Four>
          <InfoContainer style=Styles.bondedTokenContainer>
            <div className={Css.merge([CssHelper.flexBox()])}>
              <Heading value="Bonded Token" size=Heading.H4 />
              <HSpacing size={`px(4)} />
              <CTooltip tooltipText="Total amount of tokens current staked across all validators">
                <Icon name="fal fa-info-circle" size=10 />
              </CTooltip>
            </div>
            <SeperatedLine mt=32 mb=24 />
            <div className={CssHelper.flexBox()}>
              {switch (allSub) {
               | Data(({operatorAddress}, _, _)) => <HistoricalBondedGraph operatorAddress />
               | _ => <LoadingCensorBar.CircleSpin height=180 />
               }}
            </div>
          </InfoContainer>
        </Col>
        {isMobile
           ? React.null
           : <Col col=Col.Eight> <ValidatorStakingInfo validatorAddress=address /> </Col>}
      </Row>
      <Row marginBottom=24>
        <Col col=Col.Six mbSm=24>
          <InfoContainer style=Styles.customContainer>
            <div className={CssHelper.flexBox()}>
              <Heading value="Block Uptime" size=Heading.H4 />
              <HSpacing size=Spacing.xs />
              <CTooltip tooltipText="The validator's non-oracle performance">
                <Icon name="fal fa-info-circle" size=10 />
              </CTooltip>
            </div>
            <SeperatedLine mt=32 mb=32 />
            {switch (allSub) {
             | Data(({consensusAddress}, _, _)) => <BlockUptimeChart consensusAddress />
             | _ => <LoadingCensorBar.CircleSpin height=90 />
             }}
          </InfoContainer>
        </Col>
        <Col col=Col.Six>
          <InfoContainer style=Styles.customContainer>
            <div className={CssHelper.flexBox()}>
              <Heading value="Oracle Data Report" size=Heading.H4 />
              <HSpacing size=Spacing.xs />
              <CTooltip tooltipText="Last 90 days of Report" align=`center>
                <Icon name="fal fa-info-circle" size=10 />
              </CTooltip>
            </div>
            <SeperatedLine mt=32 mb=24 />
            {switch (allSub) {
             | Data(({oracleStatus}, _, _)) =>
               <OracleDataReportChart oracleStatus operatorAddress=address />
             | _ => <LoadingCensorBar.CircleSpin height=90 />
             }}
          </InfoContainer>
        </Col>
      </Row>
      <Table>
        <Tab
          tabs=[|
            {name: "Oracle Reports", route: Route.ValidatorIndexPage(address, Route.Reports)},
            {name: "Delegators", route: Route.ValidatorIndexPage(address, Route.Delegators)},
            {
              name: "Proposed Blocks",
              route: Route.ValidatorIndexPage(address, Route.ProposedBlocks),
            },
            {name: "Reporters", route: Route.ValidatorIndexPage(address, Route.Reporters)},
          |]
          currentRoute={Route.ValidatorIndexPage(address, hashtag)}>
          {switch (hashtag) {
           | Reports => <ReportsTable address />
           | Delegators => <DelegatorsTable address />
           | ProposedBlocks =>
             switch (validatorSub) {
             | Data(validator) =>
               <ProposedBlocksTable consensusAddress={validator.consensusAddress} />
             | _ => <ProposedBlocksTable.LoadingWithHeader />
             }
           | Reporters => <ReportersTable address />
           }}
        </Tab>
      </Table>
    </div>
  </Section>;
};
