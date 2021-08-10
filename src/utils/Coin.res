type t = {
  denom: string,
  amount: float,
}

let decodeCoin = json => {
  open JsonUtils.Decode
  {
    denom: json |> field("denom", string),
    amount: json |> field("amount", uamount),
  }
}

let newUBANDFromAmount = amount => {denom: "uband", amount: amount}

let newCoin = (denom, amount) => {denom: denom, amount: amount}

let getBandAmountFromCoin = coin => coin.amount /. 1e6

let getBandAmountFromCoins = coins =>
  coins
  ->Belt_List.keep(coin => coin.denom == "uband")
  ->Belt_List.get(0)
  ->Belt_Option.mapWithDefault(0., getBandAmountFromCoin)

let getUBandAmountFromCoin = coin => coin.amount

let getUBandAmountFromCoins = coins =>
  coins
  ->Belt_List.keep(coin => coin.denom == "uband")
  ->Belt_List.get(0)
  ->Belt_Option.mapWithDefault(0., getUBandAmountFromCoin)
