module Styles = {
  open Css
  let emptyContainer = style(list{
    justifyContent(#center),
    alignItems(#center),
    flexDirection(#column),
    width(#percent(100.)),
    Media.mobile(list{minHeight(#px(200))}),
  })

  let height = he => style(list{height(he)})
  let display = dp => style(list{display(dp ? #flex : #none)})
  let backgroundColor = bc => style(list{backgroundColor(bc)})
  let boxShadow = style(list{
    boxShadow(Shadow.box(~x=#zero, ~y=#px(2), ~blur=#px(4), Css.rgba(0, 0, 0, #num(0.08)))),
  })
}

@react.component
let make = (~height=#px(300), ~display=true, ~backgroundColor=?, ~boxShadow=false, ~children) => {
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
  <div
    className={Css.merge(list{
      Styles.emptyContainer,
      Styles.height(height),
      Styles.display(display),
      Styles.backgroundColor(backgroundColor->Belt.Option.getWithDefault(theme.secondaryBg)),
      boxShadow ? Styles.boxShadow : "",
    })}>
    children
  </div>
}
