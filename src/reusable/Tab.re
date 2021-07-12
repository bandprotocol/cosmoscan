module Styles = {
  open Css;

  let container = style([Media.mobile([margin2(~h=`px(-12), ~v=`zero)])]);
  let header = (theme: Theme.t) =>
    style([
      borderBottom(`px(1), `solid, theme.tableRowBorderColor),
      selector("> * + *", [marginLeft(`px(32))]),
      Media.mobile([
        overflow(`auto),
        padding2(~v=`px(1), ~h=`px(15)),
        selector("&::-webkit-scrollbar", [display(`none)]),
      ]),
    ]);

  let buttonContainer = active =>
    style([
      display(`inlineFlex),
      justifyContent(`center),
      alignItems(`center),
      cursor(`pointer),
      padding2(~v=`px(32), ~h=`zero),
      borderBottom(`px(4), `solid, active ? Theme.baseBlue : `transparent),
      Media.mobile([whiteSpace(`nowrap), padding2(~v=`px(24), ~h=`zero)]),
    ]);

  let childrenContainer = style([Media.mobile([padding2(~h=`px(16), ~v=`zero)])]);
};

module Route = {
  type t = {
    name: string,
    route: Route.t,
  };

  let button = (~name, ~route, ~active) => {
    <Link key=name isTab=true className={Styles.buttonContainer(active)} route>
      <Text value=name weight={active ? Text.Semibold : Text.Regular} size=Text.Lg />
    </Link>;
  };

  [@react.component]
  let make = (~tabs: array(t), ~currentRoute, ~children) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <div className=Styles.container>
      <div className={Css.merge([Styles.header(theme), CssHelper.flexBox(~wrap=`nowrap, ())])}>
        {tabs
         ->Belt.Array.map(({name, route}) =>
             button(~name, ~route, ~active=route == currentRoute)
           )
         ->React.array}
      </div>
      <div className=Styles.childrenContainer> children </div>
    </div>;
  };
};

module State = {
  let button = (~name, ~active, ~setTab) => {
    <div key=name className={Styles.buttonContainer(active)} onClick={_ => setTab()}>
      <Text value=name weight={active ? Text.Semibold : Text.Regular} size=Text.Lg />
    </div>;
  };

  [@react.component]
  let make = (~tabs: array(string), ~tabIndex, ~setTab, ~children) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <div className=Styles.container>
      <div className={Css.merge([Styles.header(theme), CssHelper.flexBox(~wrap=`nowrap, ())])}>
        {tabs
         ->Belt.Array.mapWithIndex((index, name) =>
             button(~name, ~active=index == tabIndex, ~setTab=() => setTab(index))
           )
         ->React.array}
      </div>
      <div className=Styles.childrenContainer> children </div>
    </div>;
  };
};
