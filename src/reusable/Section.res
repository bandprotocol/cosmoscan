module Styles = {
  open Css

  let base = (~pt, ~pb, ()) => style(list{paddingTop(#px(pt)), paddingBottom(#px(pb))})

  let mobile = (~ptSm, ~pbSm, ()) =>
    style(list{Media.mobile(list{paddingTop(#px(ptSm)), paddingBottom(#px(pbSm))})})

  let bgColor = color => style(list{backgroundColor(color)})
}

@react.component
let make = (~children, ~pt=60, ~pb=60, ~ptSm=32, ~pbSm=32, ~bg=?, ~style="") => {
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
  let css = Css.merge(list{
    Styles.bgColor(bg->Belt.Option.getWithDefault(theme.mainBg)),
    Styles.base(~pt, ~pb, ()),
    Styles.mobile(~ptSm, ~pbSm, ()),
    style,
  })

  <section className=css> children </section>
}
