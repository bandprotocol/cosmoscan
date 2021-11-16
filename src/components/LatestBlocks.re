module Styles = {
  open Css;

  let allBlockLink = (theme: Theme.t) =>
    style([
      backgroundColor(theme.baseBlue),
      borderRadius(`px(8)),
      width(`px(32)),
      height(`px(32)),
      hover([backgroundColor(theme.darkBlue)]),
    ]);

  let card = (theme: Theme.t) =>
    style([
      backgroundColor(theme.secondaryBg),
      padding2(~v=`px(20), ~h=`px(24)),
      borderRadius(`px(8)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.2)))),
      Media.mobile([padding2(~v=`px(20), ~h=`px(16))]),
    ]);
};

[@react.component]
let make = (~blocksSub) => {
  let isMobile = Media.isMobile();
  let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
  <>
    <Row marginBottom=24 marginBottomSm=16>
      <Col col=Col.Six colSm=Col.Six>
        <Heading value="Latest Blocks" size=Heading.H4 />
        <VSpacing size={`px(4)} />
        {switch (blocksSub) {
         | ApolloHooks.Subscription.Data(blocks) =>
           <Text
             value={
               blocks
               ->Belt.Array.get(0)
               ->Belt.Option.mapWithDefault(
                   0,
                   ({BlockSub.height}) => {
                     let ID.Block.ID(height_) = height;
                     height_;
                   },
                 )
               ->Format.iPretty
             }
             size=Text.Lg
             weight=Text.Medium
           />
         | _ => <LoadingCensorBar width=90 height=18 />
         }}
      </Col>
      <Col col=Col.Six colSm=Col.Six>
        <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
          {isMobile ? React.null : <Heading value="More Blocks" size=Heading.H4 />}
          <HSpacing size=Spacing.md />
          <Link className={CssHelper.flexBox(~align=`center, ())} route=Route.BlockHomePage>
            <div
              className={Css.merge([
                Styles.allBlockLink(theme),
                CssHelper.flexBox(~justify=`center, ()),
              ])}>
              <Icon name="far fa-arrow-right" color={theme.white} />
            </div>
          </Link>
        </div>
      </Col>
    </Row>
    <Row marginBottomSm=16>
      {switch (blocksSub) {
       | Data(blocks) =>
         blocks
         ->Belt.Array.map(({height, validator}) =>
             <Col mb=8 key={height |> ID.Block.toString}>
               <div className={Styles.card(theme)}>
                 <Row>
                   <Col col=Col.Six colSm=Col.Six>
                     <TypeID.Block id=height position=TypeID.Text />
                   </Col>
                   <Col col=Col.Six colSm=Col.Six>
                     <ValidatorMonikerLink
                       size=Text.Md
                       validatorAddress={validator.operatorAddress}
                       width={`percent(100.)}
                       moniker={validator.moniker}
                       identity={validator.identity}
                       avatarWidth=20
                     />
                   </Col>
                 </Row>
               </div>
             </Col>
           )
         ->React.array;
       | _ =>
         Belt_Array.make(5, ())
         ->Belt.Array.mapWithIndex((i, _) =>
             <Col mb=8 key={string_of_int(i)}>
               <div className={Styles.card(theme)}>
                 <Row>
                   <Col col=Col.Six colSm=Col.Six> <LoadingCensorBar width=60 height=15 /> </Col>
                   <Col col=Col.Six colSm=Col.Six> <LoadingCensorBar width=100 height=15 /> </Col>
                 </Row>
               </div>
             </Col>
           )
         ->React.array
       }}
    </Row>
  </>;
};
