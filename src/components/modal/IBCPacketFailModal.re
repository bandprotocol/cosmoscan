module Styles = {
  open Css;
  let container =
    style([
      width(`percent(100.)),
      maxWidth(`px(468)),
      minHeight(`px(150)),
      padding(`px(40)),
      Media.mobile([maxWidth(`px(300))]),
    ]);
};

[@react.component]
let make = (~reason) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div className=Styles.container>
    <Heading size=Heading.H4 value="Error" marginBottom=30 color={theme.failColor} />
    <Text value=reason size=Text.Lg color={theme.textPrimary} />
  </div>;
};
