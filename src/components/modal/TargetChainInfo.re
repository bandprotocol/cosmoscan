module Styles = {
  open Css;

  let selectedChain = (theme: Theme.t) =>
    style([
      padding4(~top=`px(8), ~bottom=`px(8), ~left=`px(16), ~right=`px(27)),
      backgroundColor(theme.inputContrastColor),
      border(`px(1), `solid, theme.tableRowBorderColor),
      marginRight(`px(16)),
      borderRadius(`px(4)),
      width(`percent(100.)),
      height(`px(50)),
      display(`flex),
      alignItems(`center),
      flexWrap(`nowrap),
    ]);

  let logo = style([width(`px(20)), height(`px(20)), objectFit(`cover)]);
};

[@react.component]
let make = (~targetChain) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  let (imgSrc, name) =
    switch (targetChain) {
    | IBCConnectionQuery.BAND => (Images.bandLogo, "Band")
    | IBC({imgSrc, name}) => (imgSrc, name)
    };

  <div className={Styles.selectedChain(theme)}>
    <div
      className={Css.merge([
        CssHelper.flexBox(~justify=`spaceBetween, ~wrap=`nowrap, ()),
        Css.style([Css.width(`percent(100.))]),
      ])}>
      <div
        className={Css.merge([
          CssHelper.flexBox(~wrap=`nowrap, ()),
          Css.style([Css.width(`percent(100.))]),
        ])}>
        <img alt="target chain" src=imgSrc className=Styles.logo />
        <HSpacing size={`px(8)} />
        <div className={CssHelper.flexBox(~direction=`column, ~align=`flexStart, ())}>
          <Text
            value=name
            size=Text.Lg
            weight=Text.Semibold
            color={theme.textPrimary}
            transform=Text.Uppercase
          />
          {switch (targetChain) {
           | BAND => React.null
           | IBC({chainID}) => <Text value=chainID />
           }}
        </div>
      </div>
      {switch (targetChain) {
       | BAND => React.null
       | IBC({channel}) => <Text value=channel nowrap=true />
       }}
    </div>
  </div>;
};
