module SendMsg = {
  @react.component
  let make = (~send: MsgDecoder.Send.t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="From"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=send.fromAddress />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="To" size=Heading.H4 weight=Heading.Regular marginBottom=8 color=theme.textSecondary
        />
        <AddressRender address=send.toAddress />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=send.amount pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module DelegateMsg = {
  @react.component
  let make = (~delegation: MsgDecoder.Delegate.success_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Delegator Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=delegation.delegatorAddress />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Validator"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <ValidatorMonikerLink
          validatorAddress=delegation.validatorAddress
          moniker=delegation.moniker
          identity=delegation.identity
        />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=list{delegation.amount} pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module DelegateFailMsg = {
  @react.component
  let make = (~delegation: MsgDecoder.Delegate.fail_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Delegator Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=delegation.delegatorAddress />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Validator Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=delegation.validatorAddress accountType=#validator />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=list{delegation.amount} pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module UndelegateMsg = {
  @react.component
  let make = (~undelegation: MsgDecoder.Undelegate.success_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Delegator Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=undelegation.delegatorAddress />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Validator"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <ValidatorMonikerLink
          validatorAddress=undelegation.validatorAddress
          moniker=undelegation.moniker
          identity=undelegation.identity
        />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=list{undelegation.amount} pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module UndelegateFailMsg = {
  @react.component
  let make = (~undelegation: MsgDecoder.Undelegate.fail_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Delegator Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=undelegation.delegatorAddress />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Validator Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=undelegation.validatorAddress accountType=#validator />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=list{undelegation.amount} pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module RedelegateMsg = {
  @react.component
  let make = (~redelegation: MsgDecoder.Redelegate.success_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col mb=24>
        <Heading
          value="Delegator Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=redelegation.delegatorAddress />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Source Validator"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <ValidatorMonikerLink
          validatorAddress=redelegation.validatorSourceAddress
          moniker=redelegation.monikerSource
          identity=redelegation.identitySource
        />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Destination Validator"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <ValidatorMonikerLink
          validatorAddress=redelegation.validatorDestinationAddress
          moniker=redelegation.monikerDestination
          identity=redelegation.identityDestination
        />
      </Col>
      <Col>
        <Heading
          value="Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=list{redelegation.amount} pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module RedelegateFailMsg = {
  @react.component
  let make = (~redelegation: MsgDecoder.Redelegate.fail_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col mb=24>
        <Heading
          value="Delegator Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=redelegation.delegatorAddress />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Validator Source Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=redelegation.validatorSourceAddress accountType=#validator />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Validator Destination Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=redelegation.validatorDestinationAddress accountType=#validator />
      </Col>
      <Col>
        <Heading
          value="Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=list{redelegation.amount} pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module WithdrawRewardMsg = {
  @react.component
  let make = (~withdrawal: MsgDecoder.WithdrawReward.success_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Delegator Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=withdrawal.delegatorAddress />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Validator"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <ValidatorMonikerLink
          validatorAddress=withdrawal.validatorAddress
          moniker=withdrawal.moniker
          identity=withdrawal.identity
        />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=withdrawal.amount pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module WithdrawRewardFailMsg = {
  @react.component
  let make = (~withdrawal: MsgDecoder.WithdrawReward.fail_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Delegator Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=withdrawal.delegatorAddress />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Validator Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=withdrawal.validatorAddress accountType=#validator />
      </Col>
    </Row>
  }
}

module WithdrawComissionMsg = {
  @react.component
  let make = (~withdrawal: MsgDecoder.WithdrawCommission.success_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Validator"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <ValidatorMonikerLink
          validatorAddress=withdrawal.validatorAddress
          moniker=withdrawal.moniker
          identity=withdrawal.identity
        />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=withdrawal.amount pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module WithdrawComissionFailMsg = {
  @react.component
  let make = (~withdrawal: MsgDecoder.WithdrawCommission.fail_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six>
        <Heading
          value="Validator Address"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender address=withdrawal.validatorAddress accountType=#validator />
      </Col>
    </Row>
  }
}

module MultisendMsg = {
  @react.component
  let make = (~tx: MsgDecoder.MultiSend.t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    let isMobile = Media.isMobile()
    <>
      <Row>
        <Col col=Col.Six>
          <Heading
            value="From"
            size=Heading.H4
            weight=Heading.Regular
            marginBottom=8
            color=theme.textSecondary
          />
        </Col>
        {isMobile
          ? React.null
          : <Col col=Col.Six>
              <Heading
                value="Amount"
                size=Heading.H4
                weight=Heading.Regular
                marginBottom=8
                color=theme.textSecondary
              />
            </Col>}
        {tx.inputs
        ->Belt_List.mapWithIndex((idx, input) =>
          <React.Fragment key={(idx |> string_of_int) ++ (input.address |> Address.toBech32)}>
            <Col col=Col.Six mb=16 mbSm=8> <AddressRender address=input.address /> </Col>
            <Col col=Col.Six mb=16 mbSm=12>
              <AmountRender coins=input.coins pos=AmountRender.TxIndex />
            </Col>
          </React.Fragment>
        )
        ->Belt_List.toArray
        ->React.array}
      </Row>
      <SeperatedLine mt=8 mb=24 />
      <Heading
        value="To" size=Heading.H4 weight=Heading.Regular marginBottom=8 color=theme.textSecondary
      />
      <Row>
        {tx.outputs
        ->Belt_List.mapWithIndex((idx, output) =>
          <React.Fragment key={(idx |> string_of_int) ++ (output.address |> Address.toBech32)}>
            <Col col=Col.Six mb=16 mbSm=8> <AddressRender address=output.address /> </Col>
            <Col col=Col.Six mb=16 mbSm=12>
              <AmountRender coins=output.coins pos=AmountRender.TxIndex />
            </Col>
          </React.Fragment>
        )
        ->Belt_List.toArray
        ->React.array}
      </Row>
    </>
  }
}
