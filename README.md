# Algorithms & Data Structures in C

> Recursion, pointers, linked lists and quadtrees, written in pure C11.
> Every function is checked with AddressSanitizer and UBSan: no leaks, no undefined behaviour.

University project (Algorithms course, **Université Paris-Saclay**, 2025-2026), done in pairs.
The goal was to work directly with **pointers, pointer-to-pointer parameters, recursion
(terminal and non-terminal) and manual memory management** in C, without any library.

---

## 📂 Contents

| Part | File | What it does |
|------|------|--------------|
| **1** | [`part1_basics.c`](src/part1_basics.c) | 6 versions of factorial · computing *e* in `float` / `double` / `long double` · numerical instability of $y_n = n\,y_{n-1} - 1$ · 4 versions of the Syracuse sequence · permutations (inverse, composition, check, uniform random with Fisher-Yates, **fast exponentiation in $O(\log k)$**) |
| **2** | [`part2_linked_lists.c`](src/part2_linked_lists.c) | Linked-list algorithms, each in recursive **and** iterative form: length comparison with early exit, counting zeros, filtering by position, filtering by reverse position **in a single pass** |
| **2bis** | [`part2bis_ppq.c`](src/part2bis_ppq.c) | Generates all integer compositions of $q$ with parts in $[p_1, p_2]$, in lexicographic order (lists of lists), **without memory leaks** |
| **2ter** | [`part2ter_circular_queue.c`](src/part2ter_circular_queue.c) | FIFO queue as a **circular linked list**: $O(1)$ enqueue and dequeue with a single pointer |
| **3** | [`part3_quadtree_images.c`](src/part3_quadtree_images.c) | Black & white images stored as **quadtrees**: parsing, display, black ratio, inclusion, simplification, counting grey sub-images in **one linear pass**, and **maze solving** (DFS on the decoded grid) |

---

## 🌳 Highlight: quadtree images

A quadtree image is either **white** (`o`), **black** (`Z`), or split into 4 sub-images
(`*` followed by top-left, top-right, bottom-left, bottom-right).

```
**oooZ*ooZo*oZoo*Zooo   →   a black square in the centre
```

The program can decide whether a maze is solvable, i.e. whether you can walk through white pixels
from the top-left to the bottom-right corner:

```
$ ./bin/part3_quadtree_images
...
----- Labyrinthe (attendu 1 puis 0) -----
1 0
Premier labyrinthe :
. . . . # # . . . . . . . . . .
. . . . . . . . # # . . . . # .
# # . . # # # # # # # # . . # .
# # . . # # # # # # # # . . . .
# # . . # # # # . . # # # # . #
...
```

Try your own image in interactive mode:

```bash
echo "*oZ*ooZoZ" | ./bin/part3_quadtree_images -i
```

---

## 🚀 Build & run

Requirements: `gcc` (or `clang`) and `make`.

```bash
git clone https://github.com/ia-maiga/c-algorithms-data-structures.git
cd c-algorithms-data-structures

make          # build every part into bin/
make run      # run all demos (they reproduce the examples of the assignment)
make check    # rebuild with AddressSanitizer + UBSan and run everything
```

Compilation uses `-std=c11 -Wall -Wextra -pedantic` and produces **zero warnings**.

---

## 🧠 What I learned

- **Pointer to pointer** (`Liste *L`): deleting or inserting in a list without special-casing the head.
- **Recursion styles**: plain recursion, tail recursion with an accumulator ("in") or an "inout" parameter.
- **Complexity**: going from $O(k)$ to $O(\log k)$ for permutation powers, and from $O(n \log n)$ to
  $O(n)$ for counting grey sub-images.
- **Floating-point limits**: an error $\varepsilon$ on $y_0$ becomes $n!\,\varepsilon$ on $y_n$, so the
  sequence diverges after about 9 (`float`), 16 (`double`) or 19 (`long double`) steps, even though
  it tends to 0 mathematically.
- **Memory hygiene**: checking every program with sanitizers, and avoiding shared sub-structures that
  lead to double frees.

---

## 📝 Notes

- Function names (`TuePosRec`, `QuotaNoir`, `Labyrinthe`, …) are kept **in French**, as required by
  the original assignment.
- `fact5` is intentionally wrong: it is an example from the assignment of a badly placed initialisation.

---

## 👥 Authors

**Ibrahim Aboubakarine Maiga** & **Walid Bouzid**
Double Bachelor's in Mathematics & Computer Science, Université Paris-Saclay

> *If you are currently taking this course: this repository is shared as a portfolio.
> Please write your own code; copying it would be plagiarism.*
