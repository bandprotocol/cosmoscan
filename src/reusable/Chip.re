type chip_variant_t =
  | Success
  | Warning
  | Danger
  | Default;

module Styles = {
  open Css;
  let chipStyles = (~color: chip_variant_t, theme: Theme.t) =>
    style([
      backgroundColor(
        {
          switch (color) {
          | Success => theme.activeColor
          | Warning => theme.warningColor
          | Danger => theme.failColor
          | Default => theme.baseGrey
          };
        },
      ),
      borderRadius(`px(50)),
      padding2(~v=`px(3), ~h=`px(8)),
      whiteSpace(`pre),
      display(`inlineFlex),
    ]);
};

[@react.component]
let make = (~value, ~color=Default) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div
    className={Css.merge([
      CssHelper.flexBox(~wrap=`nowrap, ~justify=`center, ()),
      Styles.chipStyles(~color, theme),
    ])}>
    <Text value size=Text.Sm color=Theme.white transform=Text.Uppercase align=Text.Center />
  </div>;
};
