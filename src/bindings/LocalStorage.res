@scope("localStorage") @val external getItem: string => option<string> = "getItem"
@scope("localStorage") @val external setItem: (string, string) => unit = "setItem"
