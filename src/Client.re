type t;
type config = {
  .
  "user": Js.undefined(string),
  "password": Js.undefined(string),
  "host": Js.undefined(string),
  "database": Js.undefined(string),
  "port": Js.undefined(int),
  "ssl":
    Js.undefined({
      .
      "rejectUnauthorized": bool,
      "ca": string,
      "key": string,
      "cert": string,
    }),
  "statement_timeout": Js.undefined(int),
};

module Internal = {
  [@bs.module "pg"] [@bs.new] external make: config => t = "Client";

  [@bs.obj]
  external makeConfig:
    (
      ~user: string=?,
      ~password: string=?,
      ~host: string=?,
      ~database: string=?,
      ~port: int=?,
      ~ssl: {
              .
              "rejectUnauthorized": bool,
              "ca": string,
              "key": string,
              "cert": string,
            }
              =?,
      ~statement_timeout: int=?,
      unit
    ) =>
    config;
};

module Callback = {
  [@bs.send] external connect: (t, Js.nullable(Js.Exn.t) => unit) => unit;

  [@bs.send]
  external query:
    (
      t,
      ~text: string,
      ~values: 'a=?,
      (~err: Js.nullable(Js.Exn.t), ~result: Result.t('b)) => unit
    ) =>
    unit;

  [@bs.send]
  external query':
    (
      t,
      Query.t('a),
      (Js.nullable(Js.Exn.t), Js.nullable(Result.t('a))) => unit
    ) =>
    unit =
    "query";

  [@bs.send]
  external end_: (t, Js.nullable(Js.Exn.t) => unit) => unit = "end";

  [@bs.send]
  external on:
    (
      t,
      [@bs.string] [
        | `error(Js.Exn.t => unit)
        | `end_(unit => unit)
        | `notification(
            {
              .
              "processId": int,
              "channel": string,
              "payload": Js.nullable(string),
            } =>
            unit,
          )
        | `notice(string => unit)
      ]
    ) =>
    unit;
};

module Promise = {
  [@bs.send] external connect: t => Js.Promise.t(unit);

  [@bs.send.pipe: t]
  external query: (string, ~values: 'a=?) => Js.Promise.t(Result.t('b));

  [@bs.send.pipe: t]
  external query': Query.t('a) => Js.Promise.t(Result.t('a)) = "query";

  [@bs.send] external end_: t => Js.Promise.t(unit) = "end";
};

let make =
    (
      ~user=?,
      ~password=?,
      ~host=?,
      ~database=?,
      ~port=?,
      ~ssl=?,
      ~statement_timeout=?,
      (),
    ) =>
  Internal.make @@
  Internal.makeConfig(
    ~user?,
    ~password?,
    ~host?,
    ~database?,
    ~port?,
    ~ssl?,
    ~statement_timeout?,
    (),
  );
