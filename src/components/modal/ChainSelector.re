module Styles = {
  open Css;

  let container = style([position(`relative), paddingBottom(`px(24))]);
};

[@react.component]
let make = (~targetChain) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  let (_, dispatchModal) = React.useContext(ModalContext.context);

  let edit = () => {
    ModalContext.CloseModal->dispatchModal;
    ModalContext.ChainSelector(targetChain)->OpenModal->dispatchModal;
  };

  <div className=Styles.container>
    <Heading
      value="Chain"
      size=Heading.H5
      marginBottom=8
      align=Heading.Left
      color={theme.neutral_600}
      weight=Heading.Regular
    />
    <div className={CssHelper.flexBox(~wrap=`nowrap, ~justify=`spaceBetween, ())}>
      <TargetChainInfo targetChain />
      <Button py=10 variant=Button.Outline onClick={_ => edit()}>
        {"Change" |> React.string}
      </Button>
    </div>
  </div>;
};
