module Styles = {
  open Css;

  let base = (~pt, ~pb, ()) => style([paddingTop(`px(pt)), paddingBottom(`px(pb))]);

  let mobile = (~ptSm, ~pbSm, ()) =>
    style([Media.mobile([paddingTop(`px(ptSm)), paddingBottom(`px(pbSm))])]);

  let bgColor = color => style([backgroundColor(color)]);
};

[@react.component]
let make = (~children, ~pt=60, ~pb=60, ~ptSm=32, ~pbSm=32, ~bg=?, ~style="") => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  let css =
    Css.merge([
      Styles.bgColor(bg->Belt.Option.getWithDefault(theme.neutral_000)),
      Styles.base(~pt, ~pb, ()),
      Styles.mobile(~ptSm, ~pbSm, ()),
      style,
    ]);

  <section className=css> children </section>;
};
