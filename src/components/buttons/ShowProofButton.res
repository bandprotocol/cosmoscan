@react.component
let make = (~showProof: bool, ~setShowProof) => {
  let isMobile = Media.isMobile()

  <Button
    variant=Button.Outline px=20 py=12 pxSm=12 pySm=10 onClick={_ => setShowProof(_ => !showProof)}>
    <div className={CssHelper.flexBox(~align=#center, ~justify=#center, ())}>
      <Icon name={showProof ? "fal fa-long-arrow-up" : "fal fa-long-arrow-down"} />
      <HSpacing size=Spacing.sm />
      {(showProof ? "Hide" : "Show") ++ (isMobile ? " Proof" : " Proof JSON") |> React.string}
    </div>
  </Button>
}
