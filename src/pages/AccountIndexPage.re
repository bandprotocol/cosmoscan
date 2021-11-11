module Styles = {
  open Css;

  let squareIcon = color =>
    style([width(`px(8)), marginRight(`px(8)), height(`px(8)), backgroundColor(color)]);

  let balance = style([minWidth(`px(150)), justifyContent(`flexEnd)]);

  let infoHeader =
    style([borderBottom(`px(1), `solid, Colors.gray9), paddingBottom(`px(16))]);

  let totalBalance =
    style([
      display(`flex),
      flexDirection(`column),
      alignItems(`flexEnd),
      Media.mobile([
        flexDirection(`row),
        justifyContent(`spaceBetween),
        alignItems(`center),
        width(`percent(100.)),
      ]),
    ]);

  let infoLeft =
    style([
      height(`percent(100.)),
      selector(
        "> div",
        [width(`percent(100.)), Media.mobile([height(`auto), marginBottom(`px(16))])],
      ),
    ]);

  let amountBoxes =
    style([
      selector("> div:nth-child(1)", [marginBottom(`px(40))]),
      selector("> div + div", [marginTop(`px(24))]),
    ]);

  let detailContainer = style([height(`percent(100.))]);

  let addressContainer = style([width(`percent(100.)), maxWidth(`px(420))]);

  let buttonContainer =
    style([marginTop(`px(24)), selector("> button + *", [marginLeft(`px(16))])]);
};

module BalanceDetails = {
  [@react.component]
  let make = (~title, ~description, ~amount, ~usdPrice, ~color, ~isCountup=false) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <Row>
      <Col col=Col.Six colSm=Col.Five>
        <div className={CssHelper.flexBox()}>
          <div className={Styles.squareIcon(color)} />
          <Text
            value=title
            size=Text.Lg
            weight=Text.Semibold
            tooltipItem={description |> React.string}
            tooltipPlacement=Text.AlignBottomStart
            color={theme.textPrimary}
          />
        </div>
      </Col>
      <Col col=Col.Six colSm=Col.Seven>
        <div className={CssHelper.flexBox(~direction=`column, ~align=`flexEnd, ())}>
          <div className={CssHelper.flexBox()}>
            {isCountup
               ? <NumberCountup
                   value=amount
                   size=Text.Lg
                   weight=Text.Regular
                   color={theme.textPrimary}
                 />
               : <Text
                   value={amount |> Format.fPretty}
                   size=Text.Lg
                   weight=Text.Regular
                   nowrap=true
                   code=true
                   color={theme.textPrimary}
                 />}
            <HSpacing size=Spacing.sm />
            <Text
              value="BAND"
              size=Text.Lg
              weight=Text.Thin
              nowrap=true
              color={theme.textPrimary}
            />
          </div>
          <VSpacing size=Spacing.xs />
          <div className={Css.merge([CssHelper.flexBox(), Styles.balance])}>
            {isCountup
               ? <NumberCountup
                   value={amount *. usdPrice}
                   size=Text.Md
                   weight=Text.Thin
                   spacing={Text.Em(0.02)}
                   color={theme.textSecondary}
                 />
               : <Text
                   value={amount *. usdPrice |> Format.fPretty}
                   size=Text.Md
                   spacing={Text.Em(0.02)}
                   weight=Text.Thin
                   nowrap=true
                   code=true
                 />}
            <HSpacing size=Spacing.sm />
            <Text
              value="USD"
              size=Text.Md
              weight=Text.Thin
              nowrap=true
              color={theme.textSecondary}
            />
          </div>
        </div>
      </Col>
    </Row>;
  };
};

module BalanceDetailLoading = {
  [@react.component]
  let make = () => {
    <Row>
      <Col col=Col.Six colSm=Col.Five> <LoadingCensorBar width=130 height=18 /> </Col>
      <Col col=Col.Six colSm=Col.Seven>
        <div className={CssHelper.flexBox(~direction=`column, ~align=`flexEnd, ())}>
          <LoadingCensorBar width=120 height=20 />
          <VSpacing size=Spacing.xs />
          <LoadingCensorBar width=120 height=16 />
        </div>
      </Col>
    </Row>;
  };
};

module TotalBalanceRender = {
  [@react.component]
  let make = (~amountBAND, ~usdPrice) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <div
        className={Css.merge([
          CssHelper.flexBox(~align=`flexEnd, ()),
          CssHelper.mb(~size=16, ()),
        ])}>
        <NumberCountup value=amountBAND size=Text.Xxxl weight=Text.Regular smallNumber=true />
        <HSpacing size=Spacing.sm />
        <Text value="BAND" size=Text.Lg code=false weight=Text.Thin color={theme.textPrimary} />
      </div>
      <div className={CssHelper.flexBox()}>
        <NumberCountup
          value={amountBAND *. usdPrice}
          size=Text.Lg
          weight=Text.Regular
          color={theme.textSecondary}
        />
        <HSpacing size=Spacing.sm />
        <Text
          value={" USD " ++ "($" ++ (usdPrice |> Js.Float.toString) ++ " / BAND)"}
          size=Text.Lg
          weight=Text.Thin
        />
      </div>
    </>;
  };
};

[@react.component]
let make = (~address, ~hashtag: Route.account_tab_t) => {
  let currentTime =
    React.useContext(TimeContext.context) |> MomentRe.Moment.format(Config.timestampUseFormat);
  let isMobile = Media.isMobile();
  let accountSub = AccountSub.get(address);
  let trackingSub = TrackingSub.use();
  let balanceAtStakeSub = DelegationSub.getTotalStakeByDelegator(address);
  let unbondingSub = UnbondingSub.getUnbondingBalance(address, currentTime);
  let infoSub = React.useContext(GlobalContext.context);
  let (_, dispatchModal) = React.useContext(ModalContext.context);
  let (accountOpt, _) = React.useContext(AccountContext.context);

  let topPartAllSub = Sub.all5(infoSub, accountSub, balanceAtStakeSub, unbondingSub, trackingSub);

  let sumBalance = (balance, amount, unbonding, reward, commission) => {
    let availableBalance = balance->Coin.getBandAmountFromCoins;
    let balanceAtStakeAmount = amount->Coin.getBandAmountFromCoin;
    let unbondingAmount = unbonding->Coin.getBandAmountFromCoin;
    let rewardAmount = reward->Coin.getBandAmountFromCoin;
    let commissionAmount = commission->Coin.getBandAmountFromCoins;

    availableBalance +. balanceAtStakeAmount +. rewardAmount +. unbondingAmount +. commissionAmount;
  };

  let send = chainID => {
    switch (accountOpt) {
    | Some({address: sender}) =>
      let openSendModal = () => Some(address)->SubmitMsg.Send->SubmitTx->OpenModal->dispatchModal;
      if (sender == address) {
        Webapi.Dom.(window |> Window.confirm("Are you sure you want to send tokens to yourself?"))
          ? openSendModal() : ();
      } else {
        openSendModal();
      };
    | None => dispatchModal(OpenModal(Connect(chainID)))
    };
  };

  let qrCode = () => {
    address->QRCode->OpenModal->dispatchModal;
  };
  let addressTag = address->Address.toBech32->VerifyAccount.parseAddressName;

  <Section>
    <div className=CssHelper.container>
      <Row marginBottom=40 marginBottomSm=16>
        <Col> <Heading value="Account Details" size=Heading.H2 /> </Col>
      </Row>
      <Row marginBottom=24 marginBottomSm=16>
        <Col col=Col.Six>
          <div
            className={Css.merge([
              CssHelper.flexBox(~direction=`column, ~justify=`spaceBetween, ~align=`stretch, ()),
              Styles.infoLeft,
            ])}>
            <InfoContainer>
              <div
                className={Css.merge([
                  CssHelper.flexBox(~direction=`column, ~justify=`start, ~align=`flexStart, ()),
                ])}>
                <div className=Styles.addressContainer>
                  <Heading size=Heading.H4 value="Address" marginBottom=25 />
                  <div className={CssHelper.flexBox()}>
                    <AddressRender
                      address
                      position=AddressRender.Subtitle
                      copy=true
                      clickable=false
                      label=false
                    />
                  </div>
                </div>
                {switch (addressTag) {
                 | Some(v) =>
                   <div className={Css.merge([CssHelper.flexBox(), CssHelper.mt(~size=8, ())])}>
                     <Text value={j| ($v) |j} size=Text.Lg block=true />
                   </div>
                 | None => React.null
                 }}
                <div className={Css.merge([CssHelper.flexBox(), Styles.buttonContainer])}>
                  <Button variant=Button.Outline py=5 onClick={_ => {qrCode()}}>
                    <div className={CssHelper.flexBox()}>
                      <Icon size=20 name="far fa-qrcode" mr=8 />
                      {"QR Code" |> React.string}
                    </div>
                  </Button>
                  {isMobile
                     ? React.null
                     : {
                       switch (topPartAllSub, accountOpt) {
                       | (Data(_), Some({address: sender}))
                           when Address.isEqual(sender, address) => React.null
                       | (Data((_, _, _, _, {chainID})), _) =>
                         <Button variant=Button.Outline onClick={_ => {send(chainID)}}>
                           {"Send BAND" |> React.string}
                         </Button>
                       | _ => <LoadingCensorBar width=90 height=26 />
                       };
                     }}
                </div>
              </div>
            </InfoContainer>
            <InfoContainer>
              <div
                className={Css.merge([
                  CssHelper.flexBox(~direction=`column, ~justify=`center, ~align=`flexStart, ()),
                  Styles.detailContainer,
                ])}>
                <Heading size=Heading.H4 value="Total Balance" marginBottom=24 />
                {switch (topPartAllSub) {
                 | Data(({financial}, {balance, commission}, {amount, reward}, unbonding, _)) =>
                   <TotalBalanceRender
                     amountBAND={sumBalance(balance, amount, unbonding, reward, commission)}
                     usdPrice={financial.usdPrice}
                   />

                 | _ =>
                   <>
                     <LoadingCensorBar width=200 height=22 mb=10 />
                     <LoadingCensorBar width=220 height=16 />
                   </>
                 }}
              </div>
            </InfoContainer>
          </div>
        </Col>
        <Col col=Col.Six>
          <InfoContainer>
            <Heading value="Balance" size=Heading.H4 marginBottom=40 />
            <div className=Styles.amountBoxes>
              {switch (topPartAllSub) {
               | Data((_, {balance, commission}, {amount, reward}, unbonding, _)) =>
                 let availableBalance = balance->Coin.getBandAmountFromCoins;
                 let balanceAtStakeAmount = amount->Coin.getBandAmountFromCoin;
                 let unbondingAmount = unbonding->Coin.getBandAmountFromCoin;
                 let rewardAmount = reward->Coin.getBandAmountFromCoin;
                 let commissionAmount = commission->Coin.getBandAmountFromCoins;
                 <AccountBarChart
                   availableBalance
                   balanceAtStake=balanceAtStakeAmount
                   reward=rewardAmount
                   unbonding=unbondingAmount
                   commission=commissionAmount
                 />;
               | _ => <LoadingCensorBar fullWidth=true height=12 radius=50 />
               }}
              <div>
                {switch (topPartAllSub) {
                 | Data(({financial}, {balance}, _, _, _)) =>
                   <BalanceDetails
                     title="Available"
                     description="Balance available to send, delegate, etc"
                     amount={balance->Coin.getBandAmountFromCoins}
                     usdPrice={financial.usdPrice}
                     color=Theme.baseBlue
                   />
                 | _ => <BalanceDetailLoading />
                 }}
              </div>
              <div>
                {switch (topPartAllSub) {
                 | Data(({financial}, _, {amount}, _, _)) =>
                   <BalanceDetails
                     title="Delegated"
                     description="Balance currently delegated to validators"
                     amount={amount->Coin.getBandAmountFromCoin}
                     usdPrice={financial.usdPrice}
                     color=Theme.lightBlue
                   />
                 | _ => <BalanceDetailLoading />
                 }}
              </div>
              <div>
                {switch (topPartAllSub) {
                 | Data(({financial}, _, _, unbonding, _)) =>
                   <BalanceDetails
                     title="Unbonding"
                     description="Amount undelegated from validators awaiting 21 days lockup period"
                     amount={unbonding->Coin.getBandAmountFromCoin}
                     usdPrice={financial.usdPrice}
                     color=Theme.lightenBlue
                   />
                 | _ => <BalanceDetailLoading />
                 }}
              </div>
              <div>
                {switch (topPartAllSub) {
                 | Data(({financial}, _, {reward}, _, _)) =>
                   <BalanceDetails
                     title="Reward"
                     description="Reward from staking to validators"
                     amount={reward->Coin.getBandAmountFromCoin}
                     usdPrice={financial.usdPrice}
                     color=Theme.darkenBlue
                     isCountup=true
                   />
                 | _ => <BalanceDetailLoading />
                 }}
              </div>
              {switch (topPartAllSub) {
               | Data(({financial}, {commission}, _, _, _)) =>
                 let commissionAmount = commission->Coin.getBandAmountFromCoins;
                 commissionAmount == 0.
                   ? React.null
                   : <div>
                       <BalanceDetails
                         title="Commission"
                         description="Reward commission from delegator's reward"
                         amount=commissionAmount
                         usdPrice={financial.usdPrice}
                         isCountup=true
                         color=Theme.darkenBlue
                       />
                     </div>;

               | _ => React.null
               }}
            </div>
          </InfoContainer>
        </Col>
      </Row>
      <Row marginBottom=24 marginBottomSm=16>
        <Col>
          <Table>
            <Tab.Route
              tabs=[|
                {
                  name: "Delegations",
                  route: Route.AccountIndexPage(address, Route.AccountDelegations),
                },
                {
                  name: "Unbonding",
                  route: Route.AccountIndexPage(address, Route.AccountUnbonding),
                },
                {
                  name: "Redelegate",
                  route: Route.AccountIndexPage(address, Route.AccountRedelegate),
                },
              |]
              currentRoute={Route.AccountIndexPage(address, hashtag)}>
              {switch (hashtag) {
               | AccountDelegations => <AccountIndexDelegations address />
               | AccountUnbonding => <AccountIndexUnbonding address />
               | AccountRedelegate => <AccountIndexRedelegate address />
               }}
            </Tab.Route>
          </Table>
        </Col>
      </Row>
      <Row>
        <Col>
          <InfoContainer>
            <Heading value="Transactions" size=Heading.H4 />
            <SeperatedLine mt=32 mb=0 />
            <AccountIndexTransactions accountAddress=address />
          </InfoContainer>
        </Col>
      </Row>
    </div>
  </Section>;
};
