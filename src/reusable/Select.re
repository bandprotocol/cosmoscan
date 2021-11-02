module Styles = {
  open Css;

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

  let selectWrapper =
    style([
      width(`percent(100.)),
      maxWidth(`px(189)),
      Media.mobile([maxWidth(`percent(100.))]),
    ]);
};

[@react.component]
let make = (~options, ~setSelectOption, ~selectLabel, ~selectedOption, ~disabled=false) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <div className=Styles.selectWrapper>
    <div className={CssHelper.mb(~size=8, ())}> <Text value=selectLabel /> </div>
    <div className={CssHelper.selectWrapper(~fontColor=theme.textPrimary, ~mW=770, ())}>
      <select
        disabled
        className={Styles.input(theme)}
        value=selectedOption
        onChange={event => {
          let newVal = ReactEvent.Form.target(event)##value;
          setSelectOption(newVal);
        }}>
        <option value=""> {"All " ++ selectLabel ++ "s" |> React.string} </option>
        {switch (options |> Belt.Array.length) {
         | 0 =>
           options
           |> Belt.Array.map(_, value =>
                <option key=value value> {value |> React.string} </option>
              )
           |> React.array
         | _ => React.null
         }}
      </select>
    </div>
  </div>;
};
