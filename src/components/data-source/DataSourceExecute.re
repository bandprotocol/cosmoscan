module Styles = {
  open Css;

  let container =
    style([
      padding2(~v=`px(40), ~h=`px(45)),
      Media.mobile([padding2(~v=`px(20), ~h=`zero)]),
    ]);

  let upperTextCotainer = style([marginBottom(`px(24))]);

  let listContainer = style([marginBottom(`px(25))]);

  let input = (theme: Theme.t) =>
    style([
      width(`percent(100.)),
      height(`px(37)),
      paddingLeft(`px(9)),
      paddingRight(`px(9)),
      borderRadius(`px(4)),
      fontSize(`px(14)),
      fontWeight(`light),
      border(`px(1), `solid, theme.tableRowBorderColor),
      backgroundColor(theme.inputContrastColor),
      outlineStyle(`none),
      color(theme.textPrimary),
      fontFamilies([`custom("Montserrat"), `custom("sans-serif")]),
    ]);

  let button = isLoading =>
    style([
      backgroundColor(isLoading ? Theme.lightenBlue : Theme.baseBlue),
      fontWeight(`num(600)),
      opacity(isLoading ? 0.8 : 1.),
      cursor(isLoading ? `auto : `pointer),
      marginTop(`px(16)),
    ]);

  let withWH = (w, h) =>
    style([
      width(w),
      height(h),
      display(`flex),
      justifyContent(`center),
      alignItems(`center),
    ]);

  let resultContainer = (theme: Theme.t) =>
    style([
      margin2(~v=`px(20), ~h=`zero),
      selector("> div + div", [borderTop(`px(1), `solid, theme.tableRowBorderColor)]),
    ]);
  let resultBox = style([padding(`px(20))]);
  let labelWrapper =
    style([
      flexShrink(0.),
      flexGrow(0.),
      flexBasis(`px(220)),
      Media.mobile([flexBasis(`px(100))]),
    ]);
  let resultWrapper =
    style([
      flexShrink(0.),
      flexGrow(0.),
      flexBasis(`calc((`sub, `percent(100.), `px(220)))),
      Media.mobile([flexBasis(`calc((`sub, `percent(100.), `px(100))))]),
    ]);
};

module ParameterInput = {
  [@react.component]
  let make = (~name, ~index, ~setCalldataList) => {
    let name = Js.String.replaceByRe([%re "/[_]/g"], " ", name);
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

    <div className=Styles.listContainer>
      <Text value=name size=Text.Md weight=Text.Semibold transform=Text.Capitalize />
      <VSpacing size=Spacing.sm />
      <input
        className={Styles.input(theme)}
        type_="text"
        // TODO: Think about placeholder later
        // placeholder="Value"
        onChange={event => {
          let newVal = ReactEvent.Form.target(event)##value;
          setCalldataList(prev => {
            prev->Belt_List.mapWithIndex((i, value) => {index == i ? newVal : value})
          });
        }}
      />
    </div>;
  };
};

type result_data_t = {
  returncode: int,
  stdout: string,
  stderr: string,
};

type result_t =
  | Nothing
  | Loading
  | Error(string)
  | Success(result_data_t);

let loadingRender = (wDiv, wImg, h) => {
  <div className={Styles.withWH(wDiv, h)}> <LoadingCensorBar.CircleSpin size=wImg /> </div>;
};

module ResultRender = {
  [@react.component]
  let make = (~result) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    switch (result) {
    | Nothing => React.null
    | Loading =>
      <>
        <VSpacing size=Spacing.xl />
        {loadingRender(`percent(100.), 30, `px(30))}
        <VSpacing size=Spacing.lg />
      </>
    | Error(err) =>
      <>
        <VSpacing size=Spacing.lg />
        <div className=Styles.resultWrapper> <Text value=err breakAll=true /> </div>
      </>
    | Success({returncode, stdout, stderr}) =>
      <div className={Styles.resultContainer(theme)}>
        <div className={Css.merge([CssHelper.flexBox(), Styles.resultBox])}>
          <div className=Styles.labelWrapper>
            <Text value="Exit Status" weight=Text.Semibold />
          </div>
          <div className=Styles.resultWrapper> <Text value={returncode |> string_of_int} /> </div>
        </div>
        <div className={Css.merge([CssHelper.flexBox(), Styles.resultBox])}>
          <div className=Styles.labelWrapper> <Text value="Output" weight=Text.Semibold /> </div>
          <div className=Styles.resultWrapper> <Text value=stdout /> </div>
        </div>
        <div className={Css.merge([CssHelper.flexBox(), Styles.resultBox])}>
          <div className=Styles.labelWrapper> <Text value="Error" weight=Text.Semibold /> </div>
          <div className=Styles.resultWrapper>
            <Text value=stderr code=true weight=Text.Semibold />
          </div>
        </div>
      </div>
    };
  };
};

[@react.component]
let make = (~executable: JsBuffer.t) => {
  let params =
    ExecutableParser.parseExecutableScript(executable)->Belt_Option.getWithDefault([]);
  let numParams = params->Belt_List.length;

  let (callDataList, setCalldataList) = React.useState(_ => Belt_List.make(numParams, ""));

  let (result, setResult) = React.useState(_ => Nothing);

  <Row>
    <Col>
      <div className=Styles.container>
        <div className={Css.merge([CssHelper.flexBox(), Styles.upperTextCotainer])}>
          <Text
            value={
              "Test data source execution"
              ++ (numParams == 0 ? "" : " with" ++ (numParams == 1 ? " a " : " ") ++ "following")
            }
            size=Text.Lg
          />
          <HSpacing size=Spacing.sm />
          {numParams == 0
             ? React.null
             : <Text
                 value={numParams > 1 ? "parameters" : "parameter"}
                 weight=Text.Bold
                 size=Text.Lg
               />}
        </div>
        {numParams > 0
           ? <>
               {params
                ->Belt_List.mapWithIndex((i, param) =>
                    <ParameterInput key=param name=param index=i setCalldataList />
                  )
                ->Belt_List.toArray
                ->React.array}
             </>
           : React.null}
        <div className="buttonContainer">
          <div className={CssHelper.flexBox()}>
            <Text value="Click" />
            <HSpacing size=Spacing.sm />
            <Text value=" Test Execution " weight=Text.Bold />
            <HSpacing size=Spacing.sm />
            <Text value="to test the data source." />
          </div>
          <Button
            fsize=14
            style={Styles.button(result == Loading)}
            px=25
            py=13
            onClick={_ =>
              if (result != Loading) {
                setResult(_ => Loading);
                let _ =
                  AxiosRequest.execute(
                    AxiosRequest.t(
                      ~executable=executable->JsBuffer.toBase64,
                      ~calldata={
                        callDataList
                        ->Belt_List.reduce("", (acc, calldata) => acc ++ " " ++ calldata)
                        ->String.trim;
                      },
                      ~timeout=5000,
                    ),
                  )
                  |> Js.Promise.then_(res => {
                       setResult(_ =>
                         Success({
                           returncode: res##data##returncode,
                           stdout: res##data##stdout,
                           stderr: res##data##stderr,
                         })
                       );
                       Js.Promise.resolve();
                     })
                  |> Js.Promise.catch(err => {
                       let errorValue =
                         Js.Json.stringifyAny(err)->Belt_Option.getWithDefault("Unknown");
                       setResult(_ => Error(errorValue));
                       Js.Promise.resolve();
                     });
                ();
              }
            }>
            {(result == Loading ? "Executing ... " : "Test Execution") |> React.string}
          </Button>
        </div>
        <ResultRender result />
      </div>
    </Col>
  </Row>;
};
