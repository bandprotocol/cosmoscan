module Styles = {
  open Css

  let input = (theme: Theme.t) =>
    style(list{
      width(#percent(100.)),
      height(#px(37)),
      paddingLeft(#px(9)),
      paddingRight(#px(9)),
      borderRadius(#px(4)),
      fontSize(#px(14)),
      fontWeight(#light),
      border(#px(1), #solid, theme.tableRowBorderColor),
      backgroundColor(theme.inputContrastColor),
      outlineStyle(#none),
      color(theme.textPrimary),
      fontFamilies(list{#custom("Montserrat"), #custom("sans-serif")}),
    })
  let searchContainer = style(list{
    height(#percent(100.)),
    width(#percent(100.)),
    selector("> div", list{width(#percent(100.))}),
  })

  let selectWrapper = style(list{
    width(#percent(100.)),
    maxWidth(#px(189)),
    Media.mobile(list{maxWidth(#percent(100.))}),
  })

  let filterSelector = style(list{
    selector("> div + div", list{marginLeft(#px(24))}),
    Media.mobile(list{selector("> div + div", list{marginLeft(#zero), marginTop(#px(24))})}),
  })

  let resetButton = style(list{
    paddingTop(#px(20)),
    cursor(#pointer),
    selector("> i", list{marginRight(#px(8))}),
    Media.mobile(list{paddingLeft(#px(24))}),
  })

  let noDataImage = style(list{width(#auto), height(#px(70)), marginBottom(#px(16))})
}

@react.component
let make = (~direction: IBCSub.packet_direction_t, ~chainID) => {
  let ({ThemeContext.theme: theme, isDarkMode}, _) = React.useContext(ThemeContext.context)

  let (packetType, setPacketType) = React.useState(_ => "")
  let (packetPort, setPacketPort) = React.useState(_ => "")
  let (packetChannel, setPacketChannel) = React.useState(_ => "")
  let (packetSequence, setPacketSequence) = React.useState(_ => "")
  let (rawPacketSequence, setRawPacketSequence) = React.useState(_ => "")

  let packetsSub = IBCSub.getList(
    ~pageSize=10,
    ~direction,
    ~packetType=packetType |> IBCSub.fromLabel,
    ~port=packetPort,
    ~channel=packetChannel,
    ~sequence=packetSequence |> int_of_string_opt,
    ~chainID,
    (),
  )

  let filters = IBCFilterSub.getFilterList(~chainID, ())

  let packetTypes = ["Oracle Request", "Oracle Response", "Fungible Token"]
  let packetPorts = ["oracle", "transfer"]

  let handlePacketPort = newVal => {
    setPacketPort(_ => newVal)
    setPacketChannel(_ => "")
    setPacketSequence(_ => "")
    setRawPacketSequence(_ => "")
  }

  let handlePacketChannel = newVal => {
    setPacketChannel(_ => newVal)
    setPacketSequence(_ => "")
    setRawPacketSequence(_ => "")
  }

  let handleReset = () => {
    setPacketPort(_ => "")
    setPacketChannel(_ => "")
    setPacketSequence(_ => "")
    setRawPacketSequence(_ => "")
  }

  React.useEffect1(() => {
    let timeoutId = Js.Global.setTimeout(() => setPacketSequence(_ => rawPacketSequence), 500)
    Some(() => Js.Global.clearTimeout(timeoutId))
  }, [rawPacketSequence])

  <>
    <Row marginTop=32>
      <Col col=Col.Nine colSm=Col.Twelve mbSm=24>
        <div className={Css.merge(list{CssHelper.flexBox(), Styles.filterSelector})}>
          {switch filters {
          | Data(filter) =>
            let newPacketChannel = filter->Js.Dict.get(packetPort)->Belt.Option.getWithDefault([])
            <>
              <Select
                options=packetPorts
                setSelectOption=handlePacketPort
                selectedOption=packetPort
                selectLabel="Port"
              />
              <Select
                options=newPacketChannel
                setSelectOption=handlePacketChannel
                selectedOption=packetChannel
                disabled={packetPort === ""}
                selectLabel="Channel"
              />
            </>
          | _ => <>
              <div>
                <LoadingCensorBar width=26 height=15 mb=8 radius=8 />
                <LoadingCensorBar width=189 height=37 radius=8 />
              </div>
              <div>
                <LoadingCensorBar width=26 height=15 mb=8 radius=8 />
                <LoadingCensorBar width=189 height=37 radius=8 />
              </div>
            </>
          }}
          <SequenceInput
            placeholder="000"
            onChange=setRawPacketSequence
            value=rawPacketSequence
            disabled={packetChannel === ""}
          />
          <div
            onClick={_ => handleReset()}
            className={Css.merge(list{Styles.resetButton, CssHelper.flexBox(~align=#center, ())})}>
            <Icon name="fas fa-times-circle" color=theme.textPrimary size=16 />
            <Text value="Reset" size=Text.Lg color=theme.textPrimary />
          </div>
        </div>
      </Col>
      <Col col=Col.Three colSm=Col.Twelve>
        <div className={CssHelper.flexBox(~justify=#flexEnd, ())}>
          <Select
            options=packetTypes
            setSelectOption=setPacketType
            selectedOption=packetType
            selectLabel="Packet Type"
          />
        </div>
      </Col>
    </Row>
    <Row marginTop=32>
      {switch packetsSub {
      | Data(packets) if packets->Belt.Array.length == 0 =>
        <EmptyContainer backgroundColor=theme.mainBg>
          <img
            src={isDarkMode ? Images.noOracleDark : Images.noOracleLight}
            className=Styles.noDataImage
          />
          <Heading
            size=Heading.H4
            value="No Packets"
            align=Heading.Center
            weight=Heading.Regular
            color=theme.textSecondary
          />
        </EmptyContainer>
      | Data(packets) =>
        packets
        ->Belt_Array.mapWithIndex((i, e) =>
          <Col col=Col.Six key={i |> string_of_int} mb=24>
            <PacketCard packetSub={Sub.resolve(e)} />
          </Col>
        )
        ->React.array
      | _ =>
        Belt_Array.make(10, ApolloHooks.Subscription.NoData)
        ->Belt_Array.mapWithIndex((i, noData) =>
          <Col col=Col.Six key={i |> string_of_int} mb=24> <PacketCard packetSub=noData /> </Col>
        )
        ->React.array
      }}
    </Row>
  </>
}
