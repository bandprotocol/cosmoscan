open Webapi.Dom;

// Sm = Small Mobile, Md = Mobile

type t =
  | Sm
  | Md
  | Lg;

let getBreakpoint =
  fun
  | Sm => 370
  | Md => 768
  | Lg => 1024;

let query = (size, styles) => {
  let breakpoint = getBreakpoint(size);
  Css.media("(max-width:" ++ string_of_int(breakpoint) ++ "px)", styles);
};

let getWindowWidth = () => window |> Window.innerWidth;

let useQuery = (~size, ()) => {
  let breakpoint = getBreakpoint(size);

  let (width, setWidth) = React.useState(_ => getWindowWidth());
  let handleWindowResize = (_: Dom.event) => {
    setWidth(_ => getWindowWidth());
  };

  React.useEffect0(() => {
    Window.addEventListener("resize", handleWindowResize, window);
    Some(() => Window.removeEventListener("resize", handleWindowResize, window));
  });

  width <= breakpoint;
};

let tablet = styles => query(Lg, styles);
let mobile = styles => query(Md, styles);
let smallMobile = styles => query(Sm, styles);

let isTablet = useQuery(~size=Lg);
let isMobile = useQuery(~size=Md);
let isSmallMobile = useQuery(~size=Sm);
