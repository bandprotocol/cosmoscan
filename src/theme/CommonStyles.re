open Css

let card = (theme: Theme.t,isDarkMode) =>
    style([
      backgroundColor(isDarkMode ? theme.neutral_100 : theme.neutral_000),
      border(`px(1), `solid, theme.neutral_100),
      borderRadius(`px(8)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.2)))),
    ]);
