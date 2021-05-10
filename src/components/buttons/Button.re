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
        borderRadius(`px(4)),
        fontSize(`px(fsize)),
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
          backgroundColor(theme.baseBlue),
          color(Colors.white),
          border(`px(1), `solid, theme.baseBlue),
          hover([backgroundColor(theme.darkBlue)]),
          active([backgroundColor(theme.darkenBlue)]),
          disabled([
            backgroundColor(theme.lightBlue),
            color(Colors.white),
            borderColor(theme.lightBlue),
          ]),
        ])
      | Outline =>
        style([
          backgroundColor(`transparent),
          color(theme.textPrimary),
          border(`px(1), `solid, theme.textPrimary),
          hover([
            backgroundColor(theme.textPrimary),
            color(isDarkMode ? Colors.black : Colors.white),
          ]),
          active([backgroundColor(Colors.buttonOutlineActive)]),
          disabled([
            borderColor(Colors.buttonDisabled),
            color(Colors.buttonDisabled),
            hover([backgroundColor(Colors.white)]),
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
      ~py=5,
      ~px=10,
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
