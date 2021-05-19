module Styles = {
  open Css;

  let tableWrapper = style([Media.mobile([padding2(~v=`px(16), ~h=`zero)])]);
};

//TODO: Will Remove After Doing on Validator index

module LoadingWithHeader = {
  [@react.component]
  let make = () => {
    <div className=Styles.tableWrapper>
      <THead>
        <Row alignItems=Row.Center>
          <Col col=Col.Two>
            <Text
              block=true
              value="Block"
              weight=Text.Semibold
              transform=Text.Uppercase
              size=Text.Sm
            />
          </Col>
          <Col col=Col.Seven>
            <Text
              block=true
              value="Block Hash"
              weight=Text.Semibold
              transform=Text.Uppercase
              size=Text.Sm
            />
          </Col>
          <Col col=Col.One>
            <Text
              block=true
              value="Txn"
              weight=Text.Semibold
              transform=Text.Uppercase
              size=Text.Sm
            />
          </Col>
          <Col col=Col.Two>
            <Text
              block=true
              value="Timestamp"
              weight=Text.Semibold
              transform=Text.Uppercase
              size=Text.Sm
              align=Text.Right
            />
          </Col>
        </Row>
      </THead>
    </div>;
  };
};

module RenderBody = {
  [@react.component]
  let make = (~blockSub: ApolloHooks.Subscription.variant(BlockSub.t)) => {
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);

    <TBody>
      <Row alignItems=Row.Center>
        <Col col=Col.Two>
          {switch (blockSub) {
           | Data({height}) => <TypeID.Block id=height />
           | _ => <LoadingCensorBar width=135 height=15 />
           }}
        </Col>
        <Col col=Col.Seven>
          {switch (blockSub) {
           | Data({hash}) =>
             <Text
               value={hash |> Hash.toHex(~upper=true)}
               block=true
               code=true
               ellipsis=true
               color={theme.textPrimary}
             />
           | _ => <LoadingCensorBar width=522 height=15 />
           }}
        </Col>
        <Col col=Col.One>
          <div className={CssHelper.flexBox(~justify=`center, ())}>
            {switch (blockSub) {
             | Data({txn}) =>
               <Text value={txn |> Format.iPretty} align=Text.Center color={theme.textPrimary} />
             | _ => <LoadingCensorBar width=20 height=15 />
             }}
          </div>
        </Col>
        <Col col=Col.Two>
          <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
            {switch (blockSub) {
             | Data({timestamp}) =>
               <Timestamp time=timestamp size=Text.Md weight=Text.Regular textAlign=Text.Right />
             | _ => <LoadingCensorBar width=80 height=15 />
             }}
          </div>
        </Col>
      </Row>
    </TBody>;
  };
};

module RenderBodyMobile = {
  [@react.component]
  let make = (~reserveIndex, ~blockSub: ApolloHooks.Subscription.variant(BlockSub.t)) => {
    let isSmallMobile = Media.isSmallMobile();

    switch (blockSub) {
    | Data({height, timestamp, txn, hash}) =>
      <MobileCard
        values=InfoMobileCard.[
          ("Block", Height(height)),
          ("Block Hash", TxHash(hash, isSmallMobile ? 170 : 200)),
          ("Txn", Count(txn)),
          ("Timestamp", Timestamp(timestamp)),
        ]
        key={height |> ID.Block.toString}
        idx={height |> ID.Block.toString}
      />
    | _ =>
      <MobileCard
        values=InfoMobileCard.[
          ("Block", Loading(isSmallMobile ? 170 : 200)),
          ("Block Hash", Loading(166)),
          ("Txn", Loading(20)),
          ("Timestamp", Loading(166)),
        ]
        key={reserveIndex |> string_of_int}
        idx={reserveIndex |> string_of_int}
      />
    };
  };
};

[@react.component]
let make = (~consensusAddress) => {
  let pageSize = 10;
  let isMobile = Media.isMobile();

  let blocksSub =
    BlockSub.getListByConsensusAddress(~address=consensusAddress, ~pageSize, ~page=1, ());

  <div className=Styles.tableWrapper>
    {isMobile
       ? React.null
       : <THead>
           <Row alignItems=Row.Center>
             <Col col=Col.Two>
               <Text
                 block=true
                 value="Block"
                 weight=Text.Semibold
                 transform=Text.Uppercase
                 size=Text.Sm
               />
             </Col>
             <Col col=Col.Seven>
               <Text
                 block=true
                 value="Block Hash"
                 weight=Text.Semibold
                 transform=Text.Uppercase
                 size=Text.Sm
               />
             </Col>
             <Col col=Col.One>
               <Text
                 block=true
                 value="Txn"
                 weight=Text.Semibold
                 transform=Text.Uppercase
                 size=Text.Sm
                 align=Text.Center
               />
             </Col>
             <Col col=Col.Two>
               <Text
                 block=true
                 value="Timestamp"
                 weight=Text.Semibold
                 transform=Text.Uppercase
                 size=Text.Sm
                 align=Text.Right
               />
             </Col>
           </Row>
         </THead>}
    {switch (blocksSub) {
     | Data(blocks) =>
       <>
         {blocks
          ->Belt_Array.mapWithIndex((i, e) =>
              isMobile
                ? <RenderBodyMobile
                    key={e.height |> ID.Block.toString}
                    reserveIndex=i
                    blockSub={Sub.resolve(e)}
                  />
                : <RenderBody key={e.height |> ID.Block.toString} blockSub={Sub.resolve(e)} />
            )
          ->React.array}
       </>
     | _ =>
       Belt_Array.make(pageSize, ApolloHooks.Subscription.NoData)
       ->Belt_Array.mapWithIndex((i, noData) =>
           isMobile
             ? <RenderBodyMobile key={string_of_int(i)} reserveIndex=i blockSub=noData />
             : <RenderBody key={string_of_int(i)} blockSub=noData />
         )
       ->React.array
     }}
  </div>;
};
