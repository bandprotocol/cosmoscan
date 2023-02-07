module Styles = {
  open Css;

  let msgContainer =
    style([
      selector("> * + *", [marginLeft(`px(5))]),
      selector("> * + i", [marginLeft(`px(5))]),
    ]);
};

module ChannelOpenCommon = {
  [@react.component]
  let make = (~portID, ~counterpartyPortID) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <div
      className={Css.merge([
        CssHelper.flexBox(~wrap=`nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      ])}>
      <Text value=portID nowrap=true block=true />
      <Icon name="far fa-arrow-right" color={theme.neutral_600} />
      <Text value=counterpartyPortID nowrap=true block=true />
    </div>;
  };
};

module ChannelOpenAck = {
  [@react.component]
  let make = (~channelID, ~counterpartyChannelID) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <div
      className={Css.merge([
        CssHelper.flexBox(~wrap=`nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      ])}>
      <Text value=channelID nowrap=true block=true />
      <Icon name="far fa-arrow-right" color={theme.neutral_600} />
      <Text value=counterpartyChannelID nowrap=true block=true />
    </div>;
  };
};

module ChannelOpenConfirm = {
  [@react.component]
  let make = (~channelID) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <div
      className={Css.merge([
        CssHelper.flexBox(~wrap=`nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      ])}>
      <Text value=channelID nowrap=true block=true />
    </div>;
  };
};

module ChannelCloseCommon = {
  [@react.component]
  let make = (~channelID) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <div
      className={Css.merge([
        CssHelper.flexBox(~wrap=`nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      ])}>
      <Text value=channelID nowrap=true block=true />
    </div>;
  };
};
