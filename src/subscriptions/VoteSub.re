type block_t = {timestamp: MomentRe.Moment.t};
type validator_t = {
  moniker: string,
  operatorAddress: Address.t,
  identity: string,
};
type account_t = {
  address: Address.t,
  validator: option(validator_t),
};
type transaction_t = {
  hash: Hash.t,
  block: block_t,
};

type internal_t = {
  account: account_t,
  transactionOpt: option(transaction_t),
};

type t = {
  voter: Address.t,
  txHashOpt: option(Hash.t),
  timestampOpt: option(MomentRe.Moment.t),
  validator: option(validator_t),
};

let toExternal = ({account: {address, validator}, transactionOpt}) => {
  voter: address,
  txHashOpt: transactionOpt->Belt.Option.map(({hash}) => hash),
  timestampOpt: transactionOpt->Belt.Option.map(({block}) => block.timestamp),
  validator,
};

type vote_t =
  | Yes
  | No
  | NoWithVeto
  | Abstain;

let toString = (~withSpace=false) =>
  fun
  | Yes => "Yes"
  | No => "No"
  | NoWithVeto => withSpace ? "No With Veto" : "NoWithVeto"
  | Abstain => "Abstain";

type answer_vote_t = {
  validatorID: int,
  valPower: float,
  valVote: option(vote_t),
  delVotes: vote_t => float,
  proposalID: ID.Proposal.t,
};

type internal_vote_t = {
  yesVote: float,
  noVote: float,
  noWithVetoVote: float,
  abstainVote: float,
};

type result_val_t = {
  validatorID: int,
  validatorPower: float,
  validatorAns: option(vote_t),
  proposalID: ID.Proposal.t,
};

type vote_stat_t = {
  proposalID: ID.Proposal.t,
  totalYes: float,
  totalYesPercent: float,
  totalNo: float,
  totalNoPercent: float,
  totalNoWithVeto: float,
  totalNoWithVetoPercent: float,
  totalAbstain: float,
  totalAbstainPercent: float,
  total: float,
};

let getAnswer = json => {
  exception NoChoice(string);
  let answer = json |> GraphQLParser.jsonToStringExn;
  switch (answer) {
  | "Yes" => Yes
  | "No" => No
  | "NoWithVeto" => NoWithVeto
  | "Abstain" => Abstain
  | _ => raise(NoChoice("There is no choice"))
  };
};

module YesVoteConfig = [%graphql
  {|
    subscription Votes($limit: Int!, $offset: Int!, $proposalID: Int! ) {
      votes(limit: $limit, offset: $offset, where: {proposal_id: {_eq: $proposalID}, yes: {_gt: "0"}}, order_by: {transaction: {block_height: desc}}) @bsRecord {
        account @bsRecord {
          address @bsDecoder(fn:"Address.fromBech32")
          validator @bsRecord {
            moniker
            operatorAddress: operator_address @bsDecoder(fn: "Address.fromBech32")
            identity
          }
        }
        transactionOpt: transaction @bsRecord {
          hash @bsDecoder(fn: "GraphQLParser.hash")
          block @bsRecord {
            timestamp @bsDecoder(fn: "GraphQLParser.timestamp")
          }
        }
      }
    }
  |}
];

module NoVoteConfig = [%graphql
  {|
    subscription Votes($limit: Int!, $offset: Int!, $proposalID: Int!, ) {
      votes(limit: $limit, offset: $offset, where: {proposal_id: {_eq: $proposalID}, no: {_gt: "0"}}, order_by: {transaction: {block_height: desc}}) @bsRecord {
        account @bsRecord {
          address @bsDecoder(fn:"Address.fromBech32")
          validator @bsRecord {
            moniker
            operatorAddress: operator_address @bsDecoder(fn: "Address.fromBech32")
            identity
          }
        }
        transactionOpt: transaction @bsRecord {
          hash @bsDecoder(fn: "GraphQLParser.hash")
          block @bsRecord {
            timestamp @bsDecoder(fn: "GraphQLParser.timestamp")
          }
        }
      }
    }
  |}
];

module NoWithVetoVoteConfig = [%graphql
  {|
    subscription Votes($limit: Int!, $offset: Int!, $proposalID: Int!, ) {
      votes(limit: $limit, offset: $offset, where: {proposal_id: {_eq: $proposalID}, no_with_veto: {_gt: "0"}}, order_by: {transaction: {block_height: desc}}) @bsRecord {
        account @bsRecord {
          address @bsDecoder(fn:"Address.fromBech32")
          validator @bsRecord {
            moniker
            operatorAddress: operator_address @bsDecoder(fn: "Address.fromBech32")
            identity
          }
        }
        transactionOpt: transaction @bsRecord {
          hash @bsDecoder(fn: "GraphQLParser.hash")
          block @bsRecord {
            timestamp @bsDecoder(fn: "GraphQLParser.timestamp")
          }
        }
      }
    }
  |}
];

module AbstainVoteConfig = [%graphql
  {|
    subscription Votes($limit: Int!, $offset: Int!, $proposalID: Int!, ) {
      votes(limit: $limit, offset: $offset, where: {proposal_id: {_eq: $proposalID}, abstain: {_gt: "0"}}, order_by: {transaction: {block_height: desc}}) @bsRecord {
        account @bsRecord {
          address @bsDecoder(fn:"Address.fromBech32")
          validator @bsRecord {
            moniker
            operatorAddress: operator_address @bsDecoder(fn: "Address.fromBech32")
            identity
          }
        }
        transactionOpt: transaction @bsRecord {
          hash @bsDecoder(fn: "GraphQLParser.hash")
          block @bsRecord {
            timestamp @bsDecoder(fn: "GraphQLParser.timestamp")
          }
        }
      }
    }
  |}
];

module YesVoteCountConfig = [%graphql
  {|
    subscription DepositCount($proposalID: Int!) {
      votes_aggregate(where: {proposal_id: {_eq: $proposalID}, yes: {_gt: "0"}}) {
        aggregate {
          count
        }
      }
    }
  |}
];

module NoVoteCountConfig = [%graphql
  {|
    subscription DepositCount($proposalID: Int!) {
      votes_aggregate(where: {proposal_id: {_eq: $proposalID}, no: {_gt: "0"}}) {
        aggregate {
          count
        }
      }
    }
  |}
];

module NoWithVetoVoteCountConfig = [%graphql
  {|
    subscription DepositCount($proposalID: Int!) {
      votes_aggregate(where: {proposal_id: {_eq: $proposalID}, no_with_veto: {_gt: "0"}}) {
        aggregate {
          count
        }
      }
    }
  |}
];

module AbstainVoteCountConfig = [%graphql
  {|
    subscription DepositCount($proposalID: Int!) {
      votes_aggregate(where: {proposal_id: {_eq: $proposalID}, abstain: {_gt: "0"}}) {
        aggregate {
          count
        }
      }
    }
  |}
];

module ValidatorVoteByProposalIDConfig = [%graphql
  {|
    subscription ValidatorVoteByProposalID($proposalID: Int!) {
      validator_vote_proposals_view(where: {proposal_id: {_eq: $proposalID}}) @bsRecord {
        yesVote: yes_vote @bsDecoder(fn: "GraphQLParser.floatExn")
        noVote: no_vote @bsDecoder(fn: "GraphQLParser.floatExn")
        noWithVetoVote: no_with_veto_vote @bsDecoder(fn: "GraphQLParser.floatExn")
        abstainVote: abstain_vote @bsDecoder(fn: "GraphQLParser.floatExn")
      }
    }
  |}
];

module DelegatorVoteByProposalIDConfig = [%graphql
  {|
    subscription DelegatorVoteByProposalID($proposalID: Int!) {
      non_validator_vote_proposals_view(where: {proposal_id: {_eq: $proposalID}}) @bsRecord {
        yesVote: yes_vote @bsDecoder(fn: "GraphQLParser.floatExn")
        noVote: no_vote @bsDecoder(fn: "GraphQLParser.floatExn")
        noWithVetoVote: no_with_veto_vote @bsDecoder(fn: "GraphQLParser.floatExn")
        abstainVote: abstain_vote @bsDecoder(fn: "GraphQLParser.floatExn")
      }
    }
  |}
];

module ValidatorVotesConfig = [%graphql
  {|
    subscription ValidatorVoteByProposalID {
      validator_vote_proposals_view @bsRecord {
        yesVote: yes_vote @bsDecoder(fn: "GraphQLParser.floatExn")
        noVote: no_vote @bsDecoder(fn: "GraphQLParser.floatExn")
        noWithVetoVote: no_with_veto_vote @bsDecoder(fn: "GraphQLParser.floatExn")
        abstainVote: abstain_vote @bsDecoder(fn: "GraphQLParser.floatExn")
      }
    }
  |}
];

module DelegatorVotesConfig = [%graphql
  {|
    subscription DelegatorVoteByProposalID {
      non_validator_vote_proposals_view @bsRecord {
        yesVote: yes_vote @bsDecoder(fn: "GraphQLParser.floatExn")
        noVote: no_vote @bsDecoder(fn: "GraphQLParser.floatExn")
        noWithVetoVote: no_with_veto_vote @bsDecoder(fn: "GraphQLParser.floatExn")
        abstainVote: abstain_vote @bsDecoder(fn: "GraphQLParser.floatExn")
      }
    }
  |}
];

let getList = (proposalID, answer, ~page, ~pageSize, ()) => {
  let offset = (page - 1) * pageSize;

  let (result, _) =
    switch (answer) {
    | Yes =>
      ApolloHooks.useSubscription(
        YesVoteConfig.definition,
        ~variables=
          YesVoteConfig.makeVariables(
            ~proposalID=proposalID |> ID.Proposal.toInt,
            ~limit=pageSize,
            ~offset,
            (),
          ),
      )
    | No =>
      ApolloHooks.useSubscription(
        NoVoteConfig.definition,
        ~variables=
          NoVoteConfig.makeVariables(
            ~proposalID=proposalID |> ID.Proposal.toInt,
            ~limit=pageSize,
            ~offset,
            (),
          ),
      )
    | NoWithVeto =>
      ApolloHooks.useSubscription(
        NoWithVetoVoteConfig.definition,
        ~variables=
          NoWithVetoVoteConfig.makeVariables(
            ~proposalID=proposalID |> ID.Proposal.toInt,
            ~limit=pageSize,
            ~offset,
            (),
          ),
      )
    | Abstain =>
      ApolloHooks.useSubscription(
        AbstainVoteConfig.definition,
        ~variables=
          AbstainVoteConfig.makeVariables(
            ~proposalID=proposalID |> ID.Proposal.toInt,
            ~limit=pageSize,
            ~offset,
            (),
          ),
      )
    };

  result |> Sub.map(_, x => x##votes->Belt.Array.map(toExternal));
};

let count = (proposalID, answer) => {
  let (result, _) =
    switch (answer) {
    | Yes =>
      ApolloHooks.useSubscription(
        YesVoteCountConfig.definition,
        ~variables=
          YesVoteCountConfig.makeVariables(~proposalID=proposalID |> ID.Proposal.toInt, ()),
      )
    | No =>
      ApolloHooks.useSubscription(
        NoVoteCountConfig.definition,
        ~variables=
          NoVoteCountConfig.makeVariables(~proposalID=proposalID |> ID.Proposal.toInt, ()),
      )
    | NoWithVeto =>
      ApolloHooks.useSubscription(
        NoWithVetoVoteCountConfig.definition,
        ~variables=
          NoWithVetoVoteCountConfig.makeVariables(
            ~proposalID=proposalID |> ID.Proposal.toInt,
            (),
          ),
      )
    | Abstain =>
      ApolloHooks.useSubscription(
        AbstainVoteCountConfig.definition,
        ~variables=
          AbstainVoteCountConfig.makeVariables(~proposalID=proposalID |> ID.Proposal.toInt, ()),
      )
    };

  result |> Sub.map(_, x => x##votes_aggregate##aggregate |> Belt_Option.getExn |> (y => y##count));
};

let getVoteStatByProposalID = proposalID => {
  let (validatorVotes, _) =
    ApolloHooks.useSubscription(
      ValidatorVoteByProposalIDConfig.definition,
      ~variables=
        ValidatorVoteByProposalIDConfig.makeVariables(
          ~proposalID=proposalID |> ID.Proposal.toInt,
          (),
        ),
    );
  let (delegatorVotes, _) =
    ApolloHooks.useSubscription(
      DelegatorVoteByProposalIDConfig.definition,
      ~variables=
        DelegatorVoteByProposalIDConfig.makeVariables(
          ~proposalID=proposalID |> ID.Proposal.toInt,
          (),
        ),
    );

  let%Sub valVotes = validatorVotes;
  let%Sub delVotes = delegatorVotes;

  let validatorVotePower =
    valVotes##validator_vote_proposals_view
    ->Belt_Array.reduce(
        {yesVote: 0., noVote: 0., noWithVetoVote: 0., abstainVote: 0.},
        (vote, {yesVote, noVote, noWithVetoVote, abstainVote}) =>
        {
          yesVote: vote.yesVote +. yesVote,
          noVote: vote.noVote +. noVote,
          noWithVetoVote: vote.noWithVetoVote +. noWithVetoVote,
          abstainVote: vote.abstainVote +. abstainVote,
        }
      );

  let delegatorVotePower =
    delVotes##non_validator_vote_proposals_view
    ->Belt_Array.reduce(
        {yesVote: 0., noVote: 0., noWithVetoVote: 0., abstainVote: 0.},
        (vote, {yesVote, noVote, noWithVetoVote, abstainVote}) =>
        {
          yesVote: vote.yesVote +. yesVote,
          noVote: vote.noVote +. noVote,
          noWithVetoVote: vote.noWithVetoVote +. noWithVetoVote,
          abstainVote: vote.abstainVote +. abstainVote,
        }
      );

  let totalYesPower = validatorVotePower.yesVote +. delegatorVotePower.yesVote;
  let totalNoPower = validatorVotePower.noVote +. delegatorVotePower.noVote;
  let totalNoWithVetoPower =
    validatorVotePower.noWithVetoVote +. delegatorVotePower.noWithVetoVote;
  let totalAbstainPower = validatorVotePower.abstainVote +. delegatorVotePower.abstainVote;
  let totalPower = totalYesPower +. totalNoPower +. totalNoWithVetoPower +. totalAbstainPower;

  Sub.resolve({
    proposalID,
    totalYes: totalYesPower /. 1e6,
    totalYesPercent: totalPower == 0. ? 0. : totalYesPower /. totalPower *. 100.,
    totalNo: totalNoPower /. 1e6,
    totalNoPercent: totalPower == 0. ? 0. : totalNoPower /. totalPower *. 100.,
    totalNoWithVeto: totalNoWithVetoPower /. 1e6,
    totalNoWithVetoPercent: totalPower == 0. ? 0. : totalNoWithVetoPower /. totalPower *. 100.,
    totalAbstain: totalAbstainPower /. 1e6,
    totalAbstainPercent: totalPower == 0. ? 0. : totalAbstainPower /. totalPower *. 100.,
    total: totalPower /. 1e6,
  });
};
