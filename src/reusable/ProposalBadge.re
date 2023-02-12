module Styles = {
  open Css;
  let badge = color => {
    style([backgroundColor(color), padding2(~v=`px(3), ~h=`px(10)), borderRadius(`px(50))]);
  };
};

let getBadgeText =
  fun
  | ProposalSub.Deposit => "Deposit Period"
  | Voting => "Voting Period"
  | Passed => "Passed"
  | Rejected => "Rejected"
  | Failed => "Failed";

let getBadgeColor = (theme: Theme.t) =>
  fun
  | ProposalSub.Deposit
  | Voting => theme.primary_600
  | Passed => theme.successColor
  | Rejected
  | Failed => theme.failColor;

[@react.component]
let make = (~status) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div
    className={Css.merge([
      Styles.badge(getBadgeColor(theme)(status)),
      CssHelper.flexBox(~justify=`center, ()),
    ])}>
    <Text value={getBadgeText(status)} size=Text.Sm transform=Text.Uppercase color=Theme.white />
  </div>;
};
