[@react.component]
let make = (~onClick, ~disabled) => {
  <Button py=10 variant=Button.Outline onClick disabled> {"Max" |> React.string} </Button>;
};
