<!-- SwiftUIのProperty Wrappersの使い方 その２ -->

# StateObjectの使い方
`@State`は値型のプロパティに付加しますが参照型のクラスのプロパティには`@StateObject`を付加します。  
`@StateObject`を付加するクラスにはObservableObjectプロトコルに準拠させ、監視するプロパティには`@Published`を付加します。  
`@Published`を付加した値をBindingに渡したいときには`@StateObject`を付加した変数の前に`$`を付けます。

```swift
import SwiftUI

struct StateObjectContentView: View {
    @StateObject var value = StateObjectValue(value: 0)
    
    var body: some View {
        VStack {
            HStack {
                Text("参照と更新")
                    .padding()
                Button(action: {
                    self.value.value += 1
                }) {
                    Text(String(self.value.value))
                        .padding()
                }
            }
            HStack {
                Text("@Bindingに渡す")
                    .padding()
                Picker("stateValue", selection: self.$value.value) {
                    ForEach(0...99, id: \.self) { v in
                        Text(String(v))
                    }
                }
            }
        }
        .frame(maxHeight: .infinity, alignment: .top)
    }
    
    class StateObjectValue: ObservableObject {
        @Published var value: Int
        
        init(value: Int) {
            self.value = value
        }
    }
}

struct StateObjectContentView_Previews: PreviewProvider {
    static var previews: some View {
        StateObjectContentView()
    }
}
```

![スクリーンショット 2022-08-24 7.15.10.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/1281693/0cfc938f-ee87-fa42-86de-6c90b7103164.png)

# ObservedObjectの使い方
`@Binding`のように外のViewからクラスを受け取りたいときには`@ObservedObject`を付加します。  
`@Published`を付加した値をBinding引数に渡したいときには`@ObservedObject`を付加した変数の前に`$`を付けます。

```swift
import SwiftUI

struct ObservedObjectContentView: View {
    @StateObject var value = ObservableObjectValue(value: 0)
    
    var body: some View {
        ObservedObjectView(value: self.value)
    }
    
    class ObservableObjectValue: ObservableObject {
        @Published var value: Int
        
        init(value: Int) {
            self.value = value
        }
    }
    
    struct ObservedObjectView: View {
        @ObservedObject var value: ObservableObjectValue
        
        var body: some View {
            VStack {
                HStack {
                    Text("参照と更新")
                        .padding()
                    Button(action: {
                        self.value.value += 1
                    }) {
                        Text(String(self.value.value))
                            .padding()
                    }
                }
                HStack {
                    Text("@Bindingに渡す")
                        .padding()
                    Picker("stateValue", selection: self.$value.value) {
                        ForEach(0...99, id: \.self) { v in
                            Text(String(v))
                        }
                    }
                }
            }
            .frame(maxHeight: .infinity, alignment: .top)
        }
    }
}

struct ObservedObjectContentView_Previews: PreviewProvider {
    static var previews: some View {
        ObservedObjectContentView()
    }
}
```

![スクリーンショット 2022-08-25 7.02.41.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/1281693/9ed901e6-bc08-445c-5dbc-fd9cacaab71a.png)

# ObservableObjectを入れ子にした場合
