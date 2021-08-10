type pos_t =
  | Landing
  | Title
  | Subtitle
  | Text
  | Mini

let fontSize = x =>
  switch x {
  | Landing => Text.Xxxl
  | Title => Text.Xxl
  | Subtitle => Text.Lg
  | Text => Text.Md
  | Mini => Text.Sm
  }

let lineHeight = x =>
  switch x {
  | Landing => Text.Px(31)
  | Title => Text.Px(23)
  | Subtitle => Text.Px(18)
  | Text => Text.Px(16)
  | Mini => Text.Px(16)
  }

module Styles = {
  open Css

  let link = (theme: Theme.t) =>
    style(list{
      cursor(#pointer),
      selector("&:hover > span", list{color(theme.baseBlue)}),
      selector("> span", list{transition(~duration=200, "all")}),
    })

  let pointerEvents = x =>
    switch x {
    | Title => style(list{pointerEvents(#none)})
    | Landing
    | Subtitle
    | Text
    | Mini =>
      style(list{pointerEvents(#auto)})
    }
}

module ComponentCreator = (RawID: ID.IDSig) => {
  @react.component
  let make = (~id, ~position=Text) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

    <Link
      className={Css.merge(list{Styles.link(theme), Styles.pointerEvents(position)})}
      route={id |> RawID.getRoute}>
      <Text
        value={id |> RawID.toString}
        size={position |> fontSize}
        weight=Text.Semibold
        height={position |> lineHeight}
        nowrap=true
        code=true
        block=true
        color=theme.textPrimary
      />
    </Link>
  }
}

module PlainLinkCreator = (RawID: ID.IDSig) => {
  @react.component
  let make = (~id, ~children, ~style="") => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Link className={Css.merge(list{Styles.link(theme), style})} route={id |> RawID.getRoute}>
      children
    </Link>
  }
}

module DataSource = ComponentCreator(ID.DataSource)
module OracleScript = ComponentCreator(ID.OracleScript)
module Request = ComponentCreator(ID.Request)
module Block = ComponentCreator(ID.Block)
module Proposal = ComponentCreator(ID.Proposal)

module DataSourceLink = PlainLinkCreator(ID.DataSource)
module OracleScriptLink = PlainLinkCreator(ID.OracleScript)
module RequestLink = PlainLinkCreator(ID.Request)
module BlockLink = PlainLinkCreator(ID.Block)
module ProposalLink = PlainLinkCreator(ID.Proposal)
