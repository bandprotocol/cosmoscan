module Packet = {
  [@react.component]
  let make = (~packet: MsgDecoder.Packet.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <SeperatedLine />
      <Row>
        <Col mb=18> <Heading value="Packet" size=Heading.H5 marginBottom=8 /> </Col>
        <Col col=Col.Six mb=24>
          <Heading
            value="Source Port"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={packet.sourcePort} />
        </Col>
        <Col col=Col.Six mb=24>
          <Heading
            value="Destination Port"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={packet.destinationPort} />
        </Col>
        <Col col=Col.Six mb=24>
          <Heading
            value="Source Channel"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={packet.sourceChannel} />
        </Col>
        <Col col=Col.Six mb=24>
          <Heading
            value="Destination Channel"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={packet.destinationChannel} />
        </Col>
        <Col col=Col.Six mbSm=24>
          <Heading value="Data" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
          <Text size=Text.Lg value={packet.data} breakAll=true />
        </Col>
        <Col col=Col.Six mbSm=24>
          <Heading
            value="Timeout Timestamp"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Timestamp time={packet.timeoutTimestamp} size=Text.Lg />
        </Col>
      </Row>
    </>;
  };
};

module RecvPacket = {
  [@react.component]
  let make = (~packet: MsgDecoder.RecvPacket.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col mb=24>
          <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
          <AddressRender address={packet.signer} />
        </Col>
        <Col col=Col.Six mbSm=24>
          <Heading
            value="Proof Height (Revision Height)"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={packet.proofHeight.revisionHeight |> string_of_int} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Proof Height (Revision Number)"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={packet.proofHeight.revisionNumber |> string_of_int} />
        </Col>
      </Row>
      <Packet packet={packet.packet} />
    </>;
  };
};

module AcknowledgePacket = {
  [@react.component]
  let make = (~packet: MsgDecoder.AcknowledgePacket.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col mb=24>
          <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
          <AddressRender address={packet.signer} />
        </Col>
        <Col col=Col.Six mbSm=24>
          <Heading
            value="Proof Height (Revision Height)"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={packet.proofHeight.revisionHeight |> string_of_int} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Proof Height (Revision Number)"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={packet.proofHeight.revisionNumber |> string_of_int} />
        </Col>
      </Row>
      <Packet packet={packet.packet} />
    </>;
  };
};

module Timeout = {
  [@react.component]
  let make = (~packet: MsgDecoder.Timeout.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col mb=24>
          <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
          <AddressRender address={packet.signer} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Proof Height (Revision Height)"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={packet.proofHeight.revisionHeight |> string_of_int} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Proof Height (Revision Number)"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={packet.proofHeight.revisionNumber |> string_of_int} />
        </Col>
      </Row>
      <Packet packet={packet.packet} />
    </>;
  };
};

module TimeoutOnClose = {
  [@react.component]
  let make = (~packet: MsgDecoder.TimeoutOnClose.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col mb=24>
          <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
          <AddressRender address={packet.signer} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Proof Height (Revision Height)"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={packet.proofHeight.revisionHeight |> string_of_int} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Proof Height (Revision Number)"
            size=Heading.H5
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={packet.proofHeight.revisionNumber |> string_of_int} />
        </Col>
      </Row>
      <Packet packet={packet.packet} />
    </>;
  };
};