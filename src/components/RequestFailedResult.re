module Styles = {
  open Css;

  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
  let kvTableContainer = style([padding(`px(1))]);
  let kvTableHeader =
    style([
      padding4(~top=`px(16), ~left=`px(24), ~right=`px(24), ~bottom=`px(12)),
      Media.mobile([padding2(~v=`px(14), ~h=`px(12))]),
    ]);

  let kvTableMobile = style([margin(`zero), boxShadow(`none)]);
  let addressContainer = style([Media.mobile([width(`px(260))])]);
  let validatorReportStatus = style([marginBottom(`px(13))]);
  let noPaddingBottom = style([paddingBottom(`zero), Media.mobile([paddingBottom(`zero)])]);
  let ibcBadge =
    style([
      display(`flex),
      padding2(~v=`px(4), ~h=`px(16)),
      background(Theme.baseBlue),
      borderRadius(`px(10)),
      marginLeft(`px(8)),
    ]);
  let reasonSection =
    style([
      padding2(~v=`px(24), ~h=`px(40)),
      border(`px(1), solid, Theme.failColor),
      borderRadius(`px(12)),
      marginTop(`px(40)),
      display(`flex),
      alignItems(`center),
    ]);
};

[@react.component]
let make = (~id) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  let requestSub = RequestSub.get(id);
  switch (requestSub) {
  | Data({reason}) =>
    switch (reason) {
    | Some(reason') when reason' !== "" =>
      <div className=Styles.reasonSection>
        <img alt="Fail Icon" src=Images.fail />
        <HSpacing size=Spacing.md />
        <Text value=reason' color={theme.textPrimary} />
      </div>
    | _ => React.null
    }
  | _ => React.null
  };
};
