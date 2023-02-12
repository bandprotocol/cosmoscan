type pos_t =
  | Landing
  | Title
  | Subtitle
  | Text
  | Mini;

let fontSize =
  fun
  | Landing => Text.Xxxl
  | Title => Text.Xxl
  | Subtitle => Text.Lg
  | Text => Text.Md
  | Mini => Text.Sm;

let lineHeight =
  fun
  | Landing => Text.Px(31)
  | Title => Text.Px(23)
  | Subtitle => Text.Px(18)
  | Text => Text.Px(16)
  | Mini => Text.Px(16);

module Styles = {
  open Css;

  let link = (theme: Theme.t) =>
    style([
      width(`percent(100.)),
      cursor(`pointer),
      selector("&:hover > span", [color(theme.primary_600)]),
      selector("> span", [transition(~duration=200, "all")]),
    ]);

  let pointerEvents =
    fun
    | Title => style([pointerEvents(`none)])
    | Landing
    | Subtitle
    | Text
    | Mini => style([pointerEvents(`auto)]);
};

module ComponentCreator = (RawID: ID.IDSig) => {
  [@react.component]
  let make = (~id, ~position=Text, ~primary=false, ~weight=Text.Regular, ~details="") => {
    let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

    <Link
      className={Css.merge([Styles.link(theme), Styles.pointerEvents(position)])}
      route={id |> RawID.getRoute}>
      <div className={CssHelper.flexBox(~wrap=`nowrap, ())}>
        <Text
          value={id |> RawID.toString}
          size={position |> fontSize}
          weight
          height={position |> lineHeight}
          nowrap=true
          code=true
          block=true
          color=theme.primary_600
        />
        {
          details != "" ? <>
            <HSpacing size=Spacing.sm />
            <Text value=details ellipsis=true />
          </> : React.null
        }
      </div>
    </Link>;
  };
};

module PlainLinkCreator = (RawID: ID.IDSig) => {
  [@react.component]
  let make = (~id, ~children, ~style="") => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <Link className={Css.merge([Styles.link(theme), style])} route={id |> RawID.getRoute}>
      children
    </Link>;
  };
};

module DataSource = ComponentCreator(ID.DataSource);
module OracleScript = ComponentCreator(ID.OracleScript);
module Request = ComponentCreator(ID.Request);
module Block = ComponentCreator(ID.Block);
module Proposal = ComponentCreator(ID.Proposal);

module DataSourceLink = PlainLinkCreator(ID.DataSource);
module OracleScriptLink = PlainLinkCreator(ID.OracleScript);
module RequestLink = PlainLinkCreator(ID.Request);
module BlockLink = PlainLinkCreator(ID.Block);
module ProposalLink = PlainLinkCreator(ID.Proposal);
