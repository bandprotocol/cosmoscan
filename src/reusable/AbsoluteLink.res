module Styles = {
  open Css

  let a = (theme: Theme.t) =>
    style(list{textDecoration(#none), color(theme.textPrimary), hover(list{color(theme.baseBlue)})})
}

@react.component
let make = (~href, ~className="", ~children) => {
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
  <a href className={Css.merge(list{Styles.a(theme), className})} target="_blank" rel="noopener">
    children
  </a>
}
