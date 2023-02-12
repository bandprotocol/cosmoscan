module Styles = {
  open Css;

  let card =
    style([
      position(`relative),
      Media.smallMobile([margin2(~v=`zero, ~h=`px(-5))]),
    ]);

  let innerCard =
    style([
      position(`relative),
      zIndex(2),
      minHeight(`px(152)),
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
  
  let longCard = 
    style([
      width(`percent(100.)),
      marginTop(`px(24)),
      padding2(~v=`zero, ~h=`px(24)),
      // margin2(~v=`zero, ~h=`px(12)),
    ]);

  let innerLongCard =
    style([
      minHeight(`px(106)),
      padding2(~v=`px(24), ~h=`px(12)),
      Media.mobile([padding2(~v=`px(10), ~h=`px(12)), minHeight(`px(304))]),
    ]);

  let halfWidth =
    style([
      width(`calc((`sub, `percent(50.), `px(17))))
    ]);

  let mr2 =
    style([
      marginRight(`px(16))
    ]);
  };

module HighlightCard = {
  [@react.component]
  let make =
      (~label, ~valueAndExtraComponentSub: ApolloHooks.Subscription.variant(_), ~special=false) => {
    let (ThemeContext.{theme,isDarkMode}, _) = React.useContext(ThemeContext.context);
    let isMobile = Media.isMobile();

    <div className={Css.merge([Styles.card, special ? Styles.specialBg : "",CommonStyles.card(theme,isDarkMode)])}>
      {special && !isMobile
         ? <img alt="Band Token" src=Images.bandToken className=Styles.bandToken /> : React.null}
      <div
        id={"highlight-" ++ label}
        className={Css.merge([
          Styles.innerCard,
          CssHelper.flexBox(~direction=`column, ~justify=`spaceBetween, ~align=`flexStart, ()),
        ])}>
        {switch (valueAndExtraComponentSub) {
         | Data((valueComponent, extraComponent)) =>
           <> <Text value=label size=Text.Xl weight=Text.Regular /> valueComponent extraComponent </>
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

let getPrevDay = _ => {
  MomentRe.momentNow()
  |> MomentRe.Moment.defaultUtc
  |> MomentRe.Moment.subtract(~duration=MomentRe.duration(1., `days))
  |> MomentRe.Moment.format(Config.timestampUseFormat);
};

let getPrevDayUnix = _ => {
  MomentRe.momentNow()
  |> MomentRe.Moment.defaultUtc
  |> MomentRe.Moment.subtract(~duration=MomentRe.duration(1., `days))
  |> MomentRe.Moment.toUnix;
};

[@react.component]
let make = (~latestBlockSub: Sub.t(BlockSub.t)) => {
  let currentTime =
    React.useContext(TimeContext.context) |> MomentRe.Moment.format(Config.timestampUseFormat);
  let (prevDayTime, setPrevDayTime) = React.useState(getPrevDay);

  let infoSub = React.useContext(GlobalContext.context);
  let (ThemeContext.{theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  let activeValidatorCountSub = ValidatorSub.countByActive(true);
  let bondedTokenCountSub = ValidatorSub.getTotalBondedAmount();
  let txsCountSub = TxSub.count();
  let last24txsCountSub = TxSub.countOffset(prevDayTime);
  let requestCountSub = RequestSub.count();
  let last24RequestCountSub = RequestSub.countOffset(getPrevDayUnix());
  let latestBlock = BlockSub.getLatest();
  let avgBlockTimeSub = BlockSub.getAvgBlockTime(prevDayTime, currentTime);
  let avgCommissionSub = ValidatorSub.avgCommission(~isActive=true, ());
  
  let txInfoSub = Sub.all2(last24txsCountSub, txsCountSub);
  let requestInfoSub = Sub.all2(last24RequestCountSub, requestCountSub);
  let validatorInfoSub = Sub.all3(activeValidatorCountSub, bondedTokenCountSub, avgBlockTimeSub);
  let allSub = Sub.all3(latestBlockSub, infoSub, validatorInfoSub);

  <>
    <Row justify=Row.Between>
      <Col col=Col.Three colSm=Col.Six mbSm=16>
        <HighlightCard
          label="BAND Price"
          special=true
          valueAndExtraComponentSub={
            let%Sub (_, {financial}, _) = allSub;
            (
              {
                let bandPriceInUSD = "$" ++ (financial.usdPrice |> Format.fPretty(~digits=2));
                <div className=CssHelper.flexBox()>
                  <div className=Styles.mr2>
                    <Text 
                      value=bandPriceInUSD
                      size=Text.Xxl 
                      weight=Text.Bold 
                      color=theme.white
                    />
                  </div>
                  <Text 
                    value={
                      (financial.usd24HrChange > 0. ? "+" : "") 
                      ++ (financial.usd24HrChange |> Format.fPretty(~digits=2)) 
                      ++ "%"
                    }
                    size=Text.Md 
                    weight=Text.Regular 
                    color=(financial.usd24HrChange > 0. ? theme.successColor : theme.failColor)
                  />
                </div>
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
                  color={theme.neutral_900}
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
              <TypeID.Block id=height position=TypeID.Landing weight=Text.Semibold />,
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
            let%Sub (_, {financial}, (activeValidatorCount, bondedTokenCount, avgBlockTime)) = allSub;
            (
              {
                let activeValidators = activeValidatorCount->Format.iPretty ++ " Nodes";
                <Text
                  value=activeValidators
                  size=Text.Xxxl
                  color={theme.primary_600}
                  weight=Text.Semibold
                />;
              },
              <Text
                value={"block time " ++ (avgBlockTime |> Format.fPretty(~digits=2)) ++ " secs"}
                size=Text.Lg 
                weight=Text.Regular 
              />,
            )
            |> Sub.resolve;
          }
        />
      </Col>
    </Row>
    <div className={Css.merge([CssHelper.flexBox(), Styles.longCard, CommonStyles.card(theme, isDarkMode)])}>
      <div className=Styles.halfWidth>
        <Row justify=Row.Between >
            <Col col=Col.Six colSm=Col.Six>
              <div
                className={Css.merge([
                  Styles.innerLongCard,
                  CssHelper.flexBox(~direction=`column, ~justify=`spaceBetween, ~align=`flexStart, ()),
                ])}>
                {switch (txInfoSub) {
                | Data((last24Tx, totalTxs)) =>
                  <>
                    <Text value="Total Transactions" size=Text.Lg weight=Text.Regular />
                    <div className=CssHelper.flexBox()>
                      <div className=Styles.mr2>
                        <Text 
                          value={totalTxs |> string_of_int} 
                          size=Text.Xxl 
                          weight=Text.Bold 
                          color=theme.neutral_900
                        />
                      </div>
                      <Text 
                        value={"( " ++ (last24Tx |> string_of_int ) ++ " last 24 hr)"}
                        size=Text.Md 
                        weight=Text.Regular 
                        color=theme.neutral_900
                      />
                    </div>
                  </>
                | _ =>
                  <>
                    <LoadingCensorBar width=90 height=18 />
                    <LoadingCensorBar width=120 height=20 />
                  </>
                }}
              </div>
            </Col>
            <Col col=Col.Six colSm=Col.Six>
              <div
                className={Css.merge([
                  Styles.innerLongCard,
                  CssHelper.flexBox(~direction=`column, ~justify=`spaceBetween, ~align=`flexStart, ()),
                ])}>
                {switch (requestInfoSub) {
                | Data((last24Request, totalRequest)) =>
                  <>
                    <Text value="Total Requests" size=Text.Lg weight=Text.Regular />
                    <div className=CssHelper.flexBox()>
                      <div className=Styles.mr2>
                        <Text 
                          value={totalRequest |> string_of_int} 
                          size=Text.Xxl 
                          weight=Text.Bold 
                          color=theme.neutral_900
                        />
                      </div>
                      <Text 
                        value={"( " ++ (last24Request |> string_of_int ) ++ " last 24 hr)"}
                        size=Text.Md 
                        weight=Text.Regular 
                        color=theme.neutral_900
                      />
                    </div>
                  </>
                | _ =>
                  <>
                    <LoadingCensorBar width=90 height=18 />
                    <LoadingCensorBar width=120 height=20 />
                  </>
                }}
              </div>
            </Col>
        </Row>
      </div>
      <Divider ml=16 mr=16 h=58 />
      <div className=Styles.halfWidth>
        <Row justify=Row.Between >
          <Col col=Col.Three colSm=Col.Six>
            <div
              className={Css.merge([
                Styles.innerLongCard,
                CssHelper.flexBox(~direction=`column, ~justify=`spaceBetween, ~align=`flexStart, ()),
              ])}>
              {switch (latestBlock) {
              | Data({inflation}) =>
                <>
                  <Text value="Inflation Rate" size=Text.Lg weight=Text.Regular />
                  <Text 
                     value={(inflation *. 100. |> Format.fPretty(~digits=2)) ++ "%"}
                    size=Text.Xxl 
                    weight=Text.Bold 
                    color=theme.neutral_900
                  />
                </>
              | _ =>
                <>
                  <LoadingCensorBar width=90 height=18 />
                  <LoadingCensorBar width=120 height=20 />
                </>
              }}
            </div>
          </Col>
          <Col col=Col.Three colSm=Col.Six>
            <div
              className={Css.merge([
                Styles.innerLongCard,
                CssHelper.flexBox(~direction=`column, ~justify=`spaceBetween, ~align=`flexStart, ()),
              ])}>
              {switch (avgCommissionSub) {
              | Data(avgCommission) =>
                <>
                  <Text value="Staking APR" size=Text.Lg weight=Text.Regular />
                  <Text 
                    value={avgCommission |> Format.fPretty(~digits=2)} 
                    size=Text.Xxl 
                    weight=Text.Bold 
                    color=theme.neutral_900
                  />
                </>
              | _ =>
                <>
                  <LoadingCensorBar width=90 height=18 />
                  <LoadingCensorBar width=120 height=20 />
                </>
              }}
            </div>
          </Col>
          <Col col=Col.Six colSm=Col.Six>
            <div
              className={Css.merge([
                Styles.innerLongCard,
                CssHelper.flexBox(~direction=`column, ~justify=`spaceBetween, ~align=`flexStart, ()),
              ])}>
              {switch (allSub) {
              | Data((_, {financial}, (_, bondedTokenCount, _))) =>
                <>
                  <Text value="BAND Bonded" size=Text.Lg weight=Text.Regular />
                  <div className=CssHelper.flexBox()>
                    <div className=Styles.mr2>
                      <Text 
                        value={
                          (
                            ((bondedTokenCount |> Coin.getBandAmountFromCoin) /. financial.circulatingSupply *. 100.) 
                            |> Format.fPretty(~digits=2)
                          )
                          ++ "%"
                        }
                        size=Text.Xxl 
                        weight=Text.Bold 
                        color=theme.neutral_900
                      />
                    </div>
                    <Text 
                      value={
                        (bondedTokenCount |> Coin.getBandAmountFromCoin |> Format.fCurrency)
                        ++ "/" ++ (financial.circulatingSupply |> Format.fCurrency)
                        ++ " BAND"
                      }
                      size=Text.Md 
                      weight=Text.Regular 
                      color=theme.neutral_900
                    />
                  </div>
                </>
              | _ =>
                <>
                  <LoadingCensorBar width=90 height=18 />
                  <LoadingCensorBar width=120 height=20 />
                </>
              }}
            </div>
          </Col>
        </Row>
      </div>
    </div>
  </>
};
