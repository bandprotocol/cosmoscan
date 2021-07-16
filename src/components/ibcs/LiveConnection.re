module Styles = {
  open Css;

  let greenDot =
    style([
      width(`px(8)),
      height(`px(8)),
      borderRadius(`percent(50.)),
      background(`hex("5FD3C8")),
      marginRight(`px(8)),
    ]);
};

module ConnectionListDesktop = {
  module Styles = {
    open Css;

    let listContainer = (theme: Theme.t) =>
      style([
        padding2(~v=`px(20), ~h=`px(32)),
        background(theme.headerBg),
        borderRadius(`px(12)),
        marginBottom(`px(8)),
      ]);

    let toggleButton = {
      style([
        display(`flex),
        alignItems(`center),
        cursor(`pointer),
        padding2(~v=`px(10), ~h=`zero),
      ]);
    };
  };

  [@react.component]
  let make = (~connectionSub: ApolloHooks.Subscription.variant(ConnectionSub.t)) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
    let (show, setShow) = React.useState(_ => false);

    let toggle = () => setShow(prev => !prev);

    <div className={Styles.listContainer(theme)}>
      <Row alignItems=Row.Center>
        <Col col=Col.Two>
          {switch (connectionSub) {
           | Data({connectionID}) =>
             <div className={CssHelper.flexBox()}>
               <Text value=connectionID color={theme.textPrimary} />
             </div>
           | _ => <LoadingCensorBar width=80 height=15 />
           }}
        </Col>
        <Col col=Col.Three>
          {switch (connectionSub) {
           | Data({counterpartyChainID}) =>
             <div className={CssHelper.flexBox()}> <Text value=counterpartyChainID /> </div>
           | _ => <LoadingCensorBar width=80 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          {switch (connectionSub) {
           | Data({clientID}) =>
             <div className={CssHelper.flexBox()}> <Text value=clientID /> </div>
           | _ => <LoadingCensorBar width=80 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          {switch (connectionSub) {
           | Data({counterpartyClientID}) =>
             <div className={CssHelper.flexBox()}> <Text value=counterpartyClientID /> </div>
           | _ => <LoadingCensorBar width=80 height=15 />
           }}
        </Col>
        <Col col=Col.Three>
          <div className={CssHelper.flexBox(~justify=`flexEnd, ())} onClick={_ => toggle()}>
            <div className=Styles.toggleButton>
              <Text value={show ? "Hide Channels" : "Show Channels"} color={theme.textPrimary} />
              <HSpacing size=Spacing.sm />
              <Icon
                name={show ? "fas fa-caret-up" : "fas fa-caret-down"}
                color={theme.textSecondary}
              />
            </div>
          </div>
        </Col>
      </Row>
      {switch (connectionSub) {
       | Data({channels}) => <ChannelTable channels show />
       | _ => React.null
       }}
    </div>;
  };
};

module ConnectionListMobile = {
  module Styles = {
    open Css;

    let root =
      style([
        marginBottom(`px(8)),
        Media.mobile([
          padding4(~top=`px(22), ~left=`px(16), ~right=`px(16), ~bottom=`px(5)),
        ]),
      ]);

    let cardContainer =
      style([position(`relative), selector("> div + div", [marginTop(`px(12))])]);

    let labelWrapper =
      style([
        display(`flex),
        flexDirection(`column),
        flexGrow(0.),
        flexShrink(0.),
        flexBasis(`percent(30.)),
        paddingRight(`px(10)),
      ]);

    let valueWrapper =
      style([
        display(`flex),
        flexDirection(`column),
        flexGrow(0.),
        flexShrink(0.),
        flexBasis(`percent(70.)),
        selector("i", [margin2(~v=`zero, ~h=`px(8))]),
      ]);

    let toggleButton = {
      style([
        display(`flex),
        width(`percent(100.)),
        justifyContent(`center),
        alignItems(`center),
        cursor(`pointer),
        padding2(~v=`px(10), ~h=`zero),
        marginTop(`px(11)),
      ]);
    };
  };

  [@react.component]
  let make = (~connectionSub: ApolloHooks.Subscription.variant(ConnectionSub.t)) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
    let (show, setShow) = React.useState(_ => false);

    let toggle = () => setShow(prev => !prev);

    <InfoContainer style=Styles.root>
      <div className=Styles.cardContainer>
        <div className={CssHelper.flexBox(~align=`center, ())}>
          <div className=Styles.labelWrapper>
            <Text value="Connection" size=Text.Sm transform=Text.Uppercase weight=Text.Semibold />
          </div>
          <div className=Styles.valueWrapper>
            {switch (connectionSub) {
             | Data({connectionID}) => <Text value=connectionID color={theme.textPrimary} />
             | _ => <LoadingCensorBar width=60 height=15 />
             }}
          </div>
        </div>
        <div className={CssHelper.flexBox(~align=`flexStart, ())}>
          <div className=Styles.labelWrapper>
            <Text
              value="Counterparty Chain ID"
              size=Text.Sm
              transform=Text.Uppercase
              weight=Text.Semibold
            />
          </div>
          <div className=Styles.valueWrapper>
            {switch (connectionSub) {
             | Data({counterpartyChainID}) =>
               <Text value=counterpartyChainID color={theme.textSecondary} />
             | _ => <LoadingCensorBar width=60 height=15 />
             }}
          </div>
        </div>
        <div className={CssHelper.flexBox(~align=`center, ())}>
          <div className=Styles.labelWrapper>
            <Text value="Client ID" size=Text.Sm transform=Text.Uppercase weight=Text.Semibold />
          </div>
          <div className=Styles.valueWrapper>
            {switch (connectionSub) {
             | Data({clientID}) => <Text value=clientID color={theme.textSecondary} />
             | _ => <LoadingCensorBar width=60 height=15 />
             }}
          </div>
        </div>
        <div className={CssHelper.flexBox(~align=`flexStart, ())}>
          <div className=Styles.labelWrapper>
            <Text
              value="Counterparty Client ID"
              size=Text.Sm
              transform=Text.Uppercase
              weight=Text.Semibold
            />
          </div>
          <div className=Styles.valueWrapper>
            {switch (connectionSub) {
             | Data({counterpartyClientID}) =>
               <Text value=counterpartyClientID color={theme.textSecondary} />
             | _ => <LoadingCensorBar width=60 height=15 />
             }}
          </div>
        </div>
      </div>
      {switch (connectionSub) {
       | Data({channels}) => <ChannelTable channels show />
       | _ => React.null
       }}
      <div className=Styles.toggleButton onClick={_ => toggle()}>
        <Text value={show ? "Hide Channels" : "Show Channels"} color={theme.textPrimary} />
        <HSpacing size=Spacing.sm />
        <Icon name={show ? "fas fa-caret-up" : "fas fa-caret-down"} color={theme.textSecondary} />
      </div>
    </InfoContainer>;
  };
};

[@react.component]
let make = (~counterpartyChainID) => {
  // TODO
  let (searchTerm, setSearchTerm) = React.useState(_ => "");
  let isMobile = Media.isMobile();
  let conntectionsSub = ConnectionSub.getList(~chainID=counterpartyChainID, ());

  <>
    <Row alignItems=Row.Center marginTop=80 marginTopSm=36>
      <Col col=Col.Twelve>
        <div className={CssHelper.flexBox()}>
          <div className=Styles.greenDot />
          <Heading value="Live Connection" size=Heading.H2 />
        </div>
      </Col>
    </Row>
    <Row marginTop=32 marginBottom=32>
      <Col col=Col.Twelve>
        <SearchInput
          placeholder="Search Port / Connection ID / Channel ID"
          onChange=setSearchTerm
          maxWidth=370
        />
      </Col>
    </Row>
    // Table Head
    {!isMobile
       ? <div className={CssHelper.px(~size=32, ())}>
           <Row alignItems=Row.Center minHeight={`px(32)}>
             <Col col=Col.Two>
               <div className={CssHelper.flexBox()}>
                 <Text value="Connection" size=Text.Sm transform=Text.Uppercase />
               </div>
             </Col>
             <Col col=Col.Three>
               <div className={CssHelper.flexBox()}>
                 <Text value="Counterparty Chain ID" size=Text.Sm transform=Text.Uppercase />
               </div>
             </Col>
             <Col col=Col.Two>
               <div className={CssHelper.flexBox()}>
                 <Text value="Client ID" size=Text.Sm transform=Text.Uppercase />
               </div>
             </Col>
             <Col col=Col.Two>
               <div className={CssHelper.flexBox()}>
                 <Text value="Counterparty Client ID" size=Text.Sm transform=Text.Uppercase />
               </div>
             </Col>
           </Row>
         </div>
       : React.null}
    // Table List
    {switch (conntectionsSub) {
     | Data(connections) =>
       connections
       ->Belt.Array.map(connection =>
           isMobile
             ? <ConnectionListMobile
                 key={connection.connectionID}
                 connectionSub={Sub.resolve(connection)}
               />
             : <ConnectionListDesktop
                 key={connection.connectionID}
                 connectionSub={Sub.resolve(connection)}
               />
         )
       ->React.array
     | _ =>
       Belt.Array.makeBy(3, i =>
         isMobile
           ? <ConnectionListMobile key={i |> string_of_int} connectionSub=NoData />
           : <ConnectionListDesktop key={i |> string_of_int} connectionSub=NoData />
       )
       ->React.array
     }}
  </>;
};
