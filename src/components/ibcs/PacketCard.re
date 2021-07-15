module Styles = {
  open Css;

  let cardContainer =
    style([position(`relative), selector(" > div + div", [marginTop(`px(12))])]);

  let infoContainer = style([height(`percent(100.))]);

  let badgeContainer = style([selector(" > div", [margin(`zero)])]);

  let failText = style([cursor(`pointer), selector("> span", [marginRight(`px(8))])]);
};

type t = {
  label: string,
  value: string,
};

[@react.component]
let make = (~packetSub: ApolloHooks.Subscription.variant(BandScan.IBCSub.Internal.t)) => {
  let (_, dispatchModal) = React.useContext(ModalContext.context);

  let errorMsg = reason => reason->IBCPacketError->OpenModal->dispatchModal;

  <InfoContainer py=24 style=Styles.infoContainer>
    <div className=Styles.cardContainer>
      <div
        className={Css.merge([
          CssHelper.flexBox(~align=`center, ~justify=`spaceBetween, ()),
          CssHelper.mb(~size=19, ()),
        ])}>
        <div className=Styles.badgeContainer>
          {switch (packetSub) {
           | Data({packetType}) => <MsgBadge name={packetType |> IBCSub.getPacketTypeText} />
           | _ => <LoadingCensorBar width=110 height=20 radius=50 />
           }}
        </div>
        {switch (packetSub) {
         | Data({acknowledgement}) =>
           switch (acknowledgement) {
           | Some({status, reason}) =>
             switch (status) {
             | Success => <img src=Images.success />
             | Pending => <img src=Images.pending />
             | Fail =>
               <div
                 className={Css.merge([CssHelper.flexBox(), Styles.failText])}
                 onClick={_ => errorMsg(reason |> Belt.Option.getExn)}>
                 <Text value="View Error Message" color=Theme.failColor />
                 <img src=Images.fail />
               </div>
             }
           | _ => React.null
           }
         | _ => React.null
         }}
      </div>
      {switch (packetSub) {
       | Data({
           srcChannel,
           dstChannel,
           srcPort,
           dstPort,
           sequence,
           blockHeight,
           counterPartyChainID,
           packetType,
           txHash,
           acknowledgement,
           data,
         }) =>
         <>
           {switch (packetType) {
            | OracleRequest
            | FungibleToken => <PacketDetail.TxHash txHash={txHash |> Belt.Option.getExn} />
            | OracleResponse => <PacketDetail.BlockID blockHeight />
            | _ => React.null
            }}
           <PacketDetail.Common
             key={srcChannel ++ dstChannel ++ srcPort ++ dstPort ++ (sequence |> string_of_int)}
             srcChannel
             dstChannel
             srcPort
             dstPort
             sequence
             counterPartyChainID
           />
           {switch (packetType) {
            | OracleRequest =>
              switch (acknowledgement) {
              | Some({data}) =>
                switch (data) {
                | Request(request) =>
                  switch (request.requestID) {
                  | Some(requestID) =>
                    <PacketDetail.RequestID requestID={requestID |> ID.Request.fromInt} />
                  | _ => React.null
                  }
                | _ => React.null
                }
              | _ => React.null
              }
            | OracleResponse =>
              switch (data) {
              | Response(response) =>
                <>
                  <PacketDetail.RequestID requestID={response.requestID |> ID.Request.fromInt} />
                  <PacketDetail.ResolveStatus status={response.resolveStatus} />
                </>
              | _ => React.null
              }
            | FungibleToken
            | _ => React.null
            }}
         </>
       | _ => <PacketDetail.Loading />
       }}
    </div>
  </InfoContainer>;
};
