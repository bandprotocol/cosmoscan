module Styles = {
  open Css;

  let infoContainer = (theme: Theme.t) =>
    style([
      backgroundColor(theme.secondaryBg),
      borderRadius(`px(12)),
      boxShadow(
        Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.08))),
      ),
      padding(`px(32)),
      position(`relative),
      Media.mobile([padding(`px(16))]),
    ]);
};

[@react.component]
let make = (~children, ~style="") => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div className={Css.merge([Styles.infoContainer(theme), style])}> children </div>;
};
