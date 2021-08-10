open Css

@react.component
let make = (~size) => <div className={style(list{paddingTop(size)})} />
