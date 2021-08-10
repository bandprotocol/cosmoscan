module Styles = {
  open Css
  let badge = color =>
    style(list{backgroundColor(color), padding2(~v=#px(3), ~h=#px(10)), borderRadius(#px(50))})
}

let getBadgeText = x =>
  switch x {
  | ProposalSub.Deposit => "Deposit Period"
  | Voting => "Voting Period"
  | Passed => "Passed"
  | Rejected => "Rejected"
  | Failed => "Failed"
  }

let getBadgeColor = x =>
  switch x {
  | ProposalSub.Deposit
  | Voting => Theme.baseBlue
  | Passed => Theme.successColor
  | Rejected
  | Failed => Theme.failColor
  }

@react.component
let make = (~status) =>
  <div
    className={Css.merge(list{
      Styles.badge(getBadgeColor(status)),
      CssHelper.flexBox(~justify=#center, ()),
    })}>
    <Text value={getBadgeText(status)} size=Text.Sm transform=Text.Uppercase color=Theme.white />
  </div>
