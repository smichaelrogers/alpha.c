## alpha.c

(wip) tiny chess engine written in c

more complete versions:
- [alpha.rb](https://github.com/smichaelrogers/alpha.rb)
- [alpha.js](https://github.com/smichaelrogers/alpha.js)


### usage

```shell
$ make
gcc -o alpha alpha.c -I.

$ ./alpha
  nodes: 6435670
  move: 52 to 44
  r n b q k b n r
  p p p p p p p p
  . . . . . . . .
  . . . . . . . .
  . . . . . . . .
  . . . . P . . .
  P P P P . P P P
  R N B Q K B N R
```
