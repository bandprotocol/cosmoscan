module Styles = {
  open Css;

  let connectContainer = (theme: Theme.t) =>
    style([height(`px(200)), backgroundColor(theme.secondaryBg)]);
  let rewardContainer = (theme: Theme.t) =>
    style([
      backgroundColor(theme.tableRowBorderColor),
      padding2(~v=`px(16), ~h=`px(24)),
      borderRadius(`px(8)),
    ]);

  let buttonContainer =
    style([
      selector("> button + button", [marginLeft(`px(15))]),
      selector("> div + div", [marginLeft(`px(15))]),
    ]);

  let linkStyle = style([color(`currentColor), transition(~duration=0, "all")]);
};

module ButtonSection = {
  [@react.component]
  let make = (~delegatorAddress, ~validatorAddress) => {
    let (_, dispatchModal) = React.useContext(ModalContext.context);
    let validatorInfoSub = ValidatorSub.get(validatorAddress);
    let accountSub = AccountSub.get(delegatorAddress);
    let balanceAtStakeSub = DelegationSub.getStakeByValidator(delegatorAddress, validatorAddress);
    let allSub = Sub.all3(validatorInfoSub, accountSub, balanceAtStakeSub);

    let delegate = () => validatorAddress->SubmitMsg.Delegate->SubmitTx->OpenModal->dispatchModal;
    let undelegate = () =>
      validatorAddress->SubmitMsg.Undelegate->SubmitTx->OpenModal->dispatchModal;
    let redelegate = () =>
      validatorAddress->SubmitMsg.Redelegate->SubmitTx->OpenModal->dispatchModal;

    switch (allSub) {
    | Data((validatorInfo, {balance}, {amount: {amount}})) =>
      let disableNoBalance = balance |> Coin.getBandAmountFromCoins == 0.;
      let disableNoStake = amount == 0.;
      <div
        className={Css.merge([CssHelper.flexBox(), Styles.buttonContainer])}
        id="validatorDelegationinfoDlegate">
        <Button
          px=20
          py=8
          disabled=disableNoBalance
          variant=Button.Outline
          onClick={_ => {
            validatorInfo.commission == 100.
              ? Webapi.Dom.(
                  window
                  |> Window.alert("Delegation to foundation validator nodes is not advised.")
                )
              : delegate()
          }}>
          {"Delegate" |> React.string}
        </Button>
        <Button
          px=20 py=8 variant=Button.Outline disabled=disableNoStake onClick={_ => undelegate()}>
          {"Undelegate" |> React.string}
        </Button>
        <Button
          px=20 py=8 variant=Button.Outline disabled=disableNoStake onClick={_ => redelegate()}>
          {"Redelegate" |> React.string}
        </Button>
      </div>;
    | _ =>
      <div className={Css.merge([CssHelper.flexBox(), Styles.buttonContainer])}>
        <LoadingCensorBar width=100 height=33 />
        <LoadingCensorBar width=100 height=33 />
        <LoadingCensorBar width=100 height=33 />
      </div>
    };
  };
};

module DisplayBalance = {
  module Loading = {
    [@react.component]
    let make = () => {
      <>
        <LoadingCensorBar width=120 height=15 />
        <VSpacing size=Spacing.xs />
        <LoadingCensorBar width=80 height=15 />
      </>;
    };
  };

  [@react.component]
  let make = (~amount, ~usdPrice, ~isCountup=false) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);

    <>
      <div className={CssHelper.flexBox()}>
        {isCountup
           ? <NumberCountup
               value={amount->Coin.getBandAmountFromCoin}
               size=Text.Lg
               weight=Text.Regular
               spacing={Text.Em(0.)}
               color={theme.textSecondary}
               code=false
             />
           : <Text
               value={amount->Coin.getBandAmountFromCoin |> Format.fPretty(~digits=6)}
               size=Text.Lg
               color={theme.textSecondary}
               block=true
             />}
        <HSpacing size=Spacing.sm />
        <Text value="BAND" size=Text.Lg color={theme.textSecondary} block=true />
      </div>
      <div className={CssHelper.flexBox()}>
        {isCountup
           ? <NumberCountup
               value={amount->Coin.getBandAmountFromCoin *. usdPrice}
               size=Text.Md
               weight=Text.Regular
               color={theme.textSecondary}
               code=false
               spacing={Text.Em(0.)}
             />
           : <Text
               value={amount->Coin.getBandAmountFromCoin *. usdPrice |> Format.fPretty(~digits=6)}
               size=Text.Md
               color={theme.textSecondary}
               block=true
             />}
        <HSpacing size=Spacing.sm />
        <Text value="USD" size=Text.Md color={theme.textSecondary} block=true />
      </div>
    </>;
  };
};

module StakingInfo = {
  [@react.component]
  let make = (~delegatorAddress, ~validatorAddress) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
    let currentTime =
      React.useContext(TimeContext.context) |> MomentRe.Moment.format(Config.timestampUseFormat);
    let (_, dispatchModal) = React.useContext(ModalContext.context);

    let infoSub = React.useContext(GlobalContext.context);
    let balanceAtStakeSub = DelegationSub.getStakeByValidator(delegatorAddress, validatorAddress);
    let unbondingSub =
      UnbondingSub.getUnbondingBalanceByValidator(
        delegatorAddress,
        validatorAddress,
        currentTime,
      );

    let allSub = Sub.all3(infoSub, balanceAtStakeSub, unbondingSub);

    let withdrawReward = () => {
      validatorAddress->SubmitMsg.WithdrawReward->SubmitTx->OpenModal->dispatchModal;
    };

    let reinvest = reward =>
      (validatorAddress, reward)->SubmitMsg.Reinvest->SubmitTx->OpenModal->dispatchModal;
    <>
      <Row marginBottom=24>
        <Col>
          <Text
            value="Note: You have non-zero pending reward on this validator. Any additional staking actions will automatically withdraw that reward your balance."
            color=Colors.gray6
            weight=Text.Thin
          />
        </Col>
      </Row>
      <Row marginBottom=24>
        <Col col=Col.Six>
          <div className={CssHelper.mb(~size=16, ())}>
            <Heading
              value="Balance at Stake"
              size=Heading.H5
              color={theme.textSecondary}
              marginBottom=8
            />
            {switch (allSub) {
             | Data(({financial: {usdPrice}}, balanceAtStake, _)) =>
               <DisplayBalance amount={balanceAtStake.amount} usdPrice />
             | _ => <DisplayBalance.Loading />
             }}
          </div>
          <ButtonSection validatorAddress delegatorAddress />
        </Col>
        <Col col=Col.Three>
          <div className={CssHelper.mb(~size=16, ())}>
            <div className={CssHelper.flexBox()}>
              <Heading
                value="Unbonding Amount"
                size=Heading.H5
                marginBottom=8
                color={theme.textSecondary}
              />
            </div>
            {switch (allSub) {
             | Data(({financial: {usdPrice}}, _, unbonding)) =>
               <DisplayBalance amount=unbonding usdPrice />
             | _ => <DisplayBalance.Loading />
             }}
          </div>
          // <Button
          //   px=20
          //   py=8
          //   variant=Button.Outline
          //   onClick={event => {
          //     let route = Route.AccountIndexPage(delegatorAddress, Route.AccountUnbonding);
          //     if (!event->ReactEvent.Mouse.defaultPrevented
          //         && event->ReactEvent.Mouse.button == 0
          //         && !event->ReactEvent.Mouse.altKey
          //         && !event->ReactEvent.Mouse.ctrlKey
          //         && !event->ReactEvent.Mouse.metaKey
          //         && !event->ReactEvent.Mouse.shiftKey) {
          //       event->ReactEvent.Mouse.preventDefault;
          //       route->Route.redirect;
          //     };
          //   }}>
          //   {"View Entries" |> React.string}
          // </Button>
          <Button px=20 py=8 variant=Button.Outline onClick={_ => ()}>
            <Link
              className=Styles.linkStyle
              route={Route.AccountIndexPage(delegatorAddress, Route.AccountUnbonding)}>
              {"View Entries" |> React.string}
            </Link>
          </Button>
        </Col>
        <Col col=Col.Three>
          <div className={CssHelper.mb(~size=16, ())}>
            <Heading value="Reward" size=Heading.H5 color={theme.textSecondary} marginBottom=8 />
            {switch (allSub) {
             | Data(({financial: {usdPrice}}, balanceAtStake, _)) =>
               <DisplayBalance amount={balanceAtStake.reward} usdPrice isCountup=true />
             | _ => <DisplayBalance.Loading />
             }}
          </div>
          <div
            className={Css.merge([CssHelper.flexBox(), Styles.buttonContainer])}
            id="withdrawRewardContainer">
            {let (disable, reward) =
               switch (allSub) {
               | Data((_, balanceAtStake, _)) => (
                   balanceAtStake.reward.amount <= 0.,
                   balanceAtStake.reward.amount,
                 )
               | _ => (true, 0.)
               };

             <>
               <Button
                 px=20
                 py=8
                 variant=Button.Outline
                 onClick={_ => withdrawReward()}
                 disabled=disable>
                 {"Claim" |> React.string}
               </Button>
               <Button
                 px=20
                 py=8
                 variant=Button.Outline
                 onClick={_ => reinvest(reward)}
                 disabled=disable>
                 {"Reinvest" |> React.string}
               </Button>
             </>}
          </div>
        </Col>
      </Row>
    </>;
  };
};

[@react.component]
let make = (~validatorAddress) => {
  let trackingSub = TrackingSub.use();
  let (accountOpt, _) = React.useContext(AccountContext.context);
  let (_, dispatchModal) = React.useContext(ModalContext.context);
  let (ThemeContext.{theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  let connect = chainID => dispatchModal(OpenModal(Connect(chainID)));

  <InfoContainer>
    <div className={CssHelper.flexBox(~justify=`spaceBetween, ())}>
      <div className={CssHelper.flexBox()}>
        <Heading value="Your Delegation Info" size=Heading.H4 />
        <HSpacing size=Spacing.xs />
        <CTooltip tooltipText="Your delegation stats on this validators">
          <Icon name="fal fa-info-circle" size=10 />
        </CTooltip>
      </div>
    </div>
    <SeperatedLine mt=32 mb=24 />
    {switch (accountOpt) {
     | Some({address: delegatorAddress}) => <StakingInfo validatorAddress delegatorAddress />
     | None =>
       switch (trackingSub) {
       | Data({chainID}) =>
         <div
           className={Css.merge([
             CssHelper.flexBox(~direction=`column, ~justify=`center, ()),
             Styles.connectContainer(theme),
           ])}>
           <Icon name="fal fa-link" size=32 color={isDarkMode ? theme.white : theme.black} />
           <VSpacing size={`px(16)} />
           <Text value="Please connect to make request" size=Text.Lg nowrap=true />
           <VSpacing size={`px(16)} />
           <Button px=20 py=5 onClick={_ => connect(chainID)}>
             {"Connect" |> React.string}
           </Button>
         </div>
       | Error(err) =>
         // log for err details
         Js.Console.log(err);
         <Text value="chain id not found" />;
       | _ => <LoadingCensorBar.CircleSpin height=200 />
       }
     }}
  </InfoContainer>;
};
