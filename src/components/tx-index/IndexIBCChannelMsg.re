module ChannelOpenInit = {
  [@react.component]
  let make = (~channel: MsgDecoder.ChannelOpenInit.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col col=Col.Six mb=24>
          <Heading
            value="Signer"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <AddressRender address={channel.signer} />
        </Col>
        <Col col=Col.Six mb=24>
          <Heading
            value="Port ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.portID} />
        </Col>
        <Col col=Col.Six mbSm=24>
          <Heading
            value="State"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.channel.state} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Order"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.channel.ordering} />
        </Col>
      </Row>
      <SeperatedLine mt=24 mb=24 />
      <Row>
        <Col mb=24>
          <Heading value="Counterparty" size=Heading.H4 color={theme.textSecondary} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Port ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.channel.counterparty.portID} />
        </Col>
      </Row>
    </>;
  };
};

module ChannelOpenTry = {
  [@react.component]
  let make = (~channel: MsgDecoder.ChannelOpenTry.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col col=Col.Six mb=24>
          <Heading
            value="Signer"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <AddressRender address={channel.signer} />
        </Col>
        <Col col=Col.Six mb=24>
          <Heading
            value="Port ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.portID} />
        </Col>
        <Col col=Col.Six mbSm=24>
          <Heading
            value="State"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.channel.state} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Order"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.channel.ordering} />
        </Col>
      </Row>
      <IndexIBCUtils.ProofHeight proofHeight={channel.proofHeight} />
      <SeperatedLine mt=24 mb=24 />
      <Row>
        <Col mb=24>
          <Heading value="Counterparty" size=Heading.H4 color={theme.textSecondary} />
        </Col>
        <Col col=Col.Six mbSm=24>
          <Heading
            value="Port ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.channel.counterparty.portID} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Channel ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.channel.counterparty.channelID} />
        </Col>
      </Row>
    </>;
  };
};

module ChannelOpenAck = {
  [@react.component]
  let make = (~channel: MsgDecoder.ChannelOpenAck.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col col=Col.Six mb=24>
          <Heading
            value="Signer"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <AddressRender address={channel.signer} />
        </Col>
        <Col col=Col.Six mb=24>
          <Heading
            value="Port ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.portID} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Channel ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.channelID} />
        </Col>
      </Row>
      <IndexIBCUtils.ProofHeight proofHeight={channel.proofHeight} />
      <SeperatedLine mt=24 mb=24 />
      <Row>
        <Col mb=24>
          <Heading value="Counterparty" size=Heading.H4 color={theme.textSecondary} />
        </Col>
        <Col col=Col.Six>
          <Heading
            value="Channel ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.counterpartyChannelID} />
        </Col>
      </Row>
    </>;
  };
};

module ChannelOpenConfirm = {
  [@react.component]
  let make = (~channel: MsgDecoder.ChannelOpenConfirm.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col col=Col.Six mb=24>
          <Heading
            value="Signer"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <AddressRender address={channel.signer} />
        </Col>
        <Col col=Col.Six mb=24>
          <Heading
            value="Port ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.portID} />
        </Col>
        <Col>
          <Heading
            value="Channel ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.channelID} />
        </Col>
      </Row>
      <IndexIBCUtils.ProofHeight proofHeight={channel.proofHeight} />
    </>;
  };
};

module ChannelCloseInit = {
  [@react.component]
  let make = (~channel: MsgDecoder.ChannelCloseInit.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Signer"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color={theme.textSecondary}
        />
        <AddressRender address={channel.signer} />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Port ID"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.portID} />
      </Col>
      <Col mb=24>
        <Heading
          value="Channel ID"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.channelID} />
      </Col>
    </Row>;
  };
};

module ChannelCloseConfirm = {
  [@react.component]
  let make = (~channel: MsgDecoder.ChannelCloseConfirm.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <Row>
        <Col col=Col.Six mb=24>
          <Heading
            value="Signer"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <AddressRender address={channel.signer} />
        </Col>
        <Col col=Col.Six mb=24>
          <Heading
            value="Port ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.portID} />
        </Col>
        <Col>
          <Heading
            value="Channel ID"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color={theme.textSecondary}
          />
          <Text size=Text.Lg value={channel.channelID} />
        </Col>
      </Row>
      <IndexIBCUtils.ProofHeight proofHeight={channel.proofHeight} />
    </>;
  };
};
