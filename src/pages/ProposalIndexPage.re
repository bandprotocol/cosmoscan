module Styles = {
  open Css;

  let statusLogo = style([width(`px(20))]);
  let resultContainer = style([selector("> div + div", [marginTop(`px(24))])]);

  let voteButton =
    fun
    | ProposalSub.Voting => style([visibility(`visible)])
    | Deposit
    | Passed
    | Rejected
    | Failed => style([visibility(`hidden)]);

  let chartContainer = style([paddingRight(`px(20)), Media.mobile([paddingRight(`zero)])]);

  let parameterChanges = (theme: Theme.t) =>
    style([padding2(~v=`px(16), ~h=`px(24)), backgroundColor(theme.secondaryTableBg)]);

  let jsonDisplay = (theme: Theme.t) =>
    style([
      resize(`none),
      fontSize(`px(12)),
      color(theme.textPrimary),
      backgroundColor(theme.contrastBg),
      border(`px(1), `solid, theme.tableRowBorderColor),
      borderRadius(`px(4)),
      width(`percent(100.)),
      minHeight(`px(100)),
      overflowY(`scroll),
      marginBottom(`px(16)),
      fontFamilies([
        `custom("IBM Plex Mono"),
        `custom("cousine"),
        `custom("sfmono-regular"),
        `custom("Consolas"),
        `custom("Menlo"),
        `custom("liberation mono"),
        `custom("ubuntu mono"),
        `custom("Courier"),
        `monospace,
      ]),
    ]);
};

module VoteButton = {
  [@react.component]
  let make = (~proposalID, ~proposalName) => {
    let trackingSub = TrackingSub.use();

    let (accountOpt, _) = React.useContext(AccountContext.context);
    let (_, dispatchModal) = React.useContext(ModalContext.context);

    let connect = chainID => dispatchModal(OpenModal(Connect(chainID)));
    let vote = () => Vote(proposalID, proposalName)->SubmitTx->OpenModal->dispatchModal;

    switch (accountOpt) {
    | Some(_) =>
      <Button px=40 py=10 fsize=14 style={CssHelper.flexBox()} onClick={_ => vote()}>
        {"Vote" |> React.string}
      </Button>
    | None =>
      switch (trackingSub) {
      | Data({chainID}) =>
        <Button px=40 py=10 fsize=14 style={CssHelper.flexBox()} onClick={_ => connect(chainID)}>
          {"Vote" |> React.string}
        </Button>
      | Error(err) =>
        // log for err details
        Js.Console.log(err);
        <Text value="chain id not found" />;
      | _ => <LoadingCensorBar width=90 height=26 />
      }
    };
  };
};

[@react.component]
let make = (~proposalID) => {
  let isMobile = Media.isMobile();
  let proposalSub = ProposalSub.get(proposalID);
  let voteStatByProposalIDSub = VoteSub.getVoteStatByProposalID(proposalID);
  let bondedTokenCountSub = ValidatorSub.getTotalBondedAmount();

  let allSub = Sub.all3(proposalSub, voteStatByProposalIDSub, bondedTokenCountSub);

  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <Section>
    <div className=CssHelper.container>
      <Row>
        <Col>
          <Heading value="Proposal Details" size=Heading.H2 marginBottom=40 marginBottomSm=24 />
        </Col>
      </Row>
      <Row alignItems=Row.Center marginBottom=40 marginBottomSm=16>
        {switch (allSub) {
         | Data(({id, name, status}, _, _)) =>
           <>
             <Col col=Col.Eight mbSm=16>
               <div
                 className={Css.merge([
                   CssHelper.flexBox(),
                   CssHelper.flexBoxSm(~direction=`column, ~align=`flexStart, ()),
                 ])}>
                 <div className={CssHelper.flexBox()}>
                   <TypeID.Proposal id position=TypeID.Title />
                   <HSpacing size=Spacing.sm />
                   <Heading size=Heading.H3 value=name />
                   <HSpacing size={`px(16)} />
                 </div>
                 <div className={CssHelper.mtSm(~size=16, ())}> <ProposalBadge status /> </div>
               </div>
             </Col>
             <Col col=Col.Four>
               {isMobile
                  ? React.null
                  : <div
                      className={Css.merge([
                        CssHelper.flexBox(~direction=`column, ~align=`flexEnd, ()),
                      ])}>
                      <div className={Styles.voteButton(status)}>
                        <VoteButton proposalID proposalName=name />
                      </div>
                    </div>}
             </Col>
           </>
         | _ =>
           <Col col=Col.Eight mbSm=16>
             <div className={CssHelper.flexBox()}>
               <LoadingCensorBar width=270 height=15 />
               <HSpacing size={`px(16)} />
               <div className={CssHelper.mtSm(~size=16, ())}>
                 <LoadingCensorBar width=100 height=15 radius=50 />
               </div>
             </div>
           </Col>
         }}
      </Row>
      <Row marginBottom=24>
        <Col>
          <InfoContainer>
            <Heading value="Information" size=Heading.H4 />
            <SeperatedLine mt=32 mb=24 />
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Proposer"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                {switch (allSub) {
                 | Data(({proposerAddressOpt}, _, _)) =>
                   switch (proposerAddressOpt) {
                   | Some(proposerAddress) =>
                     <AddressRender address=proposerAddress position=AddressRender.Subtitle />
                   | None => <Text value="Proposed on Wenchang" />
                   }
                 | _ => <LoadingCensorBar width=270 height=15 />
                 }}
              </Col>
            </Row>
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Submit Time"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                {switch (allSub) {
                 | Data(({submitTime}, _, _)) => <Timestamp size=Text.Lg time=submitTime />
                 | _ => <LoadingCensorBar width={isMobile ? 120 : 270} height=15 />
                 }}
              </Col>
            </Row>
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Proposal Type"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                {switch (allSub) {
                 | Data(({proposalType}, _, _)) =>
                   <Text value=proposalType size=Text.Lg block=true />
                 | _ => <LoadingCensorBar width=90 height=15 />
                 }}
              </Col>
            </Row>
            <Row marginBottom=24>
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
                 | Data(({description}, _, _)) => <Markdown value=description />
                 | _ => <LoadingCensorBar width=270 height=15 />
                 }}
              </Col>
            </Row>
            {switch (allSub) {
             | Data(({content}, _, _)) =>
               switch (content) {
               | Some({changes}) =>
                 switch (changes) {
                 | Some(changes_data) =>
                   <Row marginBottom=24>
                     <Col col=Col.Four mbSm=8>
                       <Heading
                         value="Parameter Changes"
                         size=Heading.H4
                         weight=Heading.Thin
                         color={theme.textSecondary}
                       />
                     </Col>
                     <Col col=Col.Eight>
                       <div className={Styles.parameterChanges(theme)}>
                         {changes_data
                          ->Belt.Array.mapWithIndex((i, value) =>
                              <div key={i->string_of_int}>
                                <Text
                                  value={value.subspace ++ "." ++ value.key ++ ": " ++ value.value}
                                  size=Text.Lg
                                  block=true
                                />
                                <VSpacing size={`px(10)} />
                              </div>
                            )
                          ->React.array}
                       </div>
                     </Col>
                   </Row>
                 | None => React.null
                 }
               | None => React.null
               }
             | _ => React.null
             }}
            {switch (allSub) {
             | Data(({content}, _, _)) =>
               switch (content) {
               | Some({plan}) =>
                 switch (plan) {
                 | Some(planObj) =>
                   <>
                     <Row marginBottom=24>
                       <Col col=Col.Four mbSm=8>
                         <Heading
                           value="Upgrade Name"
                           size=Heading.H4
                           weight=Heading.Thin
                           color={theme.textSecondary}
                         />
                       </Col>
                       <Col col=Col.Eight>
                         <Text value={planObj.name} size=Text.Lg block=true />
                       </Col>
                     </Row>
                     <Row marginBottom=24>
                       <Col col=Col.Four mbSm=8>
                         <Heading
                           value="Upgrade Height"
                           size=Heading.H4
                           weight=Heading.Thin
                           color={theme.textSecondary}
                         />
                       </Col>
                       <Col col=Col.Eight>
                         <Text value={planObj.height |> string_of_int} size=Text.Lg block=true />
                       </Col>
                     </Row>
                   </>
                 | None => React.null
                 }
               | None => React.null
               }

             | _ => React.null
             }}
          </InfoContainer>
        </Col>
      </Row>
      // Display when related to Enable IBC
      {switch (allSub) {
       | Data(({name}, _, _)) when name->Js.String2.includes("Enable IBC Oracle") =>
         <Row>
           <Col col=Col.Four mbSm=8>
             <Heading
               value="Parameter Changes"
               size=Heading.H4
               weight=Heading.Thin
               color={theme.textSecondary}
             />
           </Col>
           <Col col=Col.Eight>
             <div className={Styles.parameterChanges(theme)}>
               <Text value="IBCRequestEnabled: True" size=Text.Lg block=true />
             </div>
           </Col>
         </Row>
       | Data(({name}, _, _)) when name->Js.String2.includes("Enable IBC Transfer") =>
         <Row>
           <Col col=Col.Four mbSm=8>
             <Heading
               value="Parameter Changes"
               size=Heading.H4
               weight=Heading.Thin
               color={theme.textSecondary}
             />
           </Col>
           <Col col=Col.Eight>
             <div className={Styles.parameterChanges(theme)}>
               <Text value="HistoricalEntries: 10000" size=Text.Lg block=true />
               <Text value="SendEnabled: True" size=Text.Lg block=true />
               <Text value="ReceiveEnabled: True" size=Text.Lg block=true />
             </div>
           </Col>
         </Row>
       | Data(({name}, _, _))
           when name->Js.String2.includes("Increase Block Capacity through Request Gas Parameter") =>
         <Row>
           <Col col=Col.Four mbSm=8>
             <Heading
               value="Parameter Changes"
               size=Heading.H4
               weight=Heading.Thin
               color={theme.textSecondary}
             />
           </Col>
           <Col col=Col.Eight>
             <div className={Styles.parameterChanges(theme)}>
               <Text value="PerValidatorRequestGas: 0" size=Text.Lg block=true />
             </div>
           </Col>
         </Row>
       | Data(({name}, _, _))
           when name->Js.String2.includes("Increase max_raw_request_count from 12 to 16") =>
         <Row>
           <Col col=Col.Four mbSm=8>
             <Heading
               value="Parameter Changes"
               size=Heading.H4
               weight=Heading.Thin
               color={theme.textSecondary}
             />
           </Col>
           <Col col=Col.Eight>
             <div className={Styles.parameterChanges(theme)}>
               <Text value="MaxRawRequestCount: 16" size=Text.Lg block=true />
             </div>
           </Col>
         </Row>
       | _ => React.null
       }}
      {switch (allSub) {
       | Data((
           {status, votingStartTime, votingEndTime},
           {
             total,
             totalYes,
             totalYesPercent,
             totalNo,
             totalNoPercent,
             totalNoWithVeto,
             totalNoWithVetoPercent,
             totalAbstain,
             totalAbstainPercent,
           },
           bondedToken,
         )) =>
         switch (status) {
         | Deposit => React.null
         | Voting
         | Passed
         | Rejected
         | Failed =>
           <>
             <Row>
               <Col col=Col.Six mb=24 mbSm=16>
                 <InfoContainer>
                   <Heading value="Voting Overview" size=Heading.H4 />
                   <SeperatedLine mt=32 mb=24 />
                   <Row marginTop=38 alignItems=Row.Center>
                     <Col col=Col.Seven>
                       <div
                         className={Css.merge([
                           CssHelper.flexBoxSm(~justify=`spaceAround, ()),
                           CssHelper.flexBox(~justify=`flexEnd, ()),
                         ])}>
                         {let turnoutPercent =
                            total /. (bondedToken |> Coin.getBandAmountFromCoin) *. 100.;
                          <div className=Styles.chartContainer>
                            <TurnoutChart percent=turnoutPercent />
                          </div>}
                       </div>
                     </Col>
                     <Col col=Col.Five>
                       <Row justify=Row.Center marginTopSm=32>
                         <Col mb=24>
                           <Heading
                             value="Total Vote"
                             size=Heading.H5
                             color={theme.textSecondary}
                             marginBottom=4
                           />
                           <Text
                             value={(total |> Format.fPretty(~digits=2)) ++ " BAND"}
                             size=Text.Lg
                             block=true
                             color={theme.textPrimary}
                           />
                         </Col>
                         <Col mb=24 mbSm=0 colSm=Col.Six>
                           <Heading
                             value="Voting Start"
                             size=Heading.H5
                             color={theme.textSecondary}
                             marginBottom=4
                           />
                           <Timestamp.Grid
                             size=Text.Lg
                             time=votingStartTime
                             color={theme.textPrimary}
                           />
                         </Col>
                         <Col mbSm=0 colSm=Col.Six>
                           <Heading
                             value="Voting End"
                             size=Heading.H5
                             color={theme.textSecondary}
                             marginBottom=4
                           />
                           <Timestamp.Grid
                             size=Text.Lg
                             time=votingEndTime
                             color={theme.textPrimary}
                           />
                         </Col>
                       </Row>
                     </Col>
                   </Row>
                 </InfoContainer>
               </Col>
               <Col col=Col.Six mb=24 mbSm=16>
                 <InfoContainer>
                   <div className={Css.merge([CssHelper.flexBox(~justify=`spaceBetween, ())])}>
                     <Heading value="Results" size=Heading.H4 />
                   </div>
                   <SeperatedLine mt=24 mb=35 />
                   <div className=Styles.resultContainer>
                     <>
                       <ProgressBar.Voting
                         label=VoteSub.Yes
                         amount=totalYes
                         percent=totalYesPercent
                       />
                       <ProgressBar.Voting
                         label=VoteSub.No
                         amount=totalNo
                         percent=totalNoPercent
                       />
                       <ProgressBar.Voting
                         label=VoteSub.NoWithVeto
                         amount=totalNoWithVeto
                         percent=totalNoWithVetoPercent
                       />
                       <ProgressBar.Voting
                         label=VoteSub.Abstain
                         amount=totalAbstain
                         percent=totalAbstainPercent
                       />
                     </>
                   </div>
                 </InfoContainer>
               </Col>
             </Row>
             <Row marginBottom=24> <Col> <VoteBreakdownTable proposalID /> </Col> </Row>
           </>
         }
       | _ => React.null
       }}
      <Row marginBottom=24>
        <Col>
          <InfoContainer>
            <Heading value="Deposit" size=Heading.H4 />
            <SeperatedLine mt=32 mb=24 />
            <Row marginBottom=24 alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Deposit Status"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                {switch (proposalSub) {
                 | Data({totalDeposit, status}) =>
                   switch (status) {
                   | ProposalSub.Deposit => <ProgressBar.Deposit totalDeposit />
                   | _ =>
                     <div className={CssHelper.flexBox()}>
                       <img alt="Success Icon" src=Images.success className=Styles.statusLogo />
                       <HSpacing size=Spacing.sm />
                       // TODO: remove hard-coded later
                       <Text value="Completed Min Deposit 1,000 BAND" size=Text.Lg />
                     </div>
                   }
                 | _ => <LoadingCensorBar width={isMobile ? 120 : 270} height=15 />
                 }}
              </Col>
            </Row>
            <Row alignItems=Row.Center>
              <Col col=Col.Four mbSm=8>
                <Heading
                  value="Deposit End Time"
                  size=Heading.H4
                  weight=Heading.Thin
                  color={theme.textSecondary}
                />
              </Col>
              <Col col=Col.Eight>
                {switch (proposalSub) {
                 | Data({depositEndTime}) => <Timestamp size=Text.Lg time=depositEndTime />
                 | _ => <LoadingCensorBar width=90 height=15 />
                 }}
              </Col>
            </Row>
          </InfoContainer>
        </Col>
      </Row>
      <Row>
        <Col>
          <Table>
            <Heading value="Depositors" size=Heading.H4 marginTop=32 marginTopSm=16 />
            <SeperatedLine mt=32 mb=0 />
            <DepositorTable proposalID />
          </Table>
        </Col>
      </Row>
    </div>
  </Section>;
};
