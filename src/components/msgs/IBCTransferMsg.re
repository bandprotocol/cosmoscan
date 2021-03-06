module Styles = {
  open Css;

  let msgContainer = style([selector("> * + *", [marginLeft(`px(5))])]);
};

module Transfer = {
  [@react.component]
  let make = (~toAddress, ~amount, ~denom) => {
    <div
      className={Css.merge([
        CssHelper.flexBox(~wrap=`nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      ])}>
      <Text
        value={amount |> Format.fPretty(~digits=4)}
        code=true
        size=Text.Md
        nowrap=true
        block=true
      />
      <Text value=denom size=Text.Md nowrap=true block=true />
      <Text value={j| to |j} size=Text.Md nowrap=true block=true />
      <Text value=toAddress size=Text.Md nowrap=true block=true code=true />
    </div>;
  };
};
