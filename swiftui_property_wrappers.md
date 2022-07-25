<!-- SwiftUIのProperty Wrappersの使い方 -->

# はじめに
SwiftUIのProperty Wrappersでこんなときはどうしたらいいの？どのように使えばいいの？と
あまり理解できていなかったのでいくつかサンプルを作成しながら使い方をまとめました。  

# Stateの使い方
あるViewの中で値を変更したいプロパティには`@State`を付与することで変更が可能になります。  
PickerなどのBinding引数には`$`をプロパティの前につけて渡します。  
SwiftのArrayにもつけることができ、値の変更時にViewが更新されます

```swift
import SwiftUI

struct StateContentView: View {
    @State var stateValue1 = 0
    @State var stateValue2 = 0
    @State var stateArray: [Int] = []
    
    var body: some View {
        VStack {
            HStack {
                Text("参照と更新")
                    .padding()
                Button(action: {
                    self.stateValue1 += 1
                }) {
                    Text(String(self.stateValue1))
                        .padding()
                }
            }
            HStack {
                Text("@Bindingに渡す")
                    .padding()
                Picker("stateValue", selection: self.$stateValue2) {
                    ForEach(0...99, id: \.self) { v in
                        Text(String(v))
                    }
                }
            }
            HStack {
                Text("Array")
                    .padding()
                Button(action: {
                    self.stateArray.append(self.stateValue1 + self.stateValue2)
                }) {
                    Text("add")
                        .padding()
                }
            }
            List {
                ForEach(self.stateArray, id: \.self) { v in
                    Text(String(v))
                }
                .onDelete { offsets in
                    self.stateArray.remove(atOffsets: offsets)
                }
            }
        }
    }
}
```
