module Styles = {
  open Css;

  let container = (w, theme: Theme.t) =>
    style([
      display(`flex),
      cursor(`pointer),
      width(w),
      alignItems(`center),
      selector("> span:hover", [color(theme.primary_600)]),
      selector("> span", [transition(~duration=200, "all")]),
    ]);
};

[@react.component]
let make =
    (
      ~validatorAddress: Address.t,
      ~moniker: string,
      ~identity=?,
      ~weight=Text.Semibold,
      ~size=Text.Md,
      ~underline=false,
      ~width=`auto,
      ~avatarWidth=20,
    ) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <Link
    className={Styles.container(width, theme)}
    route={Route.ValidatorIndexPage(validatorAddress, Reports)}>
    {switch (identity) {
     | Some(identity') =>
       <> <Avatar moniker identity=identity' width=avatarWidth /> <HSpacing size=Spacing.sm /> </>
     | None => React.null
     }}
    <Text
      value=moniker
      color={theme.neutral_900}
      weight
      block=true
      size
      nowrap=true
      ellipsis=true
      underline
    />
  </Link>;
};
