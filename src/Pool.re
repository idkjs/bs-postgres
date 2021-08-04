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
  "connectionTimeoutMillis": Js.undefined(int),
  "idleTimeoutMillis": Js.undefined(int),
  "max": Js.undefined(int),
};

module Internal = {
  [@bs.module "pg"] [@bs.new] external make: config => t = "Pool";

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
      ~connectionTimeoutMillis: int=?,
      ~idleTimeoutMillis: int=?,
      ~max: int=?,
      unit
    ) =>
    config;
};

module Pool_Client = {
  include Client;

  [@bs.send] external _release: t => unit = "release";

  let release = client => client |> _release |> Js.Promise.resolve;
};

module Callback = {
  [@bs.send]
  external connect:
    (
      t,
      (
        ~err: Js.nullable(Js.Exn.t),
        ~client: Pool_Client.t,
        ~release: unit => unit
      ) =>
      unit
    ) =>
    unit;

  [@bs.send]
  external query:
    (
      t,
      ~text: string,
      ~values: 'a=?,
      (~err: Js.nullable(Js.Exn.t), ~result: Result.t('b)) => unit
    ) =>
    unit;

  [@bs.send] external end_: (t, unit => unit) => unit = "end";

  [@bs.send]
  external on:
    (
      t,
      [@bs.string] [
        | `connect(Client.t => unit)
        | `acquire(Client.t => unit)
        | `error((Js.Exn.t, Client.t) => unit)
        | `remove(Client.t => unit)
      ]
    ) =>
    unit;
};

module Promise = {
  [@bs.send] external connect: t => Js.Promise.t(Pool_Client.t);

  [@bs.send.pipe: t]
  external query: (string, ~values: 'a=?) => Js.Promise.t(Result.t('b));

  [@bs.send] external end_: t => Js.Promise.t(unit) = "end";
};

[@bs.get] external totalCount: t => int;
[@bs.get] external idleCount: t => int;
[@bs.get] external waitingCount: t => int;

let make =
    (
      ~user=?,
      ~password=?,
      ~host=?,
      ~database=?,
      ~port=?,
      ~ssl=?,
      ~statement_timeout=?,
      ~connectionTimeoutMillis=?,
      ~idleTimeoutMillis=?,
      ~max=?,
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
    ~connectionTimeoutMillis?,
    ~idleTimeoutMillis?,
    ~max?,
    (),
  );
