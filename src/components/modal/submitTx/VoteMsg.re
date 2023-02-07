module Styles = {
  open Css;

  let container = style([paddingBottom(`px(24))]);

  let buttonGroup = (theme: Theme.t) =>
    style([
      margin4(~top=`px(0), ~right=`px(-12), ~bottom=`px(23), ~left=`px(-12)),
      selector(
        "> button",
        [
          flexGrow(0.),
          flexShrink(0.),
          flexBasis(`calc((`sub, `percent(50.), `px(24)))),
          margin2(~v=`zero, ~h=`px(12)),
          borderColor(theme.loadingBaseColor),
          position(`relative),
          disabled([
            color(theme.neutral_900),
            opacity(1.),
            border(`px(1), `solid, theme.baseBlue),
            hover([backgroundColor(`transparent)]),
            after([
              contentRule(`text("\f00c")),
              fontFamily(`custom("'Font Awesome 5 Pro'")),
              fontWeight(`light),
              borderRadius(`percent(50.)),
              fontSize(`px(10)),
              lineHeight(`em(1.8)),
              display(`block),
              position(`absolute),
              pointerEvents(`none),
              top(`px(-8)),
              right(`px(-8)),
              color(theme.white),
              backgroundColor(theme.baseBlue),
              width(`px(20)),
              height(`px(20)),
            ]),
          ]),
        ],
      ),
    ]);
};

module VoteInput = {
  [@react.component]
  let make = (~setAnswerOpt, ~answerOpt) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <div className={Css.merge([CssHelper.flexBox(), Styles.buttonGroup(theme)])}>
        <Button
          variant=Button.Outline
          px=15
          py=9
          onClick={_ => setAnswerOpt(_ => Some(1))}
          disabled={answerOpt == Some(1)}>
          {"Yes" |> React.string}
        </Button>
        <Button
          variant=Button.Outline
          px=15
          py=9
          onClick={_ => setAnswerOpt(_ => Some(3))}
          disabled={answerOpt == Some(3)}>
          {"No" |> React.string}
        </Button>
      </div>
      <div className={Css.merge([CssHelper.flexBox(), Styles.buttonGroup(theme)])}>
        <Button
          variant=Button.Outline
          px=15
          py=9
          onClick={_ => setAnswerOpt(_ => Some(4))}
          disabled={answerOpt == Some(4)}>
          {"No with Veto" |> React.string}
        </Button>
        <Button
          variant=Button.Outline
          px=15
          py=9
          onClick={_ => setAnswerOpt(_ => Some(2))}
          disabled={answerOpt == Some(2)}>
          {"Abstain" |> React.string}
        </Button>
      </div>
    </>;
  };
};

[@react.component]
let make = (~proposalID, ~proposalName, ~setMsgsOpt) => {
  let (answerOpt, setAnswerOpt) = React.useState(_ => None);
  React.useEffect1(
    _ => {
      let msgsOpt = {
        let%Opt answer = answerOpt;
        Some([|TxCreator2.Vote(proposalID, answer)|]);
      };
      setMsgsOpt(_ => msgsOpt);
      None;
    },
    [|answerOpt|],
  );

  <>
    <div className=Styles.container>
      <Text value="Vote to" size=Text.Md weight=Text.Medium nowrap=true block=true />
      <VSpacing size=Spacing.sm />
      <div className={CssHelper.flexBox()}>
        <TypeID.Proposal id=proposalID position=TypeID.Text />
        <HSpacing size=Spacing.sm />
        <Text value=proposalName nowrap=true block=true />
      </div>
    </div>
    <VoteInput answerOpt setAnswerOpt />
  </>;
};
