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
           <div className=Styles.request>
             <TypeID.Request id=request key={request |> ID.Request.toString} />
           </div>
         )
       ->React.array}
    </div>;
  };
};

[@react.component]
let make = (~blockSub: ApolloHooks.Subscription.variant(BlockSub.t)) => {
  let (tabIndex, setTabIndex) = React.useState(_ => 0);
  let setTab = index => setTabIndex(_ => index);

  <Row marginBottom=24>
    <Col>
      <InfoContainer>
        <Heading value="Resolved Requests" size=Heading.H4 />
        <div className={CssHelper.mt(~size=16, ())}>
          <Text
            value="Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat."
          />
        </div>
        {switch (blockSub) {
         | Data({requests}) =>
           let onChainRequests =
             requests->Belt.Array.keepMap(({id, isIBC}) => !isIBC ? Some(id) : None);
           let ibcRequests =
             requests->Belt.Array.keepMap(({id, isIBC}) => isIBC ? Some(id) : None);
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

           <Tab.State tabs=[|{j|On Chain ($onChain)|j}, {j|IBC ($ibc)|j}|] tabIndex setTab>
             {showRequests->Belt.Array.length == 0
                ? <div className=Styles.emptyContainer>
                    <Text value="There is no resolved request." />
                  </div>
                : <div className=Styles.container>
                    <Text value="Request ID" size=Text.Sm transform=Text.Uppercase />
                    <RequestsList requests=showRequests />
                  </div>}
           </Tab.State>;
         | _ =>
           <Tab.State tabs=[|{j|On Chain|j}, {j|IBC|j}|] tabIndex setTab>
             <div className=Styles.container>
               <Text value="Request ID" size=Text.Sm transform=Text.Uppercase />
               <VSpacing size={`px(26)} />
               <LoadingCensorBar width=170 height=15 />
             </div>
           </Tab.State>
         }}
      </InfoContainer>
    </Col>
  </Row>;
};
