type placement =
  | Left
  | Right
  | Top
  | Bottom
  | BottomLeft
  | BottomRight;

module Styles = {
  open Css;
  let tooltipContainer =
    style([
      fontSize(`px(10)),
      cursor(`pointer),
      position(`relative),
      display(`block),
      hover([
        selector("> div:nth-child(1)", [opacity(1.), zIndex(99), pointerEvents(`unset)]),
      ]),
    ]);
  let hiddenTooltipSm = style([Media.mobile([display(`none)])]);
  let tooltipItem = (maxWidth_, padding_, align_, fsize_, theme: Theme.t) =>
    style([
      position(`absolute),
      display(`block),
      fontSize(`px(fsize_)),
      textAlign(align_),
      backgroundColor(`rgba((85, 85, 85, `num(1.0)))),
      borderRadius(`px(4)),
      maxWidth(`px(maxWidth_)),
      unsafe("width", "max-content"),
      color(theme.white),
      padding(`px(padding_)),
      opacity(0.),
      pointerEvents(`none),
      transition(~duration=200, "all"),
      before([
        contentRule(`text("")),
        pointerEvents(`none),
        display(`block),
        position(`absolute),
        border(`px(5), `solid, `rgba((0, 0, 0, `num(0.)))),
      ]),
      Media.mobile([padding(`px(12)), fontSize(`px(12)), maxWidth(`px(150))]),
    ]);

  let placement =
    fun
    | Left =>
      style([
        top(`percent(50.)),
        right(`percent(100.)),
        left(`px(-200)),
        transform(`translateY(`percent(-50.))),
        before([
          top(`percent(50.)),
          transform(`translateY(`percent(-50.))),
          right(`px(-10)),
          borderLeftColor(`rgba((85, 85, 85, `num(1.0)))),
        ]),
      ])
    | Right =>
      style([
        top(`percent(50.)),
        left(`percent(100.)),
        right(`px(-200)),
        transform(`translateY(`percent(-50.))),
        before([
          top(`percent(50.)),
          transform(`translateY(`percent(-50.))),
          left(`px(-10)),
          borderRightColor(`rgba((85, 85, 85, `num(1.0)))),
        ]),
      ])
    | Top =>
      style([
        bottom(`percent(100.)),
        left(`percent(50.)),
        right(`px(-200)),
        transform(`translateX(`percent(-50.))),
        before([
          transform(`translateX(`percent(-50.))),
          left(`percent(50.)),
          bottom(`px(-10)),
          borderTopColor(`rgba((85, 85, 85, `num(1.0)))),
        ]),
      ])
    | Bottom =>
      style([
        top(`percent(100.)),
        left(`percent(50.)),
        right(`px(-200)),
        transform(`translateX(`percent(-50.))),
        before([
          transform(`translateX(`percent(-50.))),
          left(`percent(50.)),
          top(`px(-10)),
          borderBottomColor(`rgba((85, 85, 85, `num(1.0)))),
        ]),
      ])
    | BottomLeft =>
      style([
        top(`percent(100.)),
        left(`percent(50.)),
        transform(`translateX(`percent(-30.))),
        before([
          transform(`translateX(`percent(-50.))),
          left(`percent(30.)),
          top(`px(-10)),
          borderBottomColor(`rgba((85, 85, 85, `num(1.0)))),
        ]),
      ])
    | BottomRight =>
      style([
        top(`percent(100.)),
        left(`percent(50.)),
        transform(`translateX(`percent(-70.))),
        before([
          transform(`translateX(`percent(-70.))),
          left(`percent(70.)),
          top(`px(-10)),
          borderBottomColor(`rgba((85, 85, 85, `num(1.0)))),
        ]),
      ]);

  let placementSm =
    fun
    | Left =>
      style([
        Media.mobile([
          top(`percent(50.)),
          right(`percent(100.)),
          transform(`translateY(`percent(-50.))),
          before([
            top(`percent(50.)),
            transform(`translateY(`percent(-50.))),
            right(`px(-10)),
            borderLeftColor(`rgba((85, 85, 85, `num(1.0)))),
          ]),
        ]),
      ])
    | Right =>
      style([
        Media.mobile([
          top(`percent(50.)),
          left(`percent(100.)),
          transform(`translateY(`percent(-50.))),
          before([
            top(`percent(50.)),
            transform(`translateY(`percent(-50.))),
            left(`px(-10)),
            borderRightColor(`rgba((85, 85, 85, `num(1.0)))),
          ]),
        ]),
      ])
    | Top =>
      style([
        Media.mobile([
          bottom(`percent(100.)),
          left(`percent(50.)),
          transform(`translateX(`percent(-50.))),
          before([
            transform(`translateX(`percent(-50.))),
            left(`percent(50.)),
            bottom(`px(-10)),
            borderTopColor(`rgba((85, 85, 85, `num(1.0)))),
          ]),
        ]),
      ])
    | Bottom =>
      style([
        Media.mobile([
          top(`percent(100.)),
          left(`percent(50.)),
          transform(`translateX(`percent(-50.))),
          before([
            transform(`translateX(`percent(-50.))),
            left(`percent(50.)),
            top(`px(-10)),
            borderBottomColor(`rgba((85, 85, 85, `num(1.0)))),
          ]),
        ]),
      ])
    | BottomLeft =>
      style([
        Media.mobile([
          top(`percent(100.)),
          left(`percent(50.)),
          transform(`translateX(`percent(-30.))),
          before([
            transform(`translateX(`percent(-50.))),
            left(`percent(30.)),
            top(`px(-10)),
            borderBottomColor(`rgba((85, 85, 85, `num(1.0)))),
          ]),
        ]),
      ])
    | BottomRight =>
      style([
        Media.mobile([
          top(`percent(100.)),
          left(`percent(50.)),
          transform(`translateX(`percent(-70.))),
          before([
            transform(`translateX(`percent(-50.))),
            left(`percent(70.)),
            top(`px(-10)),
            borderBottomColor(`rgba((85, 85, 85, `num(1.0)))),
          ]),
        ]),
      ]);
};

[@react.component]
let make =
    (
      ~width=200,
      ~pd=10,
      ~fsize=12,
      ~align=`left,
      ~tooltipText="",
      ~tooltipPlacement=Bottom,
      ~tooltipPlacementSm=tooltipPlacement,
      ~mobile=true,
      ~styles="",
      ~children,
    ) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div className={Css.merge([Styles.tooltipContainer, styles])}>
    <div
      className={Css.merge([
        Styles.tooltipItem(width, pd, align, fsize, theme),
        Styles.placement(tooltipPlacement),
        Styles.placementSm(tooltipPlacementSm),
        mobile ? "" : Styles.hiddenTooltipSm,
      ])}>
      {tooltipText |> React.string}
    </div>
    children
  </div>;
};
