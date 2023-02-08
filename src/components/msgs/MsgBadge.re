module Styles = {
  open Css;
  let msgBadge = (theme: Theme.t) =>
    style([
      backgroundColor(theme.neutral_700),
      border(`px(1), `solid, theme.neutral_600),
      borderRadius(`px(50)),
      margin2(~v=`zero, ~h=`px(5)),
      padding2(~v=`px(3), ~h=`px(8)),
      whiteSpace(`pre),
      display(`inlineFlex),
    ]);
};

[@react.component]
let make = (~name) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div
    className={Css.merge([
      CssHelper.flexBox(~wrap=`nowrap, ~justify=`center, ()),
      Styles.msgBadge(theme),
    ])}>
    <Text 
      value=name
      size=Text.Md
      weight=Text.Semibold
      color=theme.neutral_000
      transform=Text.Uppercase
      align=Text.Center
    />
  </div>;
};
