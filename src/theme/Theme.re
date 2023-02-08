open Css;

type mode_t =
  | Day
  | Dark;

type color_t = Types.Color.t;

type t = {
  primary_600: color_t,
  primary_200: color_t,
  primary_500: color_t,
  darkBlue: color_t,
  primary_800: color_t,
  white: color_t,
  black: color_t,
  successColor: color_t,
  pendingColor: color_t,
  failColor: color_t,
  activeColor: color_t,
  warningColor: color_t,
  footer: color_t,
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
let primary_200 = hex("DAD1FE");
let primary_500 = hex("6547EB");
let primary = hex("4520E6");
let darkBlue = hex("3719B8");
let primary_800 = hex("210D77");

let black = hex("000000");
let white = hex("ffffff");

let successColor = hex("3FCA7E");
let pendingColor = hex("F4D23E");
let activeColor = hex("5FD3C8");
let failColor = hex("E22E1D");
let warningColor = hex("F2CC21");


let get: mode_t => t =
  fun
  | Day => {
      primary_600: hex("4520E6"),
      primary_200,
      primary_500,
      darkBlue,
      primary_800,
      white,
      black,
      successColor,
      pendingColor,
      activeColor,
      failColor,
      warningColor,
      footer: hex("4520E6"),
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
      primary_600: hex("8871EF"),
      primary_200,
      primary_500,
      darkBlue,
      primary_800,
      white,
      black,
      successColor,
      pendingColor,
      activeColor,
      failColor,
      warningColor,
      footer: hex("21262C"),

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
