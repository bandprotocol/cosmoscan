module Styles = {
  open Css;

  let chartContainer = style([position(`relative), maxWidth(`px(220)), maxHeight(`px(220))]);

  let textContainer = (theme: Theme.t) =>
    style([
      background(theme.neutral_100),
      position(`absolute),
      top(`px(8)),
      left(`px(8)),
      right(`px(8)),
      bottom(`px(8)),
      borderRadius(`percent(50.)),
    ]);
  let circle = (percent, theme: Theme.t) => {
    style([
      width(`percent(100.)),
      height(`percent(100.)),
      borderRadius(`percent(50.)),
      selector(
        "> circle",
        [
          SVG.fill(theme.neutral_100),
          SVG.strokeWidth(`px(16)),
          SVG.stroke(theme.baseBlue),
          //TODO: it will be remove when the bs-css upgrade to have this proporty
          // 653.45 is from 2 * pi(3.141) * r(104)
          unsafe("stroke-dasharray", {j|calc($percent * 653.45 / 100) 653.45|j}),
          transforms([`rotate(`deg(-90.)), `translateX(`percent(-100.))]),
        ],
      ),
    ]);
  };
};

[@react.component]
let make = (~percent) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <div className=Styles.chartContainer>
    <svg
      className={Styles.circle(percent |> int_of_float |> string_of_int, theme)}
      viewBox="0 0 208 208">
      <circle r="104" cx="104" cy="104" />
    </svg>
    <div
      className={Css.merge([
        Styles.textContainer(theme),
        CssHelper.flexBox(~justify=`center, ~direction=`column, ()),
      ])}>
      <Heading
        size=Heading.H5
        value="Turnout"
        align=Heading.Center
        marginBottom=8
        color={theme.neutral_600}
      />
      <Text
        size=Text.Xxxl
        value={percent |> Format.fPercent(~digits=2)}
        block=true
        color={theme.neutral_900}
      />
    </div>
  </div>;
};
