module Styles = {
  open Css;

  let footerBg = style([zIndex(4), minHeight(`px(60))]);

  let externalLinkIcon = (isFirst, isMobile) =>
    style([
      width(`px(16)),
      height(`auto),
      marginLeft(isFirst ? `px(0) : `px(isMobile ? 24 : 16)),
    ]);
};

let mapImage = [|
  [|"https:/\/github.com/bandprotocol", Images.githubSvg|],
  [|"https:/\/medium.com/bandprotocol", Images.mediumSvg|],
  [|"https:/\/twitter.com/BandProtocol", Images.twitterSvg|],
  [|"https:/\/t.me/bandprotocol", Images.telegramSvg|],
  [|"https:/\/discord.com/invite/3t4bsY7", Images.discordSvg|],
  [|"https:/\/coinmarketcap.com/currencies/band-protocol/", Images.coinmarketcapWhiteSvg|],
  [|"https:/\/www.coingecko.com/en/coins/band-protocol", Images.coingeckoSvg|],
|];

module DesktopRender = {
  [@react.component]
  let make = () => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Section bg={theme.footer} pt=0 pb=0 style=Styles.footerBg>
      <div className=CssHelper.container>
        <Row alignItems=Row.Center justify=Row.Between>
          <Col col=Col.Six>
            <Row alignItems=Row.Center>
              {mapImage
               ->Belt.Array.mapWithIndex((i, e) =>
                   <A link={Array.get(e, 0)}>
                     <img
                       src={Array.get(e, 1)}
                       className={Styles.externalLinkIcon(i == 0, false)}
                     />
                   </A>
                 )
               ->React.array}
            </Row>
          </Col>
          <Col col=Col.Six>
            <Row alignItems=Row.Center justify=Row.End>
              <Text
                block=true
                code=true
                spacing={Text.Em(0.02)}
                value="Cosmoscan"
                weight=Text.Regular
                color={`hex("ffffff")}
                height={Text.Px(60)}
              />
              <HSpacing size={`px(5)} />
              <Icon name="far fa-copyright" color={`hex("ffffff")} />
              <HSpacing size={`px(5)} />
              <Text
                block=true
                code=true
                spacing={Text.Em(0.02)}
                value="2021"
                weight=Text.Regular
                color={`hex("ffffff")}
                height={Text.Px(60)}
              />
            </Row>
          </Col>
        </Row>
      </div>
    </Section>;
  };
};

module MobileRender = {
  [@react.component]
  let make = () => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Section bg={theme.footer} style=Styles.footerBg>
      <Col>
        <Row alignItems=Row.Center justify=Row.Center>
          {mapImage
           ->Belt.Array.mapWithIndex((i, e) =>
               <A link={Array.get(e, 0)}>
                 <img src={Array.get(e, 1)} className={Styles.externalLinkIcon(i == 0, true)} />
               </A>
             )
           ->React.array}
        </Row>
        <Row> <VSpacing size={`px(20)} /> </Row>
        <Row alignItems=Row.Center justify=Row.Center>
          <Text
            block=true
            code=true
            spacing={Text.Em(0.02)}
            value="Cosmoscan"
            weight=Text.Regular
            color={`hex("ffffff")}
            height={Text.Px(20)}
          />
          <HSpacing size={`px(5)} />
          <Icon name="far fa-copyright" color={`hex("ffffff")} />
          <HSpacing size={`px(5)} />
          <Text
            block=true
            code=true
            spacing={Text.Em(0.02)}
            value="2021"
            weight=Text.Regular
            color={`hex("ffffff")}
            height={Text.Px(20)}
          />
        </Row>
      </Col>
    </Section>;
  };
};

[@react.component]
let make = () => {
  Media.isMobile() ? <MobileRender /> : <DesktopRender />;
};
