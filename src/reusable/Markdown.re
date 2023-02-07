module Styles = {
  open Css;

  let container = (theme: Theme.t) =>
    style([
      selector(
        "a",
        [
          wordBreak(`breakAll),
          color(theme.neutral_900),
          textDecoration(`none),
          transition(~duration=200, "all"),
          hover([color(theme.baseBlue)]),
        ],
      ),
      selector("p, ul, ul > li, ol, ol > li", [color(theme.neutral_600), marginBottom(`em(1.))]),
      selector("p:last-child", [color(theme.neutral_600), marginBottom(`em(0.))]),
      selector(
        "h2, h3, h4, h5, h6",
        [color(theme.neutral_600), marginBottom(`px(10)), fontSize(`px(16))],
      ),
      selector("ul", [marginLeft(`em(1.2))]),
      selector("ol", [marginLeft(`em(2.0)), listStyleType(`decimal)]),
      selector(
        "ul > li",
        [
          fontSize(`px(14)),
          paddingLeft(`px(15)),
          position(`relative),
          lineHeight(`em(1.42)),
          before([
            contentRule(`text("\f105")),
            fontFamily(`custom("'Font Awesome 5 Pro'")),
            fontSize(`px(14)),
            lineHeight(`zero),
            display(`block),
            position(`absolute),
            left(`zero),
            top(`px(10)),
            pointerEvents(`none),
            color(theme.baseBlue),
          ]),
        ]
      ),
      selector(
        "ol > li",
        [
          fontSize(`px(14)),
          paddingLeft(`px(15)),
          position(`relative),
          lineHeight(`em(1.42)),
        ],
      ),
      selector(
        "ol > li::marker",
        [
          fontFamilies([`custom("Montserrat"), `custom("sans-serif")]),
          fontVariant(`inherit_)
        ],
      )
    ]);
};

[@react.component]
let make = (~value) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);
  <div className={Styles.container(theme)}> {value->MarkedJS.marked->MarkedJS.parse} </div>;
};
