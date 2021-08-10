type t = {
  address: string,
  amount: int,
}

let convert: t => 'a = %raw(`
function(data) {
  return {...data};
}
  `)

let request = (data: t) =>
  %Promise({
    let response = Axios.postData(Env.faucet, convert(data))
    Promise.ret(response)
  })
