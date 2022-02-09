module Styles = {
  open Css;

  let container =
    style([
      display(`flex),
      flexDirection(`row),
      width(`percent(100.)),
      justifyContent(`center),
      minHeight(`px(30)),
      borderRadius(`px(8)),
      padding2(~v=`px(24), ~h=`zero),
      Media.mobile([padding2(~v=`px(12), ~h=`zero)]),
    ]);

  let innerContainer =
    style([
      display(`flex),
      alignItems(`center),
      Media.mobile([
        width(`percent(100.)),
        justifyContent(`spaceBetween),
        padding2(~v=`zero, ~h=`px(5)),
      ]),
    ]);

  let clickable = (active, theme: Theme.t, isDarkMode) =>
    style([
      cursor(`pointer),
      width(`px(32)),
      height(`px(32)),
      borderRadius(`px(8)),
      border(`px(1), `solid, active ? theme.textPrimary : theme.textSecondary),
      pointerEvents(active ? `auto : `none),
      opacity(active ? 1. : 0.5),
      hover([
        backgroundColor(theme.textPrimary),
        selector("> i", [color(isDarkMode ? theme.black : theme.white)]),
      ]),
    ]);

  let paginationBox =
    style([margin2(~v=zero, ~h=`px(32)), selector("> * + *", [marginLeft(`px(20))])]);
};
module ClickableSymbol = {
  [@react.component]
  let make = (~isPrevious, ~active, ~onClick) => {
    let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

    <div
      className={Css.merge([
        Styles.clickable(active, theme, isDarkMode),
        CssHelper.flexBox(~justify=`center, ()),
      ])}
      onClick>
      {isPrevious
         ? <Icon name="far fa-angle-left" color={theme.textPrimary} size=18 />
         : <Icon name="far fa-angle-right" color={theme.textPrimary} size=18 />}
    </div>;
  };
};

[@react.component]
let make = (~currentPage, ~pageCount, ~onPageChange: int => unit) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  if (pageCount > 1) {
    <div className=Styles.container>
      <div className=Styles.innerContainer>
        <ClickableSymbol
          isPrevious=true
          active={currentPage !== 1}
          onClick={_ => onPageChange(currentPage < 1 ? 1 : currentPage - 1)}
        />
        <div
          className={Css.merge([CssHelper.flexBox(~justify=`center, ()), Styles.paginationBox])}>
          <Text
            value={currentPage |> Format.iPretty}
            size=Text.Lg
            weight=Text.Semibold
            color={theme.textPrimary}
          />
          <Text value="of" size=Text.Lg />
          <Text
            value={pageCount |> Format.iPretty}
            weight=Text.Semibold
            size=Text.Lg
            color={theme.textPrimary}
          />
        </div>
        <ClickableSymbol
          isPrevious=false
          active={currentPage !== pageCount}
          onClick={_ => onPageChange(currentPage > pageCount ? pageCount : currentPage + 1)}
        />
      </div>
    </div>;
  } else {
    React.null;
  };
};
