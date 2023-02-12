module Styles = {
  open Css;
  let buttonContainer = style([Media.mobile([width(`percent(100.))])]);
  let baseBtn =
    style([
      textAlign(`center),
      Media.mobile([flexGrow(0.), flexShrink(0.), flexBasis(`percent(50.))]),
    ]);

  let leftBtn = (state, theme: Theme.t, isDarkMode) => {
    style([
      borderTopRightRadius(`zero),
      borderBottomRightRadius(`zero),
      backgroundColor(state ? theme.neutral_900 : theme.neutral_000),
      color(state ? theme.neutral_100 : theme.neutral_900),
      hover([
        backgroundColor(state ? theme.neutral_900 : theme.neutral_100),
        color(state ? theme.neutral_100 : theme.neutral_900),
      ]),
    ]);
  };
  let rightBtn = (state, theme: Theme.t, isDarkMode) => {
    style([
      borderTopLeftRadius(`zero),
      borderBottomLeftRadius(`zero),
      color(state ? theme.neutral_900 : theme.neutral_100),
      backgroundColor(state ? theme.neutral_000 : theme.neutral_900),
      hover([
        backgroundColor(state ? theme.neutral_100 : theme.neutral_900),
        color(state ? theme.neutral_900 : theme.neutral_100),
      ]),
    ]);
  };
};

[@react.component]
let make = (~state, ~setState, ~nameArray) => {
  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  <div className={Css.merge([CssHelper.flexBox(), Styles.buttonContainer])}>
    <Button
      px=16
      py=8
      variant=Button.Outline
      onClick={_ => setState(_ => true)}
      style={Css.merge([Styles.baseBtn, Styles.leftBtn(state, theme, isDarkMode)])}>
      {nameArray[0]  |> React.string}
    </Button>
    <Button
      px=16
      py=8
      variant=Button.Outline
      onClick={_ => setState(_ => false)}
      style={Css.merge([Styles.baseBtn, Styles.rightBtn(state, theme, isDarkMode)])}>
      {nameArray[1] |> React.string}
    </Button>
  </div>;
};
