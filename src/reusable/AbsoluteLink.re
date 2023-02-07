module Styles = {
  open Css;

  let a = (theme: Theme.t) =>
    style([textDecoration(`none), color(theme.neutral_900), hover([color(theme.primary_600)])]);
};

[@react.component]
let make = (~href, ~className="", ~children) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <a href className={Css.merge([Styles.a(theme), className])} target="_blank" rel="noopener">
    children
  </a>;
};
