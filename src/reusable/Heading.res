type size =
  | H1
  | H2
  | H3
  | H4
  | H5

type weight =
  | Thin
  | Regular
  | Medium
  | Semibold
  | Bold

type align =
  | Center
  | Right
  | Left

module Styles = {
  open Css
  let lineHeight = style(list{lineHeight(#em(1.41))})
  let fontSize = x =>
    switch x {
    | H1 => style(list{fontSize(#px(24)), Media.smallMobile(list{fontSize(#px(20))})})
    | H2 => style(list{fontSize(#px(20)), Media.smallMobile(list{fontSize(#px(18))})})
    | H3 => style(list{fontSize(#px(18)), Media.smallMobile(list{fontSize(#px(16))})})
    | H4 => style(list{fontSize(#px(14)), Media.smallMobile(list{fontSize(#px(12))})})
    | H5 => style(list{fontSize(#px(12)), Media.smallMobile(list{fontSize(#px(11))})})
    }

  let fontWeight = x =>
    switch x {
    | Thin => style(list{fontWeight(#num(300))})
    | Regular => style(list{fontWeight(#num(400))})
    | Medium => style(list{fontWeight(#num(500))})
    | Semibold => style(list{fontWeight(#num(600))})
    | Bold => style(list{fontWeight(#num(700))})
    }

  let textAlign = x =>
    switch x {
    | Center => style(list{textAlign(#center)})
    | Right => style(list{textAlign(#right)})
    | Left => style(list{textAlign(#left)})
    }
  let textColor = color_ => style(list{color(color_)})
  let mb = (~mb, ~mbSm, ()) =>
    style(list{marginBottom(#px(mb)), Media.mobile(list{marginBottom(#px(mbSm))})})
  let mt = (~mt, ~mtSm, ()) =>
    style(list{marginTop(#px(mt)), Media.mobile(list{marginTop(#px(mtSm))})})
}

@react.component
let make = (
  ~value,
  ~align=Left,
  ~weight=Semibold,
  ~size=H1,
  ~marginTop=0,
  ~marginTopSm=marginTop,
  ~marginBottom=0,
  ~marginBottomSm=marginBottom,
  ~style="",
  ~color=?,
) => {
  let children_ = React.string(value)

  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

  let style_ = size =>
    Css.merge({
      open Styles
      list{
        fontSize(size),
        fontWeight(weight),
        textColor(color->Belt.Option.getWithDefault(theme.textPrimary)),
        textAlign(align),
        lineHeight,
        mb(~mb=marginBottom, ~mbSm=marginBottomSm, ()),
        mt(~mt=marginTop, ~mtSm=marginTopSm, ()),
        style,
      }
    })

  switch size {
  | H1 => <h1 className={style_(size)}> children_ </h1>
  | H2 => <h2 className={style_(size)}> children_ </h2>
  | H3 => <h3 className={style_(size)}> children_ </h3>
  | H4 => <h4 className={style_(size)}> children_ </h4>
  | H5 => <h5 className={style_(size)}> children_ </h5>
  }
}
