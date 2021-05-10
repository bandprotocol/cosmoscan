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
          backgroundColor(theme.primaryColor),
          color(Colors.white),
          border(`px(1), `solid, theme.primaryColor),
          hover([backgroundColor(Colors.buttonBaseHover)]),
          active([backgroundColor(Colors.buttonBaseActive)]),
          disabled([
            backgroundColor(Colors.buttonDisabled),
            color(Colors.white),
            borderColor(Colors.buttonDisabled),
          ]),
        ])
      | Outline =>
        style([
          backgroundColor(Colors.white),
          color(isDarkMode ? theme.primaryColor : theme.textPrimary),
          border(`px(1), `solid, isDarkMode ? theme.primaryColor : theme.textPrimary),
          hover([backgroundColor(Colors.buttonOutlineHover)]),
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
  let (ThemeContext.{theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

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
