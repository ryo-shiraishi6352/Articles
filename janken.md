<!-- じゃんけんで1人を決定する方法の比較 -->

# はじめに
複数人から1人を決めるときにじゃんけんで決めることが私は多いです。  
ただし、普通のじゃんけんだと人数が増えるとなかなか決着がつかなくなります。  
今回はじゃんけんの勝敗の決め方を変えてどの方法が1番少ない回数で1人を決めることができるかを検証します。  

# じゃんけんの勝敗の決め方
## 普通のじゃんけん
- グー、チョキ、パーのうち1種類だけのときには全員あいこ
- グー、チョキ、パーのすべての種類があるときには全員あいこ
- グー、チョキだけのときはグーの勝ち、チョキの負け
- チョキ、パーだけのときはチョキの勝ち、パーの負け
- パー、グーだけのときはパーの勝ち、グーの負け

## vsマスターじゃんけん
- 参加者とは別にマスターを1人加えてじゃんけんを行う
- マスターがグーのときにはグーはあいこ、チョキは負け、パーは勝ち
- マスターがチョキのときにはグーは勝ち、チョキはあいこ、パーは負け
- マスターがパーのときにはグーは負け、チョキは勝ち、パーはあいこ

## 少ない勝ちじゃんけん
- グー、チョキ、パーそれぞれの個数を数えて1番少ない個数が勝ち
- グー、チョキ、パーのうち1種類だけのときには全員あいこ
- グー、チョキ、パーの個数がすべて同じときには全員あいこ
- グー、チョキだけで個数が同じときにはグーの勝ち、チョキの負け
- チョキ、パーだけで個数が同じときにはチョキの勝ち、パーの負け
- パー、グーだけで個数が同じときにはパーの勝ち、グーの負け

# じゃんけんをして1人を決めるまでの流れ
1. $n$ 人でじゃんけんを行う
2. じゃんけんの勝者が $n'$ 人のとき（あいこは勝者に含めない）、  
2-1. $n' = 1$ なら終了  
2-2. $n' = 0$ なら1に戻る  
2-3. $n' > 1$ なら $n \gets n'$ して1に戻る  

# あいこの確率について
$n$ 人でじゃんけんをして $n'(\neq n)$ になる確率を $P(n \to n')$ 、  
あいこになり人数が変わらない確率を $P(n \to n)$ とすると
$$
P(n \to n) = 1 - \sum_{n'=1}^{n-1} P(n \to n')
$$

```python
def aiko_calc(n, f):
    return 1 - sum(f(n_prime) for n_prime in range(1, n))
```

# 普通のじゃんけんのシミュレーション
$n$ 人のじゃんけんの手をランダムに生成し、普通のじゃんけんの勝敗の決め方に従って
$n'$ 人になる確率をシミュレーションします。

```python
import random

def normal_janken_simulation(n, trial_count):
    count_dict = dict()
    for _ in range(trial_count):
        hands = [['gu', 'choki', 'pa'][random.randrange(3)] for _ in range(n)]

        has_gu = any(h == 'gu' for h in hands)
        has_choki = any(h == 'choki' for h in hands)
        has_pa = any(h == 'pa' for h in hands)

        if has_gu and has_choki and has_pa:
            win_hand = None
        elif has_gu and has_choki:
            win_hand = 'gu'
        elif has_choki and has_pa:
            win_hand = 'choki'
        elif has_pa and has_gu:
            win_hand = 'pa'
        else:
            win_hand = None
        
        n_prime = n if win_hand is None else sum(h == win_hand for h in hands)
        
        if n_prime in count_dict:
            count_dict[n_prime] += 1
        else:
            count_dict[n_prime] = 1
    xs = list(range(1, max(count_dict) + 1))
    ys = [count_dict[i] if i in count_dict else 0 for i in xs]
    ys = [y / trial_count for y in ys]
    return ys
```

# 普通のじゃんけんをしたときの確率
$n$ 人でじゃんけんをして $n'(\neq n)$ になる確率 $P(n \to n')$ は  
勝者の手が決まれば敗者の手は1つに決まるので、$n$ 人から $n'$人が選ばれる確率に等しい。  
したがって、
$$
P(n \to n') = 3 * \frac{{}_nC_{n'}}{3^n} = \frac{{}_nC_{n'}}{3^{n-1}}
$$

```python
import math

def normal_janken_calc(n):
    def janken_calc(n, n_prime):
        if n == n_prime:
            return aiko_calc(n, lambda x: janken_calc(n, x))
        else:
            return math.comb(n, n_prime) / (3**(n-1))
    return [janken_calc(n, n_prime) for n_prime in range(1, n + 1)]
```

# 普通のじゃんけんをして $n$ 人から $n'$ 人になる確率のグラフ
```python
def plot_normal_janken():
    trial_count = 1000
    n_max = 5
    
    plt.rcParams['font.family'] = 'MS Gothic'
    fig, ax = plt.subplots(1, 2, figsize=(10, 5), sharey=True)

    for n in range(2, n_max+1):
        xs = list(range(1, n + 1))
        ys = normal_janken_simulation(n, trial_count)
        ax[0].plot(xs, ys, label=f'n = {n}')
    ax[0].legend()
    ax[0].set_xlabel('$n\'$ 人')
    ax[0].set_title('シミュレーション')
    
    for n in range(2, n_max+1):
        xs = list(range(1, n + 1))
        ys = normal_janken_calc(n)
        ax[1].plot(xs, ys, label=f'n = {n}')
    ax[1].legend()
    ax[1].set_xlabel('$n\'$ 人')
    ax[1].set_title('計算')

    fig.suptitle('普通のじゃんけんをして $n$ 人から $n\'$ 人になる確率')

    plt.show()
```
![janken_normal_1.png](.\janken_normal_1.png)

# 人数を変えながらシミュレーションした結果
人数を2人から10人の間で変えながら100回ずつシミュレーションした結果が以下の図です。  
横軸が人数で縦軸が決着がつくまでの回数です。  

普通のじゃんけんだと人数が増えると決着がつくまでの回数が爆発します。  
vsマスターじゃんけんと少ない勝ちじゃんけんは回数が抑えられており、  
特に少ない勝ちじゃんけんが少ない回数で決着がつけることができています。  

普通のじゃんけんとvsマスターじゃんけんの決着がつくまでの回数が
3人付近で入れ替わっているので3人以下になったときには
普通のじゃんけんに切り替えるほうが効率的なようです。


また、vsマスターじゃんけんと少ない勝ちじゃんけんを
人数を2人から100人の間で変えながら1000回ずつシミュレーションした結果が以下の図です。

100人まで人数が増えたとしてもvsマスターじゃんけんだと5回程度、
少ない勝ちじゃんけんだと4回程度で決着をつけることができています。
