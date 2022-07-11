module Styles = {
  open Css;

  let container = style([paddingTop(`px(24)), paddingBottom(`px(10)), minHeight(`px(100))]);
  let emptyContainer =
    style([
      display(`flex),
      minHeight(`px(100)),
      alignItems(`center),
      justifyContent(`center),
    ]);
  let request = style([marginTop(`px(26)), marginRight(`px(12))]);
};

module RequestsList = {
  [@react.component]
  let make = (~requests) => {
    <div className={CssHelper.flexBox()}>
      {requests
       ->Belt.Array.map(request =>
           <div className=Styles.request key={request |> ID.Request.toString}>
             <TypeID.Request id=request />
           </div>
         )
       ->React.array}
    </div>;
  };
};

[@react.component]
let make = (~blockSub: ApolloHooks.Subscription.variant(BlockSub.block_t)) => {
  let (tabIndex, setTabIndex) = React.useState(_ => 0);
  let setTab = index => setTabIndex(_ => index);

  switch (blockSub) {
  | Data({requests}) when requests->Belt.Array.length !== 0 =>
    let onChainRequests =
      requests->Belt.Array.keepMap(({id, isIBC}) => !isIBC ? Some(id) : None);
    let ibcRequests = requests->Belt.Array.keepMap(({id, isIBC}) => isIBC ? Some(id) : None);
    let onChain = onChainRequests->Belt.Array.length;
    let ibc = ibcRequests->Belt.Array.length;
    let showRequests = {
      switch (tabIndex) {
      | 0 => onChainRequests
      | 1 => ibcRequests
      | _ =>
        Js.log("Please handle every case on Resolve Requests.");
        [||];
      };
    };

    <Row marginBottom=24>
      <Col>
        <InfoContainer>
          <Heading value="Resolved Requests" size=Heading.H4 />
          <div className={CssHelper.mt(~size=16, ())}>
            <Text
              value="Resolved requests are requests that were successfully processed, failed, or timed out by BandChain in this block."
            />
          </div>
          <Tab.State tabs=[|{j|On Chain ($onChain)|j}, {j|IBC ($ibc)|j}|] tabIndex setTab>
            {showRequests->Belt.Array.length === 0
               ? <div className=Styles.emptyContainer>
                   <Text value="There is no resolved request." />
                 </div>
               : <div className=Styles.container>
                   <Text value="Request ID" size=Text.Sm transform=Text.Uppercase />
                   <RequestsList requests=showRequests />
                 </div>}
          </Tab.State>
        </InfoContainer>
      </Col>
    </Row>;
  | _ => React.null
  };
};
