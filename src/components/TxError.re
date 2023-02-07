module Styles = {
  open Css;

  let errorContainer = (theme: Theme.t) =>
    style([
      padding(`px(10)),
      color(theme.failColor),
      backgroundColor(theme.neutral_000),
      border(`px(1), `solid, theme.failColor),
      borderRadius(`px(4)),
      marginBottom(`px(24)),
      selector("> i", [marginRight(`px(8))]),
    ]);
};

type log_t = {message: string};

type err_t = {log: option(string)};

let decodeLog = json => JsonUtils.Decode.{message: json |> field("message", string)};

let decode = json => JsonUtils.Decode.{log: json |> optional(field("log", string))};

let parseErr = msg => {
  let err =
    {
      let%Opt json = msg |> Json.parse;
      let%Opt x = json |> Js.Json.decodeArray;
      let%Opt y = x->Belt.Array.get(0);
      let%Opt logStr = (y |> decode).log;
      let%Opt logJson = logStr |> Json.parse;
      let log = logJson |> decodeLog;
      Opt.ret(log.message);
    }
    |> Belt.Option.getWithDefault(_, msg);
  "Error: " ++ err;
};

module Full = {
  [@react.component]
  let make = (~msg) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <div
      className={Css.merge([
        Styles.errorContainer(theme),
        CssHelper.flexBox(~wrap=`nowrap, ()),
      ])}>
      <Icon name="fal fa-exclamation-circle" size=14 color={theme.failColor} />
      <Text
        value={msg |> parseErr}
        size=Text.Lg
        spacing={Text.Em(0.02)}
        breakAll=true
        color={theme.neutral_900}
      />
    </div>;
  };
};

module Mini = {
  [@react.component]
  let make = (~msg) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Text value={msg |> parseErr} code=true size=Text.Sm breakAll=true color={theme.failColor} />;
  };
};
