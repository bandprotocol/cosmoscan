type t = {
  usdPrice: float,
  usdMarketCap: float,
  usd24HrChange: float,
  btcPrice: float,
  btcMarketCap: float,
  circulatingSupply: float,
};

let getBandUsd24Change = () => {
  let coingeckoPromise =
    Axios.get(
      "https://api.coingecko.com/api/v3/simple/price?ids=band-protocol&vs_currencies=usd&include_market_cap=true&include_24hr_change=true",
    )
    |> Js.Promise.then_(result =>
         Promise.ret(
           result##data
           |> JsonUtils.Decode.at(["band-protocol", "usd_24h_change"], JsonUtils.Decode.float),
         )
       );

  let cryptocomparePromise =
    Axios.get("https://min-api.cryptocompare.com/data/pricemultifull?fsyms=BAND&tsyms=USD")
    |> Js.Promise.then_(result =>
         Promise.ret(
           result##data
           |> JsonUtils.Decode.at(
                ["RAW", "BAND", "USD", "CHANGEPCT24HOUR"],
                JsonUtils.Decode.float,
              ),
         )
       );

  Js.Promise.race([|coingeckoPromise, cryptocomparePromise|]);
};

let getCirculatingSupply = () => {
  let bandPromise =
    Axios.get("https://supply.bandchain.org/circulating")
    |> Js.Promise.then_(result => Promise.ret(result##data |> JsonUtils.Decode.float));

  let coingeckoPromise =
    Axios.get(
      "https://api.coingecko.com/api/v3/coins/band-protocol?tickers=false&community_data=false&developer_data=false&sparkline=false",
    )
    |> Js.Promise.then_(result =>
         Promise.ret(
           result##data
           |> JsonUtils.Decode.at(["market_data", "circulating_supply"], JsonUtils.Decode.float),
         )
       );

  Js.Promise.race([|bandPromise, coingeckoPromise|]);
};

let getPrices = () => {
  Axios.get(
    "https://lcd-lp.bandchain.org/oracle/v1/request_prices?ask_count=4&min_count=3&symbols=BAND&symbols=BTC",
  )
  |> Js.Promise.then_(result =>
       Promise.ret(
         {
           let bandUsdPrice =
             (result##data##price_results[0]##px |> JsonUtils.Decode.string |> float_of_string)
             /. (
               result##data##price_results[0]##multiplier
               |> JsonUtils.Decode.string
               |> float_of_string
             );

           let bandBTCPrice =
             (result##data##price_results[0]##px |> JsonUtils.Decode.string |> float_of_string)
             *. (
               result##data##price_results[1]##multiplier
               |> JsonUtils.Decode.string
               |> float_of_string
             )
             /. (
               (result##data##price_results[1]##px |> JsonUtils.Decode.string |> float_of_string)
               *. (
                 result##data##price_results[0]##multiplier
                 |> JsonUtils.Decode.string
                 |> float_of_string
               )
             );
           (bandUsdPrice, bandBTCPrice);
         },
       )
     );
};

let getBandInfo = client => {
  // let ratesPromise = client->BandChainJS.getReferenceData([|"BAND/USD", "BAND/BTC"|]);
  let ratesPromise = getPrices();
  let supplyPromise = getCirculatingSupply();
  let usd24HrChangePromise = getBandUsd24Change();

  let%Promise (rates, usd24HrChange, supply) =
    Js.Promise.all3((ratesPromise, usd24HrChangePromise, supplyPromise));

  let (bandUsd, bandBtc) = rates;

  let bandInfo = {
    usdPrice: bandUsd,
    usdMarketCap: bandUsd *. supply,
    usd24HrChange,
    btcPrice: bandBtc,
    btcMarketCap: bandBtc *. supply,
    circulatingSupply: supply,
  };

  bandInfo->Promise.ret;
};
