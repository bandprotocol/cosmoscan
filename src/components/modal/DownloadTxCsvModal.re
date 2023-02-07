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

  let downloadContainer = style([marginTop(`px(20))]);

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

type block_t = {timestamp: MomentRe.Moment.t};

type internal_t = {
  txHash: string,
  blockHeight: ID.Block.t,
  success: bool,
  gasFee: list(Coin.t),
  gasLimit: int,
  gasUsed: int,
  from: string,
  block: block_t,
  messages: Js.Json.t,
  memo: string,
};

[@bs.send] external clickElement: Dom.element => unit = "click";

[@react.component]
let make = (~address) => {
  let inputRef = React.useRef(Js.Nullable.null);
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  let dateNow = Js.Date.make();
  let currentDateStart =
    Js.Date.fromFloat(Js.Date.setUTCHoursM(Js.Date.make(), ~hours=0., ~minutes=0., ()));

  let (dateStart, setDateStart) = React.useState(_ => currentDateStart);
  let (dateEnd, setDateEnd) = React.useState(_ => dateNow);
  let (generate, setGenerate) = React.useState(_ => Nothing);
  let (csvFile, setCsvFile) = React.useState(_ => "");

  let variables =
    TxQueryByBlockTimestamp.TxQueryByBlockTimestampConfig.make(
      ~address=address |> Address.toBech32,
      ~greater=dateStart |> Js.Date.toISOString |> Js.Json.string,
      ~less=dateEnd |> Js.Date.toISOString |> Js.Json.string,
      (),
    )##variables;

  let (_, refetch) =
    TxQueryByBlockTimestamp.get(
      address,
      dateStart |> Js.Date.toISOString,
      dateEnd |> Js.Date.toISOString,
      (),
    );

  React.useEffect1(
    () => {
      switch (inputRef->React.Ref.current->Js.Nullable.toOption) {
      | None => None
      | Some(el) =>
        if (csvFile->Js.String2.length > 0) {
          el->clickElement;
        };
        None;
      }
    },
    [|csvFile|],
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
        selected=dateStart
        onChange={date =>
          setDateStart(_ =>
            Js.Date.fromFloat(Js.Date.setUTCHoursM(date, ~hours=0.0, ~minutes=0.0, ()))
          )
        }
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
        onChange={date =>
          setDateEnd(_ =>
            Js.Date.fromFloat(Js.Date.setUTCHoursM(date, ~hours=23.0, ~minutes=59.0, ()))
          )
        }
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
      <Icon name="fal fa-exclamation-circle" color={theme.neutral_600} />
      <HSpacing size=Spacing.sm />
      <Text
        value="The exported transactions will be limited to 50 transactions. Please choose a small range to export."
        size=Text.Md
      />
    </div>
    <div className=Styles.downloadContainer>
      {switch (generate) {
       | Generating =>
         <div className={CssHelper.flexBox(~justify=`center, ())}>
           <LoadingCensorBar.CircleSpin size=30 height=30 />
           <Heading value="Generating CSV File..." size=Heading.H5 marginTop=8 />
         </div>
       | _ =>
         <Button
           style=Styles.downloadBtn
           onClick={_ => {
             setGenerate(_ => Generating);
             let _ =
               refetch(~variables, ())
               |> Js.Promise.then_(res => {
                    setCsvFile(_ =>
                      res##account_transactions
                      ->Belt_Array.map((TxQueryByBlockTimestamp.{transaction}) =>
                          TxQueryByBlockTimestamp.toExternal(transaction)
                        )
                      ->CSVGenerator.create
                    );
                    setGenerate(_ => Success);
                    Js.Promise.resolve();
                  })
               |> Js.Promise.catch(err => {
                    setGenerate(_ => Error);
                    Js.Promise.resolve();
                  });
             ();
           }}>
           {"Download" |> React.string}
         </Button>
       }}
    </div>
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
  </div>;
};
