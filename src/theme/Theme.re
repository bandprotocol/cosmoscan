open Css;

type mode_t =
  | Day
  | Dark;

type color_t = Types.Color.t;

type t = {
  baseBlue: color_t,
  lightenBlue: color_t,
  lightBlue: color_t,
  darkBlue: color_t,
  darkenBlue: color_t,
  white: color_t,
  black: color_t,
  successColor: color_t,
  pendingColor: color_t,
  failColor: color_t,
  activeColor: color_t,
  secondaryTableBg: color_t,
  loadingBaseColor: color_t,
  loadingSecondaryColor: color_t,
  footer: color_t,
  warningColor: color_t,
  // new theme
  neutral_000: color_t,
  neutral_100: color_t,
  neutral_200: color_t,
  neutral_300: color_t,
  neutral_400: color_t,
  neutral_500: color_t,
  neutral_600: color_t,
  neutral_700: color_t,
  neutral_800: color_t,
  neutral_900: color_t,

};

// Default Colors

let baseBlue = hex("4520E6");
let lightenBlue = hex("B4A5F5");
let lightBlue = hex("6A4CEB");
let darkBlue = hex("3719B8");
let darkenBlue = hex("29138A");
let black = hex("000000");
let white = hex("ffffff");

let successColor = hex("5FD3C8");
let pendingColor = hex("F4D23E");
let activeColor = hex("5FD3C8");
let failColor = hex("E84A4B");
let warningColor = hex("ffa733");

let footer = hex("1400A5");

let get: mode_t => t =
  fun
  | Day => {
      baseBlue,
      lightenBlue,
      lightBlue,
      darkBlue,
      darkenBlue,
      white,
      black,
      successColor,
      pendingColor,
      activeColor,
      failColor,
      warningColor,
      secondaryTableBg: hex("F5F5F5"),
      loadingBaseColor: hex("F5F5F5"),
      loadingSecondaryColor: hex("B2B2B2"),
      footer,
      // new theme
      neutral_000: hex("FFFFFF"),
      neutral_100: hex("F3F4F6"),
      neutral_200: hex("E5E7EB"),
      neutral_300: hex("D1D5DB"),
      neutral_400: hex("B0B6BF"),
      neutral_500: hex("9096A2"),
      neutral_600: hex("6C7889"),
      neutral_700: hex("4A5568"),
      neutral_800: hex("323A43"),
      neutral_900: hex("202327"),
    }
  | Dark => {
      baseBlue,
      lightenBlue,
      lightBlue,
      darkBlue,
      darkenBlue,
      white,
      black,
      successColor,
      pendingColor,
      activeColor,
      failColor,
      warningColor,
      secondaryTableBg: hex("2C2C2C"),
      loadingBaseColor: hex("303030"),
      loadingSecondaryColor: hex("808080"),
      footer,

       // new theme
      neutral_000: hex("101214"),
      neutral_100: hex("1A1E23"),
      neutral_200: hex("21262C"),
      neutral_300: hex("293037"),
      neutral_400: hex("39424C"),
      neutral_500: hex("4E5A6E"),
      neutral_600: hex("7D8A9D"),
      neutral_700: hex("A0AEC0"),
      neutral_800: hex("CBD5E0"),
      neutral_900: hex("FFFFFF"),
    };
