#ifndef _EVALUATE_
#define _EVALUATE_
/*
    0,  P,  S,  G,  B,  R,  K,  0,
    0, eP, eS,  0, eB, eR
 */
const int pce_score[] = {
	0, 107, 810, 907, 1291, 1670, 1000000, 0,
    0, 895, 933, 0, 1985, 2408
};
/*
    0,  P,  S,  G,  B,  R
*/
const int hnd_score[] = {
    0, 152, 1110, 1260, 1464, 1998
};

const int place_score[] = {
	10, 10, 10, 10, 10,
	10, 20, 20, 20, 10,
	10, 20, 30, 20, 10, 
	10, 20, 20, 20, 10,
	10, 10, 10, 10, 10
};

#endif