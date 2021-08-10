module Styles = {
  open Css

  let errorContainer = (theme: Theme.t) =>
    style(list{
      padding(#px(10)),
      color(theme.failColor),
      backgroundColor(theme.mainBg),
      border(#px(1), #solid, theme.failColor),
      borderRadius(#px(4)),
      marginBottom(#px(24)),
      selector("> i", list{marginRight(#px(8))}),
    })
}

type log_t = {message: string}

type err_t = {log: option<string>}

let decodeLog = json => {
  open JsonUtils.Decode
  {message: json |> field("message", string)}
}

let decode = json => {
  open JsonUtils.Decode
  {log: json |> optional(field("log", string))}
}

let parseErr = msg => {
  let err = %Opt({
    let json = msg |> Json.parse
    %Opt({
      let x = json |> Js.Json.decodeArray
      %Opt({
        let y = x->Belt.Array.get(0)
        %Opt({
          let logStr = (y |> decode).log
          %Opt({
            let logJson = logStr |> Json.parse
            let log = logJson |> decodeLog
            Opt.ret(log.message)
          })
        })
      })
    })
  }) |> Belt.Option.getWithDefault(_, msg)
  "Error: " ++ err
}

module Full = {
  @react.component
  let make = (~msg) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

    <div
      className={Css.merge(list{
        Styles.errorContainer(theme),
        CssHelper.flexBox(~wrap=#nowrap, ()),
      })}>
      <Icon name="fal fa-exclamation-circle" size=14 color=Colors.red5 />
      <Text
        value={msg |> parseErr}
        size=Text.Lg
        spacing=Text.Em(0.02)
        breakAll=true
        color=theme.textPrimary
      />
    </div>
  }
}

module Mini = {
  @react.component
  let make = (~msg) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

    <Text value={msg |> parseErr} code=true size=Text.Sm breakAll=true color=theme.failColor />
  }
}
