type alignment =
  | Start
  | Center
  | End
type column =
  | One
  | Two
  | Three
  | Four
  | Five
  | Six
  | Seven
  | Eight
  | Nine
  | Ten
  | Eleven
  | Twelve
module Styles = {
  open Css
  let colGridBase = style(list{
    flexGrow(0.),
    flexShrink(0.),
    flexBasis(#auto),
    padding2(~v=#zero, ~h=#px(12)),
    width(#percent(100.)),
  })
  let colGrid = x =>
    switch x {
    | One => style(list{maxWidth(#percent(8.333333)), flexBasis(#percent(8.333333))})
    | Two => style(list{maxWidth(#percent(16.666667)), flexBasis(#percent(16.666667))})
    | Three => style(list{maxWidth(#percent(25.)), flexBasis(#percent(25.))})
    | Four => style(list{maxWidth(#percent(33.333333)), flexBasis(#percent(33.333333))})
    | Five => style(list{maxWidth(#percent(41.666667)), flexBasis(#percent(41.666667))})
    | Six => style(list{maxWidth(#percent(50.)), flexBasis(#percent(50.))})
    | Seven => style(list{maxWidth(#percent(58.333333)), flexBasis(#percent(58.333333))})
    | Eight => style(list{maxWidth(#percent(66.666667)), flexBasis(#percent(66.666667))})
    | Nine => style(list{maxWidth(#percent(75.)), flexBasis(#percent(75.))})
    | Ten => style(list{maxWidth(#percent(83.333333)), flexBasis(#percent(83.333333))})
    | Eleven => style(list{maxWidth(#percent(91.666667)), flexBasis(#percent(91.666667))})
    | Twelve => style(list{maxWidth(#percent(100.)), flexBasis(#percent(100.))})
    }
  let colSmGrid = x =>
    switch x {
    | One =>
      style(list{Media.mobile(list{maxWidth(#percent(8.333333)), flexBasis(#percent(8.333333))})})
    | Two =>
      style(list{Media.mobile(list{maxWidth(#percent(16.666667)), flexBasis(#percent(16.666667))})})
    | Three => style(list{Media.mobile(list{maxWidth(#percent(25.)), flexBasis(#percent(25.))})})
    | Four =>
      style(list{Media.mobile(list{maxWidth(#percent(33.333333)), flexBasis(#percent(33.333333))})})
    | Five =>
      style(list{Media.mobile(list{maxWidth(#percent(41.666667)), flexBasis(#percent(41.666667))})})
    | Six => style(list{Media.mobile(list{maxWidth(#percent(50.)), flexBasis(#percent(50.))})})
    | Seven =>
      style(list{Media.mobile(list{maxWidth(#percent(58.333333)), flexBasis(#percent(58.333333))})})
    | Eight =>
      style(list{Media.mobile(list{maxWidth(#percent(66.666667)), flexBasis(#percent(66.666667))})})
    | Nine => style(list{Media.mobile(list{maxWidth(#percent(75.)), flexBasis(#percent(75.))})})
    | Ten =>
      style(list{Media.mobile(list{maxWidth(#percent(83.333333)), flexBasis(#percent(83.333333))})})
    | Eleven =>
      style(list{Media.mobile(list{maxWidth(#percent(91.666667)), flexBasis(#percent(91.666667))})})
    | Twelve => style(list{Media.mobile(list{maxWidth(#percent(100.)), flexBasis(#percent(100.))})})
    }
  let colOffset = x =>
    switch x {
    | One => style(list{marginLeft(#percent(8.333333))})
    | Two => style(list{marginLeft(#percent(16.666667))})
    | Three => style(list{marginLeft(#percent(25.))})
    | Four => style(list{marginLeft(#percent(33.333333))})
    | Five => style(list{marginLeft(#percent(41.666667))})
    | Six => style(list{marginLeft(#percent(50.))})
    | Seven => style(list{marginLeft(#percent(58.333333))})
    | Eight => style(list{marginLeft(#percent(66.666667))})
    | Nine => style(list{marginLeft(#percent(75.))})
    | Ten => style(list{marginLeft(#percent(83.333333))})
    | Eleven => style(list{marginLeft(#percent(91.666667))})
    | Twelve => style(list{})
    }

  let marginBottom = (~mb, ~mbSm, ()) =>
    style(list{marginBottom(#px(mb)), Media.mobile(list{marginBottom(#px(mbSm))})})
}
@react.component
let make = (~col=Twelve, ~colSm=Twelve, ~offset=Twelve, ~mb=0, ~mbSm=mb, ~style="", ~children) =>
  <div
    className={Css.merge(list{
      Styles.colGridBase,
      Styles.colGrid(col),
      Styles.colOffset(offset),
      Styles.colSmGrid(colSm),
      Styles.marginBottom(~mb, ~mbSm, ()),
      style,
    })}>
    children
  </div>
