module Styles = {
  open Css;

  let container = (theme: Theme.t) =>
    style([
      selector(
        "a",
        [
          wordBreak(`breakAll),
          color(theme.textPrimary),
          textDecoration(`none),
          transition(~duration=200, "all"),
          hover([color(theme.baseBlue)]),
        ],
      ),
      selector("p", [color(theme.textSecondary)]),
      selector("p + p", [marginTop(`em(1.))]),
    ]);
};

[@react.component]
let make = (~value) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div className={Styles.container(theme)}> {value->MarkedJS.marked->MarkedJS.parse} </div>;
};
