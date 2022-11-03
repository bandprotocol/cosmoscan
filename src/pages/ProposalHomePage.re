module Styles = {
  open Css;
  let idContainer = {
    style([
      selector(
        "> h3",
        [
          marginLeft(`px(10)),
          marginRight(`px(10)),
          Media.mobile([marginLeft(`zero), marginTop(`px(8)), marginBottom(`px(8))]),
        ],
      ),
    ]);
  };
  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);

  let proposalLink = (theme: Theme.t) =>
    style([
      backgroundColor(theme.baseBlue),
      borderRadius(`px(8)),
      width(`px(32)),
      height(`px(32)),
      hover([backgroundColor(theme.darkBlue)]),
    ]);
};

//TODO: Will refactor to have only one Sub
module Turnout = {
  [@react.component]
  let make = (~id) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    let voteSub = VoteSub.getVoteStatByProposalID(id);
    let bondedTokenCountSub = ValidatorSub.getTotalBondedAmount();

    let allSub = Sub.all2(voteSub, bondedTokenCountSub);
    <>
      {switch (allSub) {
       | Data(({total}, bondedTokenCount)) =>
         let turnoutRate = total /. (bondedTokenCount |> Coin.getBandAmountFromCoin) *. 100.;

         <Col col=Col.Four colSm=Col.Five>
           <Heading
             value="Turnout"
             size=Heading.H5
             marginBottom=8
             color={theme.textSecondary}
             weight=Heading.Thin
           />
           <Text
             value={turnoutRate |> Format.fPercent(~digits=2)}
             size=Text.Lg
             color={theme.textPrimary}
           />
         </Col>;
       | _ => React.null
       }}
    </>;
  };
};

module ProposalCard = {
  [@react.component]
  let make = (~reserveIndex, ~proposalSub: ApolloHooks.Subscription.variant(ProposalSub.t)) => {
    let isMobile = Media.isMobile();
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Col key={reserveIndex |> string_of_int} mb=24 mbSm=16>
      <InfoContainer>
        <Row marginBottom=18>
          <Col col=Col.Eleven colSm=Col.Ten>
            <div
              className={Css.merge([
                CssHelper.flexBox(),
                CssHelper.flexBoxSm(~direction=`column, ~align=`flexStart, ()),
                Styles.idContainer,
              ])}>
              {switch (proposalSub) {
               | Data({id, name}) =>
                 <>
                   <TypeID.Proposal id position=TypeID.Title />
                   <Heading
                     size=Heading.H3
                     value=name
                     color={theme.textSecondary}
                     weight=Heading.Thin
                   />
                 </>
               | _ =>
                 isMobile
                   ? <>
                       <LoadingCensorBar width=50 height=15 mbSm=16 />
                       <LoadingCensorBar width=100 height=15 mbSm=16 />
                     </>
                   : <LoadingCensorBar width=270 height=15 />
               }}
              <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
                {switch (proposalSub) {
                 | Data({status}) => <ProposalBadge status />
                 | _ =>
                   <>
                     {isMobile ? React.null : <HSpacing size={`px(10)} />}
                     <LoadingCensorBar width=100 height=15 radius=50 />
                   </>
                 }}
              </div>
            </div>
          </Col>
          <Col col=Col.One colSm=Col.Two>
            <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
              {switch (proposalSub) {
               | Data({id}) =>
                 <TypeID.ProposalLink id>
                   <div
                     className={Css.merge([
                       Styles.proposalLink(theme),
                       CssHelper.flexBox(~justify=`center, ()),
                     ])}>
                     <Icon name="far fa-arrow-right" color={theme.white} />
                   </div>
                 </TypeID.ProposalLink>
               | _ => <LoadingCensorBar width=32 height=32 radius=8 />
               }}
            </div>
          </Col>
        </Row>
        <Row marginBottom=24>
          <Col>
            {switch (proposalSub) {
             | Data({description}) => <Markdown value=description />
             | _ => <LoadingCensorBar width=270 height=15 />
             }}
          </Col>
        </Row>
        <SeperatedLine />
        <Row>
          <Col col=Col.Four mbSm=16>
            <Heading
              value="Proposer"
              size=Heading.H5
              marginBottom=8
              weight=Heading.Thin
              color={theme.textSecondary}
            />
            {switch (proposalSub) {
             | Data({proposerAddressOpt}) =>
               switch (proposerAddressOpt) {
               | Some(proposerAddress) =>
                 <AddressRender address=proposerAddress position=AddressRender.Subtitle />
               | None => <Text value="Proposed on Wenchang" />
               }
             | _ => <LoadingCensorBar width=270 height=15 />
             }}
          </Col>
          <Col col=Col.Four colSm=Col.Seven>
            <div className={CssHelper.mb(~size=8, ())}>
              {switch (proposalSub) {
               | Data({status}) =>
                 <Heading
                   value={
                     switch (status) {
                     | Deposit => "Deposit End Time"
                     | Voting
                     | Passed
                     | Rejected
                     | Failed => "Voting End Time"
                     }
                   }
                   size=Heading.H5
                   weight=Heading.Thin
                   color={theme.textSecondary}
                 />
               | _ => <LoadingCensorBar width=100 height=15 />
               }}
            </div>
            {switch (proposalSub) {
             | Data({depositEndTime, votingEndTime, status}) =>
               <Timestamp
                 size=Text.Lg
                 time={
                   switch (status) {
                   | Deposit => depositEndTime
                   | Voting
                   | Passed
                   | Rejected
                   | Failed => votingEndTime
                   }
                 }
                 color={theme.textPrimary}
                 suffix=" +UTC"
               />
             | _ => <LoadingCensorBar width={isMobile ? 120 : 270} height=15 />
             }}
          </Col>
          {switch (proposalSub) {
           | Data({status, id}) =>
             switch (status) {
             | Deposit => React.null
             | Voting
             | Passed
             | Rejected
             | Failed => <Turnout id />
             }
           | _ =>
             <Col col=Col.Four colSm=Col.Five>
               <LoadingCensorBar width=100 height=15 mb=8 />
               <LoadingCensorBar width=50 height=15 />
             </Col>
           }}
        </Row>
      </InfoContainer>
    </Col>;
  };
};

[@react.component]
let make = () => {
  let pageSize = 10;
  let proposalsSub = ProposalSub.getList(~pageSize, ~page=1, ());

  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  <Section>
    <div className=CssHelper.container id="proposalsSection">
      <Row alignItems=Row.Center marginBottom=40 marginBottomSm=24>
        <Col col=Col.Twelve> <Heading value="All Proposals" size=Heading.H2 /> </Col>
      </Row>
      <Row>
        {switch (proposalsSub) {
         | Data(proposals) =>
           proposals->Belt.Array.size > 0
             ? proposals
               ->Belt.Array.keep(({id}) => id != ID.Proposal.ID(8))
               ->Belt_Array.mapWithIndex((i, proposal) => {
                   <ProposalCard
                     key={i |> string_of_int}
                     reserveIndex=i
                     proposalSub={Sub.resolve(proposal)}
                   />
                 })
               ->React.array
             : <EmptyContainer>
                 <img
                   alt="No Proposal"
                   src={isDarkMode ? Images.noTxDark : Images.noTxLight}
                   className=Styles.noDataImage
                 />
                 <Heading
                   size=Heading.H4
                   value="No Proposal"
                   align=Heading.Center
                   weight=Heading.Regular
                   color={theme.textSecondary}
                 />
               </EmptyContainer>
         | _ =>
           Belt_Array.make(pageSize, ApolloHooks.Subscription.NoData)
           ->Belt_Array.mapWithIndex((i, noData) =>
               <ProposalCard key={i |> string_of_int} reserveIndex=i proposalSub=noData />
             )
           ->React.array
         }}
      </Row>
    </div>
  </Section>;
};
