module Styles = {
  open Css;

  let footerBg = style([zIndex(4)]);
  let socialContainer = style([selector("a + a", [marginLeft(`px(16))])]);
  let socialImg = style([width(`px(16))]);
};

let mapImages = [|
  [|{j|https://github.com/bandprotocol|j}, Images.githubSvg, "Github"|],
  [|{j|https://medium.com/bandprotocol|j}, Images.mediumSvg, "Medium"|],
  [|{j|https://twitter.com/BandProtocol|j}, Images.twitterSvg, "Twitter"|],
  [|{j|https://t.me/bandprotocol|j}, Images.telegramSvg, "Telegram"|],
  [|{j|https://discord.com/invite/3t4bsY7|j}, Images.discordSvg, "Discord"|],
  [|
    {j|https://coinmarketcap.com/currencies/band-protocol/|j},
    Images.coinmarketcapWhiteSvg,
    "CoinMarketCap",
  |],
  [|{j|https://www.coingecko.com/en/coins/band-protocol|j}, Images.coingeckoSvg, "CoinGecko"|],
|];

let currentYear = Js.Date.getFullYear(Js.Date.make());
let yearString = Js.Float.toString(currentYear);

[@react.component]
let make = () => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  let isMobile = Media.isMobile();

  <Section bg={theme.primary_600} pt=24 pb=24 ptSm=24 pbSm=24 style=Styles.footerBg>
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
                   <img alt={e[2] ++ " Icon"} src={Array.get(e, 1)} className=Styles.socialImg />
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
            <Text block=true value=yearString weight=Text.Semibold color={theme.white} />
          </div>
        </Col>
      </Row>
    </div>
  </Section>;
};
