module Styles = {
  open Css;

  let selectContainer = style([maxWidth(px(285))]);
  let buttonContainer = style([height(`percent(100.))]);
};

module CounterPartySelect = {
  type control_t = {
    display: string,
    height: string,
    width: string,
    fontSize: string,
    backgroundColor: string,
    borderRadius: string,
    border: string,
    color: string,
  };

  type option_t = {
    display: string,
    alignItems: string,
    height: string,
    fontSize: string,
    paddingLeft: string,
    cursor: string,
    color: string,
    backgroundColor: string,
  };

  type input_t = {color: string};

  type menu_t = {
    backgroundColor: string,
    overflowY: string,
    maxHeight: string,
  };

  type container_t = {
    width: string,
    position: string,
    boxSizing: string,
  };

  type singleValue_t = {
    margin: string,
    maxWidth: string,
    overflow: string,
    position: string,
    textOverflow: string,
    whiteSpace: string,
    top: string,
    transform: string,
    boxSizing: string,
    fontWeight: string,
    lineHeight: string,
  };

  type indicatorSeparator_t = {display: string};

  [@react.component]
  let make = () => {
    let ({ThemeContext.isDarkMode}, _) = React.useContext(ThemeContext.context);

    let (selectedValidator, setSelectedValidator) =
      React.useState(_ => ReactSelect.{value: "N/A", label: "Select Counterparty Chain"});
    let validatorList = [|
      ReactSelect.{value: "band-laozi-mainnet", label: "band-laozi-mainnet"},
      {value: "kava-7", label: "kava-7"},
    |];

    // TODO: Hack styles for react-select
    <div
      className={CssHelper.flexBox(~align=`flexStart, ~direction=`column, ())}
      id="counterPartySelection">
      <ReactSelect
        options=validatorList
        onChange={newOption => {
          let newVal = newOption;
          setSelectedValidator(_ => newVal);
        }}
        value=selectedValidator
        styles={
          ReactSelect.control: _ => {
            display: "flex",
            height: "37px",
            width: "100%",
            fontSize: "14px",
            color: isDarkMode ? "#ffffff" : "#303030",
            backgroundColor: isDarkMode ? "#2C2C2C" : "#ffffff",
            borderRadius: "8px",
            border:
              "1px solid"
              ++ {
                isDarkMode ? "#353535" : "#EDEDED";
              },
          },
          ReactSelect.option: _ => {
            fontSize: "14px",
            height: "37px",
            display: "flex",
            alignItems: "center",
            paddingLeft: "10px",
            cursor: "pointer",
            color: isDarkMode ? "#ffffff" : "#303030",
            backgroundColor: isDarkMode ? "#2C2C2C" : "#ffffff",
          },
          ReactSelect.container: _ => {
            width: "100%",
            position: "relative",
            boxSizing: "border-box",
          },
          ReactSelect.singleValue: _ => {
            margin: "0px 2px",
            maxWidth: "calc(100% - 8px)",
            overflow: "hidden",
            position: "absolute",
            textOverflow: "ellipsis",
            whiteSpace: "nowrap",
            top: "50%",
            transform: "translateY(-50%)",
            boxSizing: "border-box",
            fontWeight: "300",
            lineHeight: "1.3em",
          },
          ReactSelect.indicatorSeparator: _ => {display: "none"},
          ReactSelect.input: _ => {color: isDarkMode ? "#ffffff" : "#303030"},
          ReactSelect.menuList: _ => {
            backgroundColor: isDarkMode ? "#2C2C2C" : "#ffffff",
            overflowY: "scroll",
            maxHeight: "230px",
          },
        }
      />
    </div>;
  };
};

[@react.component]
let make = () => {
  let (tabIndex, setTabIndex) = React.useState(_ => IBCSub.Incoming);

  <Section ptSm=32 pbSm=32>
    <div className=CssHelper.container id="ibcSection">
      <Row alignItems=Row.Center marginBottom=40 marginBottomSm=24>
        <Col col=Col.Twelve> <Heading value="IBCs" size=Heading.H2 /> </Col>
      </Row>
      <Row marginBottom=40 marginBottomSm=24>
        <Col col=Col.Six colSm=Col.Six>
          <div className=Styles.selectContainer>
            <div className={CssHelper.mb(~size=8, ())}> <Text value="Counterparty Chain" /> </div>
            <CounterPartySelect />
          </div>
        </Col>
        <Col col=Col.Six colSm=Col.Six>
          <div
            className={Css.merge([
              CssHelper.flexBox(~justify=`flexEnd, ~align=`flexEnd, ()),
              Styles.buttonContainer,
            ])}>
            <Button variant=Button.Outline px=24 py=8 onClick={_ => ()}>
              {"View Live Connection" |> React.string}
            </Button>
          </div>
        </Col>
      </Row>
      <Row>
        <Col col=Col.Twelve>
          <Tab.StateFilter
            tabs=[|{name: "Incoming", index: Incoming}, {name: "Outgoing", index: Outgoing}|]
            currentIndex=tabIndex
            setIndex=setTabIndex>
            {switch (tabIndex) {
             | Incoming => <IBCTab direction=Incoming />
             | Outgoing => <IBCTab direction=Outgoing />
             }}
          </Tab.StateFilter>
        </Col>
      </Row>
      <LiveConnection counterpartyChainID="consumer" />
    </div>
  </Section>;
};
