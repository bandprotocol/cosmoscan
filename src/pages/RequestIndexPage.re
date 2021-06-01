module Styles = {
  open Css;

  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
  let kvTableContainer = style([padding(`px(1))]);
  let kvTableHeader =
    style([
      padding4(~top=`px(16), ~left=`px(24), ~right=`px(24), ~bottom=`px(12)),
      Media.mobile([padding2(~v=`px(14), ~h=`px(12))]),
    ]);

  let kvTableMobile = style([margin(`zero), boxShadow(`none)]);
  let addressContainer = style([Media.mobile([width(`px(260))])]);
  let validatorReportStatus = style([marginBottom(`px(13))]);
  let noPaddingBottom = style([paddingBottom(`zero), Media.mobile([paddingBottom(`zero)])]);
};

module ValidatorReportStatus = {
  [@react.component]
  let make = (~moniker, ~isReport, ~resolveStatus) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <div
      className={Css.merge([
        CssHelper.flexBox(~align=`center, ~wrap=`nowrap, ()),
        Styles.validatorReportStatus,
      ])}>
      {switch (isReport, resolveStatus) {
       | (true, _) => <Icon name="fas fa-check-circle" color={theme.successColor} />
       | (false, _) => <Icon name="fas fa-times-circle" color={theme.failColor} />
       }}
      <HSpacing size=Spacing.sm />
      <Text value=moniker color={theme.textSecondary} ellipsis=true />
    </div>;
  };
};

module KVTableContainer = {
  module TableHeader = {
    [@react.component]
    let make = () => {
      let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
      <THead>
        <Row alignItems=Row.Center>
          <Col col=Col.Three>
            <Heading
              value="KEY"
              size=Heading.H5
              weight=Heading.Medium
              color={theme.textSecondary}
            />
          </Col>
          <Col col=Col.Nine>
            <Heading
              value="VALUE"
              size=Heading.H5
              weight=Heading.Medium
              color={theme.textSecondary}
            />
          </Col>
        </Row>
      </THead>;
    };
  };

  module Loading = {
    [@react.component]
    let make = () => {
      let isMobile = Media.isMobile();

      isMobile
        ? <MobileCard
            values=InfoMobileCard.[("Key", Loading(60)), ("Value", Loading(60))]
            idx="1"
            styles=Styles.kvTableMobile
          />
        : <>
            <TableHeader />
            <TBody>
              <Row alignItems=Row.Center minHeight={`px(30)}>
                <Col col=Col.Three> <LoadingCensorBar width=60 height=15 /> </Col>
                <Col col=Col.Nine> <LoadingCensorBar width=100 height=15 /> </Col>
              </Row>
            </TBody>
            <TBody>
              <Row alignItems=Row.Center minHeight={`px(30)}>
                <Col col=Col.Three> <LoadingCensorBar width=60 height=15 /> </Col>
                <Col col=Col.Nine> <LoadingCensorBar width=100 height=15 /> </Col>
              </Row>
            </TBody>
          </>;
    };
  };

  [@react.component]
  let make = (~decodesOpt) => {
    let isMobile = Media.isMobile();
    let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

    switch (decodesOpt) {
    | Some(decodes) =>
      isMobile
        ? decodes
          ->Belt.Array.map(({Obi.fieldName, fieldValue}) =>
              <MobileCard
                values=InfoMobileCard.[("Key", Text(fieldName)), ("Value", Text(fieldValue))]
                key={fieldName ++ fieldValue}
                idx={fieldName ++ fieldValue}
                styles=Styles.kvTableMobile
              />
            )
          ->React.array
        : <>
            <TableHeader />
            {decodes
             ->Belt.Array.map(({Obi.fieldName, fieldValue}) => {
                 <TBody key={fieldName ++ fieldValue}>
                   <Row alignItems=Row.Center minHeight={`px(30)}>
                     <Col col=Col.Three>
                       <Text value=fieldName color={theme.textSecondary} weight=Text.Thin />
                     </Col>
                     <Col col=Col.Nine>
                       <Text
                         value=fieldValue
                         color={theme.textSecondary}
                         weight=Text.Thin
                         breakAll=true
                       />
                     </Col>
                   </Row>
                 </TBody>
               })
             ->React.array}
          </>
    | None =>
      <EmptyContainer height={`px(200)}>
        <img
          src={isDarkMode ? Images.noDataDark : Images.noDataLight}
          className=Styles.noDataImage
        />
        <Heading
          size=Heading.H4
          value="Schema not found"
          align=Heading.Center
          weight=Heading.Regular
          color={theme.textSecondary}
        />
      </EmptyContainer>
    };
  };
};

[@react.component]
let make = (~reqID) => {
  let requestSub = RequestSub.get(reqID);
  let isMobile = Media.isMobile();

  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  <Section>
    <div className=CssHelper.container>
      <Row marginBottom=40 marginBottomSm=16>
        <Col>
          <Heading value="Request Details" size=Heading.H2 marginBottom=40 marginBottomSm=24 />
          {switch (requestSub) {
           | Data({id}) => <TypeID.Request id position=TypeID.Title />
           | _ => <LoadingCensorBar width=150 height=23 />
           }}
        </Col>
      </Row>
      <Row marginBottom=24>
        <Col>
          <InfoContainer>
            <Heading value="Request Info" size=Heading.H4 />
            <SeperatedLine mt=32 mb=24 />
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Oracle Scripts"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                {switch (requestSub) {
                 | Data({oracleScript: {oracleScriptID, name}}) =>
                   <div className={CssHelper.flexBox()}>
                     <TypeID.OracleScript id=oracleScriptID position=TypeID.Subtitle />
                     <HSpacing size=Spacing.sm />
                     <Text value=name size=Text.Lg />
                   </div>
                 | _ => <LoadingCensorBar width=200 height=15 />
                 }}
              </Col>
            </Row>
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Sender"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                {switch (requestSub) {
                 | Data({requester}) =>
                   switch (requester) {
                   | Some(requester') =>
                     <div className=Styles.addressContainer>
                       <AddressRender address=requester' position=AddressRender.Subtitle />
                     </div>
                   | None => <Text value="Syncing" />
                   }
                 | _ => <LoadingCensorBar width=200 height=15 />
                 }}
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
                {switch (requestSub) {
                 | Data({transactionOpt}) =>
                   switch (transactionOpt) {
                   | Some({hash}) => <TxLink txHash=hash width={isMobile ? 260 : 360} />
                   | None => <Text value="Syncing" />
                   }
                 | _ => <LoadingCensorBar width=200 height=15 />
                 }}
              </Col>
            </Row>
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Fee"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                {switch (requestSub) {
                 | Data({transactionOpt}) =>
                   switch (transactionOpt) {
                   | Some({gasFee}) =>
                     <Text
                       block=true
                       value={
                         (gasFee |> Coin.getBandAmountFromCoins |> Format.fPretty(~digits=2))
                         ++ " BAND"
                       }
                       size=Text.Lg
                       color={theme.textSecondary}
                     />
                   | None => <Text value="Syncing" />
                   }
                 | _ => <LoadingCensorBar width=200 height=15 />
                 }}
              </Col>
            </Row>
            <SeperatedLine mt=32 mb=24 />
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Request Time"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Four>
                {switch (requestSub) {
                 | Data({requestTime}) =>
                   switch (requestTime) {
                   | Some(requestTime') =>
                     <div className={CssHelper.flexBox()}>
                       <Text
                         value={
                           requestTime'
                           |> MomentRe.Moment.format(Config.timestampDisplayFormat)
                           |> String.uppercase_ascii
                         }
                         size=Text.Lg
                       />
                       <HSpacing size=Spacing.sm />
                       <TimeAgos
                         time=requestTime'
                         prefix="("
                         suffix=")"
                         size=Text.Md
                         weight=Text.Thin
                       />
                     </div>
                   | None => <Text value="TBD" size=Text.Lg />
                   }
                 | _ => <LoadingCensorBar width=200 height=15 />
                 }}
              </Col>
            </Row>
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Resolve Time"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Four>
                {switch (requestSub) {
                 | Data({resolveTime}) =>
                   switch (resolveTime) {
                   | Some(resolveTime') =>
                     <div className={CssHelper.flexBox()}>
                       <Text
                         value={
                           resolveTime'
                           |> MomentRe.Moment.format(Config.timestampDisplayFormat)
                           |> String.uppercase_ascii
                         }
                         size=Text.Lg
                       />
                       <HSpacing size=Spacing.sm />
                       <TimeAgos time=resolveTime' prefix="(" suffix=")" size=Text.Md weight=Text.Thin />
                     </div>
                   | None => <Text value="TBD" size=Text.Lg />
                   }
                 | _ => <LoadingCensorBar width=200 height=15 />
                 }}
              </Col>
            </Row>
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Report Status"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Four>
                {switch (requestSub) {
                 | Data({minCount, requestedValidators, reports}) =>
                   <ProgressBar
                     reportedValidators={reports->Belt.Array.size}
                     minimumValidators=minCount
                     requestValidators={requestedValidators->Belt.Array.size}
                   />
                 | _ => <LoadingCensorBar width=200 height=15 />
                 }}
              </Col>
            </Row>
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Resolve Status"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                {switch (requestSub) {
                 | Data({resolveStatus}) =>
                   <RequestStatus resolveStatus display=RequestStatus.Full />
                 | _ => <LoadingCensorBar width=200 height=15 />
                 }}
              </Col>
            </Row>
            <Row alignItems=Row.Start>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Request to"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                <Row wrap=true>
                  {switch (requestSub) {
                   | Data({requestedValidators, resolveStatus, reports}) =>
                     requestedValidators
                     ->Belt.Array.map(({validator: {moniker, consensusAddress}}) => {
                         let isReport =
                           reports->Belt.Array.some(({reportValidator}) =>
                             consensusAddress == reportValidator.consensusAddress
                           );
                         <Col col=Col.Three colSm=Col.Six key=moniker>
                           <ValidatorReportStatus moniker isReport resolveStatus />
                         </Col>;
                       })
                     ->React.array
                   | _ =>
                     <Col>
                       <LoadingCensorBar width=200 height=15 />
                       <VSpacing size={`px(isMobile ? 5 : 11)} />
                     </Col>
                   }}
                </Row>
              </Col>
            </Row>
          </InfoContainer>
        </Col>
      </Row>
      // Calldata
      <Row marginBottom=24>
        <Col>
          <InfoContainer style=Styles.noPaddingBottom>
            <div className={Css.merge([CssHelper.flexBox(~justify=`spaceBetween, ())])}>
              <Heading value="Calldata" size=Heading.H4 />
              {switch (requestSub) {
               | Data({calldata}) =>
                 <CopyButton
                   data={calldata |> JsBuffer.toHex(~with0x=false)}
                   title="Copy as bytes"
                   width=125
                 />
               | _ => <LoadingCensorBar width=125 height=28 />
               }}
            </div>
            <SeperatedLine mt=32 />
            <div className=Styles.kvTableContainer>
              {switch (requestSub) {
               | Data({oracleScript: {schema}, calldata}) =>
                 let decodesOpt = Obi.decode(schema, "input", calldata);
                 <KVTableContainer decodesOpt />;
               | _ => <KVTableContainer.Loading />
               }}
            </div>
          </InfoContainer>
        </Col>
      </Row>
      // Result
      <Row marginBottom=24>
        <Col>
          <InfoContainer style=Styles.noPaddingBottom>
            <div className={Css.merge([CssHelper.flexBox(~justify=`spaceBetween, ())])}>
              <Heading value="Result" size=Heading.H4 />
              {switch (requestSub) {
               | Data({result: resultOpt, resolveStatus}) =>
                 switch (resultOpt, resolveStatus) {
                 | (Some(result), Success) =>
                   <CopyButton
                     data={result |> JsBuffer.toHex(~with0x=false)}
                     title="Copy as bytes"
                     width=125
                   />
                 | (_, _) => React.null
                 }
               | _ => <LoadingCensorBar width=125 height=28 />
               }}
            </div>
            <SeperatedLine mt=32 />
            {switch (requestSub) {
             | Data({oracleScript: {schema}, result: resultOpt, resolveStatus}) =>
               switch (resolveStatus, resultOpt) {
               | (RequestSub.Success, Some(result)) =>
                 let decodesOpt = Obi.decode(schema, "output", result);
                 <KVTableContainer decodesOpt />;
               | (Pending, _) =>
                 <EmptyContainer height={`px(200)}>
                   <LoadingCensorBar.CircleSpin size=50 height=70 />
                   <Heading
                     size=Heading.H4
                     value="Waiting for result"
                     align=Heading.Center
                     weight=Heading.Regular
                     color={theme.textSecondary}
                   />
                 </EmptyContainer>
               | (_, _) =>
                 <EmptyContainer height={`px(200)}>
                   <img
                     src={isDarkMode ? Images.noDataDark : Images.noDataLight}
                     className=Styles.noDataImage
                   />
                   <Heading
                     size=Heading.H4
                     value="This request hasn't resolved"
                     align=Heading.Center
                     weight=Heading.Regular
                     color={theme.textSecondary}
                   />
                 </EmptyContainer>
               }
             | _ => <KVTableContainer.Loading />
             }}
          </InfoContainer>
        </Col>
      </Row>
      // Proof
      <Row marginBottom=24>
        <Col>
          <InfoContainer>
            <div className={Css.merge([CssHelper.flexBox(~justify=`spaceBetween, ())])}>
              <Heading value="Proof of validity" size=Heading.H4 />
              <AbsoluteLink href="https://docs.bandchain.org/">
                <div className={CssHelper.flexBox()}>
                  <Heading value="What is proof ?" size=Heading.H5 />
                  <HSpacing size=Spacing.sm />
                  <Icon name="far fa-external-link-alt" color={theme.textPrimary} />
                </div>
              </AbsoluteLink>
            </div>
            <SeperatedLine mt=32 />
            {switch (requestSub) {
             | Data(request) =>
               switch (request.resolveStatus) {
               | Success => <RequestProof request />
               | Pending =>
                 <EmptyContainer height={`px(200)}>
                   <LoadingCensorBar.CircleSpin size=50 height=70 />
                   <Heading
                     size=Heading.H4
                     value="Waiting for result"
                     align=Heading.Center
                     weight=Heading.Regular
                     color={theme.textSecondary}
                   />
                 </EmptyContainer>
               | _ =>
                 <EmptyContainer height={`px(200)}>
                   <img
                     src={isDarkMode ? Images.noDataDark : Images.noDataLight}
                     className=Styles.noDataImage
                   />
                   <Heading
                     size=Heading.H4
                     value="This request hasn't resolved"
                     align=Heading.Center
                     weight=Heading.Regular
                     color={theme.textSecondary}
                   />
                 </EmptyContainer>
               }
             | _ => <LoadingCensorBar fullWidth=true height=100 />
             }}
          </InfoContainer>
        </Col>
      </Row>
      // External Data Table
      <Row marginBottom=24>
        <Col>
          <InfoContainer style=Styles.noPaddingBottom>
            <Heading value="External Data" size=Heading.H4 />
            <SeperatedLine mt=32 />
            <div className=Styles.kvTableContainer>
              {isMobile
                 ? React.null
                 : <THead>
                     <Row alignItems=Row.Center>
                       <Col col=Col.Three>
                         <Heading
                           value="EXTERNAL ID"
                           size=Heading.H5
                           weight=Heading.Regular
                           color={theme.textSecondary}
                         />
                       </Col>
                       <Col col=Col.Four>
                         <Heading
                           value="DATA SOURCE"
                           size=Heading.H5
                           weight=Heading.Regular
                           color={theme.textSecondary}
                         />
                       </Col>
                       <Col col=Col.Five>
                         <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
                           <Heading
                             value="PARAM"
                             size=Heading.H5
                             weight=Heading.Regular
                             color={theme.textSecondary}
                           />
                         </div>
                       </Col>
                     </Row>
                   </THead>}
              {switch (requestSub) {
               | Data({rawDataRequests}) =>
                 rawDataRequests
                 ->Belt.Array.map(({externalID, dataSource: {dataSourceID, name}, calldata}) => {
                     isMobile
                       ? <MobileCard
                           values=InfoMobileCard.[
                             ("External ID", Text(externalID)),
                             ("Data Source", DataSource(dataSourceID, name)),
                             ("Param", Text(calldata |> JsBuffer.toUTF8)),
                           ]
                           key={externalID ++ name}
                           idx={externalID ++ name}
                           styles=Styles.kvTableMobile
                         />
                       : <TBody key=externalID>
                           <Row alignItems=Row.Center minHeight={`px(30)}>
                             <Col col=Col.Three>
                               <Text
                                 value=externalID
                                 color={theme.textSecondary}
                                 weight=Text.Thin
                               />
                             </Col>
                             <Col col=Col.Four>
                               <div className={CssHelper.flexBox()}>
                                 <TypeID.DataSource id=dataSourceID position=TypeID.Text />
                                 <HSpacing size=Spacing.sm />
                                 <Text value=name color={theme.textSecondary} weight=Text.Thin />
                               </div>
                             </Col>
                             <Col col=Col.Five>
                               <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
                                 <Text
                                   value={calldata->JsBuffer.toUTF8}
                                   color={theme.textSecondary}
                                   weight=Text.Thin
                                   align=Text.Right
                                 />
                               </div>
                             </Col>
                           </Row>
                         </TBody>
                   })
                 ->React.array
               | _ =>
                 isMobile
                   ? <MobileCard
                       values=InfoMobileCard.[
                         ("External ID", Loading(60)),
                         ("Data Source", Loading(60)),
                         ("Param", Loading(60)),
                       ]
                       idx="1"
                       styles=Styles.kvTableMobile
                     />
                   : <TBody>
                       <Row alignItems=Row.Center minHeight={`px(30)}>
                         <Col col=Col.Three> <LoadingCensorBar width=60 height=15 /> </Col>
                         <Col col=Col.Four> <LoadingCensorBar width=100 height=15 /> </Col>
                         <Col col=Col.Five>
                           <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
                             <LoadingCensorBar width=50 height=15 />
                           </div>
                         </Col>
                       </Row>
                     </TBody>
               }}
            </div>
          </InfoContainer>
        </Col>
      </Row>
      // Data report
      <Row marginBottom=24>
        <Col>
          <InfoContainer>
            <Heading value="Data Report" size=Heading.H4 />
            <SeperatedLine mt=32 />
            <div className=Styles.kvTableContainer>
              {switch (requestSub) {
               | Data({reports}) => <DataReports reports />
               | _ => <LoadingCensorBar.CircleSpin height=200 />
               }}
            </div>
          </InfoContainer>
        </Col>
      </Row>
    </div>
  </Section>;
};
