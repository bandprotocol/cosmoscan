module Styles = {
  open Css;

  let containerBase = (theme: Theme.t) =>
    style([
      backgroundColor(theme.neutral_100),
      borderRadius(`px(8)),
      overflow(`hidden),
      padding2(~v=`zero, ~h=`px(32)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.2)))),
      Media.mobile([padding2(~v=`zero, ~h=`px(15))]),
    ]);
};

[@react.component]
let make = (~children) => {
  let ({ThemeContext.theme,isDarkMode}, _) = React.useContext(ThemeContext.context);
  <div className={Css.merge([Styles.containerBase(theme),CommonStyles.card(theme,isDarkMode)])}> children </div>;
};
