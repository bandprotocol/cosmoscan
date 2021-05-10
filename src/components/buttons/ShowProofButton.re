[@react.component]
let make = (~showProof: bool, ~setShowProof) => {
  let isMobile = Media.isMobile();

  <Button px=20 py=12 pxSm=12 pySm=10 onClick={_ => setShowProof(_ => !showProof)}>
    <div className={CssHelper.flexBox()}>
      <Icon
        name={showProof ? "fal fa-long-arrow-up" : "fal fa-long-arrow-down"}
        color=Colors.white
      />
      <HSpacing size=Spacing.sm />
      <Text
        value={(showProof ? "Hide" : "Show") ++ (isMobile ? " Proof" : " Proof JSON")}
        weight=Text.Medium
        block=true
        nowrap=true
      />
    </div>
  </Button>;
};
