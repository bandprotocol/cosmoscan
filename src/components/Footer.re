module Styles = {
  open Css;

  let footerBg = style([zIndex(4)]);
  let socialContainer = style([selector("a + a", [marginLeft(`px(16))])]);
  let socialImg = style([width(`px(16))]);
};

let mapImages = [|
  [|"https:/\/github.com/bandprotocol", Images.githubSvg|],
  [|"https:/\/medium.com/bandprotocol", Images.mediumSvg|],
  [|"https:/\/twitter.com/BandProtocol", Images.twitterSvg|],
  [|"https:/\/t.me/bandprotocol", Images.telegramSvg|],
  [|"https:/\/discord.com/invite/3t4bsY7", Images.discordSvg|],
  [|"https:/\/coinmarketcap.com/currencies/band-protocol/", Images.coinmarketcapWhiteSvg|],
  [|"https:/\/www.coingecko.com/en/coins/band-protocol", Images.coingeckoSvg|],
|];

[@react.component]
let make = () => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  let isMobile = Media.isMobile();

  <Section bg={theme.footer} pt=24 pb=24 ptSm=24 pbSm=24 style=Styles.footerBg>
    <div className=CssHelper.container>
      <Row alignItems=Row.Center>
        <Col col=Col.Six mbSm=24>
          <div
            className={Css.merge([
              CssHelper.flexBox(
                ~justify={
                  isMobile ? `center : `flexStart;
                },
                (),
              ),
              Styles.socialContainer,
            ])}>
            {mapImages
             ->Belt.Array.mapWithIndex((i, e) =>
                 <AbsoluteLink key={string_of_int(i)} href={Array.get(e, 0)}>
                   <img src={Array.get(e, 1)} className=Styles.socialImg />
                 </AbsoluteLink>
               )
             ->React.array}
          </div>
        </Col>
        <Col col=Col.Six>
          <div className={CssHelper.flexBox(~justify={isMobile ? `center : `flexEnd}, ())}>
            <Text block=true value="CosmoScan" weight=Text.Semibold color={theme.white} />
            <HSpacing size={`px(5)} />
            <Icon name="far fa-copyright" color={theme.white} />
            <HSpacing size={`px(5)} />
            <Text block=true value="2021" weight=Text.Semibold color={theme.white} />
          </div>
        </Col>
      </Row>
    </div>
  </Section>;
};
