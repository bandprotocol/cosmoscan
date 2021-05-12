module Styles = {
  open Css;

  let sortDrowdownContainer = style([position(`relative), zIndex(2), maxWidth(`px(60))]);
  let sortDrowdownPanel = show => {
    style([
      display(
        {
          show ? `block : `none;
        },
      ),
      boxShadow(
        Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.08))),
      ),
      backgroundColor(Colors.white),
      position(`absolute),
      right(`zero),
      top(`percent(100.)),
      width(`px(165)),
    ]);
  };
  let sortDropdownItem = (isActive, theme: Theme.t) => {
    style([
      backgroundColor(
        {
          isActive ? theme.dropdownHover : theme.secondaryBg;
        },
      ),
      transition(~duration=200, "all"),
      cursor(`pointer),
      padding2(~v=`px(8), ~h=`px(10)),
    ]);
  };
  let sortDropdownTextItem = {
    style([paddingRight(`px(15)), cursor(`pointer)]);
  };

  let iconContainer =
    style([
      display(`block),
      position(`absolute),
      top(`percent(50.)),
      right(`zero),
      transform(translateY(`percent(-50.))),
    ]);
};

[@react.component]
let make = (~sortedBy, ~setSortedBy, ~sortList) => {
  let (show, setShow) = React.useState(_ => false);
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <div className=Styles.sortDrowdownContainer>
    <div className=Styles.sortDropdownTextItem onClick={_ => setShow(prev => !prev)}>
      <Text block=true value="Sort By" align=Text.Right />
      <div className=Styles.iconContainer>
        {show
           ? <Icon name="far fa-angle-up" color={theme.textPrimary} size=14 />
           : <Icon name="far fa-angle-down" color={theme.textPrimary} size=14 />}
      </div>
    </div>
    <div className={Styles.sortDrowdownPanel(show)}>
      {sortList
       ->Belt.List.mapWithIndex((i, (value, name)) => {
           let isActive = sortedBy == value;
           <div
             key={i |> string_of_int}
             className={Css.merge([
               Styles.sortDropdownItem(isActive, theme),
               CssHelper.flexBox(~justify=`spaceBetween, ()),
             ])}
             onClick={_ => {
               setSortedBy(_ => value);
               setShow(_ => false);
             }}>
             <Text block=true value=name />
             {isActive
                ? <Icon name="far fa-check" color={theme.textPrimary} size=12 /> : React.null}
           </div>;
         })
       ->Array.of_list
       ->React.array}
    </div>
  </div>;
};
