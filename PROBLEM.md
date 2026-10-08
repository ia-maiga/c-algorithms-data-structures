# Problem statements

A short description of what each function had to do. Function names are kept in French, as in the
original course, so you can search for them in the source with `Ctrl+F`.

General constraints: pure C (no C++), no non-standard extensions (nested functions, variable-length
arrays), no undefined behaviour, compiles with `-Wall -Wextra -pedantic`, and every list or tree
modification uses **pass-by-address** (`Liste *L`, `image *I`).

---

## Part 1 — Simple computations · [`part1_basics.c`](src/part1_basics.c)

### Factorial
Several ways to write `n!`: plain recursion, recursion with an "out" parameter, tail recursion
with an "inout" accumulator, and an array-based version. One version (`fact5`) is **deliberately
wrong** and shows what goes wrong when the accumulator is initialised in the wrong place.

### Computing *e*
Approximate $e = \sum_{n \ge 0} \frac{1}{n!}$ in `float`, `double` and `long double`.
- Do not recompute `n!` from scratch at each step: get $\frac{1}{n!}$ from $\frac{1}{(n-1)!}$.
- Decide when to stop: here, when adding the next term no longer changes the sum.

### An unstable sequence
$y_0 = e - 1$ and $y_n = n\,y_{n-1} - 1$. Mathematically $\frac{1}{n+1} < y_n < \frac{1}{n}$, so
$y_n \to 0$. On a computer, $y_0$ is stored with a small error $\varepsilon$, and the error on
$y_n$ is $n!\,\varepsilon$. The sequence first decreases, then blows up. The point is to observe
when this happens for each floating-point type.

### Syracuse sequence
With a constant `CSyr` (here 2025): $\text{Syr}_0 = \text{CSyr}$, then halve if even, otherwise
$3x+1$. `Syracuse(n)` returns $\text{Syr}_n$, in four styles:
1. iterative,
2. tail-recursive with a helper **function**,
3. tail-recursive with a helper **procedure** (inout parameter),
4. recursive with no helper.

Check: `Syracuse(3) = 1519`, `Syracuse(10) = 15388`, `Syracuse(100) = 638`, `Syracuse(1000) = 4`.

### Permutations
A permutation of $\{0, \dots, n-1\}$ is stored as an array `P` where `P[i]` is the image of `i`.

| Function | Task | Example |
|---|---|---|
| `P_identite(n)` | identity permutation | `[0,1,2]` |
| `P_Inverse(P,n)` | $P^{-1}$ | `[4,5,2,1,3,0]` → `[5,3,2,4,0,1]` |
| `P_Compose(P,Q,R,n)` | write $P \circ Q$ into `R` | `P=[0,5,3,4,2,1]`, `Q=[4,5,2,1,3,0]` → `[2,1,3,5,4,0]` |
| `P_Verifie(P,n)` | is `P` a valid permutation? | `[1,0,3,1]` → false |
| `P_power(P,n,k)` | $P^k$, in 4 versions | linear recursive · linear iterative · $O(\log k)$ recursive · $O(\log k)$ iterative |
| `P_random(n)` | random permutation | every permutation must be **equally likely** |

---

## Part 2 — Linked lists · [`part2_linked_lists.c`](src/part2_linked_lists.c)

Singly linked lists of integers (`Liste` = pointer to the first block).

| Function | Task | Example |
|---|---|---|
| `UnPlusDeuxEgalTrois` | is the 3rd element the sum of the first two? (missing elements count as 0) | `[23,19,42,4,2]` → true, `[2,-2]` → true, `[2]` → false |
| `PlusCourteRec` / `PlusCourteIter` | is `L1` strictly shorter than `L2`? Must stop early when one list is much shorter | |
| `VerifiekORec` / `VerifiekOIter` | does `L` contain exactly `k` zeros? Stop as soon as the answer is known | `[2,0,0,7,0,6,2,4,0]`, k=4 → true |
| `NTAZ_*` (4 versions) | number of terms before the first 0 (an implicit 0 at the end) | `[3,2,9,5,0,6,0]` → 4 |
| `TuePosRec` / `TuePosIt` | remove every element equal to its 1-based position | `[0,4,3,9,5,0,9,2,1]` → `[0,4,9,0,9,2,1]` |
| `TueRetroPos` | remove every element equal to its position **counted from the end**, in a **single pass** | `[0,4,3,9,5,0,9,2,1]` → `[0,4,3,9,0,9]` |

The four `NTAZ` versions: iterative · plain recursive · tail-recursive helper with an "in"
counter · tail-recursive helper with an "inout" counter.

---

## Part 2bis — PPQ · [`part2bis_ppq.c`](src/part2bis_ppq.c)

`PPQ(p1, p2, q)` returns the **list of all lists** of integers between `p1` and `p2` whose sum
is `q`, in lexicographic order.

```
PPQ(2, 4, 9) = [[2,2,2,3] [2,2,3,2] [2,3,2,2] [2,3,4] [2,4,3] [3,2,2,2]
                [3,2,4] [3,3,3] [3,4,2] [4,2,3] [4,3,2]]
```

Idea: for each first value `i` from `p1` to `p2`, compute `PPQ(p1, p2, q - i)`, put `i` in front
of every list, and concatenate the results.
Edge cases: `q = 0` gives `[[]]` (one empty list); `0 < q < p1` gives `[]` (no list).
Extra goal: avoid memory leaks, which naive implementations of this recursion produce.

---

## Part 2ter — Circular queue · [`part2ter_circular_queue.c`](src/part2ter_circular_queue.c)

Implement a FIFO queue with a **circular linked list** where the queue is a pointer to the
**last** block (whose `next` field points to the first one). `entree` (enqueue) and `sortie`
(dequeue) must run in constant time.

---

## Part 3 — Quadtrees · [`part3_quadtree_images.c`](src/part3_quadtree_images.c)

A black & white image is either white, black, or split into 4 sub-images
(top-left, top-right, bottom-left, bottom-right). In memory, a `NULL` pointer is a black image.

Text notation: `o` = white, `Z` = black, `*` followed by the 4 sub-images.
Example: `**oooZ*ooZo*oZoo*Zooo` is a black square in the middle of a white image.

| Function | Task |
|---|---|
| `Wht`, `Blk`, `Cut` | build white / black / split images |
| `Affiche`, `ProfAffiche` | print an image, optionally with the depth of each node |
| `Lecture` | read an image from the keyboard (characters other than `o Z *` are ignored) |
| `DessinNoir`, `DessinBlanc` | is the image entirely black / white? |
| `QuotaNoir` | proportion of black (`*Z*oZooZ*ZZZo` → 0.75) |
| `Copie` | deep copy |
| `Diagonale(p)` | image whose depth-`p` pixels are black on the diagonal |
| `SimplifieProfP(I, p)` | **in place**, replace every single-colour subtree at depth `p` by one pixel |
| `Incluse(I1, I2)` | is `I2` black wherever `I1` is black? |
| `CompteSousImagesGrises` | count sub-images whose black ratio is between 1/3 and 2/3 |
| `Labyrinthe` | black = walls: can you go from the top-left to the bottom-right corner through white pixels, moving only through sides (not corners)? |
