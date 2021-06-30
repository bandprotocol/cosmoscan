module CreateClient = {
  [@react.component]
  let make = (~client: MsgDecoder.CreateClient.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six>
        <Heading
          value="Signer"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color={theme.textSecondary}
        />
        <AddressRender address={client.signer} />
      </Col>
    </Row>;
  };
};

module UpdateClient = {
  [@react.component]
  let make = (~client: MsgDecoder.UpdateClient.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Signer"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color={theme.textSecondary}
        />
        <AddressRender address={client.signer} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Client ID"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={client.clientID} />
      </Col>
    </Row>;
  };
};

module UpgradeClient = {
  [@react.component]
  let make = (~client: MsgDecoder.UpgradeClient.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Signer"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color={theme.textSecondary}
        />
        <AddressRender address={client.signer} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="ClientID"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={client.clientID} />
      </Col>
    </Row>;
  };
};

module SubmitClientMisbehaviour = {
  [@react.component]
  let make = (~client: MsgDecoder.SubmitClientMisbehaviour.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Signer"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color={theme.textSecondary}
        />
        <AddressRender address={client.signer} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="ClientID"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={client.clientID} />
      </Col>
    </Row>;
  };
};
