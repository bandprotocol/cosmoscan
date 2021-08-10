module Styles = {
  open Css

  let iconWrapper = style(list{
    backgroundColor(Theme.baseBlue),
    width(#px(24)),
    height(#px(24)),
    borderRadius(#percent(50.)),
    position(#relative),
    selector(
      "> i",
      list{
        position(#absolute),
        left(#percent(50.)),
        top(#percent(50.)),
        transform(translate(#percent(-50.), #percent(-50.))),
      },
    ),
  })
}

@react.component
let make = (~category: MsgDecoder.msg_cat_t) =>
  <div className=Styles.iconWrapper>
    {switch category {
    | TokenMsg => <Icon name="far fa-wallet" color=Theme.white size=14 />
    | ValidatorMsg => <Icon name="fas fa-user" color=Theme.white size=14 />
    | ProposalMsg => <Icon name="fal fa-file" color=Theme.white size=14 />
    | DataMsg => <Icon name="fal fa-globe" color=Theme.white size=14 />
    | IBCClientMsg
    | IBCConnectionMsg
    | IBCChannelMsg
    | IBCPacketMsg
    | IBCTransferMsg =>
      <img src=Images.ibcIcon />
    | _ => <Icon name="fal fa-question" color=Theme.white size=14 />
    }}
  </div>
