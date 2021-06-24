module ConnectionOpenInit = {
  [@react.component]
  let make = (~connection: MsgDecoder.ConnectionOpenInit.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mb=24>
        <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <AddressRender address={connection.signer} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading value="Client ID" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={connection.clientID} />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading value="Delay Period" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={connection.delayPeriod |> string_of_int} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Counterparty (Client ID)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.counterpartyClientID} />
      </Col>
    </Row>;
  };
};

module ConnectionOpenTry = {
  [@react.component]
  let make = (~connection: MsgDecoder.ConnectionOpenTry.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mb=24>
        <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <AddressRender address={connection.signer} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading value="Client ID" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={connection.clientID} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading value="Delay Period" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={connection.delayPeriod |> string_of_int} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Previous Connection ID"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.clientID} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Counterparty (Client ID )"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.counterparty.clientID} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Counterparty (Connection ID)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.counterparty.connectionID} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Proof Height (Revision Height)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.proofHeight.revisionHeight |> string_of_int} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Proof Height (Revision Number)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.proofHeight.revisionNumber |> string_of_int} />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Consensus Height (Revision Height)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.consensusHeight.revisionHeight |> string_of_int} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Consensus Height (Revision Number)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.consensusHeight.revisionNumber |> string_of_int} />
      </Col>
    </Row>;
  };
};

module ConnectionOpenAck = {
  [@react.component]
  let make = (~connection: MsgDecoder.ConnectionOpenAck.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mb=24>
        <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <AddressRender address={connection.signer} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Connection ID"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.connectionID} />
      </Col>
      <Col mb=24>
        <Heading
          value="Counterparty (Connection ID)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.counterpartyConnectionID} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Proof Height (Revision Height)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.proofHeight.revisionHeight |> string_of_int} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Proof Height (Revision Number)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.proofHeight.revisionNumber |> string_of_int} />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Consensus Height (Revision Height)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.consensusHeight.revisionHeight |> string_of_int} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Consensus Height (Revision Number)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.consensusHeight.revisionNumber |> string_of_int} />
      </Col>
    </Row>;
  };
};

module ConnectionOpenConfirm = {
  [@react.component]
  let make = (~connection: MsgDecoder.ConnectionOpenConfirm.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six>
        <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <AddressRender address={connection.signer} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Connection ID"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.connectionID} />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Proof Height (Revision Height)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.proofHeight.revisionHeight |> string_of_int} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Proof Height (Revision Number)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={connection.proofHeight.revisionNumber |> string_of_int} />
      </Col>
    </Row>;
  };
};
