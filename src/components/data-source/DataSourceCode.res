module Styles = {
  open Css

  let tableLowerContainer = style(list{
    padding(#px(20)),
    position(#relative),
    Media.mobile(list{padding2(~v=#px(20), ~h=#zero)}),
  })

  let scriptContainer = style(list{
    fontSize(#px(12)),
    lineHeight(#px(20)),
    fontFamilies(list{
      #custom("IBM Plex Mono"),
      #custom("cousine"),
      #custom("sfmono-regular"),
      #custom("Consolas"),
      #custom("Menlo"),
      #custom("liberation mono"),
      #custom("ubuntu mono"),
      #custom("Courier"),
      #monospace,
    }),
  })

  let padding = style(list{padding(#px(20))})
  let copyContainer = style(list{
    position(#absolute),
    top(#px(30)),
    right(#px(30)),
    zIndex(2),
    Media.mobile(list{
      position(#static),
      display(#flex),
      justifyContent(#flexEnd),
      marginBottom(#px(8)),
    }),
  })
}

let renderCode = content =>
  <div className=Styles.scriptContainer>
    <ReactHighlight className=Styles.padding> {content |> React.string} </ReactHighlight>
  </div>

@react.component
let make = (~executable) => {
  let code = executable |> JsBuffer.toUTF8
  React.useMemo1(
    () =>
      <div className=Styles.tableLowerContainer>
        <div
          className={Css.merge(list{
            CssHelper.flexBox(~justify=#flexEnd, ()),
            CssHelper.mb(~size=8, ()),
          })}>
          <CopyButton data=code title="Copy Code" />
        </div>
        {code |> renderCode}
      </div>,
    [],
  )
}
