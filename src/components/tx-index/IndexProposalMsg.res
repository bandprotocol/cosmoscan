module SubmitProposalMsg = {
  @react.component
  let make = (~proposal: MsgDecoder.SubmitProposal.success_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Proposer"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=proposal.proposer />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Proposal ID"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <TypeID.Proposal position=TypeID.Subtitle id=proposal.proposalID />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Title"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text size=Text.Lg value=proposal.title />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Deposit Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=proposal.initialDeposit pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module SubmitProposalFailMsg = {
  @react.component
  let make = (~proposal: MsgDecoder.SubmitProposal.fail_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col mb=24>
        <Heading
          value="Proposer"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=proposal.proposer />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Title"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text size=Text.Lg value=proposal.title />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Deposit Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=proposal.initialDeposit pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module DepositMsg = {
  @react.component
  let make = (~deposit: MsgDecoder.Deposit.success_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Depositor"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=deposit.depositor />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Proposal ID"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <TypeID.Proposal position=TypeID.Subtitle id=deposit.proposalID />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Title"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text size=Text.Lg value=deposit.title />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=deposit.amount pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module DepositFailMsg = {
  @react.component
  let make = (~deposit: MsgDecoder.Deposit.fail_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Depositor"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=deposit.depositor />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Proposal ID"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <TypeID.Proposal position=TypeID.Subtitle id=deposit.proposalID />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Amount"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=deposit.amount pos=AmountRender.TxIndex />
      </Col>
    </Row>
  }
}

module VoteMsg = {
  @react.component
  let make = (~vote: MsgDecoder.Vote.success_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Voter"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=vote.voterAddress />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Proposal ID"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <TypeID.Proposal position=TypeID.Subtitle id=vote.proposalID />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Title"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text size=Text.Lg value=vote.title />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Option"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text size=Text.Lg value=vote.option />
      </Col>
    </Row>
  }
}

module VoteFailMsg = {
  @react.component
  let make = (~vote: MsgDecoder.Vote.fail_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Voter"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=vote.voterAddress />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Proposal ID"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <TypeID.Proposal position=TypeID.Subtitle id=vote.proposalID />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Option"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text size=Text.Lg value=vote.option />
      </Col>
    </Row>
  }
}
