module Styles = {
  open Css;

  let base = (~pt, ~pb, ()) => style([paddingTop(`px(pt)), paddingBottom(`px(pb))]);

  let mobile = (~ptSm, ~pbSm, ()) =>
    style([Media.mobile([paddingTop(`px(ptSm)), paddingBottom(`px(pbSm))])]);

  let bgColor = color => style([backgroundColor(color)]);
};

[@react.component]
let make = (~children, ~pt=24, ~pb=24, ~ptSm=pt, ~pbSm=pb, ~bg=Colors.bg, ~style="") => {
  let css =
    Css.merge([
      Styles.bgColor(bg),
      Styles.base(~pt, ~pb, ()),
      Styles.mobile(~ptSm, ~pbSm, ()),
      style,
    ]);

  <section className=css> children </section>;
};
