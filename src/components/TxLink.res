module Styles = {
  open Css
  let withWidth = (w, theme: Theme.t) =>
    style(list{
      display(#flex),
      maxWidth(#px(w)),
      cursor(#pointer),
      selector("> span:hover", list{color(theme.baseBlue)}),
      selector("> span", list{transition(~duration=200, "all")}),
    })
}

@react.component
let make = (~txHash: Hash.t, ~width: int, ~size=Text.Md, ~weight=Text.Medium) => {
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
  <Link className={Styles.withWidth(width, theme)} route=Route.TxIndexPage(txHash)>
    <Text
      block=true
      code=true
      spacing=Text.Em(0.02)
      value={txHash |> Hash.toHex(~upper=true)}
      weight
      ellipsis=true
      size
      color=theme.textPrimary
    />
  </Link>
}
