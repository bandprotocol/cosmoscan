open Css

let flexBox = (~align=#center, ~justify=#flexStart, ~wrap=#wrap, ~direction=#row, ()) =>
  style(list{
    display(#flex),
    alignItems(align),
    justifyContent(justify),
    flexDirection(direction),
    flexWrap(wrap),
  })
let flexBoxSm = (~align=#center, ~justify=#flexStart, ~wrap=#wrap, ~direction=#row, ()) =>
  style(list{
    Media.mobile(list{
      display(#flex),
      alignItems(align),
      justifyContent(justify),
      flexDirection(direction),
      flexWrap(wrap),
    }),
  })

let mobileSpacing = style(list{Media.mobile(list{paddingBottom(#px(20))})})

let clickable = style(list{cursor(#pointer)})

let container = "container"

let mb = (~size=8, ()) => style(list{marginBottom(#px(size))})
let mbSm = (~size=8, ()) => style(list{Media.mobile(list{marginBottom(#px(size))})})

let mt = (~size=8, ()) => style(list{marginTop(#px(size))})

let mtSm = (~size=8, ()) => style(list{Media.mobile(list{marginTop(#px(size))})})

let px = (~size=0, ()) => style(list{paddingLeft(#px(size)), paddingRight(#px(size))})

let pxSm = (~size=0, ()) =>
  style(list{Media.mobile(list{paddingLeft(#px(size)), paddingRight(#px(size))})})

// Angle Icon on select input

let selectWrapper = (~size=14, ~pRight=16, ~pRightSm=pRight, ~mW=500, ~fontColor=Theme.black, ()) =>
  style(list{
    position(#relative),
    width(#percent(100.)),
    maxWidth(#px(mW)),
    after(list{
      contentRule(#text("\f107")),
      fontFamily(#custom("'Font Awesome 5 Pro'")),
      fontSize(#px(size)),
      lineHeight(#px(1)),
      display(#block),
      position(#absolute),
      pointerEvents(#none),
      top(#percent(50.)),
      right(#px(pRight)),
      color(fontColor),
      transform(#translateY(#percent(-50.))),
      Media.mobile(list{right(#px(pRightSm))}),
    }),
  })

// Informations

let infoContainer = style(list{
  backgroundColor(Colors.white),
  boxShadow(Shadow.box(~x=#zero, ~y=#px(2), ~blur=#px(4), Css.rgba(0, 0, 0, #num(0.08)))),
  padding(#px(24)),
  Media.mobile(list{padding(#px(16))}),
})

// Helper

let overflowHidden = style(list{overflow(#hidden)})
