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
let make = (~resolveStatus, ~display=Mini, ~style="") => {
  <div className={CssHelper.flexBox(~align=`center, ())}>
    {switch (resolveStatus) {
     | RequestSub.Success => <img alt="Success icon" src=Images.success className=style />
     | Failure => <img alt="Fail icon" src=Images.fail className=style />
     | Pending => <img alt="Pending icon" src=Images.pending className=style />
     | Expired => <img alt="Expired icon" src=Images.expired className=style />
     | Unknown => <img alt="Unknown icon" src=Images.unknown className=style />
     }}
    {display == Full
       ? <>
           <HSpacing size=Spacing.sm />
           <Text value={resolveStatus |> toString} size=Text.Lg />
         </>
       : React.null}
  </div>;
};
