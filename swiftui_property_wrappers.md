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
![スクリーンショット 2022-07-28 7.41.59.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/1281693/fafd2c76-36d9-5135-bfdb-9e466e7eccc4.png)

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
![スクリーンショット 2022-08-01 6.55.09.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/1281693/84e21358-9b9f-e998-0c28-e276d21c45af.png)

# 何も付けていないプロパティに値を渡した場合
Viewの外から値を受け取って変更したい場合には`@Binding`を付けますが、
値を変更する必要がない場合には何も付けずに受け取ることができます。

```swift
import SwiftUI

struct ReadOnlyContentView: View {
    @State var value1 = 0
    @State var value2 = 0
    
    var body: some View {
        VStack {
            HStack {
                Text("value1")
                    .padding()
                Button(action: {
                    self.value1 += 1
                }) {
                    Text("+1")
                        .padding()
                }
            }
            HStack {
                Text("value2")
                    .padding()
                Button(action: {
                    self.value2 += 1
                }) {
                    Text("+1")
                        .padding()
                }
            }
            ReadOnlyView1(value: self.value1 * 10) //Viewが更新される
            ReadOnlyView2(value: self.value1) //Viewが更新される
            ReadOnlyView3() //Viewが更新されない
        }
        .frame(maxHeight: .infinity, alignment: .top)
    }
}

struct ReadOnlyView1: View {
    var value: Int
    
    var body: some View {
        VStack {
            Text(String(self.value))
                .padding()
        }
    }
}

struct ReadOnlyView2: View {
    var value: Int
    
    var body: some View {
        VStack {
            Text(String(Int.random(in: 0...9)))
                .padding()
        }
    }
}

struct ReadOnlyView3: View {
    var body: some View {
        VStack {
            Text(String(Int.random(in: 0...9)))
                .padding()
        }
    }
}
```
![スクリーンショット 2022-08-03 7.03.11.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/1281693/93b954c8-634e-046c-67bf-8cb9ac894db4.png)

何も付けていないプロパティを持つViewは引数として`@State`などの変更を検知できるプロパティを受け取る場合にViewが更新されます。
