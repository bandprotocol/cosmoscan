module Styles = {
  open Css;

  let fontSize = size => style([fontSize(`px(size))]);
  let fontColor = color_ => style([color(color_)]);
  let margin = (mr, ml) => style([marginRight(`px(mr)), marginLeft(`px(ml))]);
};

[@react.component]
let make = (~name, ~color=Colors.gray7, ~size=12, ~mr=0, ~ml=0) => {
  <i
    className={Css.merge([
      name,
      Styles.fontColor(color),
      Styles.fontSize(size),
      Styles.margin(mr, ml),
    ])}
  />;
};
