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

  let innerCard = (theme: Theme.t) =>
    style([
      position(`relative),
      zIndex(2),
      minHeight(`px(188)),
      padding2(~v=`px(24), ~h=`px(32)),
      Media.mobile([padding2(~v=`px(24), ~h=`px(16)), minHeight(`px(106))]),
      firstChild([
        Media.mobile([
          after([
            contentRule(`text("")),
            height(`percent(70.)),
            borderLeft(`px(1), `solid, theme.tableRowBorderColor),
            position(`absolute),
            right(`percent(50.)),
            top(`percent(50.)),
            transform(translateY(`percent(-50.))),
          ]),
        ]),
      ]),
    ]);

  let rowHighlightCard = (theme: Theme.t) =>
    style([
      display(`flex),
      flexDirection(`row),
      flexWrap(`wrap),
      alignItems(`start),
      backgroundColor(theme.secondaryBg),
      position(`relative),
      borderRadius(`px(8)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.2)))),
      height(`px(188)),
      padding2(~v=`px(24), ~h=`zero),
      Media.smallMobile([margin2(~v=`zero, ~h=`px(-5))]),
      Media.mobile([padding2(~v=`zero, ~h=`px(16)), minHeight(`px(146)), height(`auto)]),
      selector(
        "> div",
        [
          display(`flex),
          justifyContent(`spaceBetween),
          flexDirection(`column),
          height(`percent(100.)),
          position(`relative),
          overflowY(`scroll),
          padding2(~v=`zero, ~h=`px(24)),
          firstChild([after([border(`zero, `none, white)])]),
          after([
            contentRule(`text("")),
            borderLeft(`px(1), `solid, theme.tableRowBorderColor),
            top(`zero),
            left(`zero),
            height(`percent(100.)),
            position(`absolute),
          ]),
          Media.mobile([
            overflowY(`visible),
            padding2(~v=`px(24), ~h=`zero),
            after([
              borderTop(`px(1), `solid, theme.tableRowBorderColor),
              borderLeft(`zero, `none, white),
              width(`percent(100.)),
            ]),
          ]),
        ],
      ),
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

  let mobileRow =
    style([
      Media.mobile([
        display(`flex),
        flexDirection(`row),
        justifyContent(`spaceBetween),
        alignItems(`center),
        width(`percent(100.)),
      ]),
    ]);

  let mobileAlignRight = style([Media.mobile([marginTop(`px(16)), marginLeft(`auto)])]);
};
module CardContentBlock = {
  [@react.component]
  let make =
      (
        ~label,
        ~textSize: Text.size,
        ~value,
        ~valueColor,
        ~spacing,
        ~roboto=true,
        ~suffix=false,
        ~special=false,
      ) => {
    <div className={special ? "" : Styles.mobileRow}>
      <Text value=label size=Text.Lg />
      <VSpacing size={`px(spacing)} />
      <div>
        <Text
          value
          size=textSize
          weight=Text.Semibold
          color=valueColor
          height={Text.Px(24)}
          code=roboto
        />
        {suffix
           ? <Text
               value="\nBAND"
               size=Text.Md
               weight=Text.Semibold
               color=valueColor
               height={Text.Px(24)}
             />
           : React.null}
      </div>
    </div>;
  };
};

module LoadingContentBlock = {
  [@react.component]
  let make = (~spacing, ~special=false) => {
    <div className={special ? "" : Styles.mobileRow}>
      <LoadingCensorBar width={special ? 80 : 140} height=17 />
      <VSpacing size={`px(spacing)} />
      <LoadingCensorBar width={special ? 90 : 160} height={special ? 22 : 26} />
    </div>;
  };
};

module HighlightCard = {
  [@react.component]
  let make = (~label, ~priceHookSub: ApolloHooks.Subscription.variant(BandScan.PriceHook.t)) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
    let isMobile = Media.isMobile();

    <div className={Css.merge([Styles.card(theme), Styles.specialBg])}>
      {!isMobile
         ? <img alt="Band Token" src=Images.bandToken className=Styles.bandToken /> : React.null}
      <div
        id={"highlight-" ++ label}
        className={Css.merge([
          Styles.innerCard(theme),
          CssHelper.flexBox(
            ~direction=isMobile ? `row : `column,
            ~justify=`spaceBetween,
            ~align=isMobile ? `center : `flexStart,
            (),
          ),
        ])}>
        {switch (priceHookSub) {
         | Data(financial) =>
           let bandPriceInUSD = "$" ++ (financial.usdPrice |> Format.fPretty(~digits=2));
           let marketCap = "$" ++ (financial.usdMarketCap |> Format.fCurrency);
           <>
             <CardContentBlock
               label="Band price"
               textSize=Text.Xxl
               value=bandPriceInUSD
               valueColor={theme.white}
               spacing=16
               roboto=false
               special=true
             />
             <CardContentBlock
               label="Market Cap"
               textSize=Text.Xxl
               value=marketCap
               valueColor={theme.white}
               spacing={isMobile ? 16 : 8}
               roboto=false
               special=true
             />
           </>;
         | _ =>
           <>
             <LoadingContentBlock special=true spacing=16 />
             <LoadingContentBlock special=true spacing={isMobile ? 16 : 8} />
           </>
         }}
      </div>
    </div>;
  };
};

module RowHighlightCard = {
  [@react.component]
  let make = (~label, ~valueAndExtraComponentSub: ApolloHooks.Subscription.variant(_)) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);

    <div id={"highlight-" ++ label} className={Styles.rowHighlightCard(theme)}>
      {switch (valueAndExtraComponentSub) {
       | Data(valueComponent) => valueComponent
       | _ =>
         <>
           <>
             <Col col=Col.Three>
               <LoadingContentBlock spacing=16 />
               <VSpacing size={`px(16)} />
               <LoadingContentBlock spacing=8 />
             </Col>
             <Col col=Col.Three>
               <LoadingContentBlock spacing=16 />
               <VSpacing size={`px(16)} />
               <LoadingContentBlock spacing=8 />
             </Col>
             <Col col=Col.Three>
               <div className=Styles.mobileRow>
                 <LoadingContentBlock spacing=16 />
                 <VSpacing size={`px(16)} />
               </div>
               <div className=Styles.mobileAlignRight>
                 <LoadingCensorBar width=160 height=20 />
               </div>
             </Col>
             <Col col=Col.Three>
               <LoadingContentBlock spacing=16 />
               <div className=Styles.mobileAlignRight>
                 <LoadingCensorBar width=80 height=20 />
               </div>
             </Col>
           </>
         </>
       }}
    </div>;
  };
};

[@react.component]
let make = (~latestBlockSub: Sub.t(BlockSub.t)) => {
  let infoSub = React.useContext(GlobalContext.context);
  let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
  let activeValidatorCountSub = ValidatorSub.countByActive(true);
  let inactiveValidatorCountSub = ValidatorSub.countByActive(false);
  let bondedTokenCountSub = ValidatorSub.getTotalBondedAmount();

  let validatorInfoSub =
    Sub.all3(activeValidatorCountSub, bondedTokenCountSub, inactiveValidatorCountSub);
  let allSub = Sub.all3(latestBlockSub, infoSub, validatorInfoSub);

  <Row justify=Row.Between>
    <Col col=Col.Three colSm=Col.Twelve mbSm=16>
      <HighlightCard
        label="token"
        priceHookSub={
          let%Sub (_, {financial}, _) = allSub;

          financial |> Sub.resolve;
        }
      />
    </Col>
    <Col col=Col.Nine colSm=Col.Twelve mbSm=16>
      <RowHighlightCard
        label="chain"
        valueAndExtraComponentSub={
          let%Sub (
            {height, validator: {moniker, identity, operatorAddress}},
            {financial},
            (activeValidatorCount, bondedTokenCount, inactiveValidatorCount),
          ) = allSub;
          {
            let circulatingSupply = financial.circulatingSupply;
            let onChainSupply = financial.onchainSupply;
            let totalBondedToken = bondedTokenCount |> Coin.getBandAmountFromCoin;
            let bondedRatio =
              (totalBondedToken /. onChainSupply *. 100.)
              ->Js.Float.toFixedWithPrecision(~digits=2)
              ++ "%";
            let activeValidators = activeValidatorCount->Format.iPretty ++ " Nodes";
            let inactiveValidators = inactiveValidatorCount->Format.iPretty ++ " Inactive";

            <>
              <Col col=Col.Three>
                <CardContentBlock
                  label="Circulating Supply"
                  textSize=Text.Xxl
                  value={circulatingSupply |> Format.fPretty}
                  valueColor={theme.textPrimary}
                  spacing=16
                  suffix=true
                />
                <VSpacing size={`px(16)} />
                <CardContentBlock
                  label="On-Chain Supply"
                  textSize=Text.Xxl
                  value={onChainSupply |> Format.fPretty}
                  valueColor={theme.textPrimary}
                  spacing=8
                  suffix=true
                />
              </Col>
              <Col col=Col.Three>
                <CardContentBlock
                  label="Total BAND Bonded"
                  textSize=Text.Xxl
                  value={totalBondedToken |> Format.fPretty}
                  valueColor={theme.textPrimary}
                  spacing=16
                  suffix=true
                />
                <VSpacing size={`px(16)} />
                <CardContentBlock
                  label="Bonded Ratio"
                  textSize=Text.Xxl
                  value=bondedRatio
                  valueColor={theme.textPrimary}
                  spacing=8
                />
              </Col>
              <Col col=Col.Three>
                <div className=Styles.mobileRow>
                  <Text value="Latest Block" size=Text.Lg />
                  <VSpacing size={`px(16)} />
                  <TypeID.Block id=height position=TypeID.Highlight />
                </div>
                <div className=Styles.mobileAlignRight>
                  <ValidatorMonikerLink
                    validatorAddress=operatorAddress
                    moniker
                    identity
                    width={`percent(100.)}
                    avatarWidth=20
                    size=Text.Lg
                  />
                </div>
              </Col>
              <Col col=Col.Three>
                <CardContentBlock
                  label="Active Validators"
                  textSize=Text.Xxl
                  value=activeValidators
                  valueColor={theme.textPrimary}
                  spacing=16
                />
                <div className=Styles.mobileAlignRight>
                  <Text value=inactiveValidators size=Text.Lg color={theme.textSecondary} />
                </div>
              </Col>
            </>;
          }
          |> Sub.resolve;
        }
      />
    </Col>
  </Row>;
};
