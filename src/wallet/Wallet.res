type t =
  | Mnemonic(Mnemonic.t)
  | Ledger(Ledger.t)

let createFromMnemonic = mnemonic => Mnemonic(Mnemonic.create(mnemonic))

let createFromLedger = (ledgerApp, accountIndex) =>
  %Promise({
    let ledger = Ledger.create(ledgerApp, accountIndex)
    Promise.ret(Ledger(ledger))
  })

let getAddressAndPubKey = x =>
  switch x {
  | Mnemonic(x) => x |> Mnemonic.getAddressAndPubKey |> Promise.ret
  | Ledger(x) => x |> Ledger.getAddressAndPubKey
  }

let sign = (msg, x) =>
  switch x {
  | Mnemonic(x) => x |> Mnemonic.sign(_, msg) |> Promise.ret
  | Ledger(x) => x |> Ledger.sign(_, msg)
  }

let disconnect = x =>
  switch x {
  | Mnemonic(_) => ()
  | Ledger({transport}) => transport |> LedgerJS.close
  }
