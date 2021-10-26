module Styles = {
  open Css;

  let container = style([Media.mobile([margin2(~h=`px(-12), ~v=`zero)])]);

  let header = (theme: Theme.t) =>
    style([
      borderBottom(`px(1), `solid, theme.tableRowBorderColor),
      selector("> div + div", [marginLeft(`px(32))]),
      Media.mobile([overflow(`auto), padding2(~v=`px(1), ~h=`px(15))]),
    ]);

  let buttonContainer = active =>
    style([
      display(`inlineFlex),
      justifyContent(`center),
      alignItems(`center),
      cursor(`pointer),
      padding2(~v=`px(32), ~h=`zero),
      borderBottom(`px(4), `solid, active ? Theme.baseBlue : `transparent),
      Media.mobile([whiteSpace(`nowrap), padding2(~v=`px(24), ~h=`zero)]),
    ]);

  let childrenContainer = style([Media.mobile([padding2(~h=`px(16), ~v=`zero)])]);

  let tableWrapper = style([Media.mobile([padding2(~v=`px(16), ~h=`zero)])]);
  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
};

module RenderBody = {
  [@react.component]
  let make = (~voteSub: ApolloHooks.Subscription.variant(VoteSub.t)) => {
    <TBody>
      <Row alignItems=Row.Center>
        <Col col=Col.Five>
          {switch (voteSub) {
           | Data({voter, validator}) =>
             switch (validator) {
             | Some({moniker, operatorAddress, identity}) =>
               <ValidatorMonikerLink
                 validatorAddress=operatorAddress
                 moniker
                 identity
                 width={`percent(100.)}
                 avatarWidth=20
               />
             | None => <AddressRender address=voter />
             }
           | _ => <LoadingCensorBar width=200 height=20 />
           }}
        </Col>
        <Col col=Col.Four>
          {switch (voteSub) {
           | Data({txHashOpt}) =>
             switch (txHashOpt) {
             | Some(txHash) => <TxLink txHash width=200 />
             | None => <Text value="Voted on Wenchang" />
             }
           | _ => <LoadingCensorBar width=200 height=20 />
           }}
        </Col>
        <Col col=Col.Three>
          <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
            {switch (voteSub) {
             | Data({timestampOpt}) =>
               switch (timestampOpt) {
               | Some(timestamp) =>
                 <Timestamp time=timestamp size=Text.Md weight=Text.Regular textAlign=Text.Right />
               | None => <Text value="Created on Wenchang" />
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
  let make = (~reserveIndex, ~voteSub: ApolloHooks.Subscription.variant(VoteSub.t)) => {
    switch (voteSub) {
    | Data({voter, txHashOpt, timestampOpt, validator}) =>
      let key_ = voter |> Address.toBech32;

      <MobileCard
        values=InfoMobileCard.[
          (
            "Voter",
            {switch (validator) {
             | Some({operatorAddress, moniker, identity}) =>
               Validator(operatorAddress, moniker, identity)
             | None => Address(voter, 200, `account)
             }},
          ),
          (
            "TX Hash",
            switch (txHashOpt) {
            | Some(txHash) => TxHash(txHash, 200)
            | None => Text("Voted on Wenchang")
            },
          ),
          (
            "Timestamp",
            switch (timestampOpt) {
            | Some(timestamp) => Timestamp(timestamp)
            | None => Text("Created on Wenchang")
            },
          ),
        ]
        key=key_
        idx=key_
      />;
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("Voter", Loading(230)),
          ("TX Hash", Loading(100)),
          ("Timestamp", Loading(100)),
        ]
        key={reserveIndex |> string_of_int}
        idx={reserveIndex |> string_of_int}
      />
    };
  };
};

module TabButton = {
  [@react.component]
  let make = (~tab, ~active, ~setTab) => {
    let tabString = tab |> VoteSub.toString(~withSpace=true);

    <div className={Styles.buttonContainer(active)} onClick={_ => setTab(_ => tab)}>
      <Text value=tabString weight={active ? Text.Semibold : Text.Regular} size=Text.Lg />
    </div>;
  };
};

[@react.component]
let make = (~proposalID) => {
  let isMobile = Media.isMobile();
  let (currentTab, setTab) = React.useState(_ => VoteSub.Yes);
  let (page, setPage) = React.useState(_ => 1);
  let pageSize = 5;
  let votesSub = VoteSub.getList(proposalID, currentTab, ~pageSize, ~page, ());
  let voteCountSub = VoteSub.count(proposalID, currentTab);

  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  <Table>
    <div className=Styles.container>
      <div className={Css.merge([Styles.header(theme), CssHelper.flexBox(~wrap=`nowrap, ())])}>
        {[|VoteSub.Yes, No, NoWithVeto, Abstain|]
         ->Belt.Array.map(tab =>
             <TabButton key={tab |> VoteSub.toString} tab setTab active={tab == currentTab} />
           )
         ->React.array}
      </div>
      <div className=Styles.childrenContainer>
        <div className=Styles.tableWrapper>
          {isMobile
             ? <Row marginBottom=16>
                 <Col>
                   {switch (voteCountSub) {
                    | Data(voteCount) =>
                      <div className={CssHelper.flexBox()}>
                        <Text
                          block=true
                          value={voteCount |> string_of_int}
                          weight=Text.Semibold
                          size=Text.Sm
                          transform=Text.Uppercase
                        />
                        <HSpacing size=Spacing.xs />
                        <Text
                          block=true
                          value="Voters"
                          weight=Text.Semibold
                          size=Text.Sm
                          transform=Text.Uppercase
                        />
                      </div>
                    | _ => <LoadingCensorBar width=100 height=15 />
                    }}
                 </Col>
               </Row>
             : <THead>
                 <Row alignItems=Row.Center>
                   <Col col=Col.Five>
                     {switch (voteCountSub) {
                      | Data(voteCount) =>
                        <div className={CssHelper.flexBox()}>
                          <Text
                            block=true
                            value={voteCount |> string_of_int}
                            weight=Text.Semibold
                            size=Text.Sm
                            transform=Text.Uppercase
                          />
                          <HSpacing size=Spacing.xs />
                          <Text
                            block=true
                            value="Voters"
                            weight=Text.Semibold
                            size=Text.Sm
                            transform=Text.Uppercase
                          />
                        </div>
                      | _ => <LoadingCensorBar width=100 height=15 />
                      }}
                   </Col>
                   <Col col=Col.Four>
                     <Text
                       block=true
                       value="TX Hash"
                       weight=Text.Semibold
                       size=Text.Sm
                       transform=Text.Uppercase
                     />
                   </Col>
                   <Col col=Col.Three>
                     <Text
                       block=true
                       value="Timestamp"
                       weight=Text.Semibold
                       size=Text.Sm
                       transform=Text.Uppercase
                       align=Text.Right
                     />
                   </Col>
                 </Row>
               </THead>}
          {switch (votesSub) {
           | Data(votes) =>
             votes->Belt.Array.size > 0
               ? votes
                 ->Belt_Array.mapWithIndex((i, e) =>
                     isMobile
                       ? <RenderBodyMobile
                           reserveIndex=i
                           key={e.voter |> Address.toBech32}
                           voteSub={Sub.resolve(e)}
                         />
                       : <RenderBody key={e.voter |> Address.toBech32} voteSub={Sub.resolve(e)} />
                   )
                 ->React.array
               : <EmptyContainer height={`px(250)}>
                   <img
                     alt="No Voters"
                     src={isDarkMode ? Images.noDelegatorDark : Images.noDelegatorLight}
                     className=Styles.noDataImage
                   />
                   <Heading
                     size=Heading.H4
                     value="No Voters"
                     align=Heading.Center
                     weight=Heading.Regular
                     color={theme.textSecondary}
                   />
                 </EmptyContainer>
           | _ =>
             Belt_Array.make(pageSize, ApolloHooks.Subscription.NoData)
             ->Belt_Array.mapWithIndex((i, noData) =>
                 isMobile
                   ? <RenderBodyMobile reserveIndex=i key={i |> string_of_int} voteSub=noData />
                   : <RenderBody key={i |> string_of_int} voteSub=noData />
               )
             ->React.array
           }}
          {switch (voteCountSub) {
           | Data(voteCount) =>
             let pageCount = Page.getPageCount(voteCount, pageSize);
             <Pagination
               currentPage=page
               pageCount
               onPageChange={newPage => setPage(_ => newPage)}
             />;
           | _ => React.null
           }}
        </div>
      </div>
    </div>
  </Table>;
};
