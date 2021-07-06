module Styles = {
  open Css;

  let msgContainer = style([selector("> * + *", [marginLeft(`px(5))])]);
};

module Packet = {
  [@react.component]
  let make = (~packetType) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <div
      className={Css.merge([
        CssHelper.flexBox(~wrap=`nowrap, ()),
        CssHelper.overflowHidden,
        Styles.msgContainer,
      ])}>
      <Text value=packetType nowrap=true block=true />
    </div>;
  };
};
