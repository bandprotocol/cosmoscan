module Styles = {
  open Css;

  let version = (theme: Theme.t, isDarkMode) =>
    style([
      display(`flex),
      borderRadius(`px(8)),
      border(`px(1), `solid, isDarkMode ? theme.secondaryBg : theme.textSecondary),
      backgroundColor(theme.secondaryBg),
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
      border(`px(1), `solid, isDarkMode ? theme.secondaryBg : theme.textSecondary),
      backgroundColor(theme.secondaryBg),
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
      backgroundColor(theme.secondaryBg),
      display(`block),
      padding2(~v=`px(5), ~h=`px(10)),
      hover([backgroundColor(theme.dropdownHover)]),
    ]);
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
  | "band-laozi-testnet2" => LaoziTestnet
  | "band-laozi-mainnet" => LaoziMainnet
  | "band-laozi-poa" => LaoziPOA
  | _ => Unknown;

let getLink =
  fun
  | WenchangTestnet => "https://wenchang-testnet3.cosmoscan.io/"
  | WenchangMainnet => "https://wenchang-legacy.cosmoscan.io/"
  | GuanYuMainnet => "https://cosmoscan.io/"
  | GuanYuDevnet => "https://guanyu-devnet.cosmoscan.io/"
  | GuanYuTestnet => "https://guanyu-testnet3.cosmoscan.io/"
  | GuanYuPOA => "https://guanyu-poa.cosmoscan.io/"
  | LaoziTestnet => "https://laozi-testnet2.cosmoscan.io/"
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
    let (show, setShow) = React.useState(_ => false);
    let trackingSub = TrackingSub.use();
    let ({ThemeContext.theme, isDarkMode}, _) = React.useContext(ThemeContext.context);

    let%Sub tracking = trackingSub;
    let currentChainID = tracking.chainID->parseChainID;

    <div
      className={Styles.version(theme, isDarkMode)}
      onClick={event => {
        setShow(oldVal => !oldVal);
        ReactEvent.Mouse.stopPropagation(event);
      }}>
      <Text
        value={currentChainID->getName}
        color={theme.textPrimary}
        nowrap=true
        weight=Text.Semibold
      />
      <HSpacing size=Spacing.sm />
      {show
         ? <Icon name="far fa-angle-up" color={theme.textSecondary} />
         : <Icon name="far fa-angle-down" color={theme.textSecondary} />}
      <div className={Styles.dropdown(show, theme, isDarkMode)}>
        {[|GuanYuMainnet, GuanYuTestnet, LaoziTestnet, LaoziPOA|]
         ->Belt.Array.keep(chainID => chainID != currentChainID)
         ->Belt.Array.map(chainID => {
             let name = chainID->getName;
             <AbsoluteLink href={getLink(chainID)} key=name className={Styles.link(theme)}>
               <Text value=name color={theme.textSecondary} nowrap=true weight=Text.Semibold />
             </AbsoluteLink>;
           })
         ->React.array}
      </div>
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
