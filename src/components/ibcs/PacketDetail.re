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
};

module TopPart = {
  [@react.component]
  let make = (~blockHeight, ~srcChannel, ~dstChannel, ~srcPort, ~dstPort, ~sequence) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper> <Text value="Block ID" /> </div>
        <div className=Styles.valueWrapper>
          <TypeID.Block id=blockHeight position=TypeID.Text />
        </div>
      </div>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper> <Text value="Port" /> </div>
        <div className=Styles.valueWrapper>
          <div className={CssHelper.flexBox(~align=`center, ())}>
            <Text value=srcPort />
            <Icon name="far fa-arrow-right" color={theme.textSecondary} />
            <Text value=dstPort />
          </div>
        </div>
      </div>
      <div className={CssHelper.flexBox(~align=`center, ())}>
        <div className=Styles.labelWrapper> <Text value="Channel" /> </div>
        <div className=Styles.valueWrapper>
          <div className={CssHelper.flexBox(~align=`center, ())}>
            <Text value=srcChannel />
            <Icon name="far fa-arrow-right" color={theme.textSecondary} />
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
