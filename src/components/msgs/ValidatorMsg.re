module Styles = {
  open Css;

  let msgContainer = style([selector("> * + *", [marginLeft(`px(5))])]);
};

module CreateValidator = {
  [@react.component]
  let make = (~moniker) => {
    <div
      className={Css.merge([
        CssHelper.flexBox(~wrap=`nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      ])}>
      <Text value=moniker size=Text.Md nowrap=true block=true />
    </div>;
  };
};

module EditValidator = {
  [@react.component]
  let make = (~moniker) => {
    <div
      className={Css.merge([
        CssHelper.flexBox(~wrap=`nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      ])}>
      <Text value=moniker size=Text.Md nowrap=true block=true />
    </div>;
  };
};

module Grant = {
  [@react.component]
  let make = (~reporter) => {
    <div
      className={Css.merge([
        CssHelper.flexBox(~wrap=`nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      ])}>
      <AddressRender address=reporter />
    </div>;
  };
};

module Revoke = {
  [@react.component]
  let make = (~reporter) => {
    <div
      className={Css.merge([
        CssHelper.flexBox(~wrap=`nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      ])}>
      <AddressRender address=reporter />
    </div>;
  };
};

module Exec = {
  [@react.component]
  let make = (~messages) => {
    <div
      className={Css.merge([
        CssHelper.flexBox(~wrap=`nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      ])}>
      {messages
       ->Belt.List.mapWithIndex((index, msg) => {
           <Text
             key={(msg |> ExecDecoder.getName) ++ (index |> string_of_int)}
             value={msg |> ExecDecoder.getName}
           />
         })
       ->Belt.List.toArray
       ->React.array}
    </div>;
  };
};

module SetWithdrawAddress = {
  [@react.component]
  let make = (~withdrawAddress) => {
    <div
      className={Css.merge([
        CssHelper.flexBox(~wrap=`nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      ])}>
      <Text value={j| to |j} size=Text.Md nowrap=true block=true />
      <AddressRender address=withdrawAddress />
    </div>;
  };
};
