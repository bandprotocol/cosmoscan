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
let make = (~packetSub: ApolloHooks.Subscription.variant(BandScan.IBCQuery.t)) => {
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
           | Data({packetType}) => <MsgBadge name={packetType |> IBCQuery.getPacketTypeText} />
           | _ => <LoadingCensorBar width=110 height=20 radius=50 />
           }}
        </div>
        {switch (packetSub) {
         | Data({acknowledgement}) =>
           switch (acknowledgement) {
           | Some({status, reason}) =>
             switch (status) {
             | Success => <img alt="Success Icon" src=Images.success />
             | Pending => <img alt="Pending Icon" src=Images.pending />
             | Fail =>
               <div
                 className={Css.merge([CssHelper.flexBox(), Styles.failText])}
                 onClick={_ => errorMsg(reason |> Belt.Option.getExn)}>
                 <Text value="View Error Message" color=Theme.failColor />
                 <img alt="Fail Icon" src=Images.fail />
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
           acknowledgement: acknowledgementOpt,
           data,
         }) =>
         <>
           {switch (packetType) {
            | OracleRequest
            | InterchainAccount
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
              {
                let%Opt acknowledgement = acknowledgementOpt;

                switch (acknowledgement.data) {
                | Request({requestID: requestIDOpt}) =>
                  let%Opt requestID = requestIDOpt;
                  Some(<PacketDetail.RequestID requestID={requestID |> ID.Request.fromInt} />);
                | _ => None
                };
              }
              ->Belt.Option.getWithDefault(React.null)
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
