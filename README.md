# Lambda calculus parser
grammar: <br>
M ⟶ x | ( N ) <br>
N ⟶ \lambda x . M | M L <br>
L ⟶ M | \epsilon <br>

Rules: <br>
1. M ⟶ x <br>
2. M ⟶ ( N ) <br>
3. N ⟶ \lambda x . M <br>
4. N ⟶ M L <br>
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
| M | 1 | 2 | | |
| N | 4 | 4 | | 3
| L | 5 | 5 | 6 | |

Rules: <br>
1. E -> (E E) <br>
2. E -> var <br>
3. E -> \lambda var . E <br>

| | var | ( | \lambda |
| --- | --- | --- | --- |
| M | 1 | 2 | | 
