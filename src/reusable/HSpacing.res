open Css

@react.component
let make = (~size) => <div className={style(list{paddingLeft(size)})} />
