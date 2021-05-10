open Css;

type mode_t =
  | Day
  | Dark;

type color_t = Types.Color.t;

type t = {
  textPrimary: color_t,
  textSecondaty: color_t,
  primaryColor: color_t,
  mainBg: color_t,
  secondaryBg: color_t,
  successColor: color_t,
  pendingColor: color_t,
  failColor: color_t,
  inputColor: color_t,
  activeColor: color_t,
};

let get =
  fun
  | Day => {
      textPrimary: hex("303030"),
      textSecondaty: hex("7D7D7D"),
      primaryColor: hex("4520E6"),
      mainBg: hex("fcfcfc"),
      secondaryBg: hex("fff"),
      successColor: hex("5FD3C8"),
      pendingColor: hex("F4D23E"),
      activeColor: hex("5FD3C8"),
      failColor: hex("E84A4B"),
      inputColor: hex("2C2C2C"),
    }
  | Dark => {
      textPrimary: hex("ffffff"),
      textSecondaty: hex("9A9A9A"),
      primaryColor: hex("4520E6"),
      mainBg: hex("000000"),
      secondaryBg: hex("1B1B1B"),
      successColor: hex("5FD3C8"),
      pendingColor: hex("F4D23E"),
      activeColor: hex("5FD3C8"),
      failColor: hex("E84A4B"),
      inputColor: hex("ffffff"),
    };
