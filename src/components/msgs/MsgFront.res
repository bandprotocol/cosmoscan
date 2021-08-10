module Styles = {
  open Css
  let addressWrapper = style(list{width(#px(120)), Media.smallMobile(list{width(#px(80))})})
}

@react.component
let make = (~msgType, ~name, ~fromAddress) =>
  <div className={Css.merge(list{CssHelper.flexBox(~wrap=#nowrap, ())})}>
    <div className=Styles.addressWrapper> <AddressRender address=fromAddress /> </div>
    <MsgBadge name />
  </div>
