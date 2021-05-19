module Styles = {
  open Css;

  let card = (theme: Theme.t) =>
    style([
      backgroundColor(theme.secondaryBg),
      position(`relative),
      borderRadius(`px(8)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.2)))),
      Media.smallMobile([margin2(~v=`zero, ~h=`px(-5))]),
    ]);

  let innerCard =
    style([
      position(`relative),
      zIndex(2),
      minHeight(`px(177)),
      padding2(~v=`px(24), ~h=`px(32)),
      Media.mobile([padding2(~v=`px(10), ~h=`px(12)), minHeight(`px(146))]),
    ]);

  let fullWidth = style([width(`percent(100.))]);

  let specialBg =
    style([
      backgroundImage(
        `linearGradient((
          `deg(270.),
          [(`percent(0.), hex("58595B")), (`percent(100.), hex("231F20"))],
        )),
      ),
    ]);

  let bandToken =
    style([position(`absolute), width(`percent(60.)), top(`percent(-40.)), right(`zero)]);
};

module HighlightCard = {
  [@react.component]
  let make =
      (~label, ~valueAndExtraComponentSub: ApolloHooks.Subscription.variant(_), ~special=false) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
    let isMobile = Media.isMobile();

    <div className={Css.merge([Styles.card(theme), special ? Styles.specialBg : ""])}>
      {special && !isMobile ? <img src=Images.bandToken className=Styles.bandToken /> : React.null}
      <div
        id={"highlight-" ++ label}
        className={Css.merge([
          Styles.innerCard,
          CssHelper.flexBox(~direction=`column, ~justify=`spaceBetween, ~align=`flexStart, ()),
        ])}>
        {switch (valueAndExtraComponentSub) {
         | Data((valueComponent, extraComponent)) =>
           <> <Text value=label size=Text.Lg /> valueComponent extraComponent </>
         | _ =>
           <>
             <LoadingCensorBar width=90 height=18 />
             <LoadingCensorBar width=120 height=20 />
             <LoadingCensorBar width=75 height=15 />
           </>
         }}
      </div>
    </div>;
  };
};

[@react.component]
let make = (~latestBlockSub: Sub.t(BlockSub.t)) => {
  let infoSub = React.useContext(GlobalContext.context);
  let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
  let activeValidatorCountSub = ValidatorSub.countByActive(true);
  let bondedTokenCountSub = ValidatorSub.getTotalBondedAmount();

  let validatorInfoSub = Sub.all2(activeValidatorCountSub, bondedTokenCountSub);
  let allSub = Sub.all3(latestBlockSub, infoSub, validatorInfoSub);

  <Row justify=Row.Between>
    <Col col=Col.Three colSm=Col.Six mbSm=16>
      <HighlightCard
        label="Band Price"
        special=true
        valueAndExtraComponentSub={
          let%Sub (_, {financial}, _) = allSub;
          (
            {
              let bandPriceInUSD = "$" ++ (financial.usdPrice |> Format.fPretty(~digits=2));
              <Text
                value=bandPriceInUSD
                size=Text.Xxxl
                weight=Text.Semibold
                color={theme.white}
              />;
            },
            {
              let bandPriceInBTC = financial.btcPrice;

              <div
                className={Css.merge([
                  CssHelper.flexBox(~justify=`spaceBetween, ()),
                  Styles.fullWidth,
                ])}>
                <Text value={bandPriceInBTC->Format.fPretty ++ " BTC"} />
              </div>;
            },
          )
          |> Sub.resolve;
        }
      />
    </Col>
    <Col col=Col.Three colSm=Col.Six mbSm=16>
      <HighlightCard
        label="Market Cap"
        valueAndExtraComponentSub={
          let%Sub (_, {financial}, _) = allSub;
          (
            {
              <Text
                value={"$" ++ (financial.usdMarketCap |> Format.fCurrency)}
                size=Text.Xxxl
                color={theme.textPrimary}
                weight=Text.Semibold
              />;
            },
            {
              let marketcap = financial.btcMarketCap;
              <Text value={(marketcap |> Format.fPretty) ++ " BTC"} />;
            },
          )
          |> Sub.resolve;
        }
      />
    </Col>
    <Col col=Col.Three colSm=Col.Six>
      <HighlightCard
        label="Latest Block"
        valueAndExtraComponentSub={
          let%Sub ({height, validator: {moniker, identity, operatorAddress}}, _, _) = allSub;
          (
            <TypeID.Block id=height position=TypeID.Landing />,
            <ValidatorMonikerLink
              validatorAddress=operatorAddress
              moniker
              identity
              width={`percent(100.)}
              avatarWidth=20
            />,
          )
          |> Sub.resolve;
        }
      />
    </Col>
    <Col col=Col.Three colSm=Col.Six>
      <HighlightCard
        label="Active Validators"
        valueAndExtraComponentSub={
          let%Sub (_, _, (activeValidatorCount, bondedTokenCount)) = allSub;
          (
            {
              let activeValidators = activeValidatorCount->Format.iPretty ++ " Nodes";
              <Text
                value=activeValidators
                size=Text.Xxxl
                color={theme.textPrimary}
                weight=Text.Semibold
              />;
            },
            <Text
              value={
                (bondedTokenCount |> Coin.getBandAmountFromCoin |> Format.fPretty)
                ++ " BAND Bonded"
              }
            />,
          )
          |> Sub.resolve;
        }
      />
    </Col>
  </Row>;
};
