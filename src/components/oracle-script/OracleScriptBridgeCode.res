module Styles = {
  open Css

  let tableLowerContainer = style(list{position(#relative)})
  let tableWrapper = style(list{
    padding(#px(24)),
    Media.mobile(list{padding2(~v=#px(20), ~h=#zero)}),
  })

  let codeImage = style(list{width(#px(20)), marginRight(#px(10))})
  let titleSpacing = style(list{marginBottom(#px(8))})
  let scriptContainer = style(list{
    fontSize(#px(12)),
    lineHeight(#px(20)),
    fontFamilies(list{#custom("Roboto Mono"), #monospace}),
  })

  let padding = style(list{padding(#px(20))})

  let selectWrapper = (theme: Theme.t) =>
    style(list{
      backgroundColor(theme.white),
      border(#px(1), #solid, theme.tableRowBorderColor),
      borderRadius(#px(4)),
      display(#flex),
      flexDirection(#row),
      padding2(~v=#px(10), ~h=#px(16)),
      width(#percent(100.)),
      maxWidth(#px(200)),
      minHeight(#px(37)),
      Media.mobile(list{padding2(~v=#px(10), ~h=#px(8))}),
    })

  let selectContent = style(list{
    backgroundColor(#transparent),
    borderStyle(#none),
    width(#percent(100.)),
    outlineStyle(#none),
  })

  let iconBody = style(list{width(#px(20)), height(#px(20))})
}

let renderCode = content =>
  <div className=Styles.scriptContainer>
    <ReactHighlight className=Styles.padding> {content |> React.string} </ReactHighlight>
  </div>

type target_platform_t =
  | Ethereum
  | CosmosIBC
/* | Kadena; */

type language_t =
  | Solidity
  | /* | Vyper */
  Go
/* | PACT; */

exception WrongLanugageChoice(string)
exception WrongPlatformChoice(string)

let toLanguageVariant = x =>
  switch x {
  | "Solidity" => Solidity
  /* | "Vyper" => Vyper */
  | "Go" => Go
  /* | "PACT" => PACT */
  | _ => raise(WrongLanugageChoice("Chosen language does not exist"))
  }

let toPlatformVariant = x =>
  switch x {
  | "Ethereum" => Ethereum
  | "Cosmos IBC" => CosmosIBC
  /* | "Kadena" => Kadena */
  | _ => raise(WrongPlatformChoice("Chosen platform does not exist"))
  }

let toLanguageString = x =>
  switch x {
  | Solidity => "Solidity"
  /* | Vyper => "Vyper" */
  | Go => "Go"
  }
/* | PACT => "PACT"; */

let toPlatformString = x =>
  switch x {
  | Ethereum => "Ethereum"
  | CosmosIBC => "Cosmos IBC"
  }
/* | Kadena => "Kadena"; */

let getLanguagesByPlatform = x =>
  //TODO: Add back Vyper
  switch x {
  | Ethereum => [Solidity]
  | CosmosIBC => [Go]
  }
/* | Kadena => [|PACT|]; */

module TargetPlatformIcon = {
  @react.component
  let make = (~icon) =>
    <div className={CssHelper.flexBox(~justify=#center, ())}>
      <img
        className=Styles.iconBody
        src={switch icon {
        | Ethereum => Images.ethereumIcon
        | CosmosIBC => Images.cosmosIBCIcon
        /* | Kadena => Images.kadenaIcon */
        }}
      />
    </div>
}

module LanguageIcon = {
  @react.component
  let make = (~icon) =>
    <div className={CssHelper.flexBox(~justify=#center, ())}>
      <img
        className=Styles.iconBody
        src={switch icon {
        | Solidity => Images.solidityIcon
        /* | Vyper => Images.vyperIcon */
        | Go => Images.golangIcon
        /* | PACT => Images.pactIcon */
        }}
      />
    </div>
}

let getFileNameFromLanguage = (~language, ~dataType) => {
  let dataTypeString = dataType |> Obi.dataTypeToString
  switch language {
  | Solidity => "Decoders.sol"
  | Go => j`$(dataTypeString)Decoder.go`
  }
}

let getCodeFromSchema = (~schema, ~language, ~dataType) =>
  switch language {
  | Solidity => Obi.generateDecoderSolidity(schema)
  | Go => Obi.generateDecoderGo("main", schema, dataType)
  }

module GenerateDecodeCode = {
  @react.component
  let make = (~language, ~schema, ~dataType) => {
    let codeOpt = getCodeFromSchema(~schema, ~language, ~dataType)
    let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
    let code = switch codeOpt {
    | Some(code) => code
    | _ => "Code is not available."
    }
    <>
      <Row marginBottom=24 marginTop=24 marginTopSm=12 marginBottomSm=12>
        <Col>
          <div className={CssHelper.flexBox(~justify=#spaceBetween, ())}>
            <div className={CssHelper.flexBox()}>
              <Icon name="fal fa-file" size=16 color=theme.textSecondary />
              <HSpacing size=Spacing.sm />
              <Text
                value={getFileNameFromLanguage(~language, ~dataType)}
                weight=Text.Semibold
                size=Text.Lg
                block=true
              />
            </div>
            <CopyButton data=code title="Copy Code" />
          </div>
        </Col>
      </Row>
      <div className=Styles.tableLowerContainer> {code |> renderCode} </div>
    </>
  }
}

@react.component
let make = (~schema) => {
  let (targetPlatform, setTargetPlatform) = React.useState(_ => Ethereum)
  let (language, setLanguage) = React.useState(_ => Solidity)

  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)

  <div className=Styles.tableWrapper>
    <Row marginBottom=24>
      <Col col=Col.Three colSm=Col.Six>
        <div className={Css.merge(list{CssHelper.flexBox(), Styles.titleSpacing})}>
          <Heading size=Heading.H5 value="Target Platform" />
          <HSpacing size=Spacing.xs />
          <CTooltip
            tooltipPlacementSm=CTooltip.BottomLeft
            tooltipText="The target platform to which to generate the code for">
            <Icon name="fal fa-info-circle" size=10 color=theme.textPrimary />
          </CTooltip>
        </div>
        <div className={Styles.selectWrapper(theme)}>
          <TargetPlatformIcon icon=targetPlatform />
          <div className={CssHelper.selectWrapper(~pRight=0, ())}>
            <select
              className=Styles.selectContent
              onChange={event => {
                let newPlatform = ReactEvent.Form.target(event)["value"] |> toPlatformVariant
                setTargetPlatform(_ => newPlatform)
                let newLanguage = newPlatform |> getLanguagesByPlatform |> Belt_Array.getExn(_, 0)
                setLanguage(_ => newLanguage)
              }}>
              {[Ethereum]->Belt_Array.map(symbol =>
                <option key={symbol |> toPlatformString} value={symbol |> toPlatformString}>
                  {symbol |> toPlatformString |> React.string}
                </option>
              ) |> React.array}
            </select>
          </div>
        </div>
      </Col>
      <Col col=Col.Three colSm=Col.Six>
        <div className={Css.merge(list{CssHelper.flexBox(), Styles.titleSpacing})}>
          <Heading size=Heading.H5 value="Language" />
          <HSpacing size=Spacing.xs />
          <CTooltip tooltipText="The programming language">
            <Icon name="fal fa-info-circle" size=10 color=theme.textPrimary />
          </CTooltip>
        </div>
        <div className={Styles.selectWrapper(theme)}>
          <LanguageIcon icon=language />
          <div className={CssHelper.selectWrapper(~pRight=0, ())}>
            <select
              className=Styles.selectContent
              onChange={event => {
                let newLanguage = ReactEvent.Form.target(event)["value"] |> toLanguageVariant
                setLanguage(_ => newLanguage)
              }}>
              {targetPlatform
              |> getLanguagesByPlatform
              |> Belt.Array.map(_, symbol =>
                <option key={symbol |> toLanguageString} value={symbol |> toLanguageString}>
                  {symbol |> toLanguageString |> React.string}
                </option>
              )
              |> React.array}
            </select>
          </div>
        </div>
      </Col>
    </Row>
    <Row marginBottom=24 marginBottomSm=12>
      <Col>
        <div className={CssHelper.flexBox(~justify=#spaceBetween, ())}>
          <div className={CssHelper.flexBox()}>
            <Icon name="fal fa-file" size=16 color=theme.textSecondary />
            <HSpacing size=Spacing.sm />
            <Text value="Oracle Script Schema" weight=Text.Semibold size=Text.Lg block=true />
          </div>
          <CopyButton data=schema title="Copy Code" />
        </div>
      </Col>
    </Row>
    <div className=Styles.tableLowerContainer> {schema |> renderCode} </div>
    <GenerateDecodeCode language schema dataType=Obi.Params />
  </div>
}
