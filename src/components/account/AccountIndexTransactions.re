module Styles = {
  open Css;

  let tableWrapper = style([Media.mobile([padding2(~v=`px(16), ~h=`zero)])]);
};

let transform = (account, msg: TxSub.Msg.t) => {
  switch (msg) {
  | SendMsgSuccess({toAddress, fromAddress, amount})
  | SendMsgFail({toAddress, fromAddress, amount}) when toAddress == account =>
    TxSub.Msg.ReceiveMsg({toAddress, fromAddress, amount})
  | _ => msg
  };
};

[@react.component]
let make = (~accountAddress: Address.t) => {
  let (page, setPage) = React.useState(_ => 1);
  let pageSize = 10;

  let txsSub = TxSub.getListBySender(accountAddress, ~pageSize, ~page, ());
  let txsCountSub = TxSub.countBySender(accountAddress);

  let isMobile = Media.isMobile();

  <div className=Styles.tableWrapper>
    {isMobile
       ? <Row marginBottom=16>
           <Col>
             {switch (txsCountSub) {
              | Data(txsCount) =>
                <div className={CssHelper.flexBox()}>
                  <Text
                    block=true
                    value={txsCount |> string_of_int}
                    weight=Text.Semibold
                    color=Colors.gray7
                  />
                  <HSpacing size=Spacing.xs />
                  <Text block=true value="Transactions" weight=Text.Semibold color=Colors.gray7 />
                </div>
              | _ => <LoadingCensorBar width=100 height=15 />
              }}
           </Col>
         </Row>
       : <THead>
           <Row alignItems=Row.Center>
             <Col col=Col.Two>
               {switch (txsCountSub) {
                | Data(txsCount) =>
                  <div className={CssHelper.flexBox()}>
                    <Text
                      block=true
                      value={txsCount |> string_of_int}
                      weight=Text.Semibold
                      color=Colors.gray7
                    />
                    <HSpacing size=Spacing.xs />
                    <Text
                      block=true
                      value="Transactions"
                      weight=Text.Semibold
                      color=Colors.gray7
                    />
                  </div>
                | _ => <LoadingCensorBar width=100 height=15 />
                }}
             </Col>
             <Col col=Col.One>
               <Text block=true value="Block" weight=Text.Semibold color=Colors.gray7 />
             </Col>
             <Col col=Col.One>
               <Text
                 block=true
                 value="Status"
                 size=Text.Md
                 weight=Text.Semibold
                 color=Colors.gray7
                 align=Text.Center
               />
             </Col>
             <Col col=Col.Two>
               <Text
                 block=true
                 value="Gas Fee (BAND)"
                 weight=Text.Semibold
                 color=Colors.gray7
                 align=Text.Center
               />
             </Col>
             <Col col=Col.Six>
               <Text block=true value="Actions" weight=Text.Semibold color=Colors.gray7 />
             </Col>
           </Row>
         </THead>}
    <TxsTable txsSub msgTransform={transform(accountAddress)} />
    {switch (txsCountSub) {
     | Data(txsCount) =>
       let pageCount = Page.getPageCount(txsCount, pageSize);
       <Pagination currentPage=page pageCount onPageChange={newPage => setPage(_ => newPage)} />;
     | _ => React.null
     }}
  </div>;
};
