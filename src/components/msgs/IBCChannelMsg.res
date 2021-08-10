module Styles = {
  open Css

  let msgContainer = style(list{
    selector("> * + *", list{marginLeft(#px(5))}),
    selector("> * + i", list{marginLeft(#px(5))}),
  })
}

module ChannelOpenCommon = {
  @react.component
  let make = (~portID, ~counterpartyPortID) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <Text value=portID nowrap=true block=true />
      <Icon name="far fa-arrow-right" color=theme.textSecondary />
      <Text value=counterpartyPortID nowrap=true block=true />
    </div>
  }
}

module ChannelOpenAck = {
  @react.component
  let make = (~channelID, ~counterpartyChannelID) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <Text value=channelID nowrap=true block=true />
      <Icon name="far fa-arrow-right" color=theme.textSecondary />
      <Text value=counterpartyChannelID nowrap=true block=true />
    </div>
  }
}

module ChannelOpenConfirm = {
  @react.component
  let make = (~channelID) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <Text value=channelID nowrap=true block=true />
    </div>
  }
}

module ChannelCloseCommon = {
  @react.component
  let make = (~channelID) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <Text value=channelID nowrap=true block=true />
    </div>
  }
}
