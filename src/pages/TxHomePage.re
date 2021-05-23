[@react.component]
let make = () => {
  let pageSize = 10;
  let txsSub = TxSub.getList(~pageSize, ~page=1, ());
  let latestTxsSub = TxSub.getList(~pageSize=1, ~page=1, ());
  let isMobile = Media.isMobile();

  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <Section ptSm=32 pbSm=32>
    <div className=CssHelper.container id="transactionsSection">
      <Row alignItems=Row.Center marginBottom=40 marginBottomSm=24>
        <Col col=Col.Twelve>
          <Heading value="All Transactions" size=Heading.H2 marginBottom=16 marginBottomSm=8 />
          {switch (latestTxsSub) {
           | Data(txs) =>
             <Heading
               value={
                 txs
                 ->Belt.Array.get(0)
                 ->Belt.Option.mapWithDefault(0, ({id}) => id - 2207294)
                 ->Format.iPretty
                 ++ " In total"
               }
               size=Heading.H3
               weight=Heading.Thin
               color={theme.textSecondary}
             />
           | _ => <LoadingCensorBar width=65 height=21 />
           }}
        </Col>
      </Row>
      <Table>
        {isMobile
           ? React.null
           : <THead>
               <Row alignItems=Row.Center>
                 <Col col=Col.Three>
                   <Text
                     block=true
                     value="TX Hash"
                     weight=Text.Semibold
                     transform=Text.Uppercase
                     size=Text.Sm
                   />
                 </Col>
                 <Col col=Col.One>
                   <Text
                     block=true
                     value="Block"
                     weight=Text.Semibold
                     transform=Text.Uppercase
                     size=Text.Sm
                   />
                 </Col>
                 <Col col=Col.One>
                   <Text
                     block=true
                     value="Status"
                     weight=Text.Semibold
                     transform=Text.Uppercase
                     size=Text.Sm
                     align=Text.Center
                   />
                 </Col>
                 <Col col=Col.Two>
                   <Text
                     block=true
                     value="Gas Fee (BAND)"
                     weight=Text.Semibold
                     transform=Text.Uppercase
                     size=Text.Sm
                   />
                 </Col>
                 <Col col=Col.Five>
                   <Text
                     block=true
                     value="Actions"
                     weight=Text.Semibold
                     transform=Text.Uppercase
                     size=Text.Sm
                   />
                 </Col>
               </Row>
             </THead>}
        <TxsTable txsSub />
      </Table>
    </div>
  </Section>;
};
