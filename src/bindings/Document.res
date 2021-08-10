@scope("document") @val
external addKeyboardEventListener: (string, ReactEvent.Keyboard.t => unit) => unit =
  "addEventListener"

@scope("document") @val
external removeKeyboardEventListener: (string, ReactEvent.Keyboard.t => unit) => unit =
  "removeEventListener"
