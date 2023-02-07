type search_input_t =
  | Default
  | Underline;

module Styles = {
  open Css;
  let searchContainer = style([display(`flex), alignItems(`center), position(`relative)]);
  let iconContainer = inputStyle_ => {
    switch (inputStyle_) {
    | Default =>
      style([
        position(`absolute),
        top(`percent(50.)),
        left(`px(16)),
        transform(translateY(`percent(-50.))),
      ])
    | Underline => style([position(`absolute), top(`percent(5.))])
    };
  };

  let searchBar = (theme: Theme.t, maxWidth_, inputStyle_, isDarkMode_) => {
    switch (inputStyle_) {
    | Default =>
      style([
        backgroundColor(theme.neutral_100),
        borderRadius(`px(4)),
        isDarkMode_
          ? border(`zero, `none, theme.neutral_600)
          : border(`px(2), `solid, theme.neutral_100),
        placeholder([color(theme.neutral_600)]),
        color(theme.neutral_900),
        padding(`px(16)),
        paddingLeft(`px(44)),
        focus([outlineStyle(`none)]),
        width(`percent(100.)),
        maxWidth(`px(maxWidth_)),
        fontSize(`px(14)),
      ])
    | Underline =>
      style([
        backgroundColor(`transparent),
        borderRadius(`zero),
        border(`zero, `none, theme.neutral_600),
        placeholder([color(theme.neutral_600)]),
        borderBottom(`px(1), `solid, theme.neutral_600),
        color(theme.neutral_900),
        paddingLeft(`px(20)),
        paddingBottom(`px(10)),
        focus([outlineStyle(`none)]),
        width(`percent(100.)),
        maxWidth(`px(maxWidth_)),
        fontSize(`px(12)),
      ])
    };
  };
};

[@react.component]
let make = (~placeholder, ~onChange, ~debounce=500, ~maxWidth=240, ~inputStyle=Underline) => {
  let (changeValue, setChangeValue) = React.useState(_ => "");
  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  React.useEffect1(
    () => {
      let timeoutId = Js.Global.setTimeout(() => onChange(_ => changeValue), debounce);
      Some(() => Js.Global.clearTimeout(timeoutId));
    },
    [|changeValue|],
  );

  <div className=Styles.searchContainer>
    <div className={Styles.iconContainer(inputStyle)}>
      <Icon name="far fa-search" color={theme.neutral_900} size=14 />
    </div>
    <input
      type_="text"
      className={Styles.searchBar(theme, maxWidth, inputStyle, isDarkMode)}
      placeholder
      onChange={event => {
        let newVal =
          ReactEvent.Form.target(event)##value |> String.lowercase_ascii |> String.trim;
        setChangeValue(_ => newVal);
      }}
    />
  </div>;
};
