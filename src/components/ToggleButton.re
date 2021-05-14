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
      backgroundColor(isActive ? theme.textPrimary : theme.secondaryBg),
      color(isActive ? theme.secondaryBg : theme.textPrimary),
      hover([
        backgroundColor(isActive ? theme.textPrimary : theme.secondaryBg),
        color(isActive ? theme.secondaryBg : theme.textPrimary),
      ]),
    ]);
  };
  let inActiveBtn = (isActive, theme: Theme.t, isDarkMode) => {
    style([
      borderTopLeftRadius(`zero),
      borderBottomLeftRadius(`zero),
      color(isActive ? theme.textPrimary : theme.secondaryBg),
      backgroundColor(isActive ? theme.secondaryBg : theme.textPrimary),
      hover([
        backgroundColor(isActive ? theme.secondaryBg : theme.textPrimary),
        color(isActive ? theme.textPrimary : theme.secondaryBg),
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
