module Styles = {
  open Css;

  let bar =
    style([
      backgroundColor(Css.rgba(232, 74, 75, `num(0.15))),
      padding2(~v=`px(12), ~h=`px(20)),
    ]);

  let innerBar = style([maxWidth(`px(825)), selector(" > i", [marginRight(`px(24))])]);
};

[@react.component]
let make = () => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div className={Css.merge([CssHelper.flexBox(~justify=`center, ()), Styles.bar])}>
    <div className={Css.merge([CssHelper.flexBox(~wrap=`nowrap, ()), Styles.innerBar])}>
      <Icon name="fal fa-exclamation-circle" size=24 color={theme.failColor} />
      <Text
        value="Sending transactions (token transfers, delegations, etc.) through the explorer is currently under maintenance. A patch will be released within the next week"
        block=true
        color={theme.failColor}
        size=Text.Lg
      />
    </div>
  </div>;
};
