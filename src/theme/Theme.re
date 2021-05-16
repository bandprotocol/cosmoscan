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
  textPrimary: color_t,
  textSecondary: color_t,
  mainBg: color_t,
  headerBg: color_t,
  secondaryBg: color_t,
  successColor: color_t,
  pendingColor: color_t,
  failColor: color_t,
  inputColor: color_t,
  activeColor: color_t,
  dropdownHover: color_t,
  tableRowBorderColor: color_t,
  secondaryTableBg: color_t,
  loadingBaseColor: color_t,
  loadingSecondaryColor: color_t,
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
      textPrimary: hex("303030"),
      textSecondary: hex("7D7D7D"),
      mainBg: hex("ffffff"),
      headerBg: hex("f5f5f5"),
      secondaryBg: hex("ffffff"),
      inputColor: hex("2C2C2C"),
      dropdownHover: hex("EDEDED"),
      tableRowBorderColor: hex("EDEDED"),
      secondaryTableBg: hex("F8FAFF"),
      loadingBaseColor: hex("F5F5F5"),
      loadingSecondaryColor: hex("B2B2B2"),
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
      textPrimary: hex("ffffff"),
      textSecondary: hex("9A9A9A"),
      mainBg: hex("000000"),
      headerBg: hex("1B1B1B"),
      secondaryBg: hex("1B1B1B"),
      inputColor: hex("ffffff"),
      dropdownHover: hex("0F0F0F"),
      tableRowBorderColor: hex("353535"),
      secondaryTableBg: hex("2C2C2C"),
      loadingBaseColor: hex("303030"),
      loadingSecondaryColor: hex("808080"),
    };
