let parse = (chainID, channel) =>
  switch (chainID, channel) {
  | ("consumer", "channel-25") => (Images.cosmosIBCIcon, "Consumer")
  | _ => (Images.unknownChain, "Unknown")
  };
