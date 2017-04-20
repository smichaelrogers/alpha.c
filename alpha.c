#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alpha.h"

int search(int alpha, int beta, int depth) {
  int i, n;

  if (depth == 0)
    return evaluate();

  nodes++;
  generate_moves();

  for (i = on_move[ply]; i < on_move[ply + 1]; i++) {
    if (!make_move(&moves[i]))
      continue;
    n = -search(-beta, -alpha, depth - 1);
    unmake_move(&moves[i]);

    if (n >= beta)
      return beta;
    if (n > alpha) {
      alpha = n;
      if (ply == 0) {
        m_from = moves[i].from;
        m_to = moves[i].to;
      }
    }
  }

  return alpha;
}


void generate_moves() {
  int i, f, s, t;

  on_move[ply + 1] = on_move[ply];

  for (f = 0; f < 64; f++) {
    if (colors[f] != mx)
      continue;

    if (pieces[f] == PAWN) {

      t = f + up[mx];
      if (colors[t + 1] == mn && SQ120[SQ64[t] + 1] != -1)
        add_move(f, t + 1);
      if (colors[t - 1] == mn && SQ120[SQ64[t] - 1] != -1)
        add_move(f, t - 1);
      if (colors[t] != EMPTY)
        continue;
      add_move(f, t);
      if (colors[t + up[mx]] == EMPTY && ROW(f) == pawn_rank[mx])
        add_move(f, t + up[mx]);

    } else {

      for (i = 0; i < nsteps[pieces[f]]; i++) {
        s = steps[pieces[f]][i];
        t = SQ120[SQ64[f] + s];

        while(t != -1) {
          if (colors[t] == mn || colors[t] == EMPTY)
            add_move(f, t);
          if (colors[t] != EMPTY || pieces[f] == KNIGHT || pieces[f] == KING)
            break;
          t = SQ120[SQ64[t] + s];
        }
      }
    }
  }
}


void add_move(int from, int to) {
  move_t *m;

  m = &moves[on_move[ply + 1]++];
  m->from = (char)from;
  m->to = (char)to;
  m->piece = (char)pieces[from];
  m->target = (char)pieces[to];
}


void swap_sides() {
  mx ^= 1;
  mn ^= 1;
}


bool make_move(move_t *m) {
  ply++;

  colors[m->to] = mx;
  pieces[m->to] = m->piece;
  colors[m->from] = EMPTY;
  pieces[m->from] = EMPTY;

  if (m->piece == KING)
    kings[mx] = m->to;
  else if (m->piece == PAWN && ROW(m->to) == promote_rank[mx])
    pieces[m->to] = QUEEN;

  if(in_check()) {
    swap_sides();
    unmake_move(m);
    return false;
  }
  swap_sides();
  return true;
}


void unmake_move(move_t *m) {
  ply--;
  swap_sides();

  colors[m->from] = mx;
  pieces[m->from] = m->piece;
  colors[m->to] = m->target == EMPTY ? EMPTY : mn;
  pieces[m->to] = m->target;

  if (m->piece == KING)
    kings[mx] = m->from;
}


int evaluate() {
  int i, x;

  x = 0;
  for (i = 0; i < 64; i++) {
    if      (colors[i] == mx) x += (material[i] + pst[i]);
    else if (colors[i] == mn) x -= (material[i] + pst[i]);
  }

  return x;
}


bool in_check() {
  int i, t, s, f;
  f = kings[mx];

  for(i = 0; i < 8; i++) {
    t = SQ120[SQ64[f] + steps[KNIGHT][i]];

    if (t != -1 && pieces[t] == KNIGHT && colors[t] == mn)
      return true;

    s = steps[KING][i];
    t = SQ120[SQ64[f] + s];

    while (t != -1 && colors[t] == EMPTY)
      t = SQ120[SQ64[t] + s];

    if (t == -1 || colors[t] != mn)
      continue;

    switch (pieces[t]) {
      case BISHOP:
        if (i > 3) break;
      case ROOK:
        if (i < 4) break;
      case PAWN:
        if (s - up[mn] != 1 && s - up[mn] != -1) break;
      case KING:
        if (SQ120[SQ64[f] + s] != t) break;
      case QUEEN:
        return true;
    }
  }

  return false;
}


void print_board() {
  int i;

  printf("\n nodes: %i", nodes);
  printf("\n move: %i to %i", m_from, m_to);
  printf("\n\n ");

  for (i = 0; i < 64; i++) {
    printf(" %c", piece_types[colors[i] % 6][pieces[i]]);
    if ((i + 1) % 8 == 0 && i != 63)
      printf("\n ");
  }

  printf("\n");
}


int main() {
  int i;
  move_t m;

  for(i = 0; i < 100; i++) {
    m_from = -1;
    m_to = -1;
    ply = 0;
    nodes = 0;

    search(-5000, 5000, 7);

    if (m_from < 0)
      break;

    m.from = m_from;
    m.to = m_to;
    m.piece = pieces[m_from];
    m.target = pieces[m_to];
    make_move(&m);
    print_board();
  }

  return 0;
}
