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

    let toggle = {
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
           | Data(connection) =>
             <div className={CssHelper.flexBox()}>
               <Text value={connection.connectionID} color={theme.textPrimary} />
             </div>
           | _ => <LoadingCensorBar width=80 height=15 />
           }}
        </Col>
        <Col col=Col.Three>
          {switch (connectionSub) {
           | Data(connection) =>
             <div className={CssHelper.flexBox()}>
               <Text value={connection.counterpartyChainID} />
             </div>
           | _ => <LoadingCensorBar width=80 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          {switch (connectionSub) {
           | Data(connection) =>
             <div className={CssHelper.flexBox()}> <Text value={connection.clientID} /> </div>
           | _ => <LoadingCensorBar width=80 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          {switch (connectionSub) {
           | Data(connection) =>
             <div className={CssHelper.flexBox()}>
               <Text value={connection.counterpartyClientID} />
             </div>
           | _ => <LoadingCensorBar width=80 height=15 />
           }}
        </Col>
        <Col col=Col.Three>
          <div className={CssHelper.flexBox(~justify=`flexEnd, ())} onClick={_ => toggle()}>
            <div className=Styles.toggle>
              <Text value={show ? "Hide Channels" : "Show Channels"} color={theme.textPrimary} />
              <HSpacing size=Spacing.sm />
              <Icon name="fas fa-caret-down" color={theme.textSecondary} />
            </div>
          </div>
        </Col>
      </Row>
      {switch (connectionSub) {
       | Data(connection) => <ChannelTable channels={connection.channels} show />
       | _ => React.null
       }}
    </div>;
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
    <div className={CssHelper.px(~size=32, ())}>
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
    // Table List
    {switch (conntectionsSub) {
     | Data(connections) =>
       connections
       ->Belt.Array.map(connection =>
           <ConnectionListDesktop
             key={connection.connectionID}
             connectionSub={Sub.resolve(connection)}
           />
         )
       ->React.array
     | _ =>
       Belt.Array.makeBy(3, i =>
         <ConnectionListDesktop key={i |> string_of_int} connectionSub=NoData />
       )
       ->React.array
     }}
  </>;
};
