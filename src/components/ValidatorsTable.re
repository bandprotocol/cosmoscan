module Styles = {
  open Css;

  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);

  let sortableTHead = isRight =>
    style([
      display(`flex),
      flexDirection(`row),
      alignItems(`center),
      cursor(`pointer),
      justifyContent(isRight ? `flexEnd : `flexStart),
    ]);

  let oracleStatus = style([display(`flex), justifyContent(`center)]);
  let logo = style([width(`px(20))]);
};

module RenderBody = {
  [@react.component]
  let make =
      (
        ~rank,
        ~validatorSub: ApolloHooks.Subscription.variant(ValidatorSub.t),
        ~votingPower,
        ~dispatchModal: ModalContext.a => unit,
        ~isLogin,
      ) => {
    <TBody>
      <Row alignItems=Row.Center>
        <Col col=Col.One>
          {switch (validatorSub) {
           | Data(_) => <Text value={rank |> string_of_int} block=true />
           | _ => <LoadingCensorBar width=20 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          {switch (validatorSub) {
           | Data({operatorAddress, moniker, identity}) =>
             <ValidatorMonikerLink
               validatorAddress=operatorAddress
               moniker
               identity
               width={`px(180)}
             />
           | _ => <LoadingCensorBar width=150 height=15 />
           }}
        </Col>
        <Col col=Col.Two>
          {switch (validatorSub) {
           | Data({tokens}) =>
             <div>
               <Text
                 value={tokens |> Coin.getBandAmountFromCoin |> Format.fPretty(~digits=0)}
                 block=true
               />
               <VSpacing size=Spacing.sm />
               <Text
                 value={"(" ++ (votingPower |> Format.fPercent(~digits=2)) ++ ")"}
                 block=true
               />
             </div>
           | _ =>
             <>
               <LoadingCensorBar width=100 height=15 />
               <VSpacing size=Spacing.sm />
               <LoadingCensorBar width=40 height=15 />
             </>
           }}
        </Col>
        <Col col=Col.Two>
          {switch (validatorSub) {
           | Data({commission}) =>
             <Text value={commission |> Format.fPercent(~digits=2)} block=true />
           | _ => <LoadingCensorBar width=70 height=15 />
           }}
        </Col>
        <Col col={isLogin ? Col.Two : Three}>
          {switch (validatorSub) {
           | Data({uptime}) =>
             switch (uptime) {
             | Some(uptime') =>
               <>
                 <Text value={uptime' |> Format.fPercent(~digits=2)} block=true />
                 <VSpacing size=Spacing.sm />
                 <ProgressBar.Uptime percent=uptime' />
               </>
             | None => <Text value="N/A" block=true />
             }
           | _ =>
             <>
               <LoadingCensorBar width=50 height=15 />
               <VSpacing size=Spacing.sm />
               <LoadingCensorBar width=130 height=15 />
             </>
           }}
        </Col>
        <Col col={isLogin ? Col.Three : Two}>
          <div className={CssHelper.flexBox(~justify=isLogin ? `spaceBetween : `center, ())}>
            <div className=Styles.oracleStatus>
              {switch (validatorSub) {
               | Data({oracleStatus}) =>
                 <img src={oracleStatus ? Images.success : Images.fail} className=Styles.logo />
               | _ => <LoadingCensorBar width=20 height=20 radius=50 />
               }}
            </div>
            {isLogin
               ? <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
                   {switch (validatorSub) {
                    | Data({operatorAddress, commission}) =>
                      let delegate = () =>
                        operatorAddress->SubmitMsg.Delegate->SubmitTx->OpenModal->dispatchModal;
                      <Button
                        variant=Button.Outline
                        onClick={_ => {
                          commission == 100.
                            ? Webapi.Dom.(
                                window
                                |> Window.alert(
                                     "Delegation to foundation validator nodes is not advised.",
                                   )
                              )
                            : delegate()
                        }}>
                        {"Delegate" |> React.string}
                      </Button>;
                    | _ => <LoadingCensorBar width=90 height=33 radius=8 />
                    }}
                 </div>
               : React.null}
          </div>
        </Col>
      </Row>
    </TBody>;
  };
};

module RenderBodyMobile = {
  [@react.component]
  let make =
      (~rank, ~validatorSub: ApolloHooks.Subscription.variant(ValidatorSub.t), ~votingPower) => {
    switch (validatorSub) {
    | Data({operatorAddress, moniker, identity, tokens, commission, uptime, oracleStatus}) =>
      <MobileCard
        values=InfoMobileCard.[
          ("Rank", Count(rank)),
          ("Validator", Validator(operatorAddress, moniker, identity)),
          ("Voting\nPower", VotingPower(tokens, votingPower)),
          ("Commission", Float(commission, Some(2))),
          ("Uptime (%)", Uptime(uptime)),
          ("Oracle Status", Status(oracleStatus)),
        ]
        key={rank |> string_of_int}
        idx={rank |> string_of_int}
      />
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("Rank", Loading(70)),
          ("Validator", Loading(166)),
          ("Voting\nPower", Loading(166)),
          ("Commission", Loading(136)),
          ("Uptime (%)", Loading(200)),
          ("Oracle Status", Loading(20)),
        ]
        key={rank |> string_of_int}
        idx={rank |> string_of_int}
      />
    };
  };
};

let addUptimeOnValidators =
    (validators: array(ValidatorSub.t), votesBlock: array(ValidatorSub.validator_vote_t)) => {
  validators->Belt.Array.map(validator => {
    let signedBlock =
      votesBlock
      ->Belt.Array.keep(({consensusAddress, voted}) =>
          Address.isEqual(validator.consensusAddress, consensusAddress) && voted == true
        )
      ->Belt.Array.get(0)
      ->Belt.Option.mapWithDefault(0, ({count}) => count)
      |> float_of_int;

    let missedBlock =
      votesBlock
      ->Belt.Array.keep(({consensusAddress, voted}) =>
          Address.isEqual(validator.consensusAddress, consensusAddress) && voted == false
        )
      ->Belt.Array.get(0)
      ->Belt.Option.mapWithDefault(0, ({count}) => count)
      |> float_of_int;

    {
      ...validator,
      uptime:
        signedBlock == 0. && missedBlock == 0.
          ? None : Some(signedBlock /. (signedBlock +. missedBlock) *. 100.),
    };
  });
};
type sort_direction_t =
  | ASC
  | DESC;

type sort_by_t =
  | NameAsc
  | NameDesc
  | VotingPowerAsc
  | VotingPowerDesc
  | CommissionAsc
  | CommissionDesc
  | UptimeAsc
  | UptimeDesc;

let getDirection =
  fun
  | NameAsc
  | VotingPowerAsc
  | CommissionAsc
  | UptimeAsc => ASC
  | NameDesc
  | VotingPowerDesc
  | CommissionDesc
  | UptimeDesc => DESC;

let getName =
  fun
  | NameAsc => "Validator Name (A-Z)"
  | NameDesc => "Validator name (Z-A)"
  | VotingPowerAsc => "Voting Power (Low-High)"
  | VotingPowerDesc => "Voting Power (High-Low)"
  | CommissionAsc => "Commission (Low-High)"
  | CommissionDesc => "Commission (High-Low)"
  | UptimeAsc => "Uptime (Low-High)"
  | UptimeDesc => "Uptime (High-Low)";

let compareString = (a, b) => {
  let removeEmojiRegex = [%re
    "/([\u2700-\u27BF]|[\uE000-\uF8FF]|\uD83C[\uDC00-\uDFFF]|\uD83D[\uDC00-\uDFFF]|[\u2011-\u26FF]|\uD83E[\uDD10-\uDDFF])/g"
  ];
  let a_ = a->Js.String2.replaceByRe(removeEmojiRegex, "");
  let b_ = b->Js.String2.replaceByRe(removeEmojiRegex, "");
  Js.String.localeCompare(a_, b_) |> int_of_float;
};

let defaultCompare = (a: ValidatorSub.t, b: ValidatorSub.t) =>
  if (a.tokens != b.tokens) {
    compare(b.tokens, a.tokens);
  } else {
    compareString(b.moniker, a.moniker);
  };

let sorting = (validators: array(ValidatorSub.t), sortedBy) => {
  validators
  ->Belt.List.fromArray
  ->Belt.List.sort((a, b) => {
      let result = {
        switch (sortedBy) {
        | NameAsc => compareString(b.moniker, a.moniker)
        | NameDesc => compareString(a.moniker, b.moniker)
        | VotingPowerAsc => compare(a.tokens, b.tokens)
        | VotingPowerDesc => compare(b.tokens, a.tokens)
        | CommissionAsc => compare(a.commission, b.commission)
        | CommissionDesc => compare(b.commission, a.commission)
        | UptimeAsc =>
          compare(
            a.uptime->Belt.Option.getWithDefault(0.),
            b.uptime->Belt.Option.getWithDefault(0.),
          )
        | UptimeDesc =>
          compare(
            b.uptime->Belt.Option.getWithDefault(0.),
            a.uptime->Belt.Option.getWithDefault(0.),
          )
        };
      };
      if (result != 0) {
        result;
      } else {
        defaultCompare(a, b);
      };
    })
  ->Belt.List.toArray;
};

module SortableTHead = {
  [@react.component]
  let make =
      (
        ~title,
        ~asc,
        ~desc,
        ~toggle,
        ~sortedBy,
        ~isRight=false,
        ~tooltipItem=?,
        ~tooltipPlacement=Text.AlignBottomStart,
      ) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <div className={Styles.sortableTHead(isRight)} onClick={_ => toggle(asc, desc)}>
      <Text
        block=true
        value=title
        size=Text.Sm
        weight=Text.Semibold
        transform=Text.Uppercase
        tooltipItem={tooltipItem->Belt_Option.mapWithDefault(React.null, React.string)}
        tooltipPlacement
      />
      <HSpacing size=Spacing.xs />
      {if (sortedBy == asc) {
         <Icon name="fas fa-caret-down" color={theme.textSecondary} />;
       } else if (sortedBy == desc) {
         <Icon name="fas fa-caret-up" color={theme.textSecondary} />;
       } else {
         <Icon name="fas fa-sort" color={theme.textSecondary} />;
       }}
    </div>;
  };
};

[@react.component]
let make = (~allSub, ~searchTerm, ~sortedBy, ~setSortedBy) => {
  let isMobile = Media.isMobile();
  let pageSize = 10;
  let toggle = (sortedByAsc, sortedByDesc) =>
    if (sortedBy == sortedByDesc) {
      setSortedBy(_ => sortedByAsc);
    } else {
      setSortedBy(_ => sortedByDesc);
    };

  let (accountOpt, _) = React.useContext(AccountContext.context);
  let (_, dispatchModal) = React.useContext(ModalContext.context);
  let ({ThemeContext.isDarkMode, theme}, _) = React.useContext(ThemeContext.context);

  let isLogin = accountOpt |> Belt.Option.isSome;

  <>
    {isMobile
       ? React.null
       : <THead>
           <Row alignItems=Row.Center>
             <Col col=Col.One>
               <Text
                 block=true
                 value="Rank"
                 weight=Text.Semibold
                 transform=Text.Uppercase
                 size=Text.Sm
               />
             </Col>
             <Col col=Col.Two>
               <SortableTHead
                 title="Validator"
                 asc=NameAsc
                 desc=NameDesc
                 toggle
                 sortedBy
                 isRight=false
               />
             </Col>
             <Col col=Col.Two>
               <SortableTHead
                 title="Voting Power"
                 asc=VotingPowerAsc
                 desc=VotingPowerDesc
                 toggle
                 sortedBy
                 tooltipItem="Sum of self-bonded and delegated tokens"
               />
             </Col>
             <Col col=Col.Two>
               <SortableTHead
                 title="Commision"
                 asc=CommissionAsc
                 desc=CommissionDesc
                 toggle
                 sortedBy
                 tooltipItem="Validator service fees charged to delegators"
               />
             </Col>
             <Col col={isLogin ? Col.Two : Three}>
               <SortableTHead
                 title="Uptime (%)"
                 asc=UptimeAsc
                 desc=UptimeDesc
                 toggle
                 sortedBy
                 isRight=false
                 tooltipItem="Percentage of the blocks that the validator is active for out of the last 100"
               />
             </Col>
             <Col col=Col.Two>
               <Text
                 block=true
                 transform=Text.Uppercase
                 size=Text.Sm
                 weight=Text.Semibold
                 align={isLogin ? Text.Left : Center}
                 value="Oracle Status"
                 tooltipItem={"The validator's Oracle status" |> React.string}
               />
             </Col>
           </Row>
         </THead>}
    {switch (allSub) {
     | ApolloHooks.Subscription.Data((
         (_, _, bondedTokenCount: Coin.t, _, _),
         rawValidators,
         votesBlock,
       )) =>
       let validators = addUptimeOnValidators(rawValidators, votesBlock);
       let filteredValidator =
         searchTerm |> Js.String.length == 0
           ? validators
           : validators->Belt_Array.keep(validator => {
               Js.String.includes(searchTerm, validator.moniker |> Js.String.toLowerCase)
             });
       <>
         {filteredValidator->Belt_Array.size > 0
            ? filteredValidator
              ->sorting(sortedBy)
              ->Belt_Array.map(e => {
                  let votingPower = e.votingPower /. bondedTokenCount.amount *. 100.;
                  isMobile
                    ? <RenderBodyMobile
                        key={e.rank |> string_of_int}
                        rank={e.rank}
                        validatorSub={Sub.resolve(e)}
                        votingPower
                      />
                    : <RenderBody
                        key={e.rank |> string_of_int}
                        rank={e.rank}
                        validatorSub={Sub.resolve(e)}
                        votingPower
                        dispatchModal
                        isLogin
                      />;
                })
              ->React.array
            : <EmptyContainer>
                <img
                  src={isDarkMode ? Images.noDelegatorDark : Images.noDelegatorLight}
                  className=Styles.noDataImage
                />
                <Heading
                  size=Heading.H4
                  value="No Validator"
                  align=Heading.Center
                  weight=Heading.Regular
                  color={theme.textSecondary}
                />
              </EmptyContainer>}
       </>;
     | _ =>
       Belt_Array.make(pageSize, ApolloHooks.Subscription.NoData)
       ->Belt_Array.mapWithIndex((i, noData) =>
           isMobile
             ? <RenderBodyMobile
                 key={i |> string_of_int}
                 rank=i
                 validatorSub=noData
                 votingPower=1.0
               />
             : <RenderBody
                 key={i |> string_of_int}
                 rank=i
                 validatorSub=noData
                 votingPower=1.0
                 isLogin
                 dispatchModal={_ => ()}
               />
         )
       ->React.array
     }}
  </>;
};
