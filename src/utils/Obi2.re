type flow_t =
  | Input
  | Output;

type field_key_type_t = {
  fieldName: string,
  fieldType: string,
};

type field_key_value_t = {
  fieldName: string,
  fieldValue: string,
};

let flowToString =
  fun
  | Input => "input"
  | Output => "output";

// TODO
let extractFields: (string, string) => option(array(field_key_type_t)) = [%bs.raw
  {|
  function(schema, t) {
    try {
      const normalizedSchema = schema.replace(/\s+/g, '')
      const tokens = normalizedSchema.split('/')
      let val
      if (t === 'input') {
        val = tokens[0]
      } else if (t === 'output') {
        val = tokens[1]
      } else {
        return undefined
      }
      let specs = val.slice(1, val.length - 1).split(',')
      return specs.map((spec) => {
        let x = spec.split(':')
        return {fieldName: x[0], fieldType: x[1]}
      })
    } catch {
      return undefined
    }
  }
|}
];

let encode = (schema, flow, valuePairs) => {
  BandChainJS.Obi.(
    switch (
      {
        let typePairs = extractFields(schema, flow->flowToString)->Belt.Option.getExn;
        let dataPairs =
          typePairs->Belt.Array.map(({fieldName, fieldType}) => {
            let value =
              valuePairs
              ->Belt.Array.keepMap(each =>
                  fieldName == each.fieldName ? Some(each.fieldValue) : None
                )
              ->Belt.Array.getExn(0);
            Js.log(value);

            // TODO: parse follow fieldType
            let parsed = value->Js.Json.parseExn;
            (fieldName, parsed);
          });
        let data = Js.Json.object_(Js.Dict.fromArray(dataPairs));
        let obi = create(schema);
        switch (flow) {
        | Input => obi->encodeInput(data)
        | Output => obi->encodeOutput(data)
        };
      }
    ) {
    | exception err =>
      Js.Console.error({j|Error encode $err|j}); // For debug
      None;
    | encoded => Some(encoded)
    }
  );
};
