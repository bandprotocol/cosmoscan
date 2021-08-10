module Styles = {
  open Css

  let copy = style(list{cursor(#pointer), position(#relative), zIndex(2)})
}

@react.component
let make = (~width, ~message) => {
  let (copied, setCopy) = React.useState(_ => false)
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

  copied
    ? <div> <Icon name="fal fa-check" color=theme.textPrimary size=width /> </div>
    : <div
        className=Styles.copy
        onClick={_ => {
          Copy.copy(message)
          setCopy(_ => true)
          let _ = Js.Global.setTimeout(() => setCopy(_ => false), 700)
        }}>
        <Icon name="far fa-clone" color=theme.textPrimary size=width />
      </div>
}
