module Styles = {
  open Css;
  let buttonContainer = style([Media.mobile([width(`percent(100.))])]);
  let baseBtn =
    style([
      textAlign(`center),
      Media.mobile([flexGrow(0.), flexShrink(0.), flexBasis(`percent(50.))]),
    ]);

  let activeBtn = (isActive, theme: Theme.t, isDarkMode) => {
    style([
      borderTopRightRadius(`zero),
      borderBottomRightRadius(`zero),
      backgroundColor(isActive ? theme.neutral_800 : theme.neutral_000),
      color(isActive ? theme.neutral_100 : theme.neutral_900),
      hover([
        backgroundColor(isActive ? theme.neutral_800 : theme.neutral_000),
        color(isActive ? theme.neutral_100 : theme.neutral_900),
      ]),
    ]);
  };
  let inActiveBtn = (isActive, theme: Theme.t, isDarkMode) => {
    style([
      borderTopLeftRadius(`zero),
      borderBottomLeftRadius(`zero),
      color(isActive ? theme.neutral_900 : theme.neutral_100),
      backgroundColor(isActive ? theme.neutral_000 : theme.neutral_800),
      hover([
        backgroundColor(isActive ? theme.neutral_000 : theme.neutral_800),
        color(isActive ? theme.neutral_900 : theme.neutral_100),
      ]),
    ]);
  };
};

[@react.component]
let make = (~isActive, ~setIsActive) => {
  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  <div className={Css.merge([CssHelper.flexBox(), Styles.buttonContainer])}>
    <Button
      px=30
      py=8
      variant=Button.Outline
      onClick={_ => setIsActive(_ => true)}
      style={Css.merge([Styles.baseBtn, Styles.activeBtn(isActive, theme, isDarkMode)])}>
      {"Active" |> React.string}
    </Button>
    <Button
      px=30
      py=8
      variant=Button.Outline
      onClick={_ => setIsActive(_ => false)}
      style={Css.merge([Styles.baseBtn, Styles.inActiveBtn(isActive, theme, isDarkMode)])}>
      {"Inactive" |> React.string}
    </Button>
  </div>;
};
