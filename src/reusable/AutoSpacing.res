open Css

@react.component
let make = (~dir) =>
  switch dir {
  | "left" => <div className={style(list{marginLeft(#auto)})} />
  | "right" => <div className={style(list{marginRight(#auto)})} />
  | "top" => <div className={style(list{marginTop(#auto)})} />
  | "bottom" => <div className={style(list{marginBottom(#auto)})} />
  | _ => React.null
  }
