module Styles = {
  open Css;
};

[@react.component]
let make = (~value) => {
  let (searchTerm, setSearchTerm) = React.useState(_ => "");
  <Row marginTop=40>
    <Col col=Col.Six>
      <SearchInput placeholder="Search Port / Request ID / Channel" onChange=setSearchTerm />
    </Col>
  </Row>;
};
