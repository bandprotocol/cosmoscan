module Styles = {
  open Css;
  let msgBadge =
    style([
      backgroundColor(Theme.baseBlue),
      borderRadius(`px(50)),
      margin2(~v=`zero, ~h=`px(5)),
      padding2(~v=`px(3), ~h=`px(8)),
      whiteSpace(`pre),
      display(`inlineFlex),
    ]);
};

[@react.component]
let make = (~name) => {
  <div
    className={Css.merge([
      CssHelper.flexBox(~wrap=`nowrap, ~justify=`center, ()),
      Styles.msgBadge,
    ])}>
    <Text value=name size=Text.Sm color=Theme.white transform=Text.Uppercase align=Text.Center />
  </div>;
};
