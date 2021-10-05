module Styles = {
  open Css;

  let bar =
    style([
      backgroundColor(Css.rgba(244, 210, 62, `num(0.3))),
      padding2(~v=`px(12), ~h=`px(20)),
    ]);

  let innerBar = style([maxWidth(`px(825)), selector(" > i", [marginRight(`px(24))])]);
};

[@react.component]
let make = () => {
  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);
  <div className={Css.merge([CssHelper.flexBox(~justify=`center, ()), Styles.bar])}>
    <div className={Css.merge([CssHelper.flexBox(~wrap=`nowrap, ()), Styles.innerBar])}>
      <Icon name="fal fa-exclamation-circle" size=24 color={theme.pendingColor} />
      <Text
        value="Bandchain is operating normally. Only CosmoScan is facing an issue and we are fixing it at the moment. This does not affect other explorers or 3rd party wallets."
        block=true
        color={isDarkMode ? theme.white : theme.black}
        size=Text.Lg
      />
    </div>
  </div>;
  // <Text
  //   value="The Laozi mainnet migration has successfully concluded. As part of the migration process, all pending staking rewards have been automatically withdrawn to your wallet balance."
  //   block=true
  //   color={isDarkMode ? theme.white : theme.black}
  //   size=Text.Lg
  // />
};
