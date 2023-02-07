module Styles = {
  open Css;

  let container = (show, theme: Theme.t) =>
    style([
      background(theme.neutral_100),
      padding2(~h=`px(32), ~v=`zero),
      width(`percent(100.)),
      marginTop(show ? `px(20) : `zero),
      transition(~duration=200, "all"),
      opacity(show ? 1. : 0.),
      pointerEvents(show ? `auto : `none),
      height(show ? `auto : `zero),
      Media.mobile([padding2(~h=`px(16), ~v=`zero)]),
    ]);

  let thead = (theme: Theme.t) =>
    style([borderBottom(`px(1), `solid, theme.neutral_100)]);
};

module RenderBody = {
  [@react.component]
  let make = (~channel: ConnectionSub.channel_t) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);

    <Row alignItems=Row.Center minHeight={`px(40)} style={Styles.thead(theme)}>
      <Col col=Col.Five>
        <div className={CssHelper.flexBox()}>
          <Text value={channel.port} />
          <Icon name="fal fa-long-arrow-right" ml=8 mr=8 />
          <Text value={channel.counterpartyPort} />
        </div>
      </Col>
      <Col col=Col.Four>
        {switch (channel.state) {
         | Open =>
           <div className={CssHelper.flexBox()}>
             <Text value={channel.channelID} />
             <Icon name="fal fa-long-arrow-right" ml=8 mr=8 />
             <Text value={channel.counterpartyChannelID} />
           </div>
         | _ => <Text value="Unconnected" />
         }}
      </Col>
      <Col col=Col.Two>
        <div className={CssHelper.flexBox()}>
          {switch (channel.state) {
           | Open => <img alt="Success Icon" src=Images.success />
           | _ => <img alt="Fail Icon" src=Images.fail />
           }}
        </div>
      </Col>
      <Col col=Col.One>
        <div className={CssHelper.flexBox()}> <Text value={channel.order} /> </div>
      </Col>
    </Row>;
  };
};

module RenderMobile = {
  module Styles = {
    open Css;

    let cardContainer = (theme: Theme.t) =>
      style([
        position(`relative),
        padding2(~v=`px(24), ~h=`zero),
        selector("> div + div", [marginTop(`px(24))]),
        borderBottom(`px(1), `solid, theme.neutral_100),
      ]);

    let status = style([position(`absolute), top(`px(24)), right(`zero)]);

    let labelWrapper =
      style([
        display(`flex),
        flexDirection(`column),
        flexGrow(0.),
        flexShrink(0.),
        flexBasis(`percent(40.)),
        paddingRight(`px(10)),
      ]);

    let valueWrapper =
      style([
        display(`flex),
        flexDirection(`column),
        flexGrow(0.),
        flexShrink(0.),
        flexBasis(`percent(60.)),
      ]);
  };

  [@react.component]
  let make = (~channel: ConnectionSub.channel_t) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);

    <div className={Styles.cardContainer(theme)}>
      <img
        alt="Status Icon"
        src={
          switch (channel.state) {
          | Open => Images.success
          | _ => Images.fail
          }
        }
        className=Styles.status
      />
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper>
          <div className={CssHelper.flexBox(~direction=`column, ~align=`flexStart, ())}>
            <Text value="Port" size=Text.Sm transform=Text.Uppercase weight=Text.Semibold />
            <Icon name="fal fa-arrow-down" color={theme.neutral_600} mt=4 mb=4 />
            <Text
              value="Counterparty"
              size=Text.Sm
              transform=Text.Uppercase
              weight=Text.Semibold
            />
          </div>
        </div>
        <div className=Styles.valueWrapper>
          <div className={CssHelper.flexBox(~direction=`column, ~align=`flexStart, ())}>
            <Text value={channel.port} size=Text.Sm />
            <Icon name="fal fa-arrow-down" color={theme.neutral_600} mt=4 mb=4 />
            <Text value={channel.counterpartyPort} size=Text.Sm />
          </div>
        </div>
      </div>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper>
          <div className={CssHelper.flexBox(~direction=`column, ~align=`flexStart, ())}>
            <Text value="Channel" size=Text.Sm transform=Text.Uppercase weight=Text.Semibold />
            <Icon name="fal fa-arrow-down" color={theme.neutral_600} mt=4 mb=4 />
            <Text
              value="Counterparty"
              size=Text.Sm
              transform=Text.Uppercase
              weight=Text.Semibold
            />
          </div>
        </div>
        <div className=Styles.valueWrapper>
          {switch (channel.state) {
           | Open =>
             <div className={CssHelper.flexBox(~direction=`column, ~align=`flexStart, ())}>
               <Text value={channel.channelID} size=Text.Sm />
               <Icon name="fal fa-arrow-down" color={theme.neutral_600} mt=4 mb=4 />
               <Text value={channel.counterpartyChannelID} size=Text.Sm />
             </div>
           | _ => <Text value="Unconnected" size=Text.Sm />
           }}
        </div>
      </div>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper>
          <Text value="Order" size=Text.Sm transform=Text.Uppercase weight=Text.Semibold />
        </div>
        <div className=Styles.valueWrapper> <Text value={channel.order} size=Text.Sm /> </div>
      </div>
    </div>;
  };
};

[@react.component]
let make = (~channels, ~show) => {
  let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
  let isMobile = Media.isMobile();

  <div className={Styles.container(show, theme)}>
    {isMobile
       ? React.null
       : <Row alignItems=Row.Center minHeight={`px(40)} style={Styles.thead(theme)}>
           <Col col=Col.Five>
             <div className={CssHelper.flexBox()}>
               <Text value="Port" size=Text.Sm transform=Text.Uppercase weight=Text.Semibold />
               <Icon name="fal fa-long-arrow-right" ml=8 mr=8 />
               <Text
                 value="Counterparty Port"
                 size=Text.Sm
                 transform=Text.Uppercase
                 weight=Text.Semibold
               />
             </div>
           </Col>
           <Col col=Col.Four>
             <div className={CssHelper.flexBox()}>
               <Text value="Channel" size=Text.Sm transform=Text.Uppercase weight=Text.Semibold />
               <Icon name="fal fa-long-arrow-right" ml=8 mr=8 />
               <Text
                 value="Counterparty Channel"
                 size=Text.Sm
                 transform=Text.Uppercase
                 weight=Text.Semibold
               />
             </div>
           </Col>
           <Col col=Col.Two>
             <div className={CssHelper.flexBox()}>
               <Text value="State" size=Text.Sm transform=Text.Uppercase weight=Text.Semibold />
             </div>
           </Col>
           <Col col=Col.One>
             <div className={CssHelper.flexBox()}>
               <Text value="Order" size=Text.Sm transform=Text.Uppercase weight=Text.Semibold />
             </div>
           </Col>
         </Row>}
    {channels
     ->Belt.Array.map(channel =>
         isMobile
           ? <RenderMobile key={channel.channelID ++ channel.port} channel />
           : <RenderBody key={channel.channelID ++ channel.port} channel />
       )
     ->React.array}
  </div>;
};
