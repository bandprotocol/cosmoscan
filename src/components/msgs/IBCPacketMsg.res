module Styles = {
  open Css

  let msgContainer = style(list{selector("> * + *", list{marginLeft(#px(5))})})
}

module Packet = {
  @react.component
  let make = (~packetType) =>
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <Text value=packetType nowrap=true block=true />
    </div>
}
