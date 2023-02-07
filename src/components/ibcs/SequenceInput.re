module Styles = {
  open Css;
  let searchContainer = mW =>
    style([maxWidth(`px(mW)), display(`flex), alignItems(`center), position(`relative)]);
  let iconContainer =
    style([
      position(`absolute),
      left(`px(16)),
      top(`percent(50.)),
      transform(`translateY(`percent(-50.))),
    ]);
  let searchBar = (theme: Theme.t) =>
    style([
      width(`percent(100.)),
      height(`px(37)),
      paddingLeft(`px(44)),
      paddingRight(`px(9)),
      borderRadius(`px(4)),
      fontSize(`px(14)),
      fontWeight(`light),
      border(`px(1), `solid, theme.neutral_100),
      backgroundColor(theme.neutral_100),
      outlineStyle(`none),
      color(theme.neutral_900),
      fontFamilies([`custom("Montserrat"), `custom("sans-serif")]),
    ]);
};

[@react.component]
let make = (~placeholder, ~onChange, ~value, ~disabled, ~maxWidth=150) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <div>
    <div className={CssHelper.mb(~size=8, ())}> <Text value="Sequence" /> </div>
    <div className={Styles.searchContainer(maxWidth)}>
      <div className=Styles.iconContainer>
        <Icon name="far fa-search" color={theme.neutral_600} size=14 />
      </div>
      <input
        disabled
        type_="number"
        className={Styles.searchBar(theme)}
        placeholder
        value
        onChange={event => {
          let newVal =
            ReactEvent.Form.target(event)##value |> String.lowercase_ascii |> String.trim;
          onChange(_ => newVal);
        }}
      />
    </div>
  </div>;
};
