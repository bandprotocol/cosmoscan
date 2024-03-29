type t;

[@bs.val] external from: array(int) => t = "Buffer.from";

[@bs.val] external byteLength: t => int = "Buffer.byteLength";

[@bs.val] external concat: array(t) => t = "Buffer.concat";

[@bs.val] external _from: (string, string) => t = "Buffer.from";

let fromHex = hexstr => hexstr->HexUtils.normalizeHexString->_from("hex");

let fromUTF8 = utf8 => _from(utf8, "utf-8");

let fromBase64 = hexstr => _from(hexstr, "base64");

[@bs.send] external _toString: (t, string) => string = "toString";

let toHex = (~with0x=false, buf) => (with0x ? "0x" : "") ++ buf->_toString("hex");

let toBase64 = buf => buf->_toString("base64");
let toUTF8 = buf => buf->_toString("UTF-8");

[@bs.val] external toArray: t => array(int) = "Array.from";

let base64ToHex = base64str => base64str->fromBase64->toHex;
let hexToBase64 = hexstr => hexstr->fromHex->toBase64;
let arrayToHex = arr => arr->from->toHex;
let hexToArray = hexstr => hexstr->fromHex->toArray;
let arrayToBase64 = arr => arr->from->toBase64;
let base64ToArray = base64str => base64str->fromBase64->toArray;
let hexToStringArray = hex =>
  hex->hexToArray->Js.Json.stringifyAny->Belt_Option.getWithDefault(hex);
