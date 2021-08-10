module Styles = {
  open Css

  let container = (theme: Theme.t) =>
    style(list{
      selector(
        "a",
        list{
          wordBreak(#breakAll),
          color(theme.textPrimary),
          textDecoration(#none),
          transition(~duration=200, "all"),
          hover(list{color(theme.baseBlue)}),
        },
      ),
      selector("p", list{color(theme.textSecondary)}),
      selector("p + p", list{marginTop(#em(1.))}),
    })
}

@react.component
let make = (~value) => {
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
  <div className={Styles.container(theme)}> {value->MarkedJS.marked->MarkedJS.parse} </div>
}
