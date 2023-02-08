module Styles = {
  open Css;

  let labelWrapper =
    style([
      display(`flex),
      flexDirection(`column),
      flexGrow(0.),
      flexShrink(0.),
      flexBasis(`percent(30.)),
      paddingRight(`px(10)),
    ]);

  let valueWrapper =
    style([
      display(`flex),
      flexDirection(`column),
      flexGrow(0.),
      flexShrink(0.),
      flexBasis(`percent(70.)),
      selector("i", [margin2(~v=`zero, ~h=`px(8))]),
    ]);

  let status = style([width(`px(20)), height(`px(20))]);
};

module RequestID = {
  [@react.component]
  let make = (~requestID) => {
    <div className={CssHelper.flexBox(~align=`center, ())}>
      <div className=Styles.labelWrapper> <Text value="Request ID" /> </div>
      <div className=Styles.valueWrapper>
        <TypeID.Request id=requestID position=TypeID.Text />
      </div>
    </div>;
  };
};

module ResolveStatus = {
  [@react.component]
  let make = (~status) => {
    <div className={CssHelper.flexBox(~align=`center, ())}>
      <div className=Styles.labelWrapper> <Text value="Resolve Status" /> </div>
      <div className=Styles.valueWrapper>
        {switch (status) {
         | IBCQuery.OracleResponseData.Success =>
           <img alt="Success Icon" src=Images.success className=Styles.status />
         | Fail => <img alt="Fail Icon" src=Images.fail className=Styles.status />
         | _ => React.null
         }}
      </div>
    </div>;
  };
};

module BlockID = {
  [@react.component]
  let make = (~blockHeight) => {
    <div className={CssHelper.flexBox(~align=`center, ())}>
      <div className=Styles.labelWrapper> <Text value="Block ID" /> </div>
      <div className=Styles.valueWrapper>
        <TypeID.Block id=blockHeight position=TypeID.Text />
      </div>
    </div>;
  };
};

module TxHash = {
  [@react.component]
  let make = (~txHash) => {
    <div className={CssHelper.flexBox(~align=`center, ~wrap=`nowrap, ())}>
      <div className=Styles.labelWrapper> <Text value="Tx Hash" /> </div>
      <div className=Styles.valueWrapper> <TxLink txHash width=250 size=Text.Sm /> </div>
    </div>;
  };
};

module Common = {
  [@react.component]
  let make = (~srcChannel, ~dstChannel, ~srcPort, ~dstPort, ~sequence, ~counterPartyChainID) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper> <Text value="Counterparty Chain ID" /> </div>
        <div className=Styles.valueWrapper>
          <div className={CssHelper.flexBox(~align=`center, ())}>
            <Text value=counterPartyChainID />
          </div>
        </div>
      </div>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper> <Text value="Port" /> </div>
        <div className=Styles.valueWrapper>
          <div className={CssHelper.flexBox(~align=`center, ())}>
            <Text value=srcPort />
            <Icon name="far fa-arrow-right" color={theme.neutral_600} />
            <Text value=dstPort />
          </div>
        </div>
      </div>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper> <Text value="Channel" /> </div>
        <div className=Styles.valueWrapper>
          <div className={CssHelper.flexBox(~align=`center, ())}>
            <Text value=srcChannel />
            <Icon name="far fa-arrow-right" color={theme.neutral_600} />
            <Text value=dstChannel />
          </div>
        </div>
      </div>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper> <Text value="Sequence" /> </div>
        <div className=Styles.valueWrapper> <Text value={sequence |> string_of_int} /> </div>
      </div>
    </>;
  };
};

module Loading = {
  [@react.component]
  let make = () => {
    <>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper>
          <LoadingCensorBar width=100 height=16 radius=8 />
        </div>
        <div className=Styles.valueWrapper>
          <div className={CssHelper.flexBox(~align=`center, ())}>
            <LoadingCensorBar width=180 height=16 radius=8 />
          </div>
        </div>
      </div>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper>
          <LoadingCensorBar width=100 height=16 radius=8 />
        </div>
        <div className=Styles.valueWrapper>
          <div className={CssHelper.flexBox(~align=`center, ())}>
            <LoadingCensorBar width=180 height=16 radius=8 />
          </div>
        </div>
      </div>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper>
          <LoadingCensorBar width=100 height=16 radius=8 />
        </div>
        <div className=Styles.valueWrapper>
          <div className={CssHelper.flexBox(~align=`center, ())}>
            <LoadingCensorBar width=180 height=16 radius=8 />
          </div>
        </div>
      </div>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper>
          <LoadingCensorBar width=100 height=16 radius=8 />
        </div>
        <div className=Styles.valueWrapper>
          <LoadingCensorBar width=180 height=16 radius=8 />
        </div>
      </div>
    </>;
  };
};
