module Styles = {
  open Css;

  let containerBase = (pv, ph, theme: Theme.t) =>
    style([
      backgroundColor(theme.neutral_100),
      borderTop(`px(1), `solid, theme.neutral_100),
      padding2(~v=pv, ~h=ph),
      overflow(`hidden),
    ]);
};

[@react.component]
let make = (~children, ~paddingV=`px(20), ~paddingH=`zero) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div className={Css.merge([Styles.containerBase(paddingV, paddingH, theme)])}> children </div>;
};
