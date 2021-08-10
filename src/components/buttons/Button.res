type btn_style_t =
  | Primary
  | Outline

module Styles = {
  open Css

  let btn = (
    ~variant=Primary,
    ~fsize=12,
    ~px=25,
    ~py=13,
    ~pxSm=px,
    ~pySm=py,
    theme: Theme.t,
    isDarkMode,
    (),
  ) => {
    let base = style(list{
      display(#block),
      padding2(~v=#px(py), ~h=#px(px)),
      transition(~duration=200, "all"),
      borderRadius(#px(8)),
      fontSize(#px(fsize)),
      fontWeight(#num(600)),
      cursor(#pointer),
      outlineStyle(#none),
      borderStyle(#none),
      margin(#zero),
      disabled(list{cursor(#default)}),
      Media.mobile(list{padding2(~v=#px(pySm), ~h=#px(pxSm))}),
    })

    let custom = switch variant {
    | Primary =>
      style(list{
        backgroundColor(theme.baseBlue),
        color(Theme.white),
        border(#px(1), #solid, theme.baseBlue),
        hover(list{backgroundColor(theme.darkBlue)}),
        active(list{backgroundColor(theme.darkenBlue)}),
        disabled(list{
          backgroundColor(isDarkMode ? theme.darkBlue : theme.lightBlue),
          color(Theme.white),
          borderColor(isDarkMode ? theme.darkBlue : theme.lightBlue),
          opacity(0.5),
        }),
      })
    | Outline =>
      style(list{
        backgroundColor(#transparent),
        color(theme.textPrimary),
        border(#px(1), #solid, theme.textPrimary),
        selector("i", list{color(theme.textPrimary)}),
        hover(list{
          backgroundColor(theme.textPrimary),
          color(isDarkMode ? Theme.black : Theme.white),
          selector("i", list{color(isDarkMode ? Theme.black : Theme.white)}),
        }),
        active(list{backgroundColor(Colors.buttonOutlineActive)}),
        disabled(list{
          borderColor(theme.textSecondary),
          color(theme.textSecondary),
          hover(list{backgroundColor(#transparent)}),
          opacity(0.5),
        }),
      })
    }
    merge(list{base, custom})
  }
}

@react.component
let make = (
  ~variant=Primary,
  ~children,
  ~py=8,
  ~px=16,
  ~fsize=12,
  ~pySm=py,
  ~pxSm=px,
  ~onClick,
  ~style="",
  ~disabled=false,
) => {
  let ({ThemeContext.theme: theme, isDarkMode}, _) = React.useContext(ThemeContext.context)

  <button
    className={Css.merge(list{
      Styles.btn(~variant, ~px, ~py, ~pxSm, ~pySm, ~fsize, theme, isDarkMode, ()),
      CssHelper.flexBox(~align=#center, ~justify=#center, ()),
      style,
    })}
    onClick
    disabled>
    children
  </button>
}
