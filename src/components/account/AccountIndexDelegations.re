module Styles = {
  open Css;

  let tableWrapper = style([Media.mobile([padding2(~v=`px(16), ~h=`zero)])]);
  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
  let actionText = style([cursor(`pointer), marginTop(`px(8)), marginRight(`px(24))]);
};

module RenderBody = {
  [@react.component]
  let make = (~delegationsSub: ApolloHooks.Subscription.variant(DelegationSub.stake_t)) => {
    let (_, dispatchModal) = React.useContext(ModalContext.context);
    let (accountOpt, _) = React.useContext(AccountContext.context);
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);

    <TBody>
      <Row alignItems=Row.Center>
        <Col col=Col.Four>
          {switch (delegationsSub) {
           | Data({moniker, operatorAddress, identity}) =>
             <div className={CssHelper.flexBox()}>
               <ValidatorMonikerLink
                 validatorAddress=operatorAddress
                 moniker
                 identity
                 width={`px(300)}
                 avatarWidth=30
                 size=Text.Lg
               />
             </div>
           | _ => <LoadingCensorBar width=200 height=20 />
           }}
        </Col>
        <Col col=Col.Four>
          <div className={CssHelper.flexBox(~justify=`flexStart, ())}>
            {switch (delegationsSub) {
             | Data({amount, operatorAddress, delegatorAddress}) =>
               let delegate = () =>
                 operatorAddress->SubmitMsg.Delegate->SubmitTx->OpenModal->dispatchModal;
               let undelegate = () =>
                 operatorAddress->SubmitMsg.Undelegate->SubmitTx->OpenModal->dispatchModal;
               let redelegate = () =>
                 operatorAddress->SubmitMsg.Redelegate->SubmitTx->OpenModal->dispatchModal;

               <div className={CssHelper.flexBox(~direction=`column, ~align=`flexStart, ())}>
                 <Text value={amount |> Coin.getBandAmountFromCoin |> Format.fPretty} />
                 {switch (accountOpt) {
                  | Some({address}) when Address.isEqual(address, delegatorAddress) =>
                    <div className={CssHelper.flexBox()}>
                      <div className=Styles.actionText onClick={_ => delegate()}>
                        <Text value="Delegate" underline=true color={theme.textPrimary} />
                      </div>
                      <div className=Styles.actionText onClick={_ => redelegate()}>
                        <Text value="Redelegate" underline=true color={theme.textPrimary} />
                      </div>
                      <div className=Styles.actionText onClick={_ => undelegate()}>
                        <Text value="Undelegate" underline=true color={theme.textPrimary} />
                      </div>
                    </div>
                  | _ => React.null
                  }}
               </div>;
             | _ => <LoadingCensorBar width=200 height=20 />
             }}
          </div>
        </Col>
        <Col col=Col.Four>
          <div className={CssHelper.flexBox(~justify=`flexStart, ())}>
            {switch (delegationsSub) {
             | Data({reward, operatorAddress, delegatorAddress}) =>
               let withdrawReward = () => {
                 operatorAddress->SubmitMsg.WithdrawReward->SubmitTx->OpenModal->dispatchModal;
               };
               let reinvest = _ =>
                 (operatorAddress, reward.amount)
                 ->SubmitMsg.Reinvest
                 ->SubmitTx
                 ->OpenModal
                 ->dispatchModal;

               <div className={CssHelper.flexBox(~direction=`column, ~align=`flexStart, ())}>
                 <Text value={reward |> Coin.getBandAmountFromCoin |> Format.fPretty} />
                 {switch (accountOpt) {
                  | Some({address}) when Address.isEqual(address, delegatorAddress) =>
                    <div className={CssHelper.flexBox()}>
                      <div className=Styles.actionText onClick={_ => withdrawReward()}>
                        <Text value="Claim" underline=true color={theme.textPrimary} />
                      </div>
                      <div className=Styles.actionText onClick={_ => reinvest()}>
                        <Text value="Reinvest" underline=true color={theme.textPrimary} />
                      </div>
                    </div>
                  | _ => React.null
                  }}
               </div>;
             | _ => <LoadingCensorBar width=200 height=20 />
             }}
          </div>
        </Col>
      </Row>
    </TBody>;
  };
};

module RenderBodyMobile = {
  [@react.component]
  let make =
      (~reserveIndex, ~delegationsSub: ApolloHooks.Subscription.variant(DelegationSub.stake_t)) => {
    switch (delegationsSub) {
    | Data({amount, moniker, operatorAddress, reward, identity}) =>
      let key_ =
        (operatorAddress |> Address.toHex)
        ++ (amount |> Coin.getBandAmountFromCoin |> Js.Float.toString)
        ++ (reward |> Coin.getBandAmountFromCoin |> Js.Float.toString)
        ++ (reserveIndex |> string_of_int);
      <MobileCard
        values=InfoMobileCard.[
          ("Validator", Validator(operatorAddress, moniker, identity)),
          ("Amount\n(BAND)", Coin({value: [amount], hasDenom: false})),
          ("Reward\n(BAND)", Coin({value: [reward], hasDenom: false})),
        ]
        key=key_
        idx=key_
      />;
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("Validator", Loading(230)),
          ("Amount\n(BAND)", Loading(100)),
          ("Reward\n(BAND)", Loading(100)),
        ]
        key={reserveIndex |> string_of_int}
        idx={reserveIndex |> string_of_int}
      />
    };
  };
};

[@react.component]
let make = (~address) => {
  let isMobile = Media.isMobile();
  let (page, setPage) = React.useState(_ => 1);
  let pageSize = 5;
  let delegationsCountSub = DelegationSub.getStakeCountByDelegator(address);
  let delegationsSub = DelegationSub.getStakeList(address, ~pageSize, ~page, ());

  let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

  <div className=Styles.tableWrapper>
    {isMobile
       ? <Row marginBottom=16>
           <Col>
             {switch (delegationsCountSub) {
              | Data(delegationsCount) =>
                <div className={CssHelper.flexBox()}>
                  <Text
                    block=true
                    value={delegationsCount |> string_of_int}
                    weight=Text.Semibold
                    size=Text.Sm
                    transform=Text.Uppercase
                  />
                  <HSpacing size=Spacing.xs />
                  <Text
                    block=true
                    value="Validators Delegated"
                    weight=Text.Semibold
                    size=Text.Sm
                    transform=Text.Uppercase
                  />
                </div>
              | _ => <LoadingCensorBar width=100 height=15 />
              }}
           </Col>
         </Row>
       : <THead>
           <Row alignItems=Row.Center>
             <Col col=Col.Four>
               {switch (delegationsCountSub) {
                | Data(delegationsCount) =>
                  <div className={CssHelper.flexBox()}>
                    <Text
                      block=true
                      value={delegationsCount |> string_of_int}
                      weight=Text.Semibold
                      size=Text.Sm
                      transform=Text.Uppercase
                    />
                    <HSpacing size=Spacing.xs />
                    <Text
                      block=true
                      value="Validators Delegated"
                      weight=Text.Semibold
                      size=Text.Sm
                      transform=Text.Uppercase
                    />
                  </div>
                | _ => <LoadingCensorBar width=100 height=15 />
                }}
             </Col>
             <Col col=Col.Four>
               <Text
                 block=true
                 value="Amount (BAND)"
                 weight=Text.Semibold
                 size=Text.Sm
                 transform=Text.Uppercase
               />
             </Col>
             <Col col=Col.Four>
               <Text
                 block=true
                 value="Reward (BAND)"
                 weight=Text.Semibold
                 size=Text.Sm
                 transform=Text.Uppercase
               />
             </Col>
           </Row>
         </THead>}
    {switch (delegationsSub) {
     | Data(delegations) =>
       delegations->Belt.Array.size > 0
         ? delegations
           ->Belt_Array.mapWithIndex((i, e) =>
               isMobile
                 ? <RenderBodyMobile
                     key={
                       (e.operatorAddress |> Address.toBech32)
                       ++ (address |> Address.toBech32)
                       ++ (i |> string_of_int)
                     }
                     reserveIndex=i
                     delegationsSub={Sub.resolve(e)}
                   />
                 : <RenderBody
                     key={
                       (e.operatorAddress |> Address.toBech32)
                       ++ (address |> Address.toBech32)
                       ++ (i |> string_of_int)
                     }
                     delegationsSub={Sub.resolve(e)}
                   />
             )
           ->React.array
         : <EmptyContainer>
             <img
               src={isDarkMode ? Images.noDataDark : Images.noDataLight}
               className=Styles.noDataImage
             />
             <Heading
               size=Heading.H4
               value="No Delegation"
               align=Heading.Center
               weight=Heading.Regular
               color={theme.textSecondary}
             />
           </EmptyContainer>
     | _ =>
       Belt_Array.make(pageSize, ApolloHooks.Subscription.NoData)
       ->Belt_Array.mapWithIndex((i, noData) =>
           isMobile
             ? <RenderBodyMobile key={i |> string_of_int} reserveIndex=i delegationsSub=noData />
             : <RenderBody key={i |> string_of_int} delegationsSub=noData />
         )
       ->React.array
     }}
    {switch (delegationsCountSub) {
     | Data(delegationsCount) =>
       let pageCount = Page.getPageCount(delegationsCount, pageSize);
       <Pagination currentPage=page pageCount onPageChange={newPage => setPage(_ => newPage)} />;
     | _ => React.null
     }}
  </div>;
};
