module Styles = {
  open Css

  let main = (~w, ~h, ~r, ~colorBase, ~colorLighter, ()) =>
    style(list{
      display(#flex),
      width(#px(w)),
      height(#px(h)),
      borderRadius(#px(r)),
      backgroundColor(colorBase),
      overflow(#hidden),
      position(#relative),
      before(list{
        contentRule(#text("")),
        position(#absolute),
        left(#percent(-250.)),
        width(#percent(500.)),
        height(#percent(100.)),
        backgroundColor(colorBase),
        backgroundImage(
          #linearGradient(
            #deg(270.),
            list{
              (#percent(0.), colorBase),
              (#percent(25.), colorBase),
              (#percent(50.), colorLighter),
              (#percent(75.), colorBase),
              (#percent(100.), colorBase),
            },
          ),
        ),
        animation(
          ~duration=1000,
          ~timingFunction=#linear,
          ~iterationCount=#infinite,
          keyframes(list{
            (0, list{transform(translateX(#percent(-30.)))}),
            (100, list{transform(translateX(#percent(50.)))}),
          }),
        ),
      }),
    })

  let alignRight = style(list{marginLeft(#auto)})
  let fullWidth = style(list{width(#percent(100.))})
  let mt = (~mt, ~mtSm, ()) =>
    style(list{marginTop(#px(mt)), Media.mobile(list{marginBottom(#px(mtSm))})})
  let mb = (~mb, ~mbSm, ()) =>
    style(list{marginBottom(#px(mb)), Media.mobile(list{marginBottom(#px(mbSm))})})

  let loadingCircle = size =>
    style(list{
      width(#px(size)),
      height(#px(size)),
      animation(
        ~duration=1000,
        ~timingFunction=#linear,
        ~iterationCount=#infinite,
        keyframes(list{
          (0, list{transform(rotate(#deg(0.)))}),
          (100, list{transform(rotate(#deg(360.)))}),
        }),
      ),
    })

  let loadingCircleContainer = height_ => style(list{width(#percent(100.)), height(#px(height_))})
}

module CircleSpin = {
  @react.component
  let make = (~size=76, ~height=78) => {
    let ({ThemeContext.isDarkMode: isDarkMode}, _) = React.useContext(ThemeContext.context)
    <div
      className={Css.merge(list{
        Styles.loadingCircleContainer(height),
        CssHelper.flexBox(~justify=#center, ()),
      })}>
      <img
        src={isDarkMode ? Images.loadingCircleDark : Images.loadingCircleLight}
        className={Styles.loadingCircle(size)}
      />
    </div>
  }
}

@react.component
let make = (
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
  let ({ThemeContext.theme: theme}, _) = React.useContext(ThemeContext.context)
  <div
    className={Css.merge(list{
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
    })}
  />
}
