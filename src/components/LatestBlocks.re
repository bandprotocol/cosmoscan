module Styles = {
  open Css;

  let allBlockLink = (theme: Theme.t) =>
    style([
      backgroundColor(theme.primary_600),
      borderRadius(`px(8)),
      width(`px(32)),
      height(`px(32)),
      hover([backgroundColor(theme.darkBlue)]),
    ]);

  let card = () =>
    style([
      padding2(~v=`px(20), ~h=`px(24)),
      Media.mobile([padding2(~v=`px(20), ~h=`px(16))]),
    ]);
};

[@react.component]
let make = (~blocksSub: ApolloHooks.Subscription.variant(array(BlockSub.t))) => {
  let isMobile = Media.isMobile();
  let (ThemeContext.{theme,isDarkMode}, _) = React.useContext(ThemeContext.context);
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
               <div className={Css.merge([Styles.card(), CommonStyles.card(theme,isDarkMode)])}>
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
         ->React.array
       | _ =>
         Belt_Array.make(5, ())
         ->Belt.Array.mapWithIndex((i, _) =>
             <Col mb=8 key={string_of_int(i)}>
               <div className={Css.merge([Styles.card(), CommonStyles.card(theme,isDarkMode)])}>
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
