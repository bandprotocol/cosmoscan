module RenderDesktop = {
  module Styles = {
    open Css

    let nav = (isActive, theme: Theme.t) =>
      style(list{
        padding3(~top=#px(16), ~h=#zero, ~bottom=#px(12)),
        cursor(#pointer),
        fontSize(#px(12)),
        hover(list{color(theme.textPrimary)}),
        active(list{color(theme.textPrimary)}),
        transition(~duration=400, "all"),
        color(isActive ? theme.textPrimary : theme.textSecondary),
        borderBottom(#px(4), #solid, isActive ? theme.baseBlue : #transparent),
      })
  }

  @react.component
  let make = (~routes) => {
    let currentRoute = ReasonReactRouter.useUrl() |> Route.fromUrl

    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

    <div className={CssHelper.flexBox(~justify=#spaceBetween, ())} id="navigationBar">
      {routes
      ->Belt.List.map(((v, route)) =>
        <div key=v className={CssHelper.flexBox(~justify=#spaceBetween, ())}>
          <Link className={Styles.nav(currentRoute == route, theme)} route>
            {v |> React.string}
          </Link>
        </div>
      )
      ->Array.of_list
      ->React.array}
    </div>
  }
}

module RenderMobile = {
  module Styles = {
    open Css

    let navContainer = (show, theme: Theme.t) =>
      style(list{
        display(#flex),
        flexDirection(#column),
        alignItems(#center),
        opacity(show ? 1. : 0.),
        zIndex(2),
        pointerEvents(show ? #auto : #none),
        width(#percent(100.)),
        position(#absolute),
        top(#px(58)),
        left(#zero),
        right(#zero),
        transition(~duration=400, "all"),
        backgroundColor(theme.secondaryBg),
        boxShadow(Shadow.box(~x=#zero, ~y=#px(2), ~blur=#px(4), Css.rgba(0, 0, 0, #num(0.08)))),
      })

    let nav = (theme: Theme.t) =>
      style(list{
        color(theme.textPrimary),
        width(#percent(100.)),
        padding2(~v=#px(18), ~h=#zero),
        textAlign(#center),
        boxShadow(
          Shadow.box(~inset=true, ~x=#zero, ~y=#px(-1), ~blur=#px(0), theme.tableRowBorderColor),
        ),
      })

    let menuContainer = style(list{
      marginLeft(#px(10)),
      flexBasis(#px(24)),
      flexGrow(0.),
      flexShrink(0.),
      height(#px(24)),
      textAlign(#center),
    })
    let menu = style(list{width(#px(20)), display(#block)})
    let toggleContainer = style(list{padding2(~v=#px(24), ~h=#zero)})
    let backdropContainer = show =>
      style(list{
        width(#percent(100.)),
        height(#percent(100.)),
        backgroundColor(#rgba(0, 0, 0, #num(0.5))),
        position(#fixed),
        opacity(show ? 1. : 0.),
        pointerEvents(show ? #auto : #none),
        left(#zero),
        top(#px(58)),
        transition(~duration=400, "all"),
      })
  }

  @react.component
  let make = (~routes) => {
    let (show, setShow) = React.useState(_ => false)
    let ({ThemeContext.theme: theme, isDarkMode}, _) = React.useContext(ThemeContext.context)
    let modeMsg = {
      let mode = isDarkMode ? "Lightmode" : "Darkmode"
      "Switch to " ++ mode
    }

    <>
      <div className=Styles.menuContainer onClick={_ => setShow(prev => !prev)}>
        {show
          ? <Icon name="fal fa-times" color=theme.textPrimary size=24 />
          : <Icon name="fal fa-bars" color=theme.textPrimary size=24 />}
      </div>
      <div className={Styles.navContainer(show, theme)}>
        {routes
        ->Belt.List.map(((v, route)) =>
          <Link key=v className={Styles.nav(theme)} route onClick={_ => setShow(_ => false)}>
            <Text value=v weight=Text.Semibold color=theme.textPrimary />
          </Link>
        )
        ->Array.of_list
        ->React.array}
        <div className={Css.merge(list{CssHelper.flexBox(), Styles.toggleContainer})}>
          <ToggleThemeButton />
          <HSpacing size=#px(8) />
          <Text value=modeMsg weight=Text.Semibold color=theme.textPrimary />
        </div>
      </div>
      <div onClick={_ => setShow(prev => !prev)} className={Styles.backdropContainer(show)} />
    </>
  }
}

@react.component
let make = () => {
  let routes = list{
    ("Home", Route.HomePage),
    ("Validators", ValidatorHomePage),
    ("Blocks", BlockHomePage),
    ("Transactions", TxHomePage),
    ("Proposals", ProposalHomePage),
    ("Data Sources", DataSourceHomePage),
    ("Oracle Scripts", OracleScriptHomePage),
    ("Requests", RequestHomePage),
    ("IBCs", IBCHomePage),
  }

  Media.isMobile() ? <RenderMobile routes /> : <RenderDesktop routes />
}
