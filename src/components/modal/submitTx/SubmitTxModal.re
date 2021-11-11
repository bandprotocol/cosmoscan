module Styles = {
  open Css;

  let container =
    style([
      flexDirection(`column),
      width(`px(468)),
      minHeight(`px(300)),
      height(`auto),
      padding(`px(32)),
      borderRadius(`px(5)),
      justifyContent(`flexStart),
    ]);

  let disable = isActive => style([display(isActive ? `flex : `none)]);

  let selectWrapper =
    style([
      display(`flex),
      flexDirection(`row),
      padding2(~v=`px(3), ~h=`px(8)),
      position(`static),
      width(`px(130)),
      height(`px(30)),
      left(`zero),
      top(`px(32)),
      background(rgba(255, 255, 255, `num(1.))),
      borderRadius(`px(100)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(4), ~blur=`px(4), rgba(0, 0, 0, `num(0.1)))),
      float(`left),
      fontSize(`px(14)),
    ]);

  let selectContent =
    style([
      background(rgba(255, 255, 255, `num(1.))),
      border(`px(0), `solid, hex("FFFFFF")),
      width(`px(135)),
      focus([outlineColor(Colors.white)]),
    ]);

  let nextBtn = style([width(`percent(100.)), marginTop(`px(24))]);

  let info = style([display(`flex), justifyContent(`spaceBetween), alignItems(`center)]);
};

module SubmitTxStep = {
  [@react.component]
  let make = (~account: AccountContext.t, ~setRawTx, ~isActive, ~msg) => {
    let client = React.useContext(ClientContext.context);
    let (msgsOpt, setMsgsOpt) = React.useState(_ => None);

    let gas = SubmitMsg.gasLimit(msg);
    let fee = 5000.;
    let (memo, setMemo) = React.useState(_ => EnhanceTxInput.{text: "", value: Some("")});

    <div className={Css.merge([Styles.container, Styles.disable(isActive)])}>
      <Heading value={SubmitMsg.toString(msg)} size=Heading.H4 marginBottom=24 />
      {switch (msg) {
       | SubmitMsg.Send(receiver, targetChain) =>
         <SendMsg address={account.address} receiver setMsgsOpt targetChain />
       | Delegate(validator) => <DelegateMsg address={account.address} validator setMsgsOpt />
       | Undelegate(validator) => <UndelegateMsg address={account.address} validator setMsgsOpt />
       | Redelegate(validator) => <RedelegateMsg address={account.address} validator setMsgsOpt />
       | WithdrawReward(validator) =>
         <WithdrawRewardMsg validator setMsgsOpt address={account.address} />
       | Reinvest(validator, amount) => <ReinvestMsg validator setMsgsOpt amount />
       | Vote(proposalID, proposalName) => <VoteMsg proposalID proposalName setMsgsOpt />
       }}
      <EnhanceTxInput
        width=300
        inputData=memo
        setInputData=setMemo
        parse={newVal => {
          newVal->Js.String.length <= 32 ? Result.Ok(newVal) : Err("Exceed limit length")
        }}
        msg="Memo (Optional)"
        placeholder="Memo"
        id="memoInput"
      />
      <SeperatedLine />
      <div className=Styles.info>
        <Text value="Transaction Fee" size=Text.Md weight=Text.Medium nowrap=true block=true />
        <Text value="0.005 BAND" />
      </div>
      <div id="nextButtonContainer">
        <Button
          style=Styles.nextBtn
          disabled={msgsOpt->Belt.Option.isNone}
          onClick={_ => {
            let rawTxOpt =
              {let%Opt memo' = memo.value;
               let%Opt msgs = msgsOpt;

               Some(
                 TxCreator2.createRawTx(
                   ~sender=account.address,
                   ~msgs,
                   ~chainID=account.chainID,
                   ~feeAmount=fee |> Js.Float.toString,
                   ~gas,
                   ~memo=memo',
                   ~client,
                   (),
                 ),
               )};
            let _ =
              switch (rawTxOpt) {
              | Some(rawTxPromise) =>
                let%Promise rawTx = rawTxPromise;
                setRawTx(_ => Some(rawTx));
                Promise.ret();
              | None =>
                Webapi.Dom.(window |> Window.alert("invalid msgs"));
                Promise.ret();
              };
            ();
          }}>
          {"Next" |> React.string}
        </Button>
      </div>
    </div>;
  };
};

module CreateTxFlow = {
  [@react.component]
  let make = (~account, ~msg) => {
    let (rawTx, setRawTx) = React.useState(_ => None);

    <>
      <SubmitTxStep account setRawTx isActive={rawTx->Belt.Option.isNone} msg />
      {switch (rawTx) {
       | None => React.null
       | Some(rawTx') =>
         <PreviewJsonStep rawTx=rawTx' onBack={_ => setRawTx(_ => None)} account />
       }}
    </>;
  };
};

[@react.component]
let make = (~msg) => {
  let (account, _) = React.useContext(AccountContext.context);

  switch (account) {
  | Some(account') => <CreateTxFlow account=account' msg />
  | None => <div className=Styles.container> <Text value="Please sign in" size=Text.Lg /> </div>
  };
};
