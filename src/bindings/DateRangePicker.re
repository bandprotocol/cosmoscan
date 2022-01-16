[@bs.obj]
external makeProps:
  (
    ~selected: Js.Date.t,
    ~placeholderText: string,
    ~onChange: 'a => unit,
    ~dateFormat: string,
    ~minDate: option(Js.Date.t),
    ~maxDate: option(Js.Date.t),
    ~startDate: option(Js.Date.t),
    ~endDate: option(Js.Date.t),
    ~className: string,
    ~calendarClassName: string,
    ~selectsStart: option(bool),
    ~selectsEnd: option(bool),
    unit
  ) =>
  _ =
  "";

[@bs.module "react-datepicker"]
external make:
  React.component({
    .
    "selected": Js.Date.t,
    "placeholderText": string,
    "onChange": 'a => unit,
    "dateFormat": string,
    "minDate": option(Js.Date.t),
    "maxDate": option(Js.Date.t),
    "startDate": option(Js.Date.t),
    "endDate": option(Js.Date.t),
    "className": string,
    "calendarClassName": string,
    "selectsStart": option(bool),
    "selectsEnd": option(bool),
  }) =
  "default";
