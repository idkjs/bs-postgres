type t('a) = {
  .
  "rows": array('a),
  "fields":
    array({
      .
      "name": string,
      "dataTypeID": int,
    }),
  "rowCount": int,
  "command": string,
};

let map = (f, x) => {
  "rows": Js.Array.map(f, x##rows),
  "fields": x##fields,
  "rowCount": x##rowCount,
  "command": x##command,
};
