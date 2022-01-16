let create: array(TxQueryByBlockTimestamp.t) => string = [%bs.raw
  {|
    function(data) {
        let keys = Object.keys(data[0]);
        let result = '';
        result += keys.join(',');
        result += '\n';

        data.forEach(function(item){
            keys.forEach(function(key){
                result += item[key] + ',';
            })
            result += '\n';
        })
        return result;
    }
  |}
];
