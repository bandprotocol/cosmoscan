module Styles = {
  open Css;

  let vFlex = style([display(`flex), flexDirection(`row)]);

  let pageContainer = (theme: Theme.t) =>
    style([
      width(`percent(100.)),
      paddingTop(`px(50)),
      minHeight(`px(450)),
      display(`flex),
      flexDirection(`column),
      alignItems(`center),
      justifyContent(`center),
      backgroundColor(theme.secondaryBg),
      borderRadius(`px(8)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), rgba(0, 0, 0, `num(0.1)))),
    ]);

  let linkToHome = style([display(`flex), alignItems(`center), cursor(`pointer)]);

  let rightArrow = style([width(`px(20)), filter([`saturate(50.0), `brightness(70.0)])]);

  let logo = style([width(`px(120)), marginRight(`px(10))]);
};

[@react.component]
let make = () => {
  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);
  <Section>
    <div className=CssHelper.container>
      <VSpacing size=Spacing.xxl />
      <div className={Styles.pageContainer(theme)}>
        <div className={CssHelper.flexBox()}>
          <img
            alt="Not Found"
            src={isDarkMode ? Images.noOracleDark : Images.noOracleLight}
            className=Styles.logo
          />
        </div>
        <VSpacing size=Spacing.xxl />
        <Text value="Oops! We cannot find the page you're looking for." size=Text.Lg />
        <VSpacing size=Spacing.lg />
        <Link className=Styles.linkToHome route=Route.HomePage>
          <Text value="Back to Homepage" weight=Text.Bold size=Text.Lg color={theme.textPrimary} />
          <HSpacing size=Spacing.md />
          <Icon name="far fa-arrow-right" color={theme.textPrimary} />
        </Link>
        <VSpacing size=Spacing.xxl />
      </div>
    </div>
  </Section>;
};
