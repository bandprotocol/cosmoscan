module Styles = {
  open Css

  let msgContainer = style(list{selector("> * + *", list{marginLeft(#px(5))})})
}

module CreateValidator = {
  @react.component
  let make = (~moniker) =>
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <Text value=moniker size=Text.Md nowrap=true block=true />
    </div>
}

module EditValidator = {
  @react.component
  let make = (~moniker) =>
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <Text value=moniker size=Text.Md nowrap=true block=true />
    </div>
}

module AddReporter = {
  @react.component
  let make = (~reporter) =>
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <AddressRender address=reporter />
    </div>
}

module RemoveReporter = {
  @react.component
  let make = (~reporter) =>
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <AddressRender address=reporter />
    </div>
}

module SetWithdrawAddress = {
  @react.component
  let make = (~withdrawAddress) =>
    <div
      className={Css.merge(list{
        CssHelper.flexBox(~wrap=#nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      })}>
      <Text value=j` to ` size=Text.Md nowrap=true block=true />
      <AddressRender address=withdrawAddress />
    </div>
}
