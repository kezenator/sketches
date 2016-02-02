Solves the "Color Oil" puzzles - an Android app
https://play.google.com/store/apps/details?id=com.kyworks.coloroil.free&hl=en

It's a puzzle with a grid of adjoining colored pieces.
One piece is marked the starting piece.
On each move, you recolor the starting piece, which
joins it to all ajoining pieces of the new color -
createing a new larger piece of the new color.
You win the game when all pieces are joined into
a single piece of the one color.
There is a maximum number of moves - the optimal
solution.

Colors are: <space>=blank, b=blue, r=red, g=green, y=yellow, c=cyan, m=magenta

For example:

RRRR
RGGG
BBRR

There are four pieces - 2 red, one blue, one green.
If starting in the top left red piece, you can solve
in two moves - e.g. green, blue, red.

I had what I thought was a fool-proof strategy:
1) Count the number of colors - 6 on the harder levels.
2) See the max moves - e.g. 10 moves.
3) Subtract, to find the number of "free" moves
   before you have to eliminate a whole color
   on each move. E.g. for 6 colors and 10 moves
   you have 4 free moves. Once 4 moves are made
   each of the 6 additional moves MUST eliminate
   all remaining pieces of a color.
4) Find a path in the number of free moves
   that allows you to totally remove the first color
   on the first non-free move.

This worked very well for me and I could solve
medium levels in 10 seconds and hard levels in a minute or so.
However I got totally stuck on a couple of levels - and
I'm too lazy to do lots of re-trying.

I thought maybe there was a new strategy that I
was totally missing.

So I wrote a programme to solve it!

Nope - no magic strategy - I just wasn't looking
hard enough for the "free move" path :)

It's a brute force search.
The most interesting optimization was to count the number
of colors remaining and only recurse if there are enought
"free" moves left to get rid of the remaining colors.
This sped the search up LOTS as without this it was
continuing down lots of sub-searches that were never going
to succeed.
