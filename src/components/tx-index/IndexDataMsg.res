module CreateDataSourceMsg = {
  @react.component
  let make = (~dataSource: MsgDecoder.CreateDataSource.success_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Owner"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=dataSource.owner />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Name"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <div className={CssHelper.flexBox()}>
          <TypeID.DataSource position=TypeID.Subtitle id=dataSource.id />
          <HSpacing size=Spacing.sm />
          <Text value=dataSource.name size=Text.Lg />
        </div>
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Treasury"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=dataSource.treasury />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Fee"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=dataSource.fee />
      </Col>
    </Row>
  }
}

module CreateDataSourceFailMsg = {
  @react.component
  let make = (~dataSource: MsgDecoder.CreateDataSource.fail_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Owner"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=dataSource.owner />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Name"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text value=dataSource.name size=Text.Lg />
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Treasury"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=dataSource.treasury />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Fee"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=dataSource.fee />
      </Col>
    </Row>
  }
}

module EditDataSourceMsg = {
  @react.component
  let make = (~dataSource: MsgDecoder.EditDataSource.t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Owner"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=dataSource.owner />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Name"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <div className={CssHelper.flexBox()}>
          <TypeID.DataSource position=TypeID.Subtitle id=dataSource.id />
          {dataSource.name == Config.doNotModify
            ? React.null
            : <> <HSpacing size=Spacing.sm /> <Text value=dataSource.name size=Text.Lg /> </>}
        </div>
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Treasury"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=dataSource.treasury />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Fee"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AmountRender coins=dataSource.fee />
      </Col>
    </Row>
  }
}

module CreateOracleScriptMsg = {
  @react.component
  let make = (~oracleScript: MsgDecoder.CreateOracleScript.success_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Owner"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=oracleScript.owner />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Name"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <div className={CssHelper.flexBox()}>
          <TypeID.OracleScript position=TypeID.Subtitle id=oracleScript.id />
          <HSpacing size=Spacing.sm />
          <Text value=oracleScript.name />
        </div>
      </Col>
    </Row>
  }
}

module CreateOracleScriptFailMsg = {
  @react.component
  let make = (~oracleScript: MsgDecoder.CreateOracleScript.fail_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Owner"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=oracleScript.owner />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Name"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text value=oracleScript.name size=Text.Lg />
      </Col>
    </Row>
  }
}

module EditOracleScriptMsg = {
  @react.component
  let make = (~oracleScript: MsgDecoder.EditOracleScript.t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Owner"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=oracleScript.owner />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Name"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <div className={CssHelper.flexBox()}>
          <TypeID.OracleScript position=TypeID.Subtitle id=oracleScript.id />
          {oracleScript.name == Config.doNotModify
            ? React.null
            : <> <HSpacing size=Spacing.sm /> <Text value=oracleScript.name size=Text.Lg /> </>}
        </div>
      </Col>
    </Row>
  }
}

module RequestMsg = {
  @react.component
  let make = (~request: MsgDecoder.Request.success_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    let calldataKVsOpt = Obi.decode(request.schema, "input", request.calldata)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Owner"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=request.sender />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Request ID"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <TypeID.Request position=TypeID.Subtitle id=request.id />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Oracle Script"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <div className={CssHelper.flexBox()}>
          <TypeID.OracleScript position=TypeID.Subtitle id=request.oracleScriptID />
          <HSpacing size=Spacing.sm />
          <Text value=request.oracleScriptName size=Text.Lg />
        </div>
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Prepare Gas"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text value={request.prepareGas |> string_of_int} size=Text.Lg />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Execute Gas"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text value={request.executeGas |> string_of_int} size=Text.Lg />
      </Col>
      <Col mb=24>
        <div
          className={Css.merge(list{
            CssHelper.flexBox(~justify=#spaceBetween, ()),
            CssHelper.mb(),
          })}>
          <Heading
            value="Calldata" size=Heading.H4 weight=Heading.Regular color=theme.textSecondary
          />
          <CopyButton
            data={request.calldata |> JsBuffer.toHex(~with0x=false)} title="Copy as bytes" width=125
          />
        </div>
        {switch calldataKVsOpt {
        | Some(calldataKVs) =>
          <KVTable
            rows={calldataKVs
            ->Belt_Array.map(({fieldName, fieldValue}) => list{
              KVTable.Value(fieldName),
              KVTable.Value(fieldValue),
            })
            ->Belt_List.fromArray}
          />
        | None =>
          <Text
            value="Could not decode calldata."
            spacing=Text.Em(0.02)
            nowrap=true
            ellipsis=true
            code=true
            block=true
            size=Text.Lg
          />
        }}
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Request Validator Count"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text value={request.askCount |> string_of_int} size=Text.Lg />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Sufficient Validator Count"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text value={request.minCount |> string_of_int} size=Text.Lg />
      </Col>
    </Row>
  }
}

module RequestFailMsg = {
  @react.component
  let make = (~request: MsgDecoder.Request.fail_t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Owner"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=request.sender />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Oracle Script"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <div className={CssHelper.flexBox()}>
          <TypeID.OracleScript position=TypeID.Subtitle id=request.oracleScriptID />
        </div>
      </Col>
      <Col mb=24>
        <Heading
          value="Calldata"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <div className={CssHelper.flexBox()}>
          <Text value={request.calldata |> JsBuffer.toHex} size=Text.Lg />
          <HSpacing size=Spacing.sm />
          <CopyRender width=14 message={request.calldata |> JsBuffer.toHex} />
        </div>
      </Col>
      <Col col=Col.Six mbSm=24>
        <Heading
          value="Request Validator Count"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text value={request.askCount |> string_of_int} size=Text.Lg />
      </Col>
      <Col col=Col.Six>
        <Heading
          value="Sufficient Validator Count"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <Text value={request.minCount |> string_of_int} size=Text.Lg />
      </Col>
    </Row>
  }
}

module ReportMsg = {
  @react.component
  let make = (~report: MsgDecoder.Report.t) => {
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    <Row>
      <Col col=Col.Six mb=24>
        <Heading
          value="Owner"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <AddressRender position=AddressRender.Subtitle address=report.reporter />
      </Col>
      <Col col=Col.Six mb=24>
        <Heading
          value="Request ID"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <TypeID.Request position=TypeID.Subtitle id=report.requestID />
      </Col>
      <Col>
        <Heading
          value="Raw Data Report"
          size=Heading.H4
          weight=Heading.Regular
          marginBottom=8
          color=theme.textSecondary
        />
        <KVTable
          headers=list{"External Id", "Exit Code", "Value"}
          rows={report.rawReports |> Belt_List.map(_, rawReport => list{
            KVTable.Value(rawReport.externalDataID |> string_of_int),
            KVTable.Value(rawReport.exitCode |> string_of_int),
            KVTable.Value(rawReport.data |> JsBuffer.toUTF8),
          })}
        />
      </Col>
    </Row>
  }
}
