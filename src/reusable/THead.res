module Styles = {
  open Css

  let containerBase = (height_, theme: Theme.t) =>
    style(list{backgroundColor(theme.secondaryBg), selector("> div", list{height(#px(height_))})})
}

@react.component
let make = (~children, ~height=40) => {
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

  <div className={Styles.containerBase(height, theme)}> children </div>
}
