module Styles = {
  open Css;

  let containerBase = (height_, theme: Theme.t) =>
    style([backgroundColor(theme.secondaryBg), selector("> div", [height(`px(height_))])]);
};

[@react.component]
let make = (~children, ~height=40) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <div className={Styles.containerBase(height, theme)}> children </div>;
};
