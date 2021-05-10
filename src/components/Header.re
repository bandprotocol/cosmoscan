module Styles = {
  open Css;

  let header = (theme: Theme.t) =>
    style([
      paddingTop(Spacing.lg),
      backgroundColor(theme.mainBg),
      zIndex(3),
      Media.mobile([
        padding(Spacing.md),
        marginBottom(`zero),
        position(`sticky),
        top(`zero),
        width(`percent(100.)),
      ]),
    ]);

  let leftContainer = style([display(`flex), alignItems(`center), width(`percent(100.))]);
  let bandLogo = (theme: Theme.t) =>
    style([
      width(`px(40)),
      backgroundColor(theme.baseBlue),
      borderRadius(`percent(50.)),
      padding(`px(8)),
      marginRight(`px(5)),
      Media.mobile([width(`px(34))]),
    ]);
  let cmcLogo = style([width(`px(15)), height(`px(15))]);
  let blockImage = style([display(`block)]);

  let socialLink = style([marginLeft(`px(10)), display(`flex), textDecoration(`none)]);

  let link = style([cursor(`pointer)]);

  let chainIDContainer = style([marginLeft(`px(24))]);
};

module LinkToHome = {
  [@react.component]
  let make = (~children) => {
    <Link className=Styles.link route=Route.HomePage> children </Link>;
  };
};

module ToggleThemeButton = {
  open Css;

  module Styles = {
    let button = isDarkMode =>
      style([
        backgroundColor(Colors.white),
        padding2(~v=`px(3), ~h=`px(6)),
        borderRadius(`px(8)),
        border(`px(1), `solid, isDarkMode ? Colors.white : Colors.black),
        marginLeft(`px(5)),
        cursor(`pointer),
        outlineStyle(`none),
      ]);
  };

  [@react.component]
  let make = () => {
    let ({ThemeContext.isDarkMode}, toggle) = React.useContext(ThemeContext.context);

    <button className={Styles.button(isDarkMode)} onClick={_ => toggle()}>
      <Icon name={isDarkMode ? "fas fa-sun" : "fas fa-moon"} size=14 color=Colors.black />
    </button>;
  };
};

module DesktopRender = {
  [@react.component]
  let make = () => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <header className={Styles.header(theme)}>
      <div className="container">
        <Row alignItems=Row.Center marginBottom=22>
          <Col col=Col.Six>
            <div className={CssHelper.flexBox(~align=`center, ())}>
              <LinkToHome>
                <div className={CssHelper.flexBox(~align=`center, ())}>
                  <img src=Images.bandLogo className={Styles.bandLogo(theme)} />
                  <div>
                    <Text
                      value="BANDCHAIN"
                      size=Text.Md
                      weight=Text.Bold
                      nowrap=true
                      color={theme.textPrimary}
                      special=true
                    />
                    <br />
                    <Text
                      value="Cosmoscan"
                      nowrap=true
                      size=Text.Sm
                      weight=Text.Semibold
                      color={theme.textPrimary}
                      special=true
                    />
                  </div>
                </div>
              </LinkToHome>
              <div className=Styles.chainIDContainer> <ChainIDBadge /> </div>
            </div>
          </Col>
          //   <div
          //     className={Css.merge([
          //       CssHelper.flexBox(~align=`center, ()),
          //       Styles.socialContainer,
          //     ])}>
          //     <a
          //       href="https://twitter.com/bandprotocol"
          //       target="_blank"
          //       rel="noopener"
          //       className=Styles.socialLink>
          //       <Icon
          //         name="fab fa-twitter"
          //         color={isDarkMode ? Colors.white : theme.baseBlue}
          //         size=16
          //       />
          //     </a>
          //     <a
          //       href="https://t.me/bandprotocol"
          //       target="_blank"
          //       rel="noopener"
          //       className=Styles.socialLink>
          //       <Icon
          //         name="fab fa-telegram-plane"
          //         color={isDarkMode ? Colors.white : theme.baseBlue}
          //         size=17
          //       />
          //     </a>
          //     <a
          //       href="https://coinmarketcap.com/currencies/band-protocol/"
          //       target="_blank"
          //       rel="noopener"
          //       className=Styles.socialLink>
          //       <img
          //         src={isDarkMode ? Images.cmcWhiteLogo : Images.cmcLogo}
          //         className={Css.merge([Styles.cmcLogo, Styles.blockImage])}
          //       />
          //     </a>
          //   </div>
          <Col col=Col.Six>
            <div className={CssHelper.flexBox(~align=`center, ~justify=`flexEnd, ())}>
              <SearchBar />
            </div>
          </Col>
        </Row>
      </div>
      <Section bg={theme.secondaryBg} pt=0 pb=0>
        <div className="container">
          <Row alignItems=Row.Center>
            <Col col=Col.Eight> <NavBar /> </Col>
            <Col col=Col.Four>
              <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
                <UserAccount />
                <ToggleThemeButton />
              </div>
            </Col>
          </Row>
        </div>
      </Section>
    </header>;
  };
};

module MobileRender = {
  [@react.component]
  let make = () => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <header className={Styles.header(theme)}>
      <Row alignItems=Row.Center>
        <Col colSm=Col.Six>
          <div className={CssHelper.flexBox(~align=`flexEnd, ())}>
            <LinkToHome>
              <img
                src=Images.bandLogo
                className={Css.merge([Styles.bandLogo(theme), Styles.blockImage])}
              />
            </LinkToHome>
            <HSpacing size=Spacing.sm />
            <LinkToHome>
              <div className={CssHelper.flexBox(~direction=`column, ~align=`flexStart, ())}>
                <Text
                  value="BANDCHAIN"
                  size=Text.Md
                  weight=Text.Bold
                  nowrap=true
                  color={theme.textPrimary}
                  spacing={Text.Em(0.05)}
                />
                <VSpacing size=Spacing.xs />
                <Text
                  value="CosmoScan"
                  nowrap=true
                  size=Text.Sm
                  color=Colors.gray6
                  spacing={Text.Em(0.03)}
                />
              </div>
            </LinkToHome>
          </div>
        </Col>
        <Col colSm=Col.Six>
          <div className={CssHelper.flexBox(~justify=`flexEnd, ~wrap=`nowrap, ())}>
            <ChainIDBadge />
            <NavBar />
          </div>
        </Col>
      </Row>
    </header>;
  };
};

[@react.component]
let make = () => {
  Media.isMobile() ? <MobileRender /> : <DesktopRender />;
};
