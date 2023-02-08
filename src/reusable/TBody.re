module Styles = {
  open Css;

  let containerBase = (pv, ph, theme: Theme.t,isDarkMode) =>
    style([
      backgroundColor(isDarkMode ? theme.neutral_100 : theme.neutral_000),
      borderTop(`px(1), `solid, isDarkMode ? theme.neutral_300 : theme.neutral_100),
      padding2(~v=pv, ~h=ph),
      overflow(`hidden),
    ]);
};

[@react.component]
let make = (~children, ~paddingV=`px(20), ~paddingH=`zero) => {
  let ({ThemeContext.theme,isDarkMode}, _) = React.useContext(ThemeContext.context);
  <div className={Css.merge([Styles.containerBase(paddingV, paddingH, theme,isDarkMode)])}> children </div>;
};
