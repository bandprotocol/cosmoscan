module Styles = {
  open Css
  let buttonContainer = style(list{Media.mobile(list{width(#percent(100.))})})
  let baseBtn = style(list{
    textAlign(#center),
    Media.mobile(list{flexGrow(0.), flexShrink(0.), flexBasis(#percent(50.))}),
  })

  let activeBtn = (isActive, theme: Theme.t, isDarkMode) =>
    style(list{
      borderTopRightRadius(#zero),
      borderBottomRightRadius(#zero),
      backgroundColor(isActive ? theme.textPrimary : theme.secondaryBg),
      color(isActive ? theme.secondaryBg : theme.textPrimary),
      hover(list{
        backgroundColor(isActive ? theme.textPrimary : theme.secondaryBg),
        color(isActive ? theme.secondaryBg : theme.textPrimary),
      }),
    })
  let inActiveBtn = (isActive, theme: Theme.t, isDarkMode) =>
    style(list{
      borderTopLeftRadius(#zero),
      borderBottomLeftRadius(#zero),
      color(isActive ? theme.textPrimary : theme.secondaryBg),
      backgroundColor(isActive ? theme.secondaryBg : theme.textPrimary),
      hover(list{
        backgroundColor(isActive ? theme.secondaryBg : theme.textPrimary),
        color(isActive ? theme.textPrimary : theme.secondaryBg),
      }),
    })
}

@react.component
let make = (~isActive, ~setIsActive) => {
  let ({ThemeContext.theme: theme, isDarkMode}, _) = React.useContext(ThemeContext.context)

  <div className={Css.merge(list{CssHelper.flexBox(), Styles.buttonContainer})}>
    <Button
      px=30
      py=8
      variant=Button.Outline
      onClick={_ => setIsActive(_ => true)}
      style={Css.merge(list{Styles.baseBtn, Styles.activeBtn(isActive, theme, isDarkMode)})}>
      {"Active" |> React.string}
    </Button>
    <Button
      px=30
      py=8
      variant=Button.Outline
      onClick={_ => setIsActive(_ => false)}
      style={Css.merge(list{Styles.baseBtn, Styles.inActiveBtn(isActive, theme, isDarkMode)})}>
      {"Inactive" |> React.string}
    </Button>
  </div>
}
