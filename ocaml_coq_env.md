<!-- Docker上にOCaml+Coqの環境を構築して動かしてみるまで -->

# はじめに
CoqをDocker上にインストールしたのでその手順をまとめます。
CoqのコードはOCamlに変換できるので、変換したコードをOCamlから呼び出してみます。
Dockerのインストールは済んでいる前提で進めていきます。

# OPAMのインストール
OCaml、CoqをインストールするためにまずOPAMをインストールします。
OPAMはOCamlのパッケージマネージャです。
OPAMをインストールするとOCamlは自動的に入りますが古いバージョンなので新しいバージョンを別にインストールします。

Alpine Linuxのコンテナを起動します。

```
$ docker run -it alpine
```

コンテナ内でOPAMをインストールします。

```
# apk update
# apk add alpine-sdk ncurses m4 opam
# opam --version
2.0.7
# ocaml --version
The OCaml toplevel, version 4.08.1
```

OPAMを初期化します。
DockerにOPAMを入れるときは'--disable-sandboxing'を付ける必要がありそうなので付けておきます。 https://github.com/ocaml/opam/issues/3498
'-y'は質問をすべてyesで答えて問題ないと思ったため付けています。対話的に選択したいなら外してください。

```
# opam init -y --disable-sandboxing
```

`eval $(opam env)`を実行するように表示されるので 実行します。

```
# eval $(opam env)
[WARNING] Running as root is not recommended
```

rootで実行しているので警告が出ますがこのまま進めます。

# OCamlのインストール

OPAMを使えばOCamlの複数のバージョンを切り替えることができます。
現在インストール済みのバージョンは`opam switch`で確認できます。

```
# opam switch
[WARNING] Running as root is not recommended
#   switch   compiler             description
->  default  ocaml-system.4.08.1  default
```

新しいバージョンをインストールします。(そこそこ時間がかかります)

```
# opam switch create 4.11.2
```

再び`eval $(opam env)`を実行します。

```
# eval $(opam env)
[WARNING] Running as root is not recommended
```

新しいバージョンがインストールされたことを確認します。

```
# opam switch
[WARNING] Running as root is not recommended
#   switch   compiler                    description
->  4.11.2   ocaml-base-compiler.4.11.2  4.11.2
    default  ocaml-system.4.08.1         default
```

# Coqのインストール

OPAMからCoqをインストールする方法は以下を参考にしましたので、同じように進めます。
参考：https://coq.inria.fr/opam-using.html

Coqのインストールには結構時間がかかります。

```
# opam install opam-depext
# opam-depext -y coq
# opam pin -y add coq 8.13.1
```

Coqがインストールされたことを確認します。

```
# coqc --version
The Coq Proof Assistant, version 8.13.1 (April 2021)
compiled on Apr 5 2021 10:33:10 with OCaml 4.11.2
```

# CoqのコードをOCamlに変換する

まずCoqのコードを用意します。
'id.v'という名前でファイルを作成し、関数idを定義します。
関数idは引数をそのまま返す関数です。

```id.v
Definition id (A:Set)(x:A) := x.

Require Extraction.
Extraction "id.ml" id.
```

簡単に説明をすると、1行目が関数idを定義しています。
2、3行目では関数idをOCamlに変換し'id.ml'のファイル名で保存しています。

'id.v'を`coqc`で実行します。

```
# coqc id.v
# ls
id.glob  id.mli   id.vo    id.vos
id.ml    id.v     id.vok
# cat id.ml

(** val id : 'a1 -> 'a1 **)

let id x =
  x
```

# 変換したコードをOCamlから呼び出す

OCamlのコードを用意します。
ファイル名は'hello\_coq.ml'とします。

```hello_coq.ml
let () =
  print_endline "Hello";;
  print_endline (Id.id "Coq!!");;
```

'id.ml'と'hello\_coq.ml'をコンパイルします。
mliファイルが存在する場合、一緒にコンパイルします。

```
# ocamlopt -c id.mli id.ml
# ocamlopt -c hello_coq.ml
# ocamlopt -o hello_coq id.cmx hell_coq.cmx
# ./hello_coq 
Hello
Coq!!
```

# おわりに
Coqをインストールして、OCamlから実行できるところまでできました。
ただし、Coqは定理証明支援システムなのでプログラムを証明するところからが本番です。
OCamlへの変換もCoqとOCamlでは数値やリストの型が異なるので、それぞれの変換方法を記述していく必要があります。
大変そうですがCoqと仲良くなりたいなと思います。

