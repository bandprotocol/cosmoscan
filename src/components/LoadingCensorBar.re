module Styles = {
  open Css;

  let main = (~w, ~h, ~r, ~colorBase, ~colorLighter, ()) =>
    style([
      display(`flex),
      width(`px(w)),
      height(`px(h)),
      borderRadius(`px(r)),
      backgroundColor(colorBase),
      overflow(`hidden),
      position(`relative),
      before([
        contentRule(`text("")),
        position(`absolute),
        left(`percent(-250.)),
        width(`percent(500.)),
        height(`percent(100.)),
        backgroundColor(colorBase),
        backgroundImage(
          `linearGradient((
            `deg(270.),
            [
              (`percent(0.), colorBase),
              (`percent(25.), colorBase),
              (`percent(50.), colorLighter),
              (`percent(75.), colorBase),
              (`percent(100.), colorBase),
            ],
          )),
        ),
        animation(
          ~duration=1000,
          ~timingFunction=`linear,
          ~iterationCount=`infinite,
          keyframes([
            (0, [transform(translateX(`percent(-30.)))]),
            (100, [transform(translateX(`percent(50.)))]),
          ]),
        ),
      ]),
    ]);

  let alignRight = style([marginLeft(`auto)]);
  let fullWidth = style([width(`percent(100.))]);
  let mt = (~mt, ~mtSm, ()) =>
    style([marginTop(`px(mt)), Media.mobile([marginBottom(`px(mtSm))])]);
  let mb = (~mb, ~mbSm, ()) =>
    style([marginBottom(`px(mb)), Media.mobile([marginBottom(`px(mbSm))])]);

  let loadingCircle = size =>
    style([
      width(`px(size)),
      height(`px(size)),
      animation(
        ~duration=1000,
        ~timingFunction=`linear,
        ~iterationCount=`infinite,
        keyframes([
          (0, [transform(rotate(`deg(0.)))]),
          (100, [transform(rotate(`deg(360.)))]),
        ]),
      ),
    ]);

  let loadingCircleContainer = height_ =>
    style([width(`percent(100.)), height(`px(height_))]);
};

module CircleSpin = {
  [@react.component]
  let make = (~size=76, ~height=78) => {
    let ({ThemeContext.isDarkMode}, _) = React.useContext(ThemeContext.context);
    <div
      className={Css.merge([
        Styles.loadingCircleContainer(height),
        CssHelper.flexBox(~justify=`center, ()),
      ])}>
      <img
        src={isDarkMode ? Images.loadingCircleDark : Images.loadingCircleLight}
        className={Styles.loadingCircle(size)}
        alt="Loading Icon"
      />
    </div>;
  };
};

[@react.component]
let make =
    (
      ~width=100,
      ~height,
      ~fullWidth=false,
      ~radius=50,
      ~colorBase=?,
      ~colorLighter=?,
      ~isRight=false,
      ~mt=0,
      ~mtSm=mt,
      ~mb=0,
      ~mbSm=mb,
    ) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div
    className={Css.merge([
      Styles.main(
        ~w=width,
        ~h=height,
        ~r=radius,
        ~colorBase=colorBase->Belt.Option.getWithDefault(theme.loadingBaseColor),
        ~colorLighter=colorLighter->Belt.Option.getWithDefault(theme.loadingSecondaryColor),
        (),
      ),
      Styles.mt(~mt, ~mtSm, ()),
      Styles.mb(~mb, ~mbSm, ()),
      isRight ? Styles.alignRight : "",
      fullWidth ? Styles.fullWidth : "",
    ])}
  />;
};
