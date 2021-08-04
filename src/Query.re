type t('a);

[@bs.obj]
external make:
  (~text: string, ~values: 'a=?, ~name: string=?, ~rowMode: string=?, unit) =>
  t('b);
