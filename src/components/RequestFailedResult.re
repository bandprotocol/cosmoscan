module Styles = {
  open Css;
  let reasonSection =
    style([
      padding2(~v=`px(24), ~h=`px(40)),
      important(border(`px(1), solid, Theme.failColor)),
      borderRadius(`px(12)),
      marginTop(`px(40)),
      display(`flex),
      alignItems(`center),
    ]);
  let resultBox = style([padding(`px(20))]);
  let labelWrapper = style([flexShrink(0.), flexGrow(0.), flexBasis(`px(220))]);
  let resultWrapper =
    style([
      flexShrink(0.),
      flexGrow(0.),
      flexBasis(`calc((`sub, `percent(100.), `px(220)))),
    ]);
};

[@react.component]
let make = (~id) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  let requestSub = RequestSub.get(id);
  <>
    <div className={Css.merge([CssHelper.flexBox(), Styles.resultBox])}>
      {switch (requestSub) {
       | Data({resolveStatus, resolveHeight}) =>
         <>
           <div className=Styles.labelWrapper>
             <Text value="Resolve Status" color={theme.textSecondary} weight=Text.Regular />
           </div>
           <div className={CssHelper.flexBox()}>
             <RequestStatus resolveStatus display=RequestStatus.Full size=Text.Md />
             {switch (resolveHeight) {
              | Some(height) =>
                <>
                  <HSpacing size=Spacing.md />
                  <Text value=" (" block=true color={theme.textPrimary} />
                  <TypeID.Block id=height />
                  <Text value=")" block=true color={theme.textPrimary} />
                </>
              | None => React.null
              }}
           </div>
         </>
       | _ => <LoadingCensorBar.CircleSpin height=90 />
       }}
    </div>
    {switch (requestSub) {
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
     }}
  </>;
};
