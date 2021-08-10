let getPageCount = (amount, limit) =>
  if mod(amount, limit) == 0 {
    amount / limit
  } else {
    amount / limit + 1
  }
