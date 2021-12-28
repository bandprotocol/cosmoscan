module Styles = {
  open Css;

  let noDataImage = style([width(`auto), height(`px(70)), marginBottom(`px(16))]);
  let resultContainer = (theme: Theme.t) =>
    style([
      margin2(~v=`px(20), ~h=`zero),
      selector("> div + div", [borderTop(`px(1), `solid, theme.tableRowBorderColor)]),
    ]);
  let resultBox = style([padding(`px(20))]);
  let labelWrapper = style([flexShrink(0.), flexGrow(0.), flexBasis(`px(220))]);
  let resultWrapper =
    style([
      flexShrink(0.),
      flexGrow(0.),
      flexBasis(`calc((`sub, `percent(100.), `px(220)))),
    ]);
};

[@react.component]
let make = (~txResponse: TxCreator2.tx_response_t, ~schema: string) =>
  {
    let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);
    let requestsByTxHashSub = RequestSub.Mini.getListByTxHash(txResponse.txHash);
    let%Sub requestsByTxHash = requestsByTxHashSub;
    let requestOpt = requestsByTxHash->Belt_Array.get(0);

    <>
      <div className={Styles.resultContainer(theme)}>
        <div className={Css.merge([CssHelper.flexBox(), Styles.resultBox])}>
          <div className=Styles.labelWrapper>
            <Text value="Exit Status" color={theme.textSecondary} weight=Text.Regular />
          </div>
          <Text value={txResponse.success ? "0" : "1"} />
        </div>
        {switch (requestOpt) {
         | Some({id}) =>
           <div className={Css.merge([CssHelper.flexBox(), Styles.resultBox])}>
             <div className=Styles.labelWrapper>
               <Text value="Request ID" color={theme.textSecondary} weight=Text.Regular />
             </div>
             <TypeID.Request id />
           </div>
         | None => React.null
         }}
        <div className={Css.merge([CssHelper.flexBox(), Styles.resultBox])}>
          <div className=Styles.labelWrapper>
            <Text value="Tx Hash" color={theme.textSecondary} weight=Text.Regular />
          </div>
          <TxLink txHash={txResponse.txHash} width=500 />
        </div>
        {switch (requestOpt) {
         | Some({result: Some(result), id, resolveStatus}) =>
           switch (resolveStatus, result) {
           | (RequestSub.Success, result) =>
             let outputKVsOpt = Obi.decode(schema, "output", result);
             switch (outputKVsOpt) {
             | Some(outputKVs) =>
               <>
                 <div className={Css.merge([CssHelper.flexBox(), Styles.resultBox])}>
                   <div className=Styles.labelWrapper>
                     <Text
                       value="Output"
                       color={theme.textSecondary}
                       weight=Text.Regular
                       height={Text.Px(20)}
                     />
                   </div>
                   <div className=Styles.resultWrapper>
                     <KVTable
                       rows={
                         outputKVs
                         ->Belt_Array.map(({fieldName, fieldValue}) =>
                             [KVTable.Value(fieldName), KVTable.Value(fieldValue)]
                           )
                         ->Belt_List.fromArray
                       }
                     />
                   </div>
                 </div>
                 <OracleScriptExecuteProof id />
               </>
             | None =>
               <>
                 <RequestFailedResult id />
                 <div className={Css.merge([CssHelper.flexBox(), Styles.resultBox])}>
                   <div className=Styles.labelWrapper>
                     <Text
                       value="Output"
                       color={theme.textSecondary}
                       weight=Text.Regular
                       height={Text.Px(20)}
                     />
                   </div>
                   <div className=Styles.resultWrapper>
                     <Text
                       value="Schema not found"
                       color={theme.textSecondary}
                       weight=Text.Regular
                       height={Text.Px(20)}
                     />
                   </div>
                 </div>
               </>
             };
           | (Pending, _) => React.null
           | (_, _) => <RequestFailedResult id />
           }
         | Some(request) =>
           <div className={Css.merge([CssHelper.flexBox(), Styles.resultBox])}>
             <div className=Styles.labelWrapper>
               <Text
                 value="Waiting for output and `proof`"
                 color={theme.textSecondary}
                 weight=Text.Regular
               />
             </div>
             <div className=Styles.resultWrapper>
               <ProgressBar
                 reportedValidators={request.reportsCount}
                 minimumValidators={request.minCount}
                 requestValidators={request.askCount}
               />
             </div>
           </div>
         | None => React.null
         }}
      </div>
    </>
    |> Sub.resolve;
  }
  |> Sub.default(_, React.null);
