module Styles = {
  open Css
  let chartWrapper = style(list{minHeight(px(220))})
  let chartContainer = style(list{
    width(#percent(100.)),
    maxWidth(#px(210)),
    margin2(~v=#zero, ~h=#auto),
  })
  let statusLabel = style(list{height(#px(8)), width(#px(8))})

  let blockContainer = style(list{
    flexGrow(0.),
    flexShrink(0.),
    flexBasis(#calc(#sub, #percent(10.), #px(2))),
    margin(#px(1)),
    height(#px(18)),
    display(#block),
    Media.smallMobile(list{height(#px(10))}),
  })
  let blockBase = style(list{width(#percent(100.)), height(#percent(100.))})
  let status = x =>
    switch x {
    | ValidatorSub.Missed => style(list{backgroundColor(hex("937EF1"))})
    | Proposed => style(list{backgroundColor(hex("4520E6"))})
    | Signed => style(list{backgroundColor(hex("230E81"))})
    }

  let labelBox = style(list{
    margin2(~v=#zero, ~h=#px(-12)),
    selector(
      "> div",
      list{
        flexGrow(0.),
        flexShrink(0.),
        flexBasis(#calc(#sub, #percent(33.33), #px(24))),
        margin2(~v=#zero, ~h=#px(12)),
      },
    ),
    Media.smallMobile(list{
      margin2(~v=#zero, ~h=#px(-5)),
      selector(
        "> div",
        list{flexBasis(#calc(#sub, #percent(33.33), #px(10))), margin2(~v=#zero, ~h=#px(5))},
      ),
    }),
  })
}

module UptimeBlock = {
  @react.component
  let make = (~status, ~height) => {
    let blockRoute = height |> ID.Block.getRoute
    <CTooltip
      width=90
      tooltipPlacement=CTooltip.Top
      tooltipPlacementSm=CTooltip.BottomLeft
      mobile=false
      align=#center
      pd=10
      tooltipText={height |> ID.Block.toString}
      styles=Styles.blockContainer>
      <Link route=blockRoute className=Styles.blockContainer>
        <div className={Css.merge(list{Styles.blockBase, Styles.status(status)})} />
      </Link>
    </CTooltip>
  }
}

@react.component
let make = (~consensusAddress) => {
  let getUptimeSub = ValidatorSub.getBlockUptimeByValidator(consensusAddress)
  <>
    <Row marginBottom=24>
      <Col>
        <div className={Css.merge(list{CssHelper.flexBox(), Styles.chartWrapper})}>
          <div className={Css.merge(list{CssHelper.flexBox(), Styles.chartContainer})}>
            {switch getUptimeSub {
            | Data({validatorVotes}) =>
              validatorVotes
              ->Belt.Array.map(({blockHeight, status}) =>
                <UptimeBlock key={blockHeight |> ID.Block.toString} status height=blockHeight />
              )
              ->React.array
            | _ => <LoadingCensorBar.CircleSpin height=90 />
            }}
          </div>
        </div>
      </Col>
    </Row>
    <Row>
      <Col>
        <div className={Css.merge(list{CssHelper.flexBox(), Styles.labelBox})}>
          <div className={CssHelper.flexBox(~justify=#spaceBetween, ())}>
            <div className={CssHelper.flexBox()}>
              <div
                className={Css.merge(list{
                  Styles.status(ValidatorSub.Proposed),
                  Styles.statusLabel,
                })}
              />
              <HSpacing size=Spacing.sm />
              <Text block=true value="Proposed" weight=Text.Semibold />
            </div>
            {switch getUptimeSub {
            | Data({proposedCount}) => <Text block=true value={proposedCount |> string_of_int} />
            | _ => <LoadingCensorBar width=20 height=14 />
            }}
          </div>
          <div className={CssHelper.flexBox(~justify=#spaceBetween, ())}>
            <div className={CssHelper.flexBox()}>
              <div
                className={Css.merge(list{Styles.status(ValidatorSub.Signed), Styles.statusLabel})}
              />
              <HSpacing size=Spacing.sm />
              <Text block=true value="Signed" weight=Text.Semibold />
            </div>
            {switch getUptimeSub {
            | Data({signedCount}) => <Text block=true value={signedCount |> string_of_int} />
            | _ => <LoadingCensorBar width=20 height=14 />
            }}
          </div>
          <div className={CssHelper.flexBox(~justify=#spaceBetween, ())}>
            <div className={CssHelper.flexBox()}>
              <div
                className={Css.merge(list{Styles.status(ValidatorSub.Missed), Styles.statusLabel})}
              />
              <HSpacing size=Spacing.sm />
              <Text block=true value="Missed" weight=Text.Semibold />
            </div>
            {switch getUptimeSub {
            | Data({missedCount}) => <Text block=true value={missedCount |> string_of_int} />
            | _ => <LoadingCensorBar width=20 height=14 />
            }}
          </div>
        </div>
      </Col>
    </Row>
  </>
}
