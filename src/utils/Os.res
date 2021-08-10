let checkHID: unit => bool = %raw(`
function() {
  return !!navigator.hid;
}
`)

let isWindows: unit => bool = %raw(`
function() {
  let x = navigator.userAgent;
  if (x) {
    return !!x.match(/NT/)
  } else {
    return false
  }
}
  `)
