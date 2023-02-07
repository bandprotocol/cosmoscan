module Styles = {
  open Css;

  let container = (theme: Theme.t, mt, mb) => {
    style([
      width(`percent(100.)),
      marginTop(`px(mt)),
      marginBottom(`px(mb)),
      height(`px(1)),
      backgroundColor(theme.neutral_100),
      Media.mobile([marginTop(`px(15))]),
    ]);
  };
};

[@react.component]
let make = (~mt=15, ~mb=15) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div className={Styles.container(theme, mt, mb)} />;
};
