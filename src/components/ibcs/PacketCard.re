module Styles = {
  open Css;

  let cardContainer =
    style([position(`relative), selector(" > div + div", [marginTop(`px(12))])]);

  let badgeContainer = style([selector(" > div", [margin(`zero)])]);
};

type t = {
  label: string,
  value: string,
};

module CardDetail = {
  [@react.component]
  let make = (~packetSub: ApolloHooks.Subscription.variant(BandScan.IBCSub.t)) => {
    <InfoContainer py=24>
      <div className=Styles.cardContainer>
        <div
          className={Css.merge([
            CssHelper.flexBox(~align=`center, ~justify=`spaceBetween, ()),
            CssHelper.mb(~size=19, ()),
          ])}>
          <div className=Styles.badgeContainer> <MsgBadge name="Oracle Request" /> </div>
          {switch (packetSub) {
           | Data({acknowledgement}) =>
             switch (acknowledgement) {
             | Some({success}) => <img src={success ? Images.success : Images.fail} />
             | None => React.null
             }
           | _ => React.null
           }}
        </div>
        {switch (packetSub) {
         | Data({blockHeight, srcChannel, dstChannel, srcPort, dstPort, sequence}) =>
           <PacketDetail.TopPart
             key={srcChannel ++ dstChannel ++ srcPort ++ dstPort ++ (sequence |> string_of_int)}
             blockHeight
             srcChannel
             dstChannel
             srcPort
             dstPort
             sequence
           />
         | _ => React.null
         }}
      </div>
    </InfoContainer>;
  };
};

[@react.component]
let make = (~direction: IBCSub.packet_direction_t) => {
  let packetsSub = IBCSub.getList();

  switch (packetsSub) {
  | Data(packets) =>
    packets
    ->Belt_Array.mapWithIndex((i, e) =>
        <Col col=Col.Six>
          <CardDetail packetSub={Sub.resolve(e)} key={i |> string_of_int} />
        </Col>
      )
    ->React.array
  | _ => React.null
  };
};
