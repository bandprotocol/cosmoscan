type size =
  | Xs
  | Sm
  | Md
  | Lg
  | Xl
  | Xxl
  | Xxxl

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

type spacing =
  | Unset
  | Em(float)

type lineHeight =
  | Px(int)
  | PxFloat(float)

type transform =
  | Uppercase
  | Capitalize
  | Lowercase
  | Normal

type placement =
  | AlignBottomEnd
  | AlignBottomStart
  | AlignBottom
  | AlignLeftEnd
  | AlignLeftStart
  | AlignLeft
  | AlignRightEnd
  | AlignRightStart
  | AlignRight
  | AlignTopEnd
  | AlignTopStart
  | AlignTop

let toPlacementString = x =>
  switch x {
  | AlignBottomEnd => "bottom-end"
  | AlignBottomStart => "bottom-start"
  | AlignBottom => "bottom"
  | AlignLeftEnd => "left-end"
  | AlignLeftStart => "left-start"
  | AlignLeft => "left"
  | AlignRightEnd => "right-end"
  | AlignRightStart => "right-start"
  | AlignRight => "right"
  | AlignTopEnd => "top-end"
  | AlignTopStart => "top-start"
  | AlignTop => "top"
  }

module Styles = {
  open Css
  open Belt.Option

  let fontSize = mapWithDefault(
    _,
    style(list{fontSize(#px(12)), lineHeight(#px(16)), Media.smallMobile(list{fontSize(#px(10))})}),
    x =>
      switch x {
      | Xs =>
        style(list{
          fontSize(#px(8)),
          lineHeight(#em(1.41)),
          Media.smallMobile(list{fontSize(#px(7))}),
        })
      | Sm =>
        style(list{
          fontSize(#px(10)),
          lineHeight(#em(1.41)),
          Media.smallMobile(list{fontSize(#px(8))}),
        })
      | Md =>
        style(list{
          fontSize(#px(12)),
          lineHeight(#em(1.41)),
          Media.smallMobile(list{fontSize(#px(10))}),
        })
      | Lg =>
        style(list{
          fontSize(#px(14)),
          lineHeight(#em(1.41)),
          Media.smallMobile(list{fontSize(#px(12))}),
        })
      | Xl =>
        style(list{
          fontSize(#px(16)),
          lineHeight(#em(1.41)),
          Media.smallMobile(list{fontSize(#px(14))}),
        })
      | Xxl => style(list{fontSize(#px(18)), Media.smallMobile(list{fontSize(#px(16))})})
      | Xxxl => style(list{fontSize(#px(24)), Media.smallMobile(list{fontSize(#px(22))})})
      },
  )

  let fontWeight = mapWithDefault(_, style(list{}), x =>
    switch x {
    | Thin => style(list{fontWeight(#num(300))})
    | Regular => style(list{fontWeight(#num(400))})
    | Medium => style(list{fontWeight(#num(500))})
    | Semibold => style(list{fontWeight(#num(600))})
    | Bold => style(list{fontWeight(#num(700))})
    }
  )

  let lineHeight = mapWithDefault(_, style(list{}), x =>
    switch x {
    | Px(height) => style(list{lineHeight(#px(height))})
    | PxFloat(height) => style(list{lineHeight(#pxFloat(height))})
    }
  )

  let letterSpacing = mapWithDefault(_, style(list{letterSpacing(#unset)}), x =>
    switch x {
    | Unset => style(list{letterSpacing(#unset)})
    | Em(spacing) => style(list{letterSpacing(#em(spacing))})
    }
  )

  let noWrap = style(list{whiteSpace(#nowrap)})
  let block = style(list{display(#block)})
  let ellipsis = style(list{
    overflow(#hidden),
    textOverflow(#ellipsis),
    whiteSpace(#nowrap),
    width(#auto),
  })
  let underline = style(list{textDecoration(#underline)})
  let textAlign = mapWithDefault(_, style(list{textAlign(#left)}), x =>
    switch x {
    | Center => style(list{textAlign(#center)})
    | Right => style(list{textAlign(#right)})
    | Left => style(list{textAlign(#left)})
    }
  )

  let code = style(list{
    fontFamilies(list{#custom("Roboto Mono"), #monospace}),
    paddingBottom(#em(0.1)),
  })

  let special = style(list{fontFamilies(list{#custom("Lexend Exa"), #monospace})})

  let textTransform = x =>
    switch x {
    | Uppercase => style(list{textTransform(#uppercase)})
    | Lowercase => style(list{textTransform(#lowercase)})
    | Capitalize => style(list{textTransform(#capitalize)})
    | Normal => style(list{textTransform(#unset)})
    }

  let breakAll = style(list{wordBreak(#breakAll)})

  let textColor = color_ => style(list{color(color_)})
}

@react.component
let make = (
  ~size=?,
  ~weight=?,
  ~align=?,
  ~spacing=?,
  ~height=?,
  ~nowrap=false,
  ~color=?,
  ~block=false,
  ~code=false,
  ~ellipsis=false,
  ~underline=false,
  ~breakAll=false,
  ~transform=Normal,
  ~value,
  ~tooltipItem=React.null,
  ~tooltipPlacement=AlignBottom,
  ~tooltipLeaveDelay=100,
  ~special=false,
) => {
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

  tooltipItem == React.null
    ? <span
        className={Css.merge(list{
          Styles.fontSize(size),
          Styles.fontWeight(weight),
          Styles.textAlign(align),
          Styles.letterSpacing(spacing),
          Styles.lineHeight(height),
          Styles.textTransform(transform),
          Styles.textColor(color->Belt.Option.getWithDefault(theme.textSecondary)),
          nowrap ? Styles.noWrap : "",
          block ? Styles.block : "",
          code ? Styles.code : "",
          special ? Styles.special : "",
          ellipsis ? Styles.ellipsis : "",
          underline ? Styles.underline : "",
          breakAll ? Styles.breakAll : "",
        })}>
        {React.string(value)}
      </span>
    : <Tooltip
        title=tooltipItem
        placement={tooltipPlacement |> toPlacementString}
        arrow=true
        leaveDelay=tooltipLeaveDelay>
        <span
          className={Css.merge(list{
            Styles.fontSize(size),
            Styles.fontWeight(weight),
            Styles.textAlign(align),
            Styles.letterSpacing(spacing),
            Styles.lineHeight(height),
            Styles.textTransform(transform),
            Styles.textColor(color->Belt.Option.getWithDefault(theme.textSecondary)),
            nowrap ? Styles.noWrap : "",
            block ? Styles.block : "",
            code ? Styles.code : "",
            ellipsis ? Styles.ellipsis : "",
            underline ? Styles.underline : "",
            breakAll ? Styles.breakAll : "",
          })}>
          {React.string(value)}
        </span>
      </Tooltip>
}
