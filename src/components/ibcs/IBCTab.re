module Styles = {
  open Css;

  let input = (theme: Theme.t) =>
    style([
      width(`percent(100.)),
      height(`px(37)),
      paddingLeft(`px(9)),
      paddingRight(`px(9)),
      borderRadius(`px(4)),
      fontSize(`px(14)),
      fontWeight(`light),
      border(`px(1), `solid, theme.tableRowBorderColor),
      backgroundColor(theme.inputContrastColor),
      outlineStyle(`none),
      color(theme.textPrimary),
      fontFamilies([`custom("Montserrat"), `custom("sans-serif")]),
    ]);
  let searchContainer =
    style([
      height(`percent(100.)),
      width(`percent(100.)),
      selector("> div", [width(`percent(100.))]),
    ]);

  let selectWrapper =
    style([
      width(`percent(100.)),
      maxWidth(`px(189)),
      Media.mobile([maxWidth(`percent(100.))]),
    ]);

  let filterSelector =
    style([
      // selector("> div", [flexBasis(`percent(25.))]),
      selector("> div + div", [marginLeft(`px(24))]),
      Media.mobile([selector("> div + div", [marginLeft(`zero), marginTop(`px(24))])]),
    ]);

  let resetButton =
    style([
      paddingTop(`px(20)),
      cursor(`pointer),
      selector("> i", [marginRight(`px(8))]),
      Media.mobile([paddingLeft(`px(24))]),
    ]);
};

module PacketSelect = {
  [@react.component]
  let make = (~packetOptions, ~setSelectPacket, ~selectLabel, ~selectedPacket, ~disabled=false) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <div className=Styles.selectWrapper>
      <div className={CssHelper.mb(~size=8, ())}> <Text value=selectLabel /> </div>
      <div className={CssHelper.selectWrapper(~fontColor=theme.textPrimary, ~mW=770, ())}>
        <select
          disabled
          className={Styles.input(theme)}
          value=selectedPacket
          onChange={event => {
            let newVal = ReactEvent.Form.target(event)##value;
            setSelectPacket(newVal);
          }}>
          <option value=""> {"Select " ++ selectLabel |> React.string} </option>
          {packetOptions
           |> Belt.Array.map(_, value =>
                <option key=value value> {value |> React.string} </option>
              )
           |> React.array}
        </select>
      </div>
    </div>;
  };
};

[@react.component]
let make = (~direction: IBCSub.packet_direction_t) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  let (packetType, setPacketType) = React.useState(_ => "");
  let (packetPort, setPacketPort) = React.useState(_ => "");
  let (packetChannel, setPacketChannel) = React.useState(_ => "");
  let (packetSequence, setPacketSequence) = React.useState(_ => "");
  let (rawPacketSequence, setRawPacketSequence) = React.useState(_ => "");

  let packetTypes = [|"Oracle Request", "Transfer", "Others"|];
  let packetPorts = [|"Oracle Request", "Transfer", "Others"|];
  let packetChannels = [|"Oracle Request", "Transfer", "Others"|];

  let handlePacketPort = newVal => {
    setPacketPort(_ => newVal);
    setPacketChannel(_ => "");
    setPacketSequence(_ => "");
    setRawPacketSequence(_ => "");
  };

  let handlePacketChannel = newVal => {
    setPacketChannel(_ => newVal);
    setPacketSequence(_ => "");
    setRawPacketSequence(_ => "");
  };

  let handleReset = () => {
    setPacketPort(_ => "");
    setPacketChannel(_ => "");
    setPacketSequence(_ => "");
    setRawPacketSequence(_ => "");
  };

  React.useEffect1(
    () => {
      let timeoutId = Js.Global.setTimeout(() => setPacketSequence(_ => rawPacketSequence), 500);
      Some(() => Js.Global.clearTimeout(timeoutId));
    },
    [|rawPacketSequence|],
  );

  <>
    <Row marginTop=32>
      <Col col=Col.Nine colSm=Col.Twelve mbSm=24>
        <div className={Css.merge([CssHelper.flexBox(), Styles.filterSelector])}>
          <PacketSelect
            packetOptions=packetPorts
            setSelectPacket=handlePacketPort
            selectedPacket=packetPort
            selectLabel="Port"
          />
          <PacketSelect
            packetOptions=packetChannels
            setSelectPacket=handlePacketChannel
            selectedPacket=packetChannel
            disabled={packetPort === ""}
            selectLabel="Channel"
          />
          <SequenceInput
            placeholder="000"
            onChange=setRawPacketSequence
            value=rawPacketSequence
            disabled={packetChannel === ""}
          />
          <div
            onClick={_ => handleReset()}
            className={Css.merge([Styles.resetButton, CssHelper.flexBox(~align=`center, ())])}>
            <Icon name="fas fa-times-circle" color={theme.textPrimary} size=16 />
            <Text value="Reset" size=Text.Lg color={theme.textPrimary} />
          </div>
        </div>
      </Col>
      <Col col=Col.Three colSm=Col.Twelve>
        <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
          <PacketSelect
            packetOptions=packetTypes
            setSelectPacket=setPacketType
            selectedPacket=packetType
            selectLabel="Packet Type"
          />
        </div>
      </Col>
    </Row>
    <Row marginTop=32> <PacketCard direction /> </Row>
  </>;
};
