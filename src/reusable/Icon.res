module Styles = {
  open Css

  let fontSize = size => style(list{fontSize(#px(size))})
  let fontColor = color_ => style(list{color(color_)})
  let margin = (mr, ml, mt, mb) =>
    style(list{
      marginRight(#px(mr)),
      marginLeft(#px(ml)),
      marginTop(#px(mt)),
      marginBottom(#px(mb)),
    })
}

@react.component
let make = (~name, ~color=?, ~size=12, ~mr=0, ~ml=0, ~mt=0, ~mb=0) => {
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
  <i
    className={Css.merge(list{
      name,
      Styles.fontColor(color->Belt_Option.getWithDefault(theme.textSecondary)),
      Styles.fontSize(size),
      Styles.margin(mr, ml, mt, mb),
    })}
  />
}
