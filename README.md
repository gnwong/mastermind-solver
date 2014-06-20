mastermind-solver
==============

2014 (c)  George Wong


A simple program written in C++ demonstrating several methods of solving the "mastermind" game in an efficient way.


### Organization

The program is organized into two parts, an "adversary" and a "challenger". The challenger implements the "truth" generator as well as "guess" methods which allow an adversary to query the goodness of any guess they choose.

#### Challenger

The challenger is instantiated with one integer-type input which specifies the type of PRG to use. For the purpose of guessing, its interface provides two methods:

Challenger.query(Guess g, int rw[]) and <br>
Challenger.get\_correlation(Guess g, Guess t, int rw[]).

#### Adversary

The adversary implements the automated solving routines. Its constructor takes as argument an instantiated challenger.

The object implements three solving methods:

Adversary.naive(), <br>
Adversary.efficient(), and <br>
Adversary.smart().

The naive solver randomly guesses until it finds the solution. The efficient solver only guesses in a way consistent with the returns from previous guesses. The smart solver picks guesses consistent with previous returns in a way so as to maximize the probability of splitting the set of future consistent guesses.



### Use




### Notes




