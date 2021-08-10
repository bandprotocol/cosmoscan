type data_source_tab_t =
  | DataSourceExecute
  | DataSourceCode
  | DataSourceRequests
  | DataSourceRevisions

type oracle_script_tab_t =
  | OracleScriptExecute
  | OracleScriptCode
  | OracleScriptBridgeCode
  | OracleScriptRequests
  | OracleScriptRevisions

type account_tab_t =
  | AccountDelegations
  | AccountUnbonding
  | AccountRedelegate

type validator_tab_t =
  | ProposedBlocks
  | Delegators
  | Reports
  | Reporters

type t =
  | NotFound
  | HomePage
  | DataSourceHomePage
  | DataSourceIndexPage(int, data_source_tab_t)
  | OracleScriptHomePage
  | OracleScriptIndexPage(int, oracle_script_tab_t)
  | TxHomePage
  | TxIndexPage(Hash.t)
  | BlockHomePage
  | BlockIndexPage(int)
  | RequestHomePage
  | RequestIndexPage(int)
  | AccountIndexPage(Address.t, account_tab_t)
  | ValidatorHomePage
  | ValidatorIndexPage(Address.t, validator_tab_t)
  | ProposalHomePage
  | ProposalIndexPage(int)
  | IBCHomePage

let fromUrl = (url: ReasonReactRouter.url) =>
  // TODO: We'll handle the NotFound case for Datasources and Oraclescript later
  switch (url.path, url.hash) {
  | (list{"data-sources"}, _) => DataSourceHomePage
  | (list{"data-source", dataSourceID}, hash) =>
    let urlHash = x =>
      switch x {
      | "code" => DataSourceCode
      | "execute" => DataSourceExecute
      | "revisions" => DataSourceRevisions
      | _ => DataSourceRequests
      }
    switch dataSourceID |> int_of_string_opt {
    | Some(dataSourceIDInt) => DataSourceIndexPage(dataSourceIDInt, urlHash(hash))
    | None => NotFound
    }

  | (list{"oracle-scripts"}, _) => OracleScriptHomePage
  | (list{"oracle-script", oracleScriptID}, hash) =>
    let urlHash = x =>
      switch x {
      | "code" => OracleScriptCode
      | "bridge" => OracleScriptBridgeCode
      | "execute" => OracleScriptExecute
      | "revisions" => OracleScriptRequests
      | _ => OracleScriptRequests
      }
    switch oracleScriptID |> int_of_string_opt {
    | Some(oracleScriptIDInt) => OracleScriptIndexPage(oracleScriptIDInt, urlHash(hash))
    | None => NotFound
    }

  | (list{"txs"}, _) => TxHomePage
  | (list{"tx", txHash}, _) => TxIndexPage(Hash.fromHex(txHash))
  | (list{"validators"}, _) => ValidatorHomePage
  | (list{"blocks"}, _) => BlockHomePage
  | (list{"block", blockHeight}, _) =>
    let blockHeightIntOpt = blockHeight |> int_of_string_opt
    switch blockHeightIntOpt {
    | Some(block) => BlockIndexPage(block)
    | None => NotFound
    }

  | (list{"requests"}, _) => RequestHomePage
  | (list{"request", reqID}, _) => RequestIndexPage(reqID |> int_of_string)
  | (list{"account", address}, hash) =>
    let urlHash = x =>
      switch x {
      | "unbonding" => AccountUnbonding
      | "redelegate" => AccountRedelegate
      | _ => AccountDelegations
      }
    switch address |> Address.fromBech32Opt {
    | Some(address) => AccountIndexPage(address, urlHash(hash))
    | None => NotFound
    }
  | (list{"validator", address}, hash) =>
    let urlHash = x =>
      switch x {
      | "delegators" => Delegators
      | "reporters" => Reporters
      | "proposed-blocks" => ProposedBlocks
      | _ => Reports
      }
    switch address |> Address.fromBech32Opt {
    | Some(address) => ValidatorIndexPage(address, urlHash(hash))
    | None => NotFound
    }
  | (list{"proposals"}, _) => ProposalHomePage
  | (list{"proposal", proposalID}, _) => ProposalIndexPage(proposalID |> int_of_string)
  | (list{"ibcs"}, _) => IBCHomePage
  | (list{}, _) => HomePage
  | (_, _) => NotFound
  }

let toString = x =>
  switch x {
  | DataSourceHomePage => "/data-sources"
  | DataSourceIndexPage(dataSourceID, DataSourceRequests) => j`/data-source/$dataSourceID`
  | DataSourceIndexPage(dataSourceID, DataSourceCode) => j`/data-source/$dataSourceID#code`
  | DataSourceIndexPage(dataSourceID, DataSourceExecute) => j`/data-source/$dataSourceID#execute`
  | DataSourceIndexPage(
      dataSourceID,
      DataSourceRevisions,
    ) => j`/data-source/$dataSourceID#revisions`
  | OracleScriptHomePage => "/oracle-scripts"
  | OracleScriptIndexPage(oracleScriptID, OracleScriptRequests) => j`/oracle-script/$oracleScriptID`
  | OracleScriptIndexPage(
      oracleScriptID,
      OracleScriptCode,
    ) => j`/oracle-script/$oracleScriptID#code`
  | OracleScriptIndexPage(
      oracleScriptID,
      OracleScriptBridgeCode,
    ) => j`/oracle-script/$oracleScriptID#bridge`
  | OracleScriptIndexPage(
      oracleScriptID,
      OracleScriptExecute,
    ) => j`/oracle-script/$oracleScriptID#execute`
  | OracleScriptIndexPage(
      oracleScriptID,
      OracleScriptRevisions,
    ) => j`/oracle-script/$oracleScriptID#revisions`
  | TxHomePage => "/txs"
  | TxIndexPage(txHash) => j`/tx/$txHash`
  | ValidatorHomePage => "/validators"
  | BlockHomePage => "/blocks"
  | BlockIndexPage(height) => j`/block/$height`
  | RequestHomePage => "/requests"
  | RequestIndexPage(reqID) => j`/request/$reqID`
  | AccountIndexPage(address, AccountDelegations) =>
    let addressBech32 = address |> Address.toBech32
    j`/account/$addressBech32#delegations`
  | AccountIndexPage(address, AccountUnbonding) =>
    let addressBech32 = address |> Address.toBech32
    j`/account/$addressBech32#unbonding`
  | AccountIndexPage(address, AccountRedelegate) =>
    let addressBech32 = address |> Address.toBech32
    j`/account/$addressBech32#redelegate`
  | ValidatorIndexPage(validatorAddress, Delegators) =>
    let validatorAddressBech32 = validatorAddress |> Address.toOperatorBech32
    j`/validator/$validatorAddressBech32#delegators`
  | ValidatorIndexPage(validatorAddress, Reports) =>
    let validatorAddressBech32 = validatorAddress |> Address.toOperatorBech32
    j`/validator/$validatorAddressBech32#reports`
  | ValidatorIndexPage(validatorAddress, Reporters) =>
    let validatorAddressBech32 = validatorAddress |> Address.toOperatorBech32
    j`/validator/$validatorAddressBech32#reporters`
  | ValidatorIndexPage(validatorAddress, ProposedBlocks) =>
    let validatorAddressBech32 = validatorAddress |> Address.toOperatorBech32
    j`/validator/$validatorAddressBech32#proposed-blocks`
  | ProposalHomePage => "/proposals"
  | ProposalIndexPage(proposalID) => j`/proposal/$proposalID`
  | IBCHomePage => "/ibcs"
  | HomePage => "/"
  | NotFound => "/notfound"
  }

let redirect = (route: t) => ReasonReactRouter.push(route |> toString)

let search = (str: string) => {
  let len = str |> String.length
  let capStr = str |> String.capitalize_ascii

  switch str |> int_of_string_opt {
  | Some(blockID) => Some(BlockIndexPage(blockID))
  | None =>
    if str |> Js.String.startsWith("bandvaloper") {
      Some(ValidatorIndexPage(str |> Address.fromBech32, Reports))
    } else if str |> Js.String.startsWith("band") {
      Some(AccountIndexPage(str |> Address.fromBech32, AccountDelegations))
    } else if len == 64 || (str |> Js.String.startsWith("0x") && len == 66) {
      Some(TxIndexPage(str |> Hash.fromHex))
    } else if capStr |> Js.String.startsWith("B") {
      %Opt({
        let blockID = str |> String.sub(_, 1, len - 1) |> int_of_string_opt
        Some(BlockIndexPage(blockID))
      })
    } else if capStr |> Js.String.startsWith("D") {
      %Opt({
        let dataSourceID = str |> String.sub(_, 1, len - 1) |> int_of_string_opt
        Some(DataSourceIndexPage(dataSourceID, DataSourceRequests))
      })
    } else if capStr |> Js.String.startsWith("R") {
      %Opt({
        let requestID = str |> String.sub(_, 1, len - 1) |> int_of_string_opt
        Some(RequestIndexPage(requestID))
      })
    } else if capStr |> Js.String.startsWith("O") {
      %Opt({
        let oracleScriptID = str |> String.sub(_, 1, len - 1) |> int_of_string_opt
        Some(OracleScriptIndexPage(oracleScriptID, OracleScriptRequests))
      })
    } else {
      None
    }
  } |> Belt_Option.getWithDefault(_, NotFound)
}
