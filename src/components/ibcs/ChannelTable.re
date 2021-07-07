module Styles = {
  open Css;

  let container = (show, theme: Theme.t) =>
    style([
      background(theme.secondaryTableBg),
      padding2(~h=`px(32), ~v=`zero),
      width(`percent(100.)),
      marginTop(show ? `px(20) : `zero),
      transition(~duration=300, "all"),
      opacity(show ? 1. : 0.),
      pointerEvents(show ? `auto : `none),
      height(show ? `auto : `zero),
    ]);

  let thead = (theme: Theme.t) =>
    style([borderBottom(`px(1), `solid, theme.tableRowBorderColor)]);
};

module RenderBody = {
  [@react.component]
  let make = (~channel: ConnectionSub.channel_t) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);

    <Row alignItems=Row.Center minHeight={`px(40)} style={Styles.thead(theme)}>
      <Col col=Col.Five>
        <div className={CssHelper.flexBox()}>
          <Text value={channel.port} size=Text.Sm transform=Text.Uppercase />
          <Icon name="fal fa-long-arrow-right" ml=8 mr=8 />
          <Text value={channel.counterpartyPort} size=Text.Sm transform=Text.Uppercase />
        </div>
      </Col>
      <Col col=Col.Four>
        <div className={CssHelper.flexBox()}>
          <Text value={channel.channelID} size=Text.Sm transform=Text.Uppercase />
          <Icon name="fal fa-long-arrow-right" ml=8 mr=8 />
          <Text value={channel.counterpartyChannelID} size=Text.Sm transform=Text.Uppercase />
        </div>
      </Col>
      <Col col=Col.Two>
        <div className={CssHelper.flexBox()}>
          {switch (channel.state) {
           | Open => <img src=Images.success />
           | _ => <img src=Images.fail />
           }}
        </div>
      </Col>
      <Col col=Col.One>
        <div className={CssHelper.flexBox()}> <Text value={channel.order} size=Text.Sm /> </div>
      </Col>
    </Row>;
  };
};

[@react.component]
let make = (~channels, ~show) => {
  let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);

  <div className={Styles.container(show, theme)}>
    <Row alignItems=Row.Center minHeight={`px(40)} style={Styles.thead(theme)}>
      <Col col=Col.Five>
        <div className={CssHelper.flexBox()}>
          <Text value="Port" size=Text.Sm transform=Text.Uppercase />
          <Icon name="fal fa-long-arrow-right" ml=8 mr=8 />
          <Text value="Counterparty Port" size=Text.Sm transform=Text.Uppercase />
        </div>
      </Col>
      <Col col=Col.Four>
        <div className={CssHelper.flexBox()}>
          <Text value="Channel" size=Text.Sm transform=Text.Uppercase />
          <Icon name="fal fa-long-arrow-right" ml=8 mr=8 />
          <Text value="Counterparty Channel" size=Text.Sm transform=Text.Uppercase />
        </div>
      </Col>
      <Col col=Col.Two>
        <div className={CssHelper.flexBox()}>
          <Text value="State" size=Text.Sm transform=Text.Uppercase />
        </div>
      </Col>
      <Col col=Col.One>
        <div className={CssHelper.flexBox()}>
          <Text value="Order" size=Text.Sm transform=Text.Uppercase />
        </div>
      </Col>
    </Row>
    {channels
     ->Belt.Array.map(channel => <RenderBody key={channel.channelID ++ channel.port} channel />)
     ->React.array}
  </div>;
};
