module CreateValidatorMsg = {
  [@react.component]
  let make = (~validator: MsgDecoder.CreateValidator.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Moniker"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <ValidatorMonikerLink
          validatorAddress={validator.validatorAddress}
          moniker={validator.moniker}
          identity={validator.identity}
          width={`percent(100.)}
          avatarWidth=20
          size=Text.Lg
        />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Identity"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Text size=Text.Lg value={validator.identity} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Commission Rate"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Text
          size=Text.Lg
          value={
            (validator.commissionRate *. 100.)->Js.Float.toFixedWithPrecision(~digits=4) ++ "%"
          }
        />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Commission Max Rate"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Text
          size=Text.Lg
          value={
            (validator.commissionMaxRate *. 100.)->Js.Float.toFixedWithPrecision(~digits=4) ++ "%"
          }
        />
      </Col>
      <Col mb=24>
        <Heading
          value="Commission Max Change"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Text
          size=Text.Lg
          value={
            (validator.commissionMaxChange *. 100.)->Js.Float.toFixedWithPrecision(~digits=4)
            ++ "%"
          }
        />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Delegator Address"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender position=AddressRender.Subtitle address={validator.delegatorAddress} />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Validator Address"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender
          position=AddressRender.Subtitle
          address={validator.validatorAddress}
          accountType=`validator
        />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Min Self Delegation"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AmountRender coins=[validator.minSelfDelegation] pos=AmountRender.TxIndex />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Self Delegation"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AmountRender coins=[validator.selfDelegation] pos=AmountRender.TxIndex />
      </Col>
      <Col mb=24>
        <Heading
          value="Details"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Text size=Text.Lg value={validator.details} />
      </Col>
      <Col>
        <Heading
          value="Website"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Text size=Text.Lg value={validator.website} />
      </Col>
    </Row>;
  };
};

module EditValidatorMsg = {
  [@react.component]
  let make = (~validator: BandScan.MsgDecoder.EditValidator.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Moniker"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Text
          value={validator.moniker == Config.doNotModify ? "Unchanged" : validator.moniker}
          size=Text.Lg
        />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Identity"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Text
          size=Text.Lg
          value={validator.identity == Config.doNotModify ? "Unchanged" : validator.identity}
        />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Commission Rate"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Text
          size=Text.Lg
          value={
            switch (validator.commissionRate) {
            | Some(rate) => (rate *. 100.)->Js.Float.toFixedWithPrecision(~digits=4) ++ "%"
            | None => "Unchanged"
            }
          }
        />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Validator Address"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender
          position=AddressRender.Subtitle
          address={validator.sender}
          accountType=`validator
        />
      </Col>
      <Col mb=24>
        <Heading
          value="Min Self Delegation"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        {switch (validator.minSelfDelegation) {
         | Some(minSelfDelegation') =>
           <AmountRender coins=[minSelfDelegation'] pos=AmountRender.TxIndex />
         | None => <Text value="Unchanged" size=Text.Lg />
         }}
      </Col>
      <Col>
        <Heading
          value="Details"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Text
          size=Text.Lg
          value={validator.details == Config.doNotModify ? "Unchanged" : validator.details}
        />
      </Col>
    </Row>;
  };
};

module UnjailMsg = {
  [@react.component]
  let make = (~unjail: MsgDecoder.Unjail.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <Row>
      <Col col=Col.Six>
        <Heading
          value="Validator"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender
          position=AddressRender.Subtitle
          address={unjail.address}
          accountType=`validator
        />
      </Col>
    </Row>;
  };
};

module GrantMsg = {
  [@react.component]
  let make = (~address: MsgDecoder.Grant.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Granter"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender
          position=AddressRender.Subtitle
          address={address.validator}
          accountType=`validator
        />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Grantee Address"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender position=AddressRender.Subtitle address={address.reporter} />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Authorization URL"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Text size=Text.Lg value={address.url} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Expiration Date"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Timestamp time={address.expiration} size=Text.Lg />
      </Col>
    </Row>;
  };
};

module RevokeMsg = {
  [@react.component]
  let make = (~address: MsgDecoder.Revoke.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Granter"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender
          position=AddressRender.Subtitle
          address={address.validator}
          accountType=`validator
        />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Grantee Address"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender position=AddressRender.Subtitle address={address.reporter} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Message Type URL"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <Text size=Text.Lg value={address.msgTypeUrl} />
      </Col>
    </Row>;
  };
};

module ActivateMsg = {
  [@react.component]
  let make = (~activate: MsgDecoder.Activate.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <Row>
      <Col col=Col.Six>
        <Heading
          value="Validator"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender
          position=AddressRender.Subtitle
          address={activate.validatorAddress}
          accountType=`validator
        />
      </Col>
    </Row>;
  };
};

module SetWithdrawAddressMsg = {
  [@react.component]
  let make = (~set: MsgDecoder.SetWithdrawAddress.t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <Row>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Delegator Address"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender
          position=AddressRender.Subtitle
          address={set.delegatorAddress}
          accountType=`validator
        />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Withdraw Address"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender position=AddressRender.Subtitle address={set.withdrawAddress} />
      </Col>
    </Row>;
  };
};

module ExecSuccessMsg = {
  [@react.component]
  let make = (~address: MsgDecoder.Exec.success_t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <Row>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Grantee Address"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender position=AddressRender.Subtitle address={address.grantee} />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Executed Messages"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        {address.msgs
         ->Belt.List.mapWithIndex((index, msg) => {
             <Text
               key={(msg |> ExecDecoder.getName) ++ (index |> string_of_int)}
               size=Text.Lg
               value={msg |> ExecDecoder.getName}
             />
           })
         ->Belt.List.toArray
         ->React.array}
      </Col>
    </Row>;
  };
};

module ExecFailMsg = {
  [@react.component]
  let make = (~address: MsgDecoder.Exec.fail_t) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <Row>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Grantee Address"
          size=Heading.H4
          weight=Heading.Regular
          color={theme.textSecondary}
          marginBottom=8
        />
        <AddressRender position=AddressRender.Subtitle address={address.grantee} />
      </Col>
    </Row>;
  };
};
