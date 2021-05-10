open Css;

type mode_t =
  | Day
  | Dark;

type color_t = Types.Color.t;

type t = {
  textPrimary: color_t,
  textSecondary: color_t,
  baseBlue: color_t,
  lightenBlue: color_t,
  lightBlue: color_t,
  darkBlue: color_t,
  darkenBlue: color_t,
  mainBg: color_t,
  secondaryBg: color_t,
  successColor: color_t,
  pendingColor: color_t,
  failColor: color_t,
  inputColor: color_t,
  activeColor: color_t,
  dropdownHover: color_t,
};

let get =
  fun
  | Day => {
      textPrimary: hex("303030"),
      textSecondary: hex("7D7D7D"),
      baseBlue: hex("4520E6"),
      lightenBlue: hex("B4A5F5"),
      lightBlue: hex("6A4CEB"),
      darkBlue: hex("3719B8"),
      darkenBlue: hex("29138A"),
      mainBg: hex("ffffff"),
      secondaryBg: hex("FCFCFC"),
      successColor: hex("5FD3C8"),
      pendingColor: hex("F4D23E"),
      activeColor: hex("5FD3C8"),
      failColor: hex("E84A4B"),
      inputColor: hex("2C2C2C"),
      dropdownHover: hex("EDEDED"),
    }
  | Dark => {
      textPrimary: hex("ffffff"),
      textSecondary: hex("9A9A9A"),
      baseBlue: hex("4520E6"),
      lightenBlue: hex("B4A5F5"),
      lightBlue: hex("6A4CEB"),
      darkBlue: hex("3719B8"),
      darkenBlue: hex("29138A"),
      mainBg: hex("000000"),
      secondaryBg: hex("1B1B1B"),
      successColor: hex("5FD3C8"),
      pendingColor: hex("F4D23E"),
      activeColor: hex("5FD3C8"),
      failColor: hex("E84A4B"),
      inputColor: hex("ffffff"),
      dropdownHover: hex("0F0F0F"),
    };
