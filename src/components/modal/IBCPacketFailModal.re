module Styles = {
  open Css;
  let container =
    style([
      width(`percent(100.)),
      minWidth(`px(468)),
      maxWidth(`px(468)),
      minHeight(`px(150)),
      padding(`px(40)),
      Media.mobile([maxWidth(`px(300)), minWidth(`px(300))]),
    ]);

  let jsonDisplay = (theme: Theme.t) =>
    style([
      padding(`px(10)),
      resize(`none),
      fontSize(`px(12)),
      color(theme.textPrimary),
      backgroundColor(theme.contrastBg),
      border(`px(1), `solid, theme.tableRowBorderColor),
      borderRadius(`px(4)),
      width(`percent(100.)),
      height(`px(130)),
      overflowY(`scroll),
      marginBottom(`px(16)),
      fontFamilies([
        `custom("IBM Plex Mono"),
        `custom("cousine"),
        `custom("sfmono-regular"),
        `custom("Consolas"),
        `custom("Menlo"),
        `custom("liberation mono"),
        `custom("ubuntu mono"),
        `custom("Courier"),
        `monospace,
      ]),
    ]);
};

[@react.component]
let make = (~reason) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div className=Styles.container>
    <Heading size=Heading.H4 value="Error" marginBottom=30 color={theme.failColor} />
    <textarea className={Styles.jsonDisplay(theme)} disabled=true defaultValue=reason />
  </div>;
};
