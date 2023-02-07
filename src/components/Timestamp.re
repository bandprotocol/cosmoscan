module Styles = {
  open Css;

  let timeContainer = style([display(`inlineFlex)]);
};

[@react.component]
let make =
    (
      ~time,
      ~prefix="",
      ~suffix="",
      ~size=Text.Sm,
      ~weight=Text.Regular,
      ~spacing=Text.Unset,
      ~code=false,
      ~upper=false,
      ~textAlign=Text.Left,
      ~color=?,
    ) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <div className=Styles.timeContainer>
    {prefix !== ""
       ? <>
           <Text
             value=prefix
             size
             weight
             color={color->Belt.Option.getWithDefault(theme.neutral_600)}
             spacing
             code
           />
           <HSpacing size=Spacing.sm />
         </>
       : React.null}
    <Text
      value={time |> MomentRe.Moment.format("YYYY-MM-DD HH:mm:ss")}
      size
      weight
      spacing
      code
      block=true
      align=textAlign
      color={color->Belt.Option.getWithDefault(theme.neutral_600)}
    />
    {suffix !== ""
       ? <>
           <HSpacing size=Spacing.sm />
           <Text
             value=suffix
             size
             weight
             spacing
             code
             color={color->Belt.Option.getWithDefault(theme.neutral_600)}
           />
         </>
       : React.null}
  </div>;
};

module Grid = {
  [@react.component]
  let make =
      (
        ~time,
        ~prefix="",
        ~suffix="",
        ~size=Text.Sm,
        ~weight=Text.Regular,
        ~spacing=Text.Unset,
        ~color=?,
        ~code=false,
        ~upper=false,
        ~textAlign=Text.Left,
      ) => {
    let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
    <div>
      {prefix !== ""
         ? <>
             <Text
               value=prefix
               size
               weight
               spacing
               color={color->Belt.Option.getWithDefault(theme.neutral_600)}
               code
               nowrap=true
             />
             <HSpacing size=Spacing.sm />
           </>
         : React.null}
      <div>
        <Text
          value={time |> MomentRe.Moment.format("YYYY-MM-DD")}
          size
          weight
          spacing
          color={color->Belt.Option.getWithDefault(theme.neutral_600)}
          code
          nowrap=true
          block=true
          align=textAlign
        />
      </div>
      <div>
        <Text
          value={time |> MomentRe.Moment.format("HH:mm:ss")}
          size
          weight
          spacing
          color={color->Belt.Option.getWithDefault(theme.neutral_600)}
          code
          nowrap=true
          block=true
          align=textAlign
        />
      </div>
      {suffix !== ""
         ? <>
             <HSpacing size=Spacing.sm />
             <Text
               value=suffix
               size
               weight
               spacing
               color={color->Belt.Option.getWithDefault(theme.neutral_600)}
               code
               nowrap=true
             />
           </>
         : React.null}
    </div>;
  };
};
