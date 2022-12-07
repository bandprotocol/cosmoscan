module Changes = {
  type changes_t = {
    subspace: string,
    key: string,
    value: string,
  };

  let decode = json =>
    JsonUtils.Decode.{
      subspace: json |> at(["subspace"], string),
      key: json |> at(["key"], string),
      value: json |> at(["value"], string),
    };
};

module Content = {
  type plan_t = {
    name: string,
    time: MomentRe.Moment.t,
    height: int,
  };

  type t = {
    title: string,
    description: string,
    changes: option(array(Changes.changes_t)),
    plan: option(plan_t),
  };

  let decodeChanges = json => {
    let changes = json |> Js.Json.decodeArray |> Belt.Option.getExn;
    changes |> Belt_Array.map(_, Changes.decode);
  };

  let decodePlan = json => {
    JsonUtils.Decode.{
      name: json |> field("name", string),
      time: json |> field("time", moment),
      height: json |> field("height", int),
    };
  };

  let decode = (json: Js.Json.t) =>
    JsonUtils.Decode.{
      title: json |> field("title", string),
      description: json |> field("description", string),
      changes: json |> optional(field("changes", decodeChanges)),
      plan: json |> optional(field("plan", decodePlan)),
    };
};

type proposal_status_t =
  | Deposit
  | Voting
  | Passed
  | Rejected
  | Failed;

let parseProposalStatus = json => {
  exception NotFound(string);
  let status = json |> Js.Json.decodeString |> Belt_Option.getExn;
  switch (status) {
  | "DepositPeriod" => Deposit
  | "VotingPeriod" => Voting
  | "Passed" => Passed
  | "Rejected" => Rejected
  | "Failed" => Failed
  | _ => raise(NotFound("The proposal status is not existing"))
  };
};

type account_t = {address: Address.t};

type deposit_t = {amount: list(Coin.t)};

type internal_t = {
  id: ID.Proposal.t,
  title: string,
  status: proposal_status_t,
  description: string,
  contentOpt: option(Js.Json.t),
  submitTime: MomentRe.Moment.t,
  depositEndTime: MomentRe.Moment.t,
  votingStartTime: MomentRe.Moment.t,
  votingEndTime: MomentRe.Moment.t,
  accountOpt: option(account_t),
  proposalType: string,
  yes_vote: float,
  no_vote: float,
  no_with_veto_vote: float,
  abstain_vote: float,
  total_bonded_tokens: float,
  totalDeposit: list(Coin.t),
  
};

type t = {
  id: ID.Proposal.t,
  name: string,
  status: proposal_status_t,
  description: string,
  content: option(Content.t),
  submitTime: MomentRe.Moment.t,
  depositEndTime: MomentRe.Moment.t,
  votingStartTime: MomentRe.Moment.t,
  votingEndTime: MomentRe.Moment.t,
  proposerAddressOpt: option(Address.t),
  proposalType: string,
  endTotalYes: float,
  endTotalYesPercent: float,
  endTotalNo: float,
  endTotalNoPercent: float,
  endTotalNoWithVeto: float,
  endTotalNoWithVetoPercent: float,
  endTotalAbstain: float,
  endTotalAbstainPercent: float,
  endTotalVote: float,
  totalBondedTokens: float,
  totalDeposit: list(Coin.t),
};

let toExternal =
    (
      {
        id,
        title,
        status,
        description,
        contentOpt,
        submitTime,
        depositEndTime,
        votingStartTime,
        votingEndTime,
        accountOpt,
        proposalType,
        yes_vote,
        no_vote,
        no_with_veto_vote,
        abstain_vote,
        total_bonded_tokens,
        totalDeposit,
      },
    ) => {
  id,
  name: title,
  status,
  description,
  content: {
    let%Opt content = contentOpt;
    Some(content |> Content.decode);
  },
  submitTime,
  depositEndTime,
  votingStartTime,
  votingEndTime,
  proposerAddressOpt: accountOpt->Belt.Option.map(({address}) => address),
  proposalType,
  endTotalYes: yes_vote /. 1e6,
  endTotalYesPercent: yes_vote /. (yes_vote +. no_vote +. no_with_veto_vote +. abstain_vote) *. 100.,
  endTotalNo: no_vote /. 1e6,
  endTotalNoPercent: no_vote /. (yes_vote +. no_vote +. no_with_veto_vote +. abstain_vote) *. 100.,
  endTotalNoWithVeto: no_with_veto_vote /. 1e6,
  endTotalNoWithVetoPercent: no_with_veto_vote /. (yes_vote +. no_vote +. no_with_veto_vote +. abstain_vote) *. 100.,
  endTotalAbstain: abstain_vote /. 1e6,
  endTotalAbstainPercent: abstain_vote /. (yes_vote +. no_vote +. no_with_veto_vote +. abstain_vote) *. 100.,
  endTotalVote: (yes_vote +. no_vote +. no_with_veto_vote +. abstain_vote) /. 1e6,
  totalBondedTokens: total_bonded_tokens /. 1e6,
  totalDeposit,
};

module MultiConfig = [%graphql
  {|
  subscription Proposals($limit: Int!, $offset: Int!) {
    proposals(limit: $limit, offset: $offset, order_by: {id: desc}, where: {status: {_neq: "Inactive"}}) @bsRecord {
      id @bsDecoder(fn: "ID.Proposal.fromInt")
      title
      status @bsDecoder(fn: "parseProposalStatus")
      description
      contentOpt: content
      submitTime: submit_time @bsDecoder(fn: "GraphQLParser.timestamp")
      depositEndTime: deposit_end_time @bsDecoder(fn: "GraphQLParser.timestamp")
      votingStartTime: voting_time @bsDecoder(fn: "GraphQLParser.timestamp")
      votingEndTime: voting_end_time @bsDecoder(fn: "GraphQLParser.timestamp")
      proposalType: type
      accountOpt: account @bsRecord {
        address @bsDecoder(fn: "Address.fromBech32")
      }
      yes_vote @bsDecoder(fn: "GraphQLParser.floatExn")
      no_vote @bsDecoder(fn: "GraphQLParser.floatExn")
      no_with_veto_vote @bsDecoder(fn: "GraphQLParser.floatExn")
      abstain_vote @bsDecoder(fn: "GraphQLParser.floatExn")
      total_bonded_tokens @bsDecoder(fn: "GraphQLParser.floatExn")
      totalDeposit: total_deposit @bsDecoder(fn: "GraphQLParser.coins")
    }
  }
|}
];

module SingleConfig = [%graphql
  {|
  subscription Proposal($id: Int!) {
    proposals_by_pk(id: $id) @bsRecord {
      id @bsDecoder(fn: "ID.Proposal.fromInt")
      title
      status @bsDecoder(fn: "parseProposalStatus")
      description
      contentOpt: content
      submitTime: submit_time @bsDecoder(fn: "GraphQLParser.timestamp")
      depositEndTime: deposit_end_time @bsDecoder(fn: "GraphQLParser.timestamp")
      votingStartTime: voting_time @bsDecoder(fn: "GraphQLParser.timestamp")
      votingEndTime: voting_end_time @bsDecoder(fn: "GraphQLParser.timestamp")
      proposalType: type
      accountOpt: account @bsRecord {
          address @bsDecoder(fn: "Address.fromBech32")
      }
      yes_vote @bsDecoder(fn: "GraphQLParser.floatExn")
      no_vote @bsDecoder(fn: "GraphQLParser.floatExn")
      no_with_veto_vote @bsDecoder(fn: "GraphQLParser.floatExn")
      abstain_vote @bsDecoder(fn: "GraphQLParser.floatExn")
      total_bonded_tokens @bsDecoder(fn: "GraphQLParser.floatExn")
      totalDeposit: total_deposit @bsDecoder(fn: "GraphQLParser.coins")
    }
  }
|}
];

module ProposalsCountConfig = [%graphql
  {|
  subscription ProposalsCount {
    proposals_aggregate{
      aggregate{
        count
      }
    }
  }
|}
];

let getList = (~page, ~pageSize, ()) => {
  let offset = (page - 1) * pageSize;
  let (result, _) =
    ApolloHooks.useSubscription(
      MultiConfig.definition,
      ~variables=MultiConfig.makeVariables(~limit=pageSize, ~offset, ()),
    );
  result |> Sub.map(_, internal => internal##proposals->Belt_Array.map(toExternal));
};

let get = id => {
  let (result, _) =
    ApolloHooks.useSubscription(
      SingleConfig.definition,
      ~variables=SingleConfig.makeVariables(~id=id |> ID.Proposal.toInt, ()),
    );

  let%Sub x = result;
  switch (x##proposals_by_pk) {
  | Some(data) => Sub.resolve(data |> toExternal)
  | None => NoData
  };
};

let count = () => {
  let (result, _) = ApolloHooks.useSubscription(ProposalsCountConfig.definition);
  result
  |> Sub.map(_, x => x##proposals_aggregate##aggregate |> Belt_Option.getExn |> (y => y##count));
};
