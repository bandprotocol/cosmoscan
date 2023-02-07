module Styles = {
  open Css;

  let datePickerWrapper = (theme: Theme.t) =>
    style([
      important(fontFamily(`inherit_)),
      selector(" .react-datepicker__day--selected", [backgroundColor(theme.primary_600)]),
      selector(" .react-datepicker__day--in-range", [backgroundColor(theme.primary_200)]),
      selector(
        " .react-datepicker__day--in-selecting-range",
        [backgroundColor(theme.primary_200)],
      ),
      selector(" .react-datepicker__day--range-start", [backgroundColor(theme.primary_600)]),
      selector(
        " .react-datepicker__day--selecting-range-end",
        [backgroundColor(theme.primary_600)],
      ),
      selector(
        " .react-datepicker__header",
        [borderBottom(`zero, `solid, theme.neutral_100), fontFamily(`inherit_)],
      ),
      selector(" .react-datepicker", [fontFamily(`inherit_)]),
    ]);

  let container = style([position(`relative), paddingBottom(`px(24))]);
  let input = (theme: Theme.t) =>
    style([
      width(`percent(100.)),
      height(`px(37)),
      paddingLeft(`px(16)),
      paddingRight(`px(9)),
      borderRadius(`px(4)),
      fontSize(`px(14)),
      fontWeight(`light),
      border(`px(1), `solid, theme.neutral_100),
      backgroundColor(theme.neutral_100),
      outlineStyle(`none),
      color(theme.neutral_900),
      fontFamilies([`custom("Montserrat"), `custom("sans-serif")]),
    ]);
};

type input_t('a) = {
  text: string,
  value: option('a),
};

[@react.component]
let make =
    (
      ~msg,
      ~selected,
      ~minDate=?,
      ~maxDate=?,
      ~startDate=?,
      ~endDate=?,
      ~selectsStart=?,
      ~selectsEnd=?,
      ~onChange,
      ~placeholderText,
    ) => {
  let ({ThemeContext.theme}, _) = React.useContext(ThemeContext.context);

  <div className=Styles.container>
    <Heading
      value=msg
      size=Heading.H5
      marginBottom=8
      align=Heading.Left
      color={theme.neutral_600}
      weight=Heading.Regular
    />
    <div className={CssHelper.flexBox(~wrap=`nowrap, ())}>
      <DateRangePicker
        placeholderText
        onChange
        dateFormat="yyyy-dd-MM"
        minDate
        maxDate
        startDate
        selected
        endDate
        selectsStart
        selectsEnd
        className={Css.merge([Styles.input(theme)])}
        calendarClassName={Styles.datePickerWrapper(theme)}
      />
    </div>
  </div>;
};
