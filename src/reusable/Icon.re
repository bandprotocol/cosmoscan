module Styles = {
  open Css;

  let fontSize = size => style([fontSize(`px(size))]);
  let fontColor = color_ => style([color(color_)]);
  let margin = (mr, ml) => style([marginRight(`px(mr)), marginLeft(`px(ml))]);
};

[@react.component]
let make = (~name, ~color=?, ~size=12, ~mr=0, ~ml=0) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <i
    className={Css.merge([
      name,
      Styles.fontColor(color->Belt_Option.getWithDefault(theme.textSecondary)),
      Styles.fontSize(size),
      Styles.margin(mr, ml),
    ])}
  />;
};
