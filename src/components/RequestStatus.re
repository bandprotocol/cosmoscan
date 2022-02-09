type display_t =
  | Full
  | Mini;

let toString =
  fun
  | RequestSub.Success => "Success"
  | Failure => "Failure"
  | Pending => "Pending"
  | Expired => "Expired"
  | Unknown => "Unknown";

[@react.component]
let make = (~resolveStatus, ~display=Mini, ~style="", ~size=Text.Lg) => {
  <div className={CssHelper.flexBox(~align=`center, ())}>
    {switch (resolveStatus) {
     | RequestSub.Success => <img alt="Success Icon" src=Images.success className=style />
     | Failure => <img alt="Fail Icon" src=Images.fail className=style />
     | Pending => <img alt="Pending Icon" src=Images.pending className=style />
     | Expired => <img alt="Expired Icon" src=Images.expired className=style />
     | Unknown => <img alt="Unknown Icon" src=Images.unknown className=style />
     }}
    {display == Full
       ? <> <HSpacing size=Spacing.sm /> <Text value={resolveStatus |> toString} size /> </>
       : React.null}
  </div>;
};
