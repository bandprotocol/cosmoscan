module Styles = {
  open Css;

  let copy = style([cursor(`pointer), position(`relative), zIndex(2)]);
};

[@react.component]
let make = (~width, ~message) => {
  let (copied, setCopy) = React.useState(_ => false);
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  copied
    ? <div className=Styles.copy>
        <Icon name="fal fa-check" color={theme.textPrimary} size=width />
      </div>
    : <div
        onClick={_ => {
          Copy.copy(message);
          setCopy(_ => true);
          let _ = Js.Global.setTimeout(() => setCopy(_ => false), 700);
          ();
        }}>
        <Icon name="far fa-clone" color={theme.textPrimary} size=width />
      </div>;
};
