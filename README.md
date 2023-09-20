# Lambda calculus parser
grammar: <br>
M ⟶ x | ( N ) <br>
N ⟶ \lambda x . M | M L <br>
L ⟶ M | \epsilon <br>

Rules: <br>
1. M ⟶ x <br>
2. M ⟶ ( N ) <br>
3. N -> \lambda x . M <br>
4. N -> M L <br>
5. L ⟶ M <br>
6. L ⟶ \epsilon <br>

FIRST(M) = {x, (} <br>
FIRST(N) = {\lambda, x, (} <br>
FIRST(L) = {x , \epsilon, (} <br>
FOLLOW() = {x, (, ), $} <br>
FOLLOW() = {)} <br>
FOLLOW() = {)} <br>

| | x | ( | ) | \lambda |
| --- | --- | --- | --- | --- |
| M | M ⟶ x | M ⟶ ( N ) | | |
| N | N ⟶ M L | N ⟶ M L | | N ⟶ \lambda x . M
| L | L ⟶M | L ⟶ M | L ⟶ \epsilon | |
