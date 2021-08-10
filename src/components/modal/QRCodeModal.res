module Styles = {
  open Css
  let container = style(list{
    width(#percent(100.)),
    maxWidth(#px(468)),
    minHeight(#px(360)),
    padding(#px(40)),
    Media.mobile(list{maxWidth(#px(300))}),
  })
  let qrCodeContainer = style(list{
    backgroundColor(Theme.white),
    maxWidth(#px(220)),
    margin3(~top=#px(40), ~h=#auto, ~bottom=#zero),
    padding(#px(12)),
    borderRadius(#px(8)),
  })
}

@react.component
let make = (~address) =>
  <div className=Styles.container>
    <Heading size=Heading.H4 value="QR Code" marginBottom=24 />
    <AddressRender address position=AddressRender.Subtitle clickable=false />
    <div
      className={Css.merge(list{CssHelper.flexBox(~justify=#center, ()), Styles.qrCodeContainer})}>
      <QRCode value={address |> Address.toBech32} size=200 />
    </div>
  </div>
