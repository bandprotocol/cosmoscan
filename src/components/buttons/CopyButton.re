module Styles = {
  open Css;

  let logo = style([width(`px(12))]);
};

[@react.component]
let make = (~data, ~title, ~width=105, ~py=5, ~px=10, ~pySm=py, ~pxSm=px) => {
  let (copied, setCopy) = React.useState(_ => false);
  <Button
    variant=Button.Outline
    onClick={_ => {
      Copy.copy(data);
      setCopy(_ => true);
      let _ = Js.Global.setTimeout(() => setCopy(_ => false), 700);
      ();
    }}
    py
    px
    pySm
    pxSm>
    <div className={CssHelper.flexBox(~align=`center, ~justify=`center, ())}>
      {copied ? <Icon name="fal fa-check" size=12 /> : <Icon name="far fa-clone" size=12 />}
      <HSpacing size=Spacing.sm />
      {title |> React.string}
    </div>
  </Button>;
};
