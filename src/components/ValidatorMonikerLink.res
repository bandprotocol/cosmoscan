module Styles = {
  open Css

  let container = (w, theme: Theme.t) =>
    style(list{
      display(#flex),
      cursor(#pointer),
      width(w),
      alignItems(#center),
      selector("> span:hover", list{color(theme.baseBlue)}),
      selector("> span", list{transition(~duration=200, "all")}),
    })
}

@react.component
let make = (
  ~validatorAddress: Address.t,
  ~moniker: string,
  ~identity=?,
  ~weight=Text.Semibold,
  ~size=Text.Md,
  ~underline=false,
  ~width=#auto,
  ~avatarWidth=20,
) => {
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

  <Link
    className={Styles.container(width, theme)}
    route=Route.ValidatorIndexPage(validatorAddress, Reports)>
    {switch identity {
    | Some(identity') => <>
        <Avatar moniker identity=identity' width=avatarWidth /> <HSpacing size=Spacing.sm />
      </>
    | None => React.null
    }}
    <Text
      value=moniker
      color=theme.textPrimary
      weight
      block=true
      size
      nowrap=true
      ellipsis=true
      underline
    />
  </Link>
}
