type position =
  | Title
  | Subtitle
  | Text;

module Styles = {
  open Css;

  let container = style([display(`flex), cursor(`pointer), overflow(`hidden)]);

  let clickable = (isActive, theme: Theme.t) =>
    isActive
      ? style([
          pointerEvents(`auto),
          transition(~duration=200, "all"),
          color(theme.textPrimary),
          hover([color(theme.baseBlue)]),
          active([color(theme.baseBlue)]),
        ])
      : style([
          pointerEvents(`none),
          color(Colors.gray7),
          hover([color(Colors.gray7)]),
          active([color(Colors.gray7)]),
        ]);

  let prefix = style([fontWeight(`num(600))]);

  let font =
    fun
    | Title =>
      style([fontSize(`px(18)), lineHeight(`em(1.41)), Media.mobile([fontSize(px(14))])])
    | Subtitle =>
      style([fontSize(`px(14)), lineHeight(`em(1.41)), Media.mobile([fontSize(`px(12))])])
    | Text => style([fontSize(`px(12)), lineHeight(`em(1.41))]);

  let base =
    style([overflow(`hidden), textOverflow(`ellipsis), whiteSpace(`nowrap), display(`block)]);

  let wordBreak =
    style([Media.mobile([textOverflow(`unset), whiteSpace(`unset), wordBreak(`breakAll)])]);

  let copy = style([width(`px(15)), marginLeft(`px(10)), cursor(`pointer)]);

  let setWidth =
    fun
    | Title => style([Media.mobile([width(`percent(90.))])])
    | _ => "";

  let mobileWidth =
    style([
      width(`calc((`sub, `percent(100.), `px(45)))),
      Media.mobile([width(`calc((`sub, `percent(100.), `px(20))))]),
    ]);
};

[@react.component]
let make =
    (
      ~address,
      ~position=Text,
      ~accountType=`account,
      ~copy=false,
      ~clickable=true,
      ~wordBreak=false,
    ) => {
  let isValidator = accountType == `validator;
  let prefix = isValidator ? "bandvaloper" : "band";

  let noPrefixAddress =
    isValidator
      ? address |> Address.toOperatorBech32 |> Js.String.sliceToEnd(~from=11)
      : address |> Address.toBech32 |> Js.String.sliceToEnd(~from=4);

  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <>
    <Link
      className={Css.merge([
        Styles.container,
        Styles.clickable(clickable, theme),
        Text.Styles.code,
        Styles.setWidth(position),
        copy ? Styles.mobileWidth : "",
      ])}
      route={
        isValidator
          ? Route.ValidatorIndexPage(address, Route.ProposedBlocks)
          : Route.AccountIndexPage(address, Route.AccountTransactions)
      }>
      <span
        className={Css.merge([
          Styles.base,
          Styles.font(position),
          wordBreak ? Styles.wordBreak : "",
        ])}>
        <span className=Styles.prefix> {prefix |> React.string} </span>
        {noPrefixAddress |> React.string}
      </span>
    </Link>
    {copy
       ? <>
           {switch (position) {
            | Title => <HSpacing size=Spacing.md />
            | _ => <HSpacing size=Spacing.sm />
            }}
           <CopyRender
             width={
               switch (position) {
               | Title => 15
               | _ => 12
               }
             }
             message={
               isValidator ? address |> Address.toOperatorBech32 : address |> Address.toBech32
             }
           />
         </>
       : React.null}
  </>;
};
