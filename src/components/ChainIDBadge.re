module Styles = {
  open Css;

  let version = (theme: Theme.t, isDarkMode) =>
    style([
      display(`flex),
      borderRadius(`px(8)),
      border(`px(1), `solid, isDarkMode ? theme.neutral_100 : theme.neutral_600),
      backgroundColor(theme.neutral_000),
      padding2(~v=`px(8), ~h=`px(10)),
      minWidth(`px(153)),
      justifyContent(`spaceBetween),
      alignItems(`center),
      position(`relative),
      cursor(`pointer),
      zIndex(5),
      Media.mobile([padding2(~v=`px(5), ~h=`px(10))]),
      Media.smallMobile([minWidth(`px(90))]),
    ]);

  let dropdown = (show, theme: Theme.t, isDarkMode) =>
    style([
      position(`absolute),
      width(`percent(100.)),
      border(`px(1), `solid, isDarkMode ? theme.neutral_100 : theme.neutral_600),
      backgroundColor(theme.neutral_100),
      borderRadius(`px(8)),
      transition(~duration=200, "all"),
      top(`percent(110.)),
      left(`zero),
      height(`auto),
      opacity(show ? 1. : 0.),
      pointerEvents(show ? `auto : `none),
      overflow(`hidden),
      Media.mobile([top(`px(35))]),
    ]);

  let link = (theme: Theme.t) =>
    style([
      textDecoration(`none),
      backgroundColor(theme.neutral_100),
      display(`block),
      padding2(~v=`px(5), ~h=`px(10)),
      hover([backgroundColor(theme.neutral_100)]),
    ]);
  let buttonContainer = style([Media.mobile([width(`percent(100.))])]);
  let baseBtn =
    style([
      textAlign(`center),
      Media.mobile([flexGrow(0.), flexShrink(0.), flexBasis(`percent(50.))]),
    ]);

  let leftBtn = (state, theme: Theme.t, isDarkMode) => {
    style([
      borderTopRightRadius(`zero),
      borderBottomRightRadius(`zero),
      backgroundColor(state ? theme.neutral_900 : theme.neutral_000),
      color(state ? theme.neutral_100 : theme.neutral_900),
      hover([
        backgroundColor(state ? theme.neutral_900 : theme.neutral_100),
        color(state ? theme.neutral_100 : theme.neutral_900),
      ]),
    ]);
  };
  let rightBtn = (state, theme: Theme.t, isDarkMode) => {
    style([
      borderTopLeftRadius(`zero),
      borderBottomLeftRadius(`zero),
      color(state ? theme.neutral_900 : theme.neutral_100),
      backgroundColor(state ? theme.neutral_000 : theme.neutral_900),
      hover([
        backgroundColor(state ? theme.neutral_100 : theme.neutral_900),
        color(state ? theme.neutral_900 : theme.neutral_100),
      ]),
    ]);
  };
};


type chainID =
  | WenchangTestnet
  | WenchangMainnet
  | GuanYuDevnet
  | GuanYuTestnet
  | GuanYuPOA
  | GuanYuMainnet
  | LaoziTestnet
  | LaoziMainnet
  | LaoziPOA
  | Unknown;

let parseChainID =
  fun
  | "band-wenchang-testnet3" => WenchangTestnet
  | "band-wenchang-mainnet" => WenchangMainnet
  | "band-guanyu-devnet5"
  | "band-guanyu-devnet6"
  | "band-guanyu-devnet7"
  | "band-guanyu-devnet8"
  | "bandchain" => GuanYuDevnet
  | "band-guanyu-testnet1"
  | "band-guanyu-testnet2"
  | "band-guanyu-testnet3"
  | "band-guanyu-testnet4" => GuanYuTestnet
  | "band-guanyu-poa" => GuanYuPOA
  | "band-guanyu-mainnet" => GuanYuMainnet
  | "band-laozi-testnet1"
  | "band-laozi-testnet2"
  | "band-laozi-testnet3"
  | "band-laozi-testnet4"
  | "band-laozi-testnet5"
  | "band-laozi-testnet6" => LaoziTestnet
  | "laozi-mainnet" => LaoziMainnet
  | "band-laozi-poa" => LaoziPOA
  | _ => Unknown;

let getLink =
  fun
  | WenchangTestnet => "https://wenchang-testnet3.cosmoscan.io/"
  | WenchangMainnet => "https://wenchang-legacy.cosmoscan.io/"
  | GuanYuMainnet => "https://guanyu-legacy.cosmoscan.io/"
  | GuanYuDevnet => "https://guanyu-devnet.cosmoscan.io/"
  | GuanYuTestnet => "https://guanyu-testnet4.cosmoscan.io/"
  | GuanYuPOA => "https://guanyu-poa.cosmoscan.io/"
  | LaoziTestnet => "https://laozi-testnet6.cosmoscan.io/"
  | LaoziMainnet => "https://cosmoscan.io/"
  | LaoziPOA => "https://laozi-poa.cosmoscan.io/"
  | Unknown => "";

let getName =
  fun
  | WenchangTestnet => "wenchang-testnet"
  | WenchangMainnet => "wenchang-mainnet"
  | GuanYuDevnet => "guanyu-devnet"
  | GuanYuTestnet => "guanyu-testnet"
  | GuanYuPOA => "guanyu-poa"
  | GuanYuMainnet => "guanyu-mainnet"
  | LaoziTestnet => "laozi-testnet"
  | LaoziMainnet => "laozi-mainnet"
  | LaoziPOA => "laozi-poa"
  | Unknown => "unknown";

[@react.component]
let make = () =>
  {
    let trackingSub = TrackingSub.use();
    let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

    let%Sub tracking = trackingSub;
    let currentChainID = tracking.chainID->parseChainID;

    let networkNames = [|LaoziMainnet, LaoziTestnet|] -> Belt.Array.map(chainID => chainID->getName);
    
    let isMainnet = (currentChainID->getName) == "laozi-mainnet"
    let handleToggle = (value) => {
      Js.log(value)
      Js.log(currentChainID->getName)
      if(!value && isMainnet){
        ReasonReactRouter.push(LaoziMainnet |> getLink)
      }
    };

    <div className={Css.merge([CssHelper.flexBox(), Styles.buttonContainer])}>
      <AbsoluteLink href={isMainnet ? "" : getLink(LaoziMainnet)} >
        <Button
          px=16
          py=8
          variant=Button.Outline
          style={Css.merge([Styles.baseBtn, Styles.leftBtn(isMainnet, theme, isDarkMode)])}>
          {networkNames[0]  |> React.string}
        </Button>
      </AbsoluteLink>
      <AbsoluteLink href={isMainnet ? getLink(LaoziTestnet) : ""} >
        <Button
          px=16
          py=8
          variant=Button.Outline
          style={Css.merge([Styles.baseBtn, Styles.rightBtn(isMainnet, theme, isDarkMode)])}>
          {networkNames[1] |> React.string}
        </Button>
      </AbsoluteLink>
    </div>

    |> Sub.resolve;
  }
  |> Sub.default(
       _,
       {
         let width = Media.isSmallMobile() ? 80 : 153;
         <LoadingCensorBar width height=30 />;
       },
     );
