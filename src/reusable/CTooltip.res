type placement =
  | Left
  | Right
  | Top
  | Bottom
  | BottomLeft
  | BottomRight

module Styles = {
  open Css
  let tooltipContainer = style(list{
    fontSize(#px(10)),
    cursor(#pointer),
    position(#relative),
    display(#block),
    hover(list{
      selector("> div:nth-child(1)", list{opacity(1.), zIndex(99), pointerEvents(#unset)}),
    }),
  })

  let hiddenTooltipSm = style(list{Media.mobile(list{display(#none)})})
  let tooltipItem = (maxWidth_, padding_, align_, fsize_) =>
    style(list{
      position(#absolute),
      display(#block),
      fontSize(#px(fsize_)),
      textAlign(align_),
      backgroundColor(Colors.gray7),
      borderRadius(#px(4)),
      maxWidth(#px(maxWidth_)),
      unsafe("width", "max-content"),
      color(Colors.white),
      padding(#px(padding_)),
      opacity(0.),
      pointerEvents(#none),
      transition(~duration=200, "all"),
      before(list{
        contentRule(#text("")),
        pointerEvents(#none),
        display(#block),
        position(#absolute),
        border(#px(5), #solid, Colors.transparent),
      }),
      Media.mobile(list{padding(#px(12)), fontSize(#px(12)), maxWidth(#px(150))}),
    })

  let placement = x =>
    switch x {
    | Left =>
      style(list{
        top(#percent(50.)),
        right(#percent(100.)),
        left(#px(-200)),
        transform(#translateY(#percent(-50.))),
        before(list{
          top(#percent(50.)),
          transform(#translateY(#percent(-50.))),
          right(#px(-10)),
          borderLeftColor(Colors.gray7),
        }),
      })
    | Right =>
      style(list{
        top(#percent(50.)),
        left(#percent(100.)),
        right(#px(-200)),
        transform(#translateY(#percent(-50.))),
        before(list{
          top(#percent(50.)),
          transform(#translateY(#percent(-50.))),
          left(#px(-10)),
          borderRightColor(Colors.gray7),
        }),
      })
    | Top =>
      style(list{
        bottom(#percent(100.)),
        left(#percent(50.)),
        right(#px(-200)),
        transform(#translateX(#percent(-50.))),
        before(list{
          transform(#translateX(#percent(-50.))),
          left(#percent(50.)),
          bottom(#px(-10)),
          borderTopColor(Colors.gray7),
        }),
      })
    | Bottom =>
      style(list{
        top(#percent(100.)),
        left(#percent(50.)),
        right(#px(-200)),
        transform(#translateX(#percent(-50.))),
        before(list{
          transform(#translateX(#percent(-50.))),
          left(#percent(50.)),
          top(#px(-10)),
          borderBottomColor(Colors.gray7),
        }),
      })
    | BottomLeft =>
      style(list{
        top(#percent(100.)),
        left(#percent(50.)),
        transform(#translateX(#percent(-30.))),
        before(list{
          transform(#translateX(#percent(-50.))),
          left(#percent(30.)),
          top(#px(-10)),
          borderBottomColor(Colors.gray7),
        }),
      })
    | BottomRight =>
      style(list{
        top(#percent(100.)),
        left(#percent(50.)),
        transform(#translateX(#percent(-70.))),
        before(list{
          transform(#translateX(#percent(-70.))),
          left(#percent(70.)),
          top(#px(-10)),
          borderBottomColor(Colors.gray7),
        }),
      })
    }

  let placementSm = x =>
    switch x {
    | Left =>
      style(list{
        Media.mobile(list{
          top(#percent(50.)),
          right(#percent(100.)),
          transform(#translateY(#percent(-50.))),
          before(list{
            top(#percent(50.)),
            transform(#translateY(#percent(-50.))),
            right(#px(-10)),
            borderLeftColor(Colors.gray7),
          }),
        }),
      })
    | Right =>
      style(list{
        Media.mobile(list{
          top(#percent(50.)),
          left(#percent(100.)),
          transform(#translateY(#percent(-50.))),
          before(list{
            top(#percent(50.)),
            transform(#translateY(#percent(-50.))),
            left(#px(-10)),
            borderRightColor(Colors.gray7),
          }),
        }),
      })
    | Top =>
      style(list{
        Media.mobile(list{
          bottom(#percent(100.)),
          left(#percent(50.)),
          transform(#translateX(#percent(-50.))),
          before(list{
            transform(#translateX(#percent(-50.))),
            left(#percent(50.)),
            bottom(#px(-10)),
            borderTopColor(Colors.gray7),
          }),
        }),
      })
    | Bottom =>
      style(list{
        Media.mobile(list{
          top(#percent(100.)),
          left(#percent(50.)),
          transform(#translateX(#percent(-50.))),
          before(list{
            transform(#translateX(#percent(-50.))),
            left(#percent(50.)),
            top(#px(-10)),
            borderBottomColor(Colors.gray7),
          }),
        }),
      })
    | BottomLeft =>
      style(list{
        Media.mobile(list{
          top(#percent(100.)),
          left(#percent(50.)),
          transform(#translateX(#percent(-30.))),
          before(list{
            transform(#translateX(#percent(-50.))),
            left(#percent(30.)),
            top(#px(-10)),
            borderBottomColor(Colors.gray7),
          }),
        }),
      })
    | BottomRight =>
      style(list{
        Media.mobile(list{
          top(#percent(100.)),
          left(#percent(50.)),
          transform(#translateX(#percent(-70.))),
          before(list{
            transform(#translateX(#percent(-50.))),
            left(#percent(70.)),
            top(#px(-10)),
            borderBottomColor(Colors.gray7),
          }),
        }),
      })
    }
}

@react.component
let make = (
  ~width=200,
  ~pd=10,
  ~fsize=12,
  ~align=#left,
  ~tooltipText="",
  ~tooltipPlacement=Bottom,
  ~tooltipPlacementSm=tooltipPlacement,
  ~mobile=true,
  ~styles="",
  ~children,
) =>
  <div className={Css.merge(list{Styles.tooltipContainer, styles})}>
    <div
      className={Css.merge(list{
        Styles.tooltipItem(width, pd, align, fsize),
        Styles.placement(tooltipPlacement),
        Styles.placementSm(tooltipPlacementSm),
        mobile ? "" : Styles.hiddenTooltipSm,
      })}>
      {tooltipText |> React.string}
    </div>
    children
  </div>
