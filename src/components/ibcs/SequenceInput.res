module Styles = {
  open Css
  let searchContainer = mW =>
    style(list{maxWidth(#px(mW)), display(#flex), alignItems(#center), position(#relative)})
  let iconContainer = style(list{
    position(#absolute),
    left(#px(16)),
    top(#percent(50.)),
    transform(#translateY(#percent(-50.))),
  })
  let searchBar = (theme: Theme.t) =>
    style(list{
      width(#percent(100.)),
      height(#px(37)),
      paddingLeft(#px(44)),
      paddingRight(#px(9)),
      borderRadius(#px(4)),
      fontSize(#px(14)),
      fontWeight(#light),
      border(#px(1), #solid, theme.tableRowBorderColor),
      backgroundColor(theme.inputContrastColor),
      outlineStyle(#none),
      color(theme.textPrimary),
      fontFamilies(list{#custom("Montserrat"), #custom("sans-serif")}),
    })
}

@react.component
let make = (~placeholder, ~onChange, ~value, ~disabled, ~maxWidth=150) => {
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

  <div>
    <div className={CssHelper.mb(~size=8, ())}> <Text value="Sequence" /> </div>
    <div className={Styles.searchContainer(maxWidth)}>
      <div className=Styles.iconContainer>
        <Icon name="far fa-search" color=theme.textSecondary size=14 />
      </div>
      <input
        disabled
        type_="number"
        className={Styles.searchBar(theme)}
        placeholder
        value
        onChange={event => {
          let newVal =
            ReactEvent.Form.target(event)["value"] |> String.lowercase_ascii |> String.trim
          onChange(_ => newVal)
        }}
      />
    </div>
  </div>
}
