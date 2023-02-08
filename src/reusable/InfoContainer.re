module Styles = {
  open Css;

  let infoContainer = ( px, py) =>
    style([
      padding2(~v=`px(py), ~h=`px(px)),
      position(`relative),
      Media.mobile([padding(`px(16))]),
    ]);
};

[@react.component]
let make = (~children, ~px=32, ~py=32, ~style="") => {
  let ({ThemeContext.theme,isDarkMode}, _) = React.useContext(ThemeContext.context);
  <div className={Css.merge([Styles.infoContainer(px, py), style,CommonStyles.card(theme,isDarkMode)])}> children </div>;
};
