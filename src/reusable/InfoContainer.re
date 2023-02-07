module Styles = {
  open Css;

  let infoContainer = (theme: Theme.t, px, py) =>
    style([
      backgroundColor(theme.neutral_100),
      borderRadius(`px(12)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.2)))),
      padding2(~v=`px(py), ~h=`px(px)),
      position(`relative),
      Media.mobile([padding(`px(16))]),
    ]);
};

[@react.component]
let make = (~children, ~px=32, ~py=32, ~style="") => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div className={Css.merge([Styles.infoContainer(theme, px, py), style])}> children </div>;
};
