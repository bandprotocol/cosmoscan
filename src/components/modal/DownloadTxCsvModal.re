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
  let currentDate = Js.Date.make();

  let (dateFrom, setDateFrom) = React.useState(_ => currentDate);
  let (dateTo, setDateTo) = React.useState(_ => currentDate);
  let (generate, setGenerate) = React.useState(_ => Nothing);
  let (csvFile, setCsvFile) = React.useState(_ => "data:text/csv;charset=utf-8,");
  let (csvString, setCsvString) = React.useState(_ => "0 transaction found");

  let (txQuery, fetchMore) =
    TxQueryByBlockTimestamp.get(
      address,
      dateFrom |> Js.Date.toISOString,
      dateTo |> Js.Date.toISOString,
      (),
    );

  let download = () => {
    let x =
      fetchMore(
        ~variables=
          TxQueryByBlockTimestamp.TxQueryByBlockTimestampConfig.makeVariables(
            ~address=address |> Address.toBech32,
            ~greater=dateFrom |> Js.Date.toISOString |> Js.Json.string,
            ~less=dateTo |> Js.Date.toISOString |> Js.Json.string,
            (),
          ),
      );

    switch (inputRef->React.Ref.current->Js.Nullable.toOption) {
    | None => ()
    | Some(el) => el->clickElement
    };
  };

  React.useEffect2(
    () => {
      switch (txQuery) {
      | Data(data) =>
        if (data->Belt.Array.size > 0) {
          setCsvString(_ => CSVGenerator.create(data));
        };
        let csv = "data:text/csv;charset=utf-8," ++ csvString;
        let excel = Js.Global.encodeURI(csv);
        setCsvFile(_ => excel);
      | _ => setGenerate(_ => Nothing)
      };
      None;
    },
    (dateTo, dateFrom),
  );

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
        selected=dateFrom
        onChange=setDateFrom
        selectsStart=true
        startDate=dateFrom
        endDate=dateTo
        maxDate=currentDate
      />
      <HSpacing size=Spacing.md />
      <DateInput
        msg="To"
        placeholderText="To"
        selected=dateTo
        onChange=setDateTo
        selectsStart=true
        selectsEnd=true
        startDate=dateFrom
        endDate=dateTo
        maxDate=currentDate
        minDate=dateFrom
      />
    </div>
    <div
      className={Css.merge([
        CssHelper.flexBox(~justify=`center, ~direction=`row, ()),
        CssHelper.mt(~size=20, ()),
      ])}>
      <Icon name="fal fa-exclamation-circle" color={theme.textSecondary} />
      <HSpacing size=Spacing.sm />
      <Text value="To avoid the 30 secs timeout, please select a small date range" size=Text.Md />
    </div>
    <Button style=Styles.downloadBtn onClick={_ => {download()}}>
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
          ++ (dateFrom |> Js.Date.toLocaleDateString)
          ++ "-"
          ++ (dateTo |> Js.Date.toLocaleDateString)
          ++ ".csv"
        }
        ref={ReactDOMRe.Ref.domRef(inputRef)}
        className=Styles.csvAnchor>
        {"show more" |> React.string}
      </a>
    </div>
  </div>;
};
