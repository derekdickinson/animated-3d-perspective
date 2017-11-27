
MYPOLY thepolys[]=
{
  {    /* Three dimensional object */
    36, 60,
    {
      {  0.0,   2.0,  0.0 },  { -1.0,   1.0,  0.0 },  { -2.5,   1.0,  0.0 },
      { -1.75,  0.0,  0.0 },  { -2.5,  -1.0,  0.0 },  { -1.0,  -1.0,  0.0 },
      {  0.0,  -2.0,  0.0 },  {  1.0,  -1.0,  0.0 },  {  2.5,  -1.0,  0.0 },
      {  1.75,  0.0,  0.0 },  {  2.5,   1.0,  0.0 },  {  1.0,   1.0,  0.0 },
      {  0.0,   1.0,  1.0 },  { -0.5,   0.5,  1.0 },  { -1.25,  0.5,  1.0 },
      { -0.875, 0.0,  1.0 },  { -1.25, -0.5,  1.0 },  { -0.5,  -0.5,  1.0 },
      {  0.0,  -1.0,  1.0 },  {  0.5,  -0.5,  1.0 },  {  1.25, -0.5,  1.0 },
      {  0.875, 0.0,  1.0 },  {  1.25,  0.5,  1.0 },  {  0.5,   0.5,  1.0 },
      {  0.0,   2.0,  2.0 },  { -1.0,   1.0,  2.0 },  { -2.5,   1.0,  2.0 },
      { -1.75,  0.0,  2.0 },  { -2.5,  -1.0,  2.0 },  { -1.0,  -1.0,  2.0 },
      {  0.0,  -2.0,  2.0 },  {  1.0,  -1.0,  2.0 },  {  2.5,  -1.0,  2.0 },
      {  1.75,  0.0,  2.0 },  {  2.5,   1.0,  2.0 },  {  1.0,   1.0,  2.0 }
    },
    {
      {  0,  1, L1 }, {  1,  2, L1 }, {  2,  3, L1 }, {  3,  4, L1 },
      {  4,  5, L1 }, {  5,  6, L1 }, {  6,  7, L1 }, {  7,  8, L1 },
      {  8,  9, L1 }, {  9, 10, L1 }, { 10, 11, L1 }, { 11,  0, L1 },

      { 12, 13, L2 }, { 13, 14, L2 }, { 14, 15, L2 }, { 15, 16, L2 },
      { 16, 17, L2 }, { 17, 18, L2 }, { 18, 19, L2 }, { 19, 20, L2 },
      { 20, 21, L2 }, { 21, 22, L2 }, { 22, 23, L2 }, { 23, 12, L2 },

      { 24, 25, L3 }, { 25, 26, L3 }, { 26, 27, L3 }, { 27, 28, L3 },
      { 28, 29, L3 }, { 29, 30, L3 }, { 30, 31, L3 }, { 31, 32, L3 },
      { 32, 33, L3 }, { 33, 34, L3 }, { 34, 35, L3 }, { 35, 24, L3 },

      {  0, 12, S1 }, {  1, 13, S1 }, {  2, 14, S1 }, {  3, 15, S1 },
      {  4, 16, S1 }, {  5, 17, S1 }, {  6, 18, S1 }, {  7, 19, S1 },
      {  8, 20, S1 }, {  9, 21, S1 }, { 10, 22, S1 }, { 11, 23, S1 },

      { 12, 24, S2 }, { 13, 25, S2 }, { 14, 26, S2 }, { 15, 27, S2 },
      { 16, 28, S2 }, { 17, 29, S2 }, { 18, 30, S2 }, { 19, 31, S2 },
      { 20, 32, S2 }, { 21, 33, S2 }, { 22, 34, S2 }, { 23, 35, S2 }
    }
  },
  {         /* Two dimensional object */
    12, 12,
    {
      {  0.0,   2.0,  0.0 },  { -1.0,   1.0,  0.0 },  { -2.5,   1.0,  0.0 },
      { -1.75,  0.0,  0.0 },  { -2.5,  -1.0,  0.0 },  { -1.0,  -1.0,  0.0 },
      {  0.0,  -2.0,  0.0 },  {  1.0,  -1.0,  0.0 },  {  2.5,  -1.0,  0.0 },
      {  1.75,  0.0,  0.0 },  {  2.5,   1.0,  0.0 },  {  1.0,   1.0,  0.0 }
    },
    {
      {  0,  1, L1 }, {  1,  2, L1 }, {  2,  3, L1 }, {  3,  4, L1 },
      {  4,  5, L1 }, {  5,  6, L1 }, {  6,  7, L1 }, {  7,  8, L1 },
      {  8,  9, L1 }, {  9, 10, L1 }, { 10, 11, L1 }, { 11,  0, L1 }
    }
  },
  {         /* Cube */
    8, 12,
    {
      {  1.0,   1.0,  1.0 },
      {  1.0,  -1.0,  1.0 },
      { -1.0,  -1.0,  1.0 },
      { -1.0,   1.0,  1.0 },
      {  1.0,   1.0, -1.0 },
      {  1.0,  -1.0, -1.0 },
      { -1.0,  -1.0, -1.0 },
      { -1.0,   1.0, -1.0 }
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 4, 5, BOTCOL }, { 5, 6, BOTCOL }, { 6, 7, BOTCOL }, { 7, 4, BOTCOL },
      { 0, 4, SIDCOL }, { 1, 5, SIDCOL }, { 2, 6, SIDCOL }, { 3, 7, SIDCOL }
    }
  },
  {         /* Tetrahedron */
    5, 8,
    {
      {  1.0,   0.0,  1.0 },
      {  1.0,   0.0, -1.0 },
      { -1.0,   0.0, -1.0 },
      { -1.0,   0.0,  1.0 },
      {  0.0,   2.0,  0.0 }
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 0, 4, SIDCOL }, { 1, 4, BOTCOL }, { 2, 4, SIDCOL }, { 3, 4, BOTCOL }
    }
  },
  {         /* Hmmmm */
    8, 12,
    {
      {  1.0,   1.0,  1.0 },
      {  1.0,  -1.0,  1.0 },
      { -1.0,  -1.0,  1.0 },
      { -1.0,   1.0,  1.0 },
      {  1.0,   1.0, -1.0 },
      {  1.0,  -1.0, -1.0 },
      { -1.0,  -1.0, -1.0 },
      { -1.0,   1.0, -1.0 }
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 0, 5, SIDCOL }, { 1, 6, SIDCOL }, { 2, 7, SIDCOL }, { 3, 4, SIDCOL },
      { 0, 4, BOTCOL }, { 1, 5, BOTCOL }, { 2, 6, BOTCOL }, { 3, 7, BOTCOL }
    }
  },
  {         /* Cube 2 */
    8, 16,
    {
      {  1.0,   1.0,  1.0 },
      {  1.0,  -1.0,  1.0 },
      { -1.0,  -1.0,  1.0 },
      { -1.0,   1.0,  1.0 },
      {  1.0,   1.0, -1.0 },
      {  1.0,  -1.0, -1.0 },
      { -1.0,  -1.0, -1.0 },
      { -1.0,   1.0, -1.0 }
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 4, 5, BOTCOL }, { 5, 6, BOTCOL }, { 6, 7, BOTCOL }, { 7, 4, BOTCOL },
      { 0, 4, SIDCOL }, { 1, 5, SIDCOL }, { 2, 6, SIDCOL }, { 3, 7, SIDCOL },

      { 0, 2, TOPCOL }, { 1, 3, TOPCOL }, { 4, 6, BOTCOL }, { 5, 7, BOTCOL }
    }
  },
  {         /* Cube 3 */
    10, 17,
    {
      {  1.0,   1.0,  1.0 },
      {  1.0,  -1.0,  1.0 },
      { -1.0,  -1.0,  1.0 },
      { -1.0,   1.0,  1.0 },
      {  1.0,   1.0, -1.0 },
      {  1.0,  -1.0, -1.0 },
      { -1.0,  -1.0, -1.0 },
      { -1.0,   1.0, -1.0 },
      {  0.0,   1.0,  0.0 },
      {  0.0,  -1.0,  0.0 }
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 4, 5, BOTCOL }, { 5, 6, BOTCOL }, { 6, 7, BOTCOL }, { 7, 4, BOTCOL },
      { 0, 2, TOPCOL }, { 1, 3, TOPCOL }, { 4, 6, BOTCOL }, { 5, 7, BOTCOL },
      { 8, 9, SIDCOL }
    }
  },
  {         /*  */
    6, 5,
    {
      {  1.0,  0.0,  1.0 },
      {  1.0,  0.0, -1.0 },
      { -1.0,  0.0, -1.0 },
      { -1.0,  0.0,  1.0 },

      {  0.0,   1.0,  0.0 },
      {  0.0,  -1.0,  0.0 },
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 4, 5, BOTCOL }
    }
  },
  {         /* Cubeman */
    14, 16,
    {
      {  1.0,   1.0,  1.0 },
      {  1.0,  -1.0,  1.0 },
      { -1.0,  -1.0,  1.0 },
      { -1.0,   1.0,  1.0 },
      {  1.0,   1.0, -1.0 },
      {  1.0,  -1.0, -1.0 },
      { -1.0,  -1.0, -1.0 },
      { -1.0,   1.0, -1.0 },
      { -1.0,  -1.0, -0.2 },
      { -1.0,  -1.0,  0.2 },
      { -0.5,  -0.5,  0.0 },
      {  0.25,  0.25, 0.0 },
      { -0.125, -0.125, -0.25},
      { -0.125, -0.125,  0.25}
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 4, 5, BOTCOL }, { 5, 6, BOTCOL }, { 6, 7, BOTCOL }, { 7, 4, BOTCOL },
      { 0, 4, SIDCOL }, { 1, 5, SIDCOL }, { 2, 6, SIDCOL }, { 3, 7, SIDCOL },
      { 8,10, WHITE  }, { 9,10, WHITE  }, { 10,11, WHITE  }, { 12, 13, WHITE }
    }
  },
  {         /* Cubeman */
    14, 16,
    {
      {  1.0,   1.0,  1.0 },
      {  1.0,  -1.0,  1.0 },
      { -1.0,  -1.0,  1.0 },
      { -1.0,   1.0,  1.0 },
      {  1.0,   1.0, -1.0 },
      {  1.0,  -1.0, -1.0 },
      { -1.0,  -1.0, -1.0 },
      { -1.0,   1.0, -1.0 },
      {  0.25,  0.25,-0.3 },
      {  0.25,  0.25, 0.3 },
      { -1.0,  -1.0, -0.15 },
      { -1.0,  -1.0,  0.15 },
      { -0.125, -0.125, -0.4 },
      { -0.125, -0.125,  0.4 }
    },
    {
      { 0, 1, TOPCOL }, { 1, 2, TOPCOL }, { 2, 3, TOPCOL }, { 3, 0, TOPCOL },
      { 4, 5, BOTCOL }, { 5, 6, BOTCOL }, { 6, 7, BOTCOL }, { 7, 4, BOTCOL },
      { 0, 4, SIDCOL }, { 1, 5, SIDCOL }, { 2, 6, SIDCOL }, { 3, 7, SIDCOL },
      { 8, 9, WHITE  }, { 9,10, WHITE  }, { 8,11, WHITE  }, { 12, 13, WHITE }
    }
  }
};
