module Styles = {
  open Css

  let containerBase = (pv, ph, theme: Theme.t) =>
    style(list{
      backgroundColor(theme.secondaryBg),
      borderTop(#px(1), #solid, theme.tableRowBorderColor),
      padding2(~v=pv, ~h=ph),
      overflow(#hidden),
    })
}

@react.component
let make = (~children, ~paddingV=#px(20), ~paddingH=#zero) => {
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
  <div className={Css.merge(list{Styles.containerBase(paddingV, paddingH, theme)})}> children </div>
}
