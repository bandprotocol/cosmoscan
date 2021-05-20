module Styles = {
  open Css;

  let container = style([position(`relative), paddingBottom(`px(24))]);

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

  let code =
    style([fontFamilies([`custom("Roboto Mono"), `monospace]), paddingBottom(`em(0.1))]);

  let errMsg = style([position(`absolute), bottom(`px(7))]);
};

type input_t('a) = {
  text: string,
  value: option('a),
};

type status('a) =
  | Untouched
  | Touched(Result.t('a));

let empty = {text: "", value: None};

[@react.component]
let make =
    (
      ~inputData,
      ~setInputData,
      ~msg,
      ~parse,
      ~maxValue=?,
      ~width,
      ~code=false,
      ~placeholder="",
      ~inputType="text",
      ~autoFocus=false,
      ~maxWarningMsg=false,
      ~id,
    ) => {
  let (status, setStatus) = React.useState(_ => Untouched);

  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  let onNewText = newText => {
    let newVal = parse(newText);
    setStatus(_ => Touched(newVal));
    switch (newVal) {
    | Ok(newVal') => setInputData(_ => {text: newText, value: Some(newVal')})
    | Err(_) => setInputData(_ => {text: newText, value: None})
    };
  };

  <div className=Styles.container>
    <Heading
      value=msg
      size=Heading.H5
      marginBottom=8
      align=Heading.Left
      color={theme.textSecondary}
      weight=Heading.Regular
    />
    <div className={CssHelper.flexBox(~wrap=`nowrap, ())}>
      <input
        id
        value={inputData.text}
        className={Css.merge([Styles.input(theme), code ? Styles.code : ""])}
        placeholder
        type_=inputType
        spellCheck=false
        autoFocus
        onChange={event => {
          let newText = ReactEvent.Form.target(event)##value;
          onNewText(newText);
        }}
      />
      {switch (maxValue) {
       | Some(maxValue') =>
         <>
           <HSpacing size=Spacing.md />
           <MaxButton
             onClick={_ =>
               maxWarningMsg
                 ? {
                   let isConfirm =
                     Webapi.Dom.(
                       window
                       |> Window.confirm(
                            {j|You will not have balance to do any action after using max balance.|j},
                          )
                     );
                   isConfirm ? onNewText(maxValue') : ();
                 }
                 : onNewText(maxValue')
             }
             disabled={inputData.text == maxValue'}
           />
         </>
       | None => React.null
       }}
    </div>
    {switch (status) {
     | Touched(Err(errMsg)) =>
       <div className=Styles.errMsg>
         <Text value=errMsg size=Text.Sm color=Theme.failColor />
       </div>
     | _ => React.null
     }}
  </div>;
};

module Loading = {
  [@react.component]
  let make = (~msg, ~useMax=false, ~code=false, ~placeholder) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <div className=Styles.container>
      <Heading
        value=msg
        size=Heading.H5
        marginBottom=8
        align=Heading.Left
        color={theme.textSecondary}
        weight=Heading.Regular
      />
      <div className={CssHelper.flexBox(~wrap=`nowrap, ())}>
        <input
          className={Css.merge([Styles.input(theme), code ? Styles.code : ""])}
          placeholder
          disabled=true
        />
        {useMax
           ? <> <HSpacing size=Spacing.md /> <MaxButton disabled=true onClick={_ => ()} /> </>
           : React.null}
      </div>
    </div>;
  };
};
