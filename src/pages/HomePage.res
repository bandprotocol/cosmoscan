module Styles = {
  open Css

  let root = style(list{position(#relative)})
  let content = style(list{position(#relative), zIndex(1)})
  let baseBg = style(list{position(#absolute), top(#px(40))})
  let left = style(list{left(#zero)})
  let right = style(list{right(#zero), transform(rotateZ(#deg(180.)))})
}

@react.component
let make = () => {
  // Subscribe for latest 5 blocks here so both "LatestBlocks" and "ChainInfoHighLights"
  // share the same infomation.
  let pageSize = 5
  let latest5BlocksSub = BlockSub.getList(~pageSize, ~page=1, ())
  let latestBlockSub = latest5BlocksSub->Sub.map(blocks => blocks->Belt_Array.getExn(0))
  let latest5RequestSub = RequestSub.getList(~pageSize, ~page=1, ())
  let ({ThemeContext.theme: theme, isDarkMode}, _) = React.useContext(ThemeContext.context)
  let isMobile = Media.isMobile()

  <Section pt=80 pb=80 ptSm=0 pbSm=24 bg=theme.mainBg style=Styles.root>
    {!isMobile
      ? <>
          <img
            src={isDarkMode ? Images.bgLeftDark : Images.bgLeftLight}
            className={Css.merge(list{Styles.baseBg, Styles.left})}
          />
          <img
            src={isDarkMode ? Images.bgLeftDark : Images.bgLeftLight}
            className={Css.merge(list{Styles.baseBg, Styles.right})}
          />
        </>
      : React.null}
    <div className={Css.merge(list{CssHelper.container, Styles.content})}>
      <ChainInfoHighlights latestBlockSub />
      <Row marginTop=24>
        <Col col=Col.Six mbSm=24> <TotalRequestsGraph latest5RequestSub /> </Col>
        <Col col=Col.Six> <LatestRequests latest5RequestSub /> </Col>
      </Row>
      <Row marginTop=24>
        <Col col=Col.Four> <LatestBlocks blocksSub=latest5BlocksSub /> </Col>
        <Col col=Col.Eight> <LatestTxTable /> </Col>
      </Row>
    </div>
  </Section>
}
