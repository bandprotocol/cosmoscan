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

  let selectWrapper = style([width(`percent(100.)), maxWidth(`px(189))]);
};

module PacketSelect = {
  [@react.component]
  let make = (~packetOptions, ~setSelectPacket) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <div className=Styles.selectWrapper>
      <div className={CssHelper.mb(~size=8, ())}> <Text value="Packet Type" /> </div>
      <div className={CssHelper.selectWrapper(~fontColor=theme.textPrimary, ())}>
        <select
          className={Styles.input(theme)}
          onChange={event => {
            let newVal = ReactEvent.Form.target(event)##value;
            setSelectPacket(_ => newVal);
          }}>
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
  let (searchTerm, setSearchTerm) = React.useState(_ => "");
  let (selectPacket, setSelectPacket) = React.useState(_ => "");

  let packetOptions = [|"Oracle Request", "Transfer", "Others"|];
  <>
    <Row marginTop=32>
      <Col>
        <div className={CssHelper.flexBox(~justify=`flexEnd, ())}>
          <PacketSelect packetOptions setSelectPacket />
        </div>
      </Col>
    </Row>
    <Row marginTop=32> <PacketCard direction /> </Row>
  </>;
};
