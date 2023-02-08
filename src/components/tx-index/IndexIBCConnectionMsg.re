module ConnectionOpenInit = {
  [@react.component]
  let make = (~connection: MsgDecoder.ConnectionOpenInit.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col col=Col.Six mb=24>
          <Heading
            value="Signer"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <AddressRender address={connection.signer} />
        </Col>
        <Col col=Col.Six mb=24>
          <Heading
            value="Client ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <Text size=Text.Lg value={connection.clientID} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Delay Period"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <Text size=Text.Lg value={(connection.delayPeriod |> string_of_int) ++ "ns"} />
        </Col>
      </Row>
      <SeperatedLine mt=24 mb=24 />
      <Row>
        <Col mb=24>
          <Heading value="Counterparty" size=Heading.H4 color={theme.neutral_600} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Client ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <Text size=Text.Lg value={connection.counterparty.clientID} />
        </Col>
      </Row>
    </>;
  };
};

module ConnectionOpenTry = {
  [@react.component]
  let make = (~connection: MsgDecoder.ConnectionOpenTry.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col col=Col.Six mb=24>
          <Heading
            value="Signer"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <AddressRender address={connection.signer} />
        </Col>
        <Col col=Col.Six mb=24>
          <Heading
            value="Client ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <Text size=Text.Lg value={connection.clientID} />
        </Col>
        <Col col=Col.Six mbSm=24>
          <Heading
            value="Delay Period"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <Text size=Text.Lg value={(connection.delayPeriod |> string_of_int) ++ "ns"} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Previous Connection ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <Text size=Text.Lg value={connection.clientID} />
        </Col>
      </Row>
      <SeperatedLine mt=24 mb=24 />
      <Row>
        <Col mb=24>
          <Heading value="Counterparty" size=Heading.H4 color={theme.neutral_600} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Client ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <Text size=Text.Lg value={connection.counterparty.clientID} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Connection ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <Text size=Text.Lg value={connection.counterparty.connectionID} />
        </Col>
      </Row>
      <IndexIBCUtils.ProofHeight proofHeight={connection.proofHeight} />
      <IndexIBCUtils.ConsensusHeight proofHeight={connection.consensusHeight} />
    </>;
  };
};

module ConnectionOpenAck = {
  [@react.component]
  let make = (~connection: MsgDecoder.ConnectionOpenAck.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col col=Col.Six mbSm=24>
          <Heading
            value="Signer"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <AddressRender address={connection.signer} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Connection ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <Text size=Text.Lg value={connection.connectionID} />
        </Col>
      </Row>
      <SeperatedLine mt=24 mb=24 />
      <Row>
        <Col mb=24>
          <Heading value="Counterparty" size=Heading.H4 color={theme.neutral_600} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Connection ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <Text size=Text.Lg value={connection.counterpartyConnectionID} />
        </Col>
      </Row>
      <IndexIBCUtils.ProofHeight proofHeight={connection.proofHeight} />
      <IndexIBCUtils.ConsensusHeight proofHeight={connection.consensusHeight} />
    </>;
  };
};

module ConnectionOpenConfirm = {
  [@react.component]
  let make = (~connection: MsgDecoder.ConnectionOpenConfirm.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col col=Col.Six mbSm=24>
          <Heading
            value="Signer"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <AddressRender address={connection.signer} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Connection ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.neutral_600}
          />
          <Text size=Text.Lg value={connection.connectionID} />
        </Col>
      </Row>
      <IndexIBCUtils.ProofHeight proofHeight={connection.proofHeight} />
    </>;
  };
};
