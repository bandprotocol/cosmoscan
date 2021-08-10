module SyncStaus = {
  @react.component
  let make = () => {
    let (syncing, setSyncing) = React.useState(_ => false)
    let (_, dispatchModal) = React.useContext(ModalContext.context)
    let trackingSub = TrackingSub.use()

    // If database is syncing the state (when replayOffset = -2).
    React.useEffect2(() => {
      switch trackingSub {
      | Data({replayOffset}) if replayOffset != -2 && !syncing =>
        Syncing->OpenModal->dispatchModal
        setSyncing(_ => true)
      | _ => ()
      }
      None
    }, (trackingSub, syncing))

    React.null
  }
}

@react.component
let make = () => <SyncStaus />
