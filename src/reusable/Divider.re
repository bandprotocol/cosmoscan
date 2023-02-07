module Styles = {
  open Css;

  let container = (theme: Theme.t, ml, mr) => {
    style([
      height(`px(32)),
      width(`px(1)),
      marginLeft(`px(ml)),
      marginRight(`px(mr)),
      backgroundColor(theme.neutral_300),
      Media.mobile([marginTop(`px(15))]),
    ]);
  };
};

[@react.component]
let make = (~ml=0, ~mr=0) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div className={Styles.container(theme, ml, mr)} />;
};
