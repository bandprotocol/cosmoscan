type direction_t =
  | Stretch
  | Start
  | Center
  | Between
  | End

module Styles = {
  open Css

  let justify = x =>
    switch x {
    | Start => style(list{justifyContent(#flexStart)})
    | Center => style(list{justifyContent(#center)})
    | Between => style(list{justifyContent(#spaceBetween)})
    | End => style(list{justifyContent(#flexEnd)})
    | _ => style(list{justifyContent(#flexStart)})
    }

  let alignItems = x =>
    switch x {
    | Stretch => style(list{alignItems(#stretch)})
    | Start => style(list{alignItems(#flexStart)})
    | Center => style(list{alignItems(#center)})
    | End => style(list{alignItems(#flexEnd)})
    | _ => style(list{alignItems(#stretch)})
    }

  let wrap = style(list{flexWrap(#wrap)})

  let minHeight = mh => style(list{minHeight(mh)})
  let rowBase = style(list{display(#flex), margin2(~v=#zero, ~h=#px(-12))})

  let mb = size => style(list{marginBottom(#px(size))})
  let mbSm = size => style(list{Media.mobile(list{marginBottom(#px(size))})})
  let mt = size => style(list{marginTop(#px(size))})
  let mtSm = size => style(list{Media.mobile(list{marginTop(#px(size))})})
}

@react.component
let make = (
  ~justify=Start,
  ~alignItems=Stretch,
  ~minHeight=#auto,
  ~wrap=true,
  ~style="",
  ~children,
  ~marginBottom=0,
  ~marginBottomSm=marginBottom,
  ~marginTop=0,
  ~marginTopSm=marginTop,
) =>
  <div
    className={Css.merge(list{
      Styles.rowBase,
      Styles.justify(justify),
      Styles.minHeight(minHeight),
      Styles.alignItems(alignItems),
      Styles.mb(marginBottom),
      Styles.mbSm(marginBottomSm),
      Styles.mt(marginTop),
      Styles.mtSm(marginTopSm),
      wrap ? Styles.wrap : "",
      style,
    })}>
    children
  </div>
