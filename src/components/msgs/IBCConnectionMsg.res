module Styles = {
  open Css

  let msgContainer = style(list{
    selector("> * + *", list{marginLeft(#px(5))}),
    selector("> * + i", list{marginLeft(#px(5))}),
  })
}

module ConnectionCommon = {
  @react.component
  let make = (~clientID, ~counterpartyClientID) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <Text value=clientID nowrap=true block=true />
      <Icon name="far fa-arrow-right" color=theme.textSecondary />
      <Text value=counterpartyClientID nowrap=true block=true />
    </div>
  }
}

module ConnectionOpenAck = {
  @react.component
  let make = (~connectionID, ~counterpartyConnectionID) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <Text value=connectionID nowrap=true block=true />
      <Icon name="far fa-arrow-right" color=theme.textSecondary />
      <Text value=counterpartyConnectionID nowrap=true block=true />
    </div>
  }
}

module ConnectionOpenConfirm = {
  @react.component
  let make = (~connectionID) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <Text value=connectionID nowrap=true block=true />
    </div>
  }
}
