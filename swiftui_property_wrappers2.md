<!-- SwiftUIのProperty Wrappersの使い方 その２ -->

# StateObjectの使い方
`@State`は値型のプロパティに付加しますが参照型のクラスのプロパティには`@StateObject`を付加します。  
`@StateObject`を付加するクラスにはObservableObjectプロトコルに準拠させ、監視するプロパティには`@Published`を付加します

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
