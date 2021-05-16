module Styles = {
  open Css;

  let proofContainer =
    style([
      // padding4(~top=`zero, ~left=`px(24), ~right=`px(24), ~bottom=`px(24)),
      // Media.mobile([padding4(~top=`zero, ~left=`px(12), ~right=`px(12), ~bottom=`px(24))]),
      selector(
        "> button + button",
        [
          marginLeft(`px(24)),
          Media.mobile([marginLeft(`px(16))]),
          Media.smallMobile([marginLeft(`px(10))]),
        ],
      ),
    ]);

  let scriptContainer =
    style([
      fontSize(`px(12)),
      lineHeight(`px(20)),
      fontFamilies([
        `custom("IBM Plex Mono"),
        `custom("cousine"),
        `custom("sfmono-regular"),
        `custom("Consolas"),
        `custom("Menlo"),
        `custom("liberation mono"),
        `custom("ubuntu mono"),
        `custom("Courier"),
        `monospace,
      ]),
    ]);

  let padding = style([padding(`px(20))]);
};

[@react.component]
let make = (~request: RequestSub.t) => {
  let (proofOpt, reload) = ProofHook.get(request.id);
  let (showProof, setShowProof) = React.useState(_ => false);
  let isMobile = Media.isMobile();
  let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);

  React.useEffect1(
    () => {
      let intervalID =
        Js.Global.setInterval(
          () =>
            if (proofOpt == None) {
              reload((), ());
            },
          2000,
        );
      Some(() => Js.Global.clearInterval(intervalID));
    },
    [|proofOpt|],
  );

  switch (proofOpt) {
  | Some(proof) =>
    <>
      <div className={Css.merge([CssHelper.flexBox(), Styles.proofContainer])}>
        <ShowProofButton showProof setShowProof />
        <CopyButton
          data={proof.evmProofBytes |> JsBuffer.toHex(~with0x=false)}
          title={isMobile ? "EVM" : "Copy EVM proof"}
          py=12
          px=20
          pySm=10
          pxSm=12
        />
        {let nonEVMProofOpt = proof.jsonProof->NonEVMProof.createProofFromJson;
         switch (nonEVMProofOpt) {
         | Some(proof) =>
           <CopyButton
             data={proof |> JsBuffer.toHex(~with0x=false)}
             title={isMobile ? "non-EVM" : "Copy non-EVM proof"}
             py=10
             px=14
           />
         | _ => React.null
         }}
      </div>
      {showProof
         ? <div className=Styles.scriptContainer>
             <ReactHighlight className=Styles.padding>
               {proof.jsonProof |> Js.Json.stringifyWithSpace(_, 2) |> React.string}
             </ReactHighlight>
           </div>
         : React.null}
    </>
  | None =>
    <EmptyContainer height={`px(130)} backgroundColor={theme.secondaryTableBg}>
      <Loading marginBottom={`px(16)} />
      <Heading
        size=Heading.H4
        value="Waiting for proof"
        align=Heading.Center
        weight=Heading.Regular
      />
    </EmptyContainer>
  };
};
