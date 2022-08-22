<!-- SwiftUIのProperty Wrappersの使い方 その２ -->

# StateObjectの使い方
`@State`は値型のプロパティに付加しますが参照型のクラスのプロパティには`@StateObject`を付加します。  
`@StateObject`を付加するクラスにはObservableObjectプロトコルに準拠させ、監視するプロパティには`@Published`を付加します
