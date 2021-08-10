module Styles = {
  open Css

  let msgContainer = style(list{selector("> * + *", list{marginLeft(#px(5))})})
}

module Client = {
  @react.component
  let make = (~clientID) =>
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <Text value=clientID nowrap=true block=true />
    </div>
}
