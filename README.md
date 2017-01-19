# Langton's ant
Langton's ant is a two-dimensional Turing machine with a very simple set of rules but complex emergent behavior. It was invented by Chris Langton in 1986 and runs on a square lattice of black and white cells.


## Rules

Squares on a plane are colored variously either black or white. We arbitrarily identify one square as the "ant". The ant can travel in any of the four cardinal directions at each step it takes. The "ant" moves according to the rules below:

    At a white square, turn 90° right, flip the color of the square, move forward one unit
    At a black square, turn 90° left, flip the color of the square, move forward one unit


![image](https://s29.postimg.org/ylz8rdxiv/lant.png)

![image](https://s28.postimg.org/dugqlcnp9/lant2.png)
