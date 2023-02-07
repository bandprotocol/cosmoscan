module Styles = {
  open Css;
  let emptyContainer =
    style([
      justifyContent(`center),
      alignItems(`center),
      flexDirection(`column),
      width(`percent(100.)),
      Media.mobile([minHeight(`px(200))]),
    ]);

  let height = he => style([height(he)]);
  let display = dp => style([display(dp ? `flex : `none)]);
  let backgroundColor = bc => style([backgroundColor(bc)]);
  let boxShadow =
    style([
      boxShadow(
        Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.08))),
      ),
    ]);
};

[@react.component]
let make = (~height=`px(300), ~display=true, ~backgroundColor=?, ~boxShadow=false, ~children) => {
  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);
  <div
    className={Css.merge([
      Styles.emptyContainer,
      Styles.height(height),
      Styles.display(display),
      Styles.backgroundColor(backgroundColor->Belt.Option.getWithDefault(isDarkMode ? theme.neutral_100 : theme.neutral_000)),
      boxShadow ? Styles.boxShadow : "",
    ])}>
    children
  </div>;
};
