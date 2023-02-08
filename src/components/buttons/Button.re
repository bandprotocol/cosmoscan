type btn_style_t =
  | Primary
  | Outline;

module Styles = {
  open Css;

  let btn =
      (
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
    let base =
      style([
        display(`block),
        padding2(~v=`px(py), ~h=`px(px)),
        transition(~duration=200, "all"),
        borderRadius(`px(8)),
        fontSize(`px(fsize)),
        fontWeight(`num(600)),
        cursor(`pointer),
        outlineStyle(`none),
        borderStyle(`none),
        margin(`zero),
        disabled([cursor(`default)]),
        Media.mobile([padding2(~v=`px(pySm), ~h=`px(pxSm))]),
      ]);

    let custom =
      switch (variant) {
      | Primary =>
        style([
          backgroundColor(theme.primary_600),
          color(Theme.white),
          border(`px(1), `solid, theme.primary_600),
          hover([backgroundColor(theme.darkBlue)]),
          active([backgroundColor(theme.primary_800)]),
          disabled([
            backgroundColor(isDarkMode ? theme.darkBlue : theme.primary_500),
            color(Theme.white),
            borderColor(isDarkMode ? theme.darkBlue : theme.primary_500),
            opacity(0.5),
          ]),
        ])
      | Outline =>
        style([
          backgroundColor(`transparent),
          color(theme.neutral_900),
          border(`px(1), `solid, theme.neutral_900),
          selector("i", [color(theme.neutral_900)]),
          hover([
            backgroundColor(theme.neutral_900),
            color(isDarkMode ? Theme.black : Theme.white),
            selector("i", [color(isDarkMode ? Theme.black : Theme.white)]),
          ]),
          active([
            backgroundColor(theme.neutral_900),
            color(isDarkMode ? Theme.black : Theme.white),
            selector("i", [color(isDarkMode ? Theme.black : Theme.white)]),
          ]),
          disabled([
            borderColor(theme.neutral_600),
            color(theme.neutral_600),
            hover([backgroundColor(`transparent)]),
            opacity(0.5),
          ]),
        ])
      };
    merge([base, custom]);
  };
};

[@react.component]
let make =
    (
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
  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  <button
    className={Css.merge([
      Styles.btn(~variant, ~px, ~py, ~pxSm, ~pySm, ~fsize, theme, isDarkMode, ()),
      CssHelper.flexBox(~align=`center, ~justify=`center, ()),
      style,
    ])}
    onClick
    disabled>
    children
  </button>;
};
