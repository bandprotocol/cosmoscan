module Styles = {
  open Css;
  let withWidth = (w, theme: Theme.t) =>
    style([
      display(`flex),
      maxWidth(`px(w)),
      cursor(`pointer),
      selector("> span:hover", [color(theme.baseBlue)]),
      selector("> span", [transition(~duration=200, "all")]),
    ]);
};

[@react.component]
let make = (~txHash: Hash.t, ~width: int, ~size=Text.Md, ~weight=Text.Medium) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <Link className={Styles.withWidth(width, theme)} route={Route.TxIndexPage(txHash)}>
    <Text
      block=true
      code=true
      spacing={Text.Em(0.02)}
      value={txHash |> Hash.toHex(~upper=true)}
      weight
      ellipsis=true
      size
      color={theme.neutral_900}
    />
  </Link>;
};
