module Styles = {
  open Css;

  let button = (isDarkMode, theme: Theme.t) =>
    style([
      backgroundColor(isDarkMode ? theme.white : theme.black),
      position(`relative),
      borderRadius(`px(8)),
      border(`px(1), `solid, isDarkMode ? theme.white : theme.black),
      cursor(`pointer),
      outlineStyle(`none),
      width(`px(32)),
      height(`px(32)),
      selector("> *",[position(`absolute), top(`percent(50.)), left(`percent(50.)), transform(translate(`percent(-50.), `percent(-50.))),])
    ]);

  let icon = style([width(`px(20)), height(`px(20))])
};

[@react.component]
let make = () => {
  let ({ThemeContext.isDarkMode, theme}, toggle) = React.useContext(ThemeContext.context);

  <button className={Styles.button(isDarkMode, theme)} onClick={_ => toggle()}>
    {
        isDarkMode ? <img src=Images.sunIcon  className={Styles.icon}/> :
        <Icon name="fal fa-moon" size=14 color=theme.white /> 
    }
    
  </button>;
};
