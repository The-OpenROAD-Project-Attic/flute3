////////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2018, Iowa State University All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////

#ifndef __FLUTE_H__
#define __FLUTE_H__

namespace Flute {

/*****************************/
/*  User-Defined Parameters  */
/*****************************/
#define FLUTE_ACCURACY 10             // Default accuracy
#define ROUTING 1               // 1 to construct routing, 0 to estimate WL only
#define LOCAL_REFINEMENT 1      // Suggestion: Set to 1 if ACCURACY >= 5
#define REMOVE_DUPLICATE_PIN 0  // Remove dup. pin for flute_wl() & flute()

#ifndef FLUTE_DTYPE  // Data type for distance
#define FLUTE_DTYPE int
#endif

#define POWVFILE "POWV9.dat"  // LUT for POWV (Wirelength Vector)
#define POSTFILE "POST9.dat"  // LUT for POST (Steiner Tree)
#define D 9                   // LUT is used for d <= D, D <= 9
#define TAU(A) (8 + 1.3 * (A))
#define D1(A) (25 + 120 / ((A) * (A)))  // flute_mr is used for D1 < d <= D2
#define D2(A) ((A) <= 6 ? 500 : 75 + 5 * (A))

typedef struct {
        FLUTE_DTYPE x, y;  // starting point of the branch
        int n;       // index of neighbor
} Branch;

typedef struct {
        int deg;         // degree
        FLUTE_DTYPE length;    // total wirelength
        Branch *branch;  // array of tree branches
} Tree;

// User-Callable Functions
extern void readLUT();
extern FLUTE_DTYPE flute_wl(int d, FLUTE_DTYPE x[], FLUTE_DTYPE y[], int acc);
//Macro: FLUTE_DTYPE flutes_wl(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc);
extern Tree flute(int d, FLUTE_DTYPE x[], FLUTE_DTYPE y[], int acc);
//Macro: Tree flutes(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc);
extern FLUTE_DTYPE wirelength(Tree t);
extern void printtree(Tree t);
extern void plottree(Tree t);

// Other useful functions
extern FLUTE_DTYPE flutes_wl_LD(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[]);
extern FLUTE_DTYPE flutes_wl_MD(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc);
extern FLUTE_DTYPE flutes_wl_RDP(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc);
extern Tree flutes_LD(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[]);
extern Tree flutes_MD(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc);
extern Tree flutes_HD(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc);
extern Tree flutes_RDP(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc);

inline FLUTE_DTYPE flutes_wl_LMD(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc) {
        if (d <= D) {
                return flutes_wl_LD(d, xs, ys, s);
        } else {
                return flutes_wl_MD(d, xs, ys, s, acc);
        }
}

inline FLUTE_DTYPE flutes_wl_ALLD(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc) {
        return flutes_wl_LMD(d, xs, ys, s, acc);
}

inline FLUTE_DTYPE flutes_wl(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc) {
        if (REMOVE_DUPLICATE_PIN == 1) {
                return flutes_wl_RDP(d, xs, ys, s, acc);
        } else {
                return flutes_wl_ALLD(d, xs, ys, s, acc);
        }
}

inline Tree flutes_ALLD(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc) {
        if (d <= D) {
                return flutes_LD(d, xs, ys, s);
        } else {
                return flutes_MD(d, xs, ys, s, acc);
        }
}

inline Tree flutes(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc) {
        if (REMOVE_DUPLICATE_PIN == 1) {
                return flutes_RDP(d, xs, ys, s, acc);
        } else {
                return flutes_ALLD(d, xs, ys, s, acc);
        }
}

inline Tree flutes_LMD(int d, FLUTE_DTYPE xs[], FLUTE_DTYPE ys[], int s[], int acc) {
        if (d <= D) {
                return flutes_LD(d, xs, ys, s);
        } else {
                return flutes_MD(d, xs, ys, s, acc);
        }
}

template <class T> inline T ADIFF(T x, T y) {
        if (x > y) {
            return (x - y);
        } else {
            return (y - x);
        }
}
}  // namespace Flute
#endif /* __FLUTE_H__ */
