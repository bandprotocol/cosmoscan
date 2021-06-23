module Styles = {
  open Css;
  let msgBadge = msgType =>
    style([
      backgroundColor(
        switch (msgType) {
        | MsgDecoder.TokenMsg => Theme.baseBlue
        | ValidatorMsg => Theme.baseBlue
        | ProposalMsg => Theme.baseBlue
        | DataMsg => Theme.baseBlue
        | _ => Theme.baseBlue
        },
      ),
      borderRadius(`px(50)),
      margin2(~v=`zero, ~h=`px(5)),
      padding2(~v=`px(3), ~h=`px(8)),
      whiteSpace(`pre),
    ]);
};

[@react.component]
let make = (~msgType: MsgDecoder.msg_cat_t, ~name) => {
  <div
    className={Css.merge([
      Styles.msgBadge(msgType),
      CssHelper.flexBox(~wrap=`nowrap, ~justify=`center, ()),
    ])}>
    <Text value=name size=Text.Sm color=Theme.white transform=Text.Uppercase align=Text.Center />
  </div>;
};
