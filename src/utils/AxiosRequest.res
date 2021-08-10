@deriving(abstract)
type t = {
  executable: string,
  calldata: string,
  timeout: int,
}

/* TODO: FIX THIS MESS */
let convert: t => 'a = %raw(`
function(data) {
  return {...data};
}
  `)

let execute = (data: t) =>
  %Promise({
    let response = Axios.postData(Env.lambda, convert(data))
    Promise.ret(response)
  })
