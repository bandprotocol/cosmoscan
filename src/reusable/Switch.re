module Styles = {
  open Css;

  let container = style([position(`relative), width(`px(32)), marginLeft(`px(18))]);
  let slide = checked =>
    style([
      position(`absolute),
      top(`px(-2)),
      left(`zero),
      width(`px(32)),
      height(`px(4)),
      background(checked ? `hex("4520E6") : `hex("353535")),
      borderRadius(`px(4)),
    ]);
  let button = (checked, theme: Theme.t) =>
    style([
      position(`absolute),
      top(`px(-8)),
      left(`px(-8)),
      width(`px(16)),
      height(`px(16)),
      borderRadius(`percent(50.)),
      background(theme.textPrimary),
      transform(`translateX(checked ? `percent(200.) : `percent(0.))),
      transition(~duration=200, "all"),
    ]);
};

[@react.component]
let make = (~checked) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <div className=Styles.container>
    <div className={Styles.slide(checked)} />
    <div className={Styles.button(checked, theme)} />
  </div>;
};
