module Styles = {
  open Css;
  let container =
    style([
      width(`percent(100.)),
      maxWidth(`px(560)),
      minHeight(`px(200)),
      padding(`px(40)),
      Media.mobile([maxWidth(`px(300))]),
    ]);

  let downloadBtn = style([width(`percent(100.)), marginTop(`px(10))]);

  let csvAnchor = style([display(`none)]);
  let tips =
    style([
      marginTop(`px(20)),
      selector("> i", [display(`inlineBlock), marginTop(`px(3))]),
      selector("> span", [display(`inlineBlock)]),
    ]);
};

type generate_t =
  | Nothing
  | Generating
  | Success
  | Error;

[@bs.send] external clickElement: Dom.element => unit = "click";

[@react.component]
let make = (~address) => {
  let inputRef = React.useRef(Js.Nullable.null);
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  let dateNow = Js.Date.make();
  let currentDateStart = Js.Date.fromFloat(Js.Date.setHours(Js.Date.make(), 0.00));

  let (dateStart, setDateStart) = React.useState(_ => currentDateStart);
  let (dateEnd, setDateEnd) = React.useState(_ => dateNow);
  let (generate, setGenerate) = React.useState(_ => Nothing);
  let (csvFile, setCsvFile) =
    React.useState(_ => Js.Global.encodeURI("data:text/csv;charset=utf-8,"));

  let variables =
    TxQueryByBlockTimestamp.TxQueryByBlockTimestampConfig.make(
      ~address=address |> Address.toBech32,
      ~greater=dateStart |> Js.Date.toISOString |> Js.Json.string,
      ~less=dateEnd |> Js.Date.toISOString |> Js.Json.string,
      (),
    )##variables;

  let (txQuery, refetch) =
    TxQueryByBlockTimestamp.get(
      address,
      dateStart |> Js.Date.toISOString,
      dateEnd |> Js.Date.toISOString,
      (),
    );

  let download = () => {
    refetch(~variables, ())
    |> Js.Promise.then_(res => {
         setGenerate(_ => Generating);
         switch (inputRef->React.Ref.current->Js.Nullable.toOption) {
         | None => ()
         | Some(el) =>
           setCsvFile(_ =>
             Js.Global.encodeURI("data:text/csv;charset=utf-8,There is 0 transaction found")
           );
           switch (txQuery) {
           | Data(data) =>
             if (data->Belt.Array.size > 0) {
               let csvString = CSVGenerator.create(data);
               let csvFile = Js.Global.encodeURI("data:text/csv;charset=utf-8," ++ csvString);
               setCsvFile(_ => csvFile);
             };
             setGenerate(_ => Nothing);
             el->clickElement;
           | _ => setGenerate(_ => Nothing)
           };
         };
         Js.Promise.resolve();
       })
    |> ignore;
  };

  <div className=Styles.container>
    <Heading size=Heading.H4 value="Export CSV Transactions" marginBottom=10 />
    <Heading size=Heading.H5 value="Address" marginTop=24 marginBottom=8 />
    <AddressRender address position=AddressRender.Subtitle clickable=false />
    <div
      className={Css.merge([
        CssHelper.flexBox(~justify=`center, ~direction=`row, ()),
        CssHelper.mt(~size=20, ()),
      ])}>
      <DateInput
        msg="From"
        placeholderText="From"
        selected=dateStart
        onChange=setDateStart
        selectsStart=true
        startDate=dateStart
        endDate=dateEnd
        maxDate=dateNow
      />
      <HSpacing size=Spacing.md />
      <DateInput
        msg="To"
        placeholderText="To"
        selected=dateEnd
        onChange=setDateEnd
        selectsStart=true
        selectsEnd=true
        startDate=dateStart
        endDate=dateEnd
        maxDate=dateNow
        minDate=dateStart
      />
    </div>
    <div
      className={Css.merge([
        CssHelper.flexBox(~justify=`center, ~direction=`row, ~wrap=`nowrap, ~align=`start, ()),
        Styles.tips,
      ])}>
      <Icon name="fal fa-exclamation-circle" color={theme.textSecondary} />
      <HSpacing size=Spacing.sm />
      <Text
        value="The exported transactions will be limited to 400 transactions. Please choose a small range to export."
        size=Text.Md
      />
    </div>
    <Button style=Styles.downloadBtn onClick={_ => download()}>
      {switch (generate) {
       | Generating => "Generating CSV..." |> React.string
       | _ => "Download" |> React.string
       }}
    </Button>
    <div>
      <a
        href=csvFile
        download={
          "transactions-"
          ++ (dateStart |> Js.Date.toLocaleDateString)
          ++ "-"
          ++ (dateEnd |> Js.Date.toLocaleDateString)
          ++ ".csv"
        }
        ref={ReactDOMRe.Ref.domRef(inputRef)}
        className=Styles.csvAnchor>
        {"show more" |> React.string}
      </a>
    </div>
  </div>;
};
