module Styles = {
  open Css;

  let statusImg = style([
    width(`px(20)),
    // marginTop(`px(-3)),
    marginRight(`px(8))
  ]);
};

[@react.component]
  let make = (~status) => {
    let isMobile = Media.isMobile();
    let (ThemeContext.{theme}, _) = React.useContext(ThemeContext.context);
          
    <div className={CssHelper.flexBox( ~align=`center, ())}>
      <img
        alt="Status Icon"
        src={status ? Images.success : Images.fail}
        className=Styles.statusImg
      />
      {
        isMobile ? 
        <Text 
          value={status ? "Success" : "Fail"} 
          size=Text.Md
          color={status ? theme.successColor : theme.failColor}
        /> 
        : React.null
      }
    </div>
  }

