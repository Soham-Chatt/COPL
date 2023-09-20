# COPL
Lambda calculus parser
grammar:
M -> x | ( N )
N -> \lambda x . M | M L
L -> M | \epsilon

FIRST(M) = {x, (}
FIRST(N) = {\lambda, x, (}
FIRST(L) = {x , \epsilon, (}
FOLLOW() = {x, (, ), $}
FOLLOW() = {)}
FOLLOW() = {)}

| | x | ( | ) | \lambda |
| --- | --- | --- | --- | --- |
| M | M -> x | M -> ( N ) | | |
| N | N -> M L | N -> M L | | N -> \lambda x . M
| L | L ->M | L -> M | L -> \epsilon | |
