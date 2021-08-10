module Transfer = {
  @react.component
  let make = (~msg: MsgDecoder.Transfer.t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Sender"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=msg.sender />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Receiver"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text size=Text.Lg value=msg.receiver />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Source Port"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text size=Text.Lg value=msg.sourcePort />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Source Channel"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text size=Text.Lg value=msg.sourceChannel />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Token"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text
          size=Text.Lg
          value={(msg.token.amount |> Format.fPretty) ++ (" " ++ msg.token.denom)}
          breakAll=true
        />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Timeout Timestamp"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Timestamp time=msg.timeoutTimestamp size=Text.Lg />
      </Col>
    </Row>
  }
}
