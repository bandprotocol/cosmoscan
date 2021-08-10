type position =
  | Title
  | Subtitle
  | Text

module Styles = {
  open Css

  let container = style(list{display(#flex), cursor(#pointer), overflow(#hidden)})

  let clickable = (isActive, theme: Theme.t) =>
    isActive
      ? style(list{
          pointerEvents(#auto),
          transition(~duration=200, "all"),
          color(theme.textPrimary),
          hover(list{color(theme.baseBlue)}),
          active(list{color(theme.baseBlue)}),
        })
      : style(list{
          pointerEvents(#none),
          color(theme.textSecondary),
          hover(list{color(theme.textSecondary)}),
          active(list{color(theme.textSecondary)}),
        })

  let prefix = style(list{fontWeight(#num(600))})

  let font = x =>
    switch x {
    | Title =>
      style(list{fontSize(#px(18)), lineHeight(#em(1.41)), Media.mobile(list{fontSize(px(14))})})
    | Subtitle =>
      style(list{fontSize(#px(14)), lineHeight(#em(1.41)), Media.mobile(list{fontSize(#px(12))})})
    | Text => style(list{fontSize(#px(12)), lineHeight(#em(1.41))})
    }

  let base = style(list{
    overflow(#hidden),
    textOverflow(#ellipsis),
    whiteSpace(#nowrap),
    display(#block),
  })

  let wordBreak = style(list{
    Media.mobile(list{textOverflow(#unset), whiteSpace(#unset), wordBreak(#breakAll)}),
  })

  let copy = style(list{width(#px(15)), marginLeft(#px(10)), cursor(#pointer)})

  let setWidth = x =>
    switch x {
    | Title => style(list{Media.mobile(list{width(#percent(90.))})})
    | _ => ""
    }

  let mobileWidth = style(list{
    width(#calc(#sub, #percent(100.), #px(45))),
    Media.mobile(list{width(#calc(#sub, #percent(100.), #px(20)))}),
  })
}

@react.component
let make = (
  ~address,
  ~position=Text,
  ~accountType=#account,
  ~copy=false,
  ~clickable=true,
  ~wordBreak=false,
) => {
  let isValidator = accountType == #validator
  let prefix = isValidator ? "bandvaloper" : "band"

  let noPrefixAddress = isValidator
    ? address |> Address.toOperatorBech32 |> Js.String.sliceToEnd(~from=11)
    : address |> Address.toBech32 |> Js.String.sliceToEnd(~from=4)

  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

  <>
    <Link
      className={Css.merge(list{
        Styles.container,
        Styles.clickable(clickable, theme),
        Text.Styles.code,
        Styles.setWidth(position),
        copy ? Styles.mobileWidth : "",
      })}
      route={isValidator
        ? Route.ValidatorIndexPage(address, Route.ProposedBlocks)
        : Route.AccountIndexPage(address, Route.AccountDelegations)}>
      <span
        className={Css.merge(list{
          Styles.base,
          Styles.font(position),
          wordBreak ? Styles.wordBreak : "",
        })}>
        <span className=Styles.prefix> {prefix |> React.string} </span>
        {noPrefixAddress |> React.string}
      </span>
    </Link>
    {copy
      ? <>
          {switch position {
          | Title => <HSpacing size=Spacing.md />
          | _ => <HSpacing size=Spacing.sm />
          }}
          <CopyRender
            width={switch position {
            | Title => 15
            | _ => 12
            }}
            message={isValidator
              ? address |> Address.toOperatorBech32
              : address |> Address.toBech32}
          />
        </>
      : React.null}
  </>
}
