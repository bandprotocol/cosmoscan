type size =
  | Xs
  | Sm
  | Md
  | Lg
  | Xl
  | Xxl
  | Xxxl;

type weight =
  | Thin
  | Regular
  | Medium
  | Semibold
  | Bold;

type align =
  | Center
  | Right
  | Left;

type spacing =
  | Unset
  | Em(float);

type lineHeight =
  | Px(int)
  | PxFloat(float);

type transform =
  | Uppercase
  | Capitalize
  | Lowercase
  | Normal;

type placement =
  | AlignBottomEnd
  | AlignBottomStart
  | AlignBottom
  | AlignLeftEnd
  | AlignLeftStart
  | AlignLeft
  | AlignRightEnd
  | AlignRightStart
  | AlignRight
  | AlignTopEnd
  | AlignTopStart
  | AlignTop;

let toPlacementString = {
  fun
  | AlignBottomEnd => "bottom-end"
  | AlignBottomStart => "bottom-start"
  | AlignBottom => "bottom"
  | AlignLeftEnd => "left-end"
  | AlignLeftStart => "left-start"
  | AlignLeft => "left"
  | AlignRightEnd => "right-end"
  | AlignRightStart => "right-start"
  | AlignRight => "right"
  | AlignTopEnd => "top-end"
  | AlignTopStart => "top-start"
  | AlignTop => "top";
};

module Styles = {
  open Css;
  open Belt.Option;

  let fontSize =
    mapWithDefault(
      _,
      style([
        fontSize(`px(12)),
        lineHeight(`px(16)),
        Media.smallMobile([fontSize(`px(10))]),
      ]),
      fun
      | Xs =>
        style([
          fontSize(`px(8)),
          lineHeight(`em(1.41)),
          Media.smallMobile([fontSize(`px(7))]),
        ])
      | Sm =>
        style([
          fontSize(`px(10)),
          lineHeight(`em(1.41)),
          Media.smallMobile([fontSize(`px(8))]),
        ])
      | Md =>
        style([
          fontSize(`px(12)),
          lineHeight(`em(1.41)),
          Media.smallMobile([fontSize(`px(10))]),
        ])
      | Lg =>
        style([
          fontSize(`px(14)),
          lineHeight(`em(1.41)),
          Media.smallMobile([fontSize(`px(12))]),
        ])
      | Xl =>
        style([
          fontSize(`px(16)),
          lineHeight(`em(1.41)),
          Media.smallMobile([fontSize(`px(14))]),
        ])
      | Xxl => style([fontSize(`px(18)), Media.smallMobile([fontSize(`px(16))])])
      | Xxxl => style([fontSize(`px(24)), Media.smallMobile([fontSize(`px(22))])]),
    );

  let fontWeight =
    mapWithDefault(
      _,
      style([]),
      fun
      | Thin => style([fontWeight(`num(300))])
      | Regular => style([fontWeight(`num(400))])
      | Medium => style([fontWeight(`num(500))])
      | Semibold => style([fontWeight(`num(600))])
      | Bold => style([fontWeight(`num(700))]),
    );

  let lineHeight =
    mapWithDefault(
      _,
      style([]),
      fun
      | Px(height) => style([lineHeight(`px(height))])
      | PxFloat(height) => style([lineHeight(`pxFloat(height))]),
    );

  let letterSpacing =
    mapWithDefault(
      _,
      style([letterSpacing(`unset)]),
      fun
      | Unset => style([letterSpacing(`unset)])
      | Em(spacing) => style([letterSpacing(`em(spacing))]),
    );

  let noWrap = style([whiteSpace(`nowrap)]);
  let block = style([display(`block)]);
  let ellipsis =
    style([overflow(`hidden), textOverflow(`ellipsis), whiteSpace(`nowrap), width(`auto)]);
  let underline = style([textDecoration(`underline)]);
  let textAlign =
    mapWithDefault(
      _,
      style([textAlign(`left)]),
      fun
      | Center => style([textAlign(`center)])
      | Right => style([textAlign(`right)])
      | Left => style([textAlign(`left)]),
    );

  let code =
    style([fontFamilies([`custom("Roboto Mono"), `monospace]), paddingBottom(`em(0.1))]);

  let special = style([fontFamilies([`custom("Lexend Exa"), `monospace])]);

  let textTransform =
    fun
    | Uppercase => style([textTransform(`uppercase)])
    | Lowercase => style([textTransform(`lowercase)])
    | Capitalize => style([textTransform(`capitalize)])
    | Normal => style([textTransform(`unset)]);

  let breakAll = style([wordBreak(`breakAll)]);

  let textColor = color_ => {
    style([color(color_)]);
  };
};

[@react.component]
let make =
    (
      ~size=?,
      ~weight=?,
      ~align=?,
      ~spacing=?,
      ~height=?,
      ~nowrap=false,
      ~color=?,
      ~block=false,
      ~code=false,
      ~ellipsis=false,
      ~underline=false,
      ~breakAll=false,
      ~transform=Normal,
      ~value,
      ~tooltipItem=React.null,
      ~tooltipPlacement=AlignBottom,
      ~tooltipLeaveDelay=100,
      ~special=false,
    ) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  tooltipItem == React.null
    ? <span
        className={Css.merge([
          Styles.fontSize(size),
          Styles.fontWeight(weight),
          Styles.textAlign(align),
          Styles.letterSpacing(spacing),
          Styles.lineHeight(height),
          Styles.textTransform(transform),
          Styles.textColor(color->Belt.Option.getWithDefault(theme.neutral_600)),
          nowrap ? Styles.noWrap : "",
          block ? Styles.block : "",
          code ? Styles.code : "",
          special ? Styles.special : "",
          ellipsis ? Styles.ellipsis : "",
          underline ? Styles.underline : "",
          breakAll ? Styles.breakAll : "",
        ])}>
        {React.string(value)}
      </span>
    : <Tooltip
        title=tooltipItem
        placement={tooltipPlacement |> toPlacementString}
        arrow=true
        leaveDelay=tooltipLeaveDelay>
        <span
          className={Css.merge([
            Styles.fontSize(size),
            Styles.fontWeight(weight),
            Styles.textAlign(align),
            Styles.letterSpacing(spacing),
            Styles.lineHeight(height),
            Styles.textTransform(transform),
            Styles.textColor(color->Belt.Option.getWithDefault(theme.neutral_600)),
            nowrap ? Styles.noWrap : "",
            block ? Styles.block : "",
            code ? Styles.code : "",
            ellipsis ? Styles.ellipsis : "",
            underline ? Styles.underline : "",
            breakAll ? Styles.breakAll : "",
          ])}>
          {React.string(value)}
        </span>
      </Tooltip>;
};
