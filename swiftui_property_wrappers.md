<!-- SwiftUIのProperty Wrappersの使い方 その１ -->

# はじめに
SwiftUIのProperty wrappersでこんなときはどうしたらいいの？どのように使えばいいの？と
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
![スクリーンショット 2022-07-26 7.02.21.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/1281693/10b2f22c-3b4b-1b42-5d66-28cb3f83d8ce.png)

# Computed propertyの使い方
`@State`はStored propertyに付与しますが、Computed propertyには付与することができません。  
ただし、Computed propertyが`@State`を付与したプロパティを持っているとき、
その`@State`を付与したプロパティが変更されると、Computed propertyとViewが更新されます。
これは`@State`だけでなく、`@Binding`などのProperty wrapperでも同様です。

```swift
import SwiftUI

struct ComputedPropertyContentView: View {
    @State var value1 = 0
    @State var value2 = 0
    
    var sumValue: Int { self.value1 + self.value2 }
    
    var body: some View {
        VStack {
            Button(action: {
                self.value1 += 1
            }) {
                Text("value1")
                    .padding()
            }
            Button(action: {
                self.value2 += 1
            }) {
                Text("value2")
                    .padding()
            }
            Text(String(self.sumValue)).padding() //*
            Text(String(Int.random(in: 0..<10)))
                .padding()
        }
        .frame(maxHeight: .infinity, alignment: .top)
    }
}
```

上のコードではComputed propertyとしてsumValueを定義しています。  
value1とvalue2を変更するとViewにsumValueの表示があるのでViewが更新され、乱数値も更新されます。  
コード上の`*`をつけた行をコメントアウトするとvalue1とvalue2を変更してもViewが更新されず乱数値が更新されなくなります。

# Bindingの使い方
Viewの外から受け取った値を変更したいプロパティには`@Binding`を付与することで変更が可能になります。  
そのプロパティをさらにBinding引数に渡すには`$`をプロパティの前につけて渡します。  

```swift
import SwiftUI

struct BindingContentView: View {
    @State var value1 = 0
    @State var value2 = 0
    @State var array: [Int] = []
    
    var body: some View {
        BindingView(value1: self.$value1, value2: self.$value2, array: self.$array)
    }
}

struct BindingView: View {
    @Binding var value1: Int
    @Binding var value2: Int
    @Binding var array: [Int]
    
    var body: some View {
        VStack {
            HStack {
                Text("参照と更新")
                    .padding()
                Button(action: {
                    self.value1 += 1
                }) {
                    Text(String(self.value1))
                        .padding()
                }
            }
            HStack {
                Text("@Bindingに渡す")
                    .padding()
                Picker("value", selection: self.$value2) {
                    ForEach(0...99, id: \.self) { v in
                        Text(String(v))
                    }
                }
            }
            HStack {
                Text("Array")
                    .padding()
                Button(action: {
                    self.array.append(self.value1 + self.value2)
                }) {
                    Text("add")
                        .padding()
                }
            }
            List {
                ForEach(self.array, id: \.self) { v in
                    Text(String(v))
                }
                .onDelete { offsets in
                    self.array.remove(atOffsets: offsets)
                }
            }
        }
    }
}
```
