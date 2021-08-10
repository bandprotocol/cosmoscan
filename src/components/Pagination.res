module Styles = {
  open Css

  let container = style(list{
    display(#flex),
    flexDirection(#row),
    width(#percent(100.)),
    justifyContent(#center),
    minHeight(#px(30)),
    borderRadius(#px(8)),
    padding2(~v=#px(24), ~h=#zero),
    Media.mobile(list{padding2(~v=#px(12), ~h=#zero)}),
  })

  let innerContainer = style(list{
    display(#flex),
    alignItems(#center),
    Media.mobile(list{
      width(#percent(100.)),
      justifyContent(#spaceBetween),
      padding2(~v=#zero, ~h=#px(5)),
    }),
  })

  let clickable = (active, theme: Theme.t, isDarkMode) =>
    style(list{
      cursor(#pointer),
      width(#px(32)),
      height(#px(32)),
      borderRadius(#px(8)),
      border(#px(1), #solid, active ? theme.textPrimary : theme.textSecondary),
      pointerEvents(active ? #auto : #none),
      opacity(active ? 1. : 0.5),
      hover(list{
        backgroundColor(theme.textPrimary),
        selector("> i", list{color(isDarkMode ? theme.black : theme.white)}),
      }),
    })

  let paginationBox = style(list{
    margin2(~v=zero, ~h=#px(32)),
    selector("> * + *", list{marginLeft(#px(20))}),
  })
}
module ClickableSymbol = {
  @react.component
  let make = (~isPrevious, ~active, ~onClick) => {
    let ({ThemeContext.theme: theme, isDarkMode}, _) = React.useContext(ThemeContext.context)

    <div
      className={Css.merge(list{
        Styles.clickable(active, theme, isDarkMode),
        CssHelper.flexBox(~justify=#center, ()),
      })}
      onClick>
      {isPrevious
        ? <Icon name="far fa-angle-left" color=theme.textPrimary size=18 />
        : <Icon name="far fa-angle-right" color=theme.textPrimary size=18 />}
    </div>
  }
}

@react.component
let make = (~currentPage, ~pageCount, ~onPageChange: int => unit) => {
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

  if pageCount > 1 {
    <div className=Styles.container>
      <div className=Styles.innerContainer>
        <ClickableSymbol
          isPrevious=true
          active={currentPage != 1}
          onClick={_ => onPageChange(currentPage < 1 ? 1 : currentPage - 1)}
        />
        <div
          className={Css.merge(list{
            CssHelper.flexBox(~justify=#center, ()),
            Styles.paginationBox,
          })}>
          <Text
            value={currentPage |> Format.iPretty}
            size=Text.Lg
            weight=Text.Semibold
            color=theme.textPrimary
          />
          <Text value="of" size=Text.Lg />
          <Text
            value={pageCount |> Format.iPretty}
            weight=Text.Semibold
            size=Text.Lg
            color=theme.textPrimary
          />
        </div>
        <ClickableSymbol
          isPrevious=false
          active={currentPage != pageCount}
          onClick={_ => onPageChange(currentPage > pageCount ? pageCount : currentPage + 1)}
        />
      </div>
    </div>
  } else {
    React.null
  }
}
