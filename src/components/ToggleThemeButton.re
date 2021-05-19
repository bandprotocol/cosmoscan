module Styles = {
  open Css;

  let button = (isDarkMode, theme: Theme.t) =>
    style([
      backgroundColor(isDarkMode ? theme.white : theme.black),
      padding2(~v=`px(8), ~h=`px(10)),
      borderRadius(`px(8)),
      border(`px(1), `solid, isDarkMode ? theme.white : theme.black),
      cursor(`pointer),
      outlineStyle(`none),
    ]);
};

[@react.component]
let make = () => {
  let ({ThemeContext.isDarkMode, theme}, toggle) = React.useContext(ThemeContext.context);

  <button className={Styles.button(isDarkMode, theme)} onClick={_ => toggle()}>
    <Icon
      name={isDarkMode ? "fal fa-sun" : "fal fa-moon"}
      size=14
      color={isDarkMode ? theme.black : theme.white}
    />
  </button>;
};
