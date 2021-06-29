module ChannelOpenInit = {
  [@react.component]
  let make = (~channel: MsgDecoder.ChannelOpenInit.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mb=24>
        <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <AddressRender address={channel.signer} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading value="Port ID" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.portID} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading value="State" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.channel.state |> string_of_int} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading value="Order" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.channel.ordering |> string_of_int} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Counterparty (Port ID)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.channel.counterparty.portID} />
      </Col>
    </Row>;
  };
};

module ChannelOpenTry = {
  [@react.component]
  let make = (~channel: MsgDecoder.ChannelOpenTry.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mb=24>
        <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <AddressRender address={channel.signer} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading value="Port ID" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.portID} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading value="State" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.channel.state |> string_of_int} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading value="Order" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.channel.ordering |> string_of_int} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Counterparty (channel ID)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.channel.counterparty.channelID} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Proof Height (Revision Height)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.proofHeight.revisionHeight |> string_of_int} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Proof Height (Revision Number)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.proofHeight.revisionNumber |> string_of_int} />
      </Col>
    </Row>;
  };
};

module ChannelOpenAck = {
  [@react.component]
  let make = (~channel: MsgDecoder.ChannelOpenAck.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mb=24>
        <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <AddressRender address={channel.signer} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading value="Port ID" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.portID} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading value="Channel ID" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.channelID} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Counterparty (Channel ID)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.counterpartyChannelID} />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Proof Height (Revision Height)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.proofHeight.revisionHeight |> string_of_int} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Proof Height (Revision Number)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.proofHeight.revisionNumber |> string_of_int} />
      </Col>
    </Row>;
  };
};

module ChannelOpenConfirm = {
  [@react.component]
  let make = (~channel: MsgDecoder.ChannelOpenConfirm.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mb=24>
        <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <AddressRender address={channel.signer} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading value="Port ID" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.portID} />
      </Col>
      <Col mb=24>
        <Heading value="Channel ID" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.channelID} />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Proof Height (Revision Height)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.proofHeight.revisionHeight |> string_of_int} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Proof Height (Revision Number)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.proofHeight.revisionNumber |> string_of_int} />
      </Col>
    </Row>;
  };
};

module ChannelCloseInit = {
  [@react.component]
  let make = (~channel: MsgDecoder.ChannelCloseInit.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mb=24>
        <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <AddressRender address={channel.signer} />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading value="Port ID" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.portID} />
      </Col>
      <Col mb=24>
        <Heading value="Channel ID" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.channelID} />
      </Col>
    </Row>;
  };
};

module ChannelCloseConfirm = {
  [@react.component]
  let make = (~channel: MsgDecoder.ChannelCloseConfirm.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <Row>
      <Col col=Col.Six mb=24>
        <Heading value="Signer" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <AddressRender address={channel.signer} />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading value="Port ID" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.portID} />
      </Col>
      <Col mb=24>
        <Heading value="Channel ID" size=Heading.H5 marginBottom=8 color={theme.textSecondary} />
        <Text size=Text.Lg value={channel.channelID} />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Proof Height (Revision Height)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.proofHeight.revisionHeight |> string_of_int} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Proof Height (Revision Number)"
          size=Heading.H5
          marginBottom=8
          color={theme.textSecondary}
        />
        <Text size=Text.Lg value={channel.proofHeight.revisionNumber |> string_of_int} />
      </Col>
    </Row>;
  };
};
