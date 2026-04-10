#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_7430843458597553837) {
   out_7430843458597553837[0] = delta_x[0] + nom_x[0];
   out_7430843458597553837[1] = delta_x[1] + nom_x[1];
   out_7430843458597553837[2] = delta_x[2] + nom_x[2];
   out_7430843458597553837[3] = delta_x[3] + nom_x[3];
   out_7430843458597553837[4] = delta_x[4] + nom_x[4];
   out_7430843458597553837[5] = delta_x[5] + nom_x[5];
   out_7430843458597553837[6] = delta_x[6] + nom_x[6];
   out_7430843458597553837[7] = delta_x[7] + nom_x[7];
   out_7430843458597553837[8] = delta_x[8] + nom_x[8];
   out_7430843458597553837[9] = delta_x[9] + nom_x[9];
   out_7430843458597553837[10] = delta_x[10] + nom_x[10];
   out_7430843458597553837[11] = delta_x[11] + nom_x[11];
   out_7430843458597553837[12] = delta_x[12] + nom_x[12];
   out_7430843458597553837[13] = delta_x[13] + nom_x[13];
   out_7430843458597553837[14] = delta_x[14] + nom_x[14];
   out_7430843458597553837[15] = delta_x[15] + nom_x[15];
   out_7430843458597553837[16] = delta_x[16] + nom_x[16];
   out_7430843458597553837[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_8627048357023842126) {
   out_8627048357023842126[0] = -nom_x[0] + true_x[0];
   out_8627048357023842126[1] = -nom_x[1] + true_x[1];
   out_8627048357023842126[2] = -nom_x[2] + true_x[2];
   out_8627048357023842126[3] = -nom_x[3] + true_x[3];
   out_8627048357023842126[4] = -nom_x[4] + true_x[4];
   out_8627048357023842126[5] = -nom_x[5] + true_x[5];
   out_8627048357023842126[6] = -nom_x[6] + true_x[6];
   out_8627048357023842126[7] = -nom_x[7] + true_x[7];
   out_8627048357023842126[8] = -nom_x[8] + true_x[8];
   out_8627048357023842126[9] = -nom_x[9] + true_x[9];
   out_8627048357023842126[10] = -nom_x[10] + true_x[10];
   out_8627048357023842126[11] = -nom_x[11] + true_x[11];
   out_8627048357023842126[12] = -nom_x[12] + true_x[12];
   out_8627048357023842126[13] = -nom_x[13] + true_x[13];
   out_8627048357023842126[14] = -nom_x[14] + true_x[14];
   out_8627048357023842126[15] = -nom_x[15] + true_x[15];
   out_8627048357023842126[16] = -nom_x[16] + true_x[16];
   out_8627048357023842126[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_8834050469615675398) {
   out_8834050469615675398[0] = 1.0;
   out_8834050469615675398[1] = 0.0;
   out_8834050469615675398[2] = 0.0;
   out_8834050469615675398[3] = 0.0;
   out_8834050469615675398[4] = 0.0;
   out_8834050469615675398[5] = 0.0;
   out_8834050469615675398[6] = 0.0;
   out_8834050469615675398[7] = 0.0;
   out_8834050469615675398[8] = 0.0;
   out_8834050469615675398[9] = 0.0;
   out_8834050469615675398[10] = 0.0;
   out_8834050469615675398[11] = 0.0;
   out_8834050469615675398[12] = 0.0;
   out_8834050469615675398[13] = 0.0;
   out_8834050469615675398[14] = 0.0;
   out_8834050469615675398[15] = 0.0;
   out_8834050469615675398[16] = 0.0;
   out_8834050469615675398[17] = 0.0;
   out_8834050469615675398[18] = 0.0;
   out_8834050469615675398[19] = 1.0;
   out_8834050469615675398[20] = 0.0;
   out_8834050469615675398[21] = 0.0;
   out_8834050469615675398[22] = 0.0;
   out_8834050469615675398[23] = 0.0;
   out_8834050469615675398[24] = 0.0;
   out_8834050469615675398[25] = 0.0;
   out_8834050469615675398[26] = 0.0;
   out_8834050469615675398[27] = 0.0;
   out_8834050469615675398[28] = 0.0;
   out_8834050469615675398[29] = 0.0;
   out_8834050469615675398[30] = 0.0;
   out_8834050469615675398[31] = 0.0;
   out_8834050469615675398[32] = 0.0;
   out_8834050469615675398[33] = 0.0;
   out_8834050469615675398[34] = 0.0;
   out_8834050469615675398[35] = 0.0;
   out_8834050469615675398[36] = 0.0;
   out_8834050469615675398[37] = 0.0;
   out_8834050469615675398[38] = 1.0;
   out_8834050469615675398[39] = 0.0;
   out_8834050469615675398[40] = 0.0;
   out_8834050469615675398[41] = 0.0;
   out_8834050469615675398[42] = 0.0;
   out_8834050469615675398[43] = 0.0;
   out_8834050469615675398[44] = 0.0;
   out_8834050469615675398[45] = 0.0;
   out_8834050469615675398[46] = 0.0;
   out_8834050469615675398[47] = 0.0;
   out_8834050469615675398[48] = 0.0;
   out_8834050469615675398[49] = 0.0;
   out_8834050469615675398[50] = 0.0;
   out_8834050469615675398[51] = 0.0;
   out_8834050469615675398[52] = 0.0;
   out_8834050469615675398[53] = 0.0;
   out_8834050469615675398[54] = 0.0;
   out_8834050469615675398[55] = 0.0;
   out_8834050469615675398[56] = 0.0;
   out_8834050469615675398[57] = 1.0;
   out_8834050469615675398[58] = 0.0;
   out_8834050469615675398[59] = 0.0;
   out_8834050469615675398[60] = 0.0;
   out_8834050469615675398[61] = 0.0;
   out_8834050469615675398[62] = 0.0;
   out_8834050469615675398[63] = 0.0;
   out_8834050469615675398[64] = 0.0;
   out_8834050469615675398[65] = 0.0;
   out_8834050469615675398[66] = 0.0;
   out_8834050469615675398[67] = 0.0;
   out_8834050469615675398[68] = 0.0;
   out_8834050469615675398[69] = 0.0;
   out_8834050469615675398[70] = 0.0;
   out_8834050469615675398[71] = 0.0;
   out_8834050469615675398[72] = 0.0;
   out_8834050469615675398[73] = 0.0;
   out_8834050469615675398[74] = 0.0;
   out_8834050469615675398[75] = 0.0;
   out_8834050469615675398[76] = 1.0;
   out_8834050469615675398[77] = 0.0;
   out_8834050469615675398[78] = 0.0;
   out_8834050469615675398[79] = 0.0;
   out_8834050469615675398[80] = 0.0;
   out_8834050469615675398[81] = 0.0;
   out_8834050469615675398[82] = 0.0;
   out_8834050469615675398[83] = 0.0;
   out_8834050469615675398[84] = 0.0;
   out_8834050469615675398[85] = 0.0;
   out_8834050469615675398[86] = 0.0;
   out_8834050469615675398[87] = 0.0;
   out_8834050469615675398[88] = 0.0;
   out_8834050469615675398[89] = 0.0;
   out_8834050469615675398[90] = 0.0;
   out_8834050469615675398[91] = 0.0;
   out_8834050469615675398[92] = 0.0;
   out_8834050469615675398[93] = 0.0;
   out_8834050469615675398[94] = 0.0;
   out_8834050469615675398[95] = 1.0;
   out_8834050469615675398[96] = 0.0;
   out_8834050469615675398[97] = 0.0;
   out_8834050469615675398[98] = 0.0;
   out_8834050469615675398[99] = 0.0;
   out_8834050469615675398[100] = 0.0;
   out_8834050469615675398[101] = 0.0;
   out_8834050469615675398[102] = 0.0;
   out_8834050469615675398[103] = 0.0;
   out_8834050469615675398[104] = 0.0;
   out_8834050469615675398[105] = 0.0;
   out_8834050469615675398[106] = 0.0;
   out_8834050469615675398[107] = 0.0;
   out_8834050469615675398[108] = 0.0;
   out_8834050469615675398[109] = 0.0;
   out_8834050469615675398[110] = 0.0;
   out_8834050469615675398[111] = 0.0;
   out_8834050469615675398[112] = 0.0;
   out_8834050469615675398[113] = 0.0;
   out_8834050469615675398[114] = 1.0;
   out_8834050469615675398[115] = 0.0;
   out_8834050469615675398[116] = 0.0;
   out_8834050469615675398[117] = 0.0;
   out_8834050469615675398[118] = 0.0;
   out_8834050469615675398[119] = 0.0;
   out_8834050469615675398[120] = 0.0;
   out_8834050469615675398[121] = 0.0;
   out_8834050469615675398[122] = 0.0;
   out_8834050469615675398[123] = 0.0;
   out_8834050469615675398[124] = 0.0;
   out_8834050469615675398[125] = 0.0;
   out_8834050469615675398[126] = 0.0;
   out_8834050469615675398[127] = 0.0;
   out_8834050469615675398[128] = 0.0;
   out_8834050469615675398[129] = 0.0;
   out_8834050469615675398[130] = 0.0;
   out_8834050469615675398[131] = 0.0;
   out_8834050469615675398[132] = 0.0;
   out_8834050469615675398[133] = 1.0;
   out_8834050469615675398[134] = 0.0;
   out_8834050469615675398[135] = 0.0;
   out_8834050469615675398[136] = 0.0;
   out_8834050469615675398[137] = 0.0;
   out_8834050469615675398[138] = 0.0;
   out_8834050469615675398[139] = 0.0;
   out_8834050469615675398[140] = 0.0;
   out_8834050469615675398[141] = 0.0;
   out_8834050469615675398[142] = 0.0;
   out_8834050469615675398[143] = 0.0;
   out_8834050469615675398[144] = 0.0;
   out_8834050469615675398[145] = 0.0;
   out_8834050469615675398[146] = 0.0;
   out_8834050469615675398[147] = 0.0;
   out_8834050469615675398[148] = 0.0;
   out_8834050469615675398[149] = 0.0;
   out_8834050469615675398[150] = 0.0;
   out_8834050469615675398[151] = 0.0;
   out_8834050469615675398[152] = 1.0;
   out_8834050469615675398[153] = 0.0;
   out_8834050469615675398[154] = 0.0;
   out_8834050469615675398[155] = 0.0;
   out_8834050469615675398[156] = 0.0;
   out_8834050469615675398[157] = 0.0;
   out_8834050469615675398[158] = 0.0;
   out_8834050469615675398[159] = 0.0;
   out_8834050469615675398[160] = 0.0;
   out_8834050469615675398[161] = 0.0;
   out_8834050469615675398[162] = 0.0;
   out_8834050469615675398[163] = 0.0;
   out_8834050469615675398[164] = 0.0;
   out_8834050469615675398[165] = 0.0;
   out_8834050469615675398[166] = 0.0;
   out_8834050469615675398[167] = 0.0;
   out_8834050469615675398[168] = 0.0;
   out_8834050469615675398[169] = 0.0;
   out_8834050469615675398[170] = 0.0;
   out_8834050469615675398[171] = 1.0;
   out_8834050469615675398[172] = 0.0;
   out_8834050469615675398[173] = 0.0;
   out_8834050469615675398[174] = 0.0;
   out_8834050469615675398[175] = 0.0;
   out_8834050469615675398[176] = 0.0;
   out_8834050469615675398[177] = 0.0;
   out_8834050469615675398[178] = 0.0;
   out_8834050469615675398[179] = 0.0;
   out_8834050469615675398[180] = 0.0;
   out_8834050469615675398[181] = 0.0;
   out_8834050469615675398[182] = 0.0;
   out_8834050469615675398[183] = 0.0;
   out_8834050469615675398[184] = 0.0;
   out_8834050469615675398[185] = 0.0;
   out_8834050469615675398[186] = 0.0;
   out_8834050469615675398[187] = 0.0;
   out_8834050469615675398[188] = 0.0;
   out_8834050469615675398[189] = 0.0;
   out_8834050469615675398[190] = 1.0;
   out_8834050469615675398[191] = 0.0;
   out_8834050469615675398[192] = 0.0;
   out_8834050469615675398[193] = 0.0;
   out_8834050469615675398[194] = 0.0;
   out_8834050469615675398[195] = 0.0;
   out_8834050469615675398[196] = 0.0;
   out_8834050469615675398[197] = 0.0;
   out_8834050469615675398[198] = 0.0;
   out_8834050469615675398[199] = 0.0;
   out_8834050469615675398[200] = 0.0;
   out_8834050469615675398[201] = 0.0;
   out_8834050469615675398[202] = 0.0;
   out_8834050469615675398[203] = 0.0;
   out_8834050469615675398[204] = 0.0;
   out_8834050469615675398[205] = 0.0;
   out_8834050469615675398[206] = 0.0;
   out_8834050469615675398[207] = 0.0;
   out_8834050469615675398[208] = 0.0;
   out_8834050469615675398[209] = 1.0;
   out_8834050469615675398[210] = 0.0;
   out_8834050469615675398[211] = 0.0;
   out_8834050469615675398[212] = 0.0;
   out_8834050469615675398[213] = 0.0;
   out_8834050469615675398[214] = 0.0;
   out_8834050469615675398[215] = 0.0;
   out_8834050469615675398[216] = 0.0;
   out_8834050469615675398[217] = 0.0;
   out_8834050469615675398[218] = 0.0;
   out_8834050469615675398[219] = 0.0;
   out_8834050469615675398[220] = 0.0;
   out_8834050469615675398[221] = 0.0;
   out_8834050469615675398[222] = 0.0;
   out_8834050469615675398[223] = 0.0;
   out_8834050469615675398[224] = 0.0;
   out_8834050469615675398[225] = 0.0;
   out_8834050469615675398[226] = 0.0;
   out_8834050469615675398[227] = 0.0;
   out_8834050469615675398[228] = 1.0;
   out_8834050469615675398[229] = 0.0;
   out_8834050469615675398[230] = 0.0;
   out_8834050469615675398[231] = 0.0;
   out_8834050469615675398[232] = 0.0;
   out_8834050469615675398[233] = 0.0;
   out_8834050469615675398[234] = 0.0;
   out_8834050469615675398[235] = 0.0;
   out_8834050469615675398[236] = 0.0;
   out_8834050469615675398[237] = 0.0;
   out_8834050469615675398[238] = 0.0;
   out_8834050469615675398[239] = 0.0;
   out_8834050469615675398[240] = 0.0;
   out_8834050469615675398[241] = 0.0;
   out_8834050469615675398[242] = 0.0;
   out_8834050469615675398[243] = 0.0;
   out_8834050469615675398[244] = 0.0;
   out_8834050469615675398[245] = 0.0;
   out_8834050469615675398[246] = 0.0;
   out_8834050469615675398[247] = 1.0;
   out_8834050469615675398[248] = 0.0;
   out_8834050469615675398[249] = 0.0;
   out_8834050469615675398[250] = 0.0;
   out_8834050469615675398[251] = 0.0;
   out_8834050469615675398[252] = 0.0;
   out_8834050469615675398[253] = 0.0;
   out_8834050469615675398[254] = 0.0;
   out_8834050469615675398[255] = 0.0;
   out_8834050469615675398[256] = 0.0;
   out_8834050469615675398[257] = 0.0;
   out_8834050469615675398[258] = 0.0;
   out_8834050469615675398[259] = 0.0;
   out_8834050469615675398[260] = 0.0;
   out_8834050469615675398[261] = 0.0;
   out_8834050469615675398[262] = 0.0;
   out_8834050469615675398[263] = 0.0;
   out_8834050469615675398[264] = 0.0;
   out_8834050469615675398[265] = 0.0;
   out_8834050469615675398[266] = 1.0;
   out_8834050469615675398[267] = 0.0;
   out_8834050469615675398[268] = 0.0;
   out_8834050469615675398[269] = 0.0;
   out_8834050469615675398[270] = 0.0;
   out_8834050469615675398[271] = 0.0;
   out_8834050469615675398[272] = 0.0;
   out_8834050469615675398[273] = 0.0;
   out_8834050469615675398[274] = 0.0;
   out_8834050469615675398[275] = 0.0;
   out_8834050469615675398[276] = 0.0;
   out_8834050469615675398[277] = 0.0;
   out_8834050469615675398[278] = 0.0;
   out_8834050469615675398[279] = 0.0;
   out_8834050469615675398[280] = 0.0;
   out_8834050469615675398[281] = 0.0;
   out_8834050469615675398[282] = 0.0;
   out_8834050469615675398[283] = 0.0;
   out_8834050469615675398[284] = 0.0;
   out_8834050469615675398[285] = 1.0;
   out_8834050469615675398[286] = 0.0;
   out_8834050469615675398[287] = 0.0;
   out_8834050469615675398[288] = 0.0;
   out_8834050469615675398[289] = 0.0;
   out_8834050469615675398[290] = 0.0;
   out_8834050469615675398[291] = 0.0;
   out_8834050469615675398[292] = 0.0;
   out_8834050469615675398[293] = 0.0;
   out_8834050469615675398[294] = 0.0;
   out_8834050469615675398[295] = 0.0;
   out_8834050469615675398[296] = 0.0;
   out_8834050469615675398[297] = 0.0;
   out_8834050469615675398[298] = 0.0;
   out_8834050469615675398[299] = 0.0;
   out_8834050469615675398[300] = 0.0;
   out_8834050469615675398[301] = 0.0;
   out_8834050469615675398[302] = 0.0;
   out_8834050469615675398[303] = 0.0;
   out_8834050469615675398[304] = 1.0;
   out_8834050469615675398[305] = 0.0;
   out_8834050469615675398[306] = 0.0;
   out_8834050469615675398[307] = 0.0;
   out_8834050469615675398[308] = 0.0;
   out_8834050469615675398[309] = 0.0;
   out_8834050469615675398[310] = 0.0;
   out_8834050469615675398[311] = 0.0;
   out_8834050469615675398[312] = 0.0;
   out_8834050469615675398[313] = 0.0;
   out_8834050469615675398[314] = 0.0;
   out_8834050469615675398[315] = 0.0;
   out_8834050469615675398[316] = 0.0;
   out_8834050469615675398[317] = 0.0;
   out_8834050469615675398[318] = 0.0;
   out_8834050469615675398[319] = 0.0;
   out_8834050469615675398[320] = 0.0;
   out_8834050469615675398[321] = 0.0;
   out_8834050469615675398[322] = 0.0;
   out_8834050469615675398[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_6950331196878070812) {
   out_6950331196878070812[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_6950331196878070812[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_6950331196878070812[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_6950331196878070812[3] = dt*state[12] + state[3];
   out_6950331196878070812[4] = dt*state[13] + state[4];
   out_6950331196878070812[5] = dt*state[14] + state[5];
   out_6950331196878070812[6] = state[6];
   out_6950331196878070812[7] = state[7];
   out_6950331196878070812[8] = state[8];
   out_6950331196878070812[9] = state[9];
   out_6950331196878070812[10] = state[10];
   out_6950331196878070812[11] = state[11];
   out_6950331196878070812[12] = state[12];
   out_6950331196878070812[13] = state[13];
   out_6950331196878070812[14] = state[14];
   out_6950331196878070812[15] = state[15];
   out_6950331196878070812[16] = state[16];
   out_6950331196878070812[17] = state[17];
}
void F_fun(double *state, double dt, double *out_2568972817437578706) {
   out_2568972817437578706[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2568972817437578706[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2568972817437578706[2] = 0;
   out_2568972817437578706[3] = 0;
   out_2568972817437578706[4] = 0;
   out_2568972817437578706[5] = 0;
   out_2568972817437578706[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2568972817437578706[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2568972817437578706[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2568972817437578706[9] = 0;
   out_2568972817437578706[10] = 0;
   out_2568972817437578706[11] = 0;
   out_2568972817437578706[12] = 0;
   out_2568972817437578706[13] = 0;
   out_2568972817437578706[14] = 0;
   out_2568972817437578706[15] = 0;
   out_2568972817437578706[16] = 0;
   out_2568972817437578706[17] = 0;
   out_2568972817437578706[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2568972817437578706[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2568972817437578706[20] = 0;
   out_2568972817437578706[21] = 0;
   out_2568972817437578706[22] = 0;
   out_2568972817437578706[23] = 0;
   out_2568972817437578706[24] = 0;
   out_2568972817437578706[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2568972817437578706[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2568972817437578706[27] = 0;
   out_2568972817437578706[28] = 0;
   out_2568972817437578706[29] = 0;
   out_2568972817437578706[30] = 0;
   out_2568972817437578706[31] = 0;
   out_2568972817437578706[32] = 0;
   out_2568972817437578706[33] = 0;
   out_2568972817437578706[34] = 0;
   out_2568972817437578706[35] = 0;
   out_2568972817437578706[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2568972817437578706[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2568972817437578706[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2568972817437578706[39] = 0;
   out_2568972817437578706[40] = 0;
   out_2568972817437578706[41] = 0;
   out_2568972817437578706[42] = 0;
   out_2568972817437578706[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2568972817437578706[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2568972817437578706[45] = 0;
   out_2568972817437578706[46] = 0;
   out_2568972817437578706[47] = 0;
   out_2568972817437578706[48] = 0;
   out_2568972817437578706[49] = 0;
   out_2568972817437578706[50] = 0;
   out_2568972817437578706[51] = 0;
   out_2568972817437578706[52] = 0;
   out_2568972817437578706[53] = 0;
   out_2568972817437578706[54] = 0;
   out_2568972817437578706[55] = 0;
   out_2568972817437578706[56] = 0;
   out_2568972817437578706[57] = 1;
   out_2568972817437578706[58] = 0;
   out_2568972817437578706[59] = 0;
   out_2568972817437578706[60] = 0;
   out_2568972817437578706[61] = 0;
   out_2568972817437578706[62] = 0;
   out_2568972817437578706[63] = 0;
   out_2568972817437578706[64] = 0;
   out_2568972817437578706[65] = 0;
   out_2568972817437578706[66] = dt;
   out_2568972817437578706[67] = 0;
   out_2568972817437578706[68] = 0;
   out_2568972817437578706[69] = 0;
   out_2568972817437578706[70] = 0;
   out_2568972817437578706[71] = 0;
   out_2568972817437578706[72] = 0;
   out_2568972817437578706[73] = 0;
   out_2568972817437578706[74] = 0;
   out_2568972817437578706[75] = 0;
   out_2568972817437578706[76] = 1;
   out_2568972817437578706[77] = 0;
   out_2568972817437578706[78] = 0;
   out_2568972817437578706[79] = 0;
   out_2568972817437578706[80] = 0;
   out_2568972817437578706[81] = 0;
   out_2568972817437578706[82] = 0;
   out_2568972817437578706[83] = 0;
   out_2568972817437578706[84] = 0;
   out_2568972817437578706[85] = dt;
   out_2568972817437578706[86] = 0;
   out_2568972817437578706[87] = 0;
   out_2568972817437578706[88] = 0;
   out_2568972817437578706[89] = 0;
   out_2568972817437578706[90] = 0;
   out_2568972817437578706[91] = 0;
   out_2568972817437578706[92] = 0;
   out_2568972817437578706[93] = 0;
   out_2568972817437578706[94] = 0;
   out_2568972817437578706[95] = 1;
   out_2568972817437578706[96] = 0;
   out_2568972817437578706[97] = 0;
   out_2568972817437578706[98] = 0;
   out_2568972817437578706[99] = 0;
   out_2568972817437578706[100] = 0;
   out_2568972817437578706[101] = 0;
   out_2568972817437578706[102] = 0;
   out_2568972817437578706[103] = 0;
   out_2568972817437578706[104] = dt;
   out_2568972817437578706[105] = 0;
   out_2568972817437578706[106] = 0;
   out_2568972817437578706[107] = 0;
   out_2568972817437578706[108] = 0;
   out_2568972817437578706[109] = 0;
   out_2568972817437578706[110] = 0;
   out_2568972817437578706[111] = 0;
   out_2568972817437578706[112] = 0;
   out_2568972817437578706[113] = 0;
   out_2568972817437578706[114] = 1;
   out_2568972817437578706[115] = 0;
   out_2568972817437578706[116] = 0;
   out_2568972817437578706[117] = 0;
   out_2568972817437578706[118] = 0;
   out_2568972817437578706[119] = 0;
   out_2568972817437578706[120] = 0;
   out_2568972817437578706[121] = 0;
   out_2568972817437578706[122] = 0;
   out_2568972817437578706[123] = 0;
   out_2568972817437578706[124] = 0;
   out_2568972817437578706[125] = 0;
   out_2568972817437578706[126] = 0;
   out_2568972817437578706[127] = 0;
   out_2568972817437578706[128] = 0;
   out_2568972817437578706[129] = 0;
   out_2568972817437578706[130] = 0;
   out_2568972817437578706[131] = 0;
   out_2568972817437578706[132] = 0;
   out_2568972817437578706[133] = 1;
   out_2568972817437578706[134] = 0;
   out_2568972817437578706[135] = 0;
   out_2568972817437578706[136] = 0;
   out_2568972817437578706[137] = 0;
   out_2568972817437578706[138] = 0;
   out_2568972817437578706[139] = 0;
   out_2568972817437578706[140] = 0;
   out_2568972817437578706[141] = 0;
   out_2568972817437578706[142] = 0;
   out_2568972817437578706[143] = 0;
   out_2568972817437578706[144] = 0;
   out_2568972817437578706[145] = 0;
   out_2568972817437578706[146] = 0;
   out_2568972817437578706[147] = 0;
   out_2568972817437578706[148] = 0;
   out_2568972817437578706[149] = 0;
   out_2568972817437578706[150] = 0;
   out_2568972817437578706[151] = 0;
   out_2568972817437578706[152] = 1;
   out_2568972817437578706[153] = 0;
   out_2568972817437578706[154] = 0;
   out_2568972817437578706[155] = 0;
   out_2568972817437578706[156] = 0;
   out_2568972817437578706[157] = 0;
   out_2568972817437578706[158] = 0;
   out_2568972817437578706[159] = 0;
   out_2568972817437578706[160] = 0;
   out_2568972817437578706[161] = 0;
   out_2568972817437578706[162] = 0;
   out_2568972817437578706[163] = 0;
   out_2568972817437578706[164] = 0;
   out_2568972817437578706[165] = 0;
   out_2568972817437578706[166] = 0;
   out_2568972817437578706[167] = 0;
   out_2568972817437578706[168] = 0;
   out_2568972817437578706[169] = 0;
   out_2568972817437578706[170] = 0;
   out_2568972817437578706[171] = 1;
   out_2568972817437578706[172] = 0;
   out_2568972817437578706[173] = 0;
   out_2568972817437578706[174] = 0;
   out_2568972817437578706[175] = 0;
   out_2568972817437578706[176] = 0;
   out_2568972817437578706[177] = 0;
   out_2568972817437578706[178] = 0;
   out_2568972817437578706[179] = 0;
   out_2568972817437578706[180] = 0;
   out_2568972817437578706[181] = 0;
   out_2568972817437578706[182] = 0;
   out_2568972817437578706[183] = 0;
   out_2568972817437578706[184] = 0;
   out_2568972817437578706[185] = 0;
   out_2568972817437578706[186] = 0;
   out_2568972817437578706[187] = 0;
   out_2568972817437578706[188] = 0;
   out_2568972817437578706[189] = 0;
   out_2568972817437578706[190] = 1;
   out_2568972817437578706[191] = 0;
   out_2568972817437578706[192] = 0;
   out_2568972817437578706[193] = 0;
   out_2568972817437578706[194] = 0;
   out_2568972817437578706[195] = 0;
   out_2568972817437578706[196] = 0;
   out_2568972817437578706[197] = 0;
   out_2568972817437578706[198] = 0;
   out_2568972817437578706[199] = 0;
   out_2568972817437578706[200] = 0;
   out_2568972817437578706[201] = 0;
   out_2568972817437578706[202] = 0;
   out_2568972817437578706[203] = 0;
   out_2568972817437578706[204] = 0;
   out_2568972817437578706[205] = 0;
   out_2568972817437578706[206] = 0;
   out_2568972817437578706[207] = 0;
   out_2568972817437578706[208] = 0;
   out_2568972817437578706[209] = 1;
   out_2568972817437578706[210] = 0;
   out_2568972817437578706[211] = 0;
   out_2568972817437578706[212] = 0;
   out_2568972817437578706[213] = 0;
   out_2568972817437578706[214] = 0;
   out_2568972817437578706[215] = 0;
   out_2568972817437578706[216] = 0;
   out_2568972817437578706[217] = 0;
   out_2568972817437578706[218] = 0;
   out_2568972817437578706[219] = 0;
   out_2568972817437578706[220] = 0;
   out_2568972817437578706[221] = 0;
   out_2568972817437578706[222] = 0;
   out_2568972817437578706[223] = 0;
   out_2568972817437578706[224] = 0;
   out_2568972817437578706[225] = 0;
   out_2568972817437578706[226] = 0;
   out_2568972817437578706[227] = 0;
   out_2568972817437578706[228] = 1;
   out_2568972817437578706[229] = 0;
   out_2568972817437578706[230] = 0;
   out_2568972817437578706[231] = 0;
   out_2568972817437578706[232] = 0;
   out_2568972817437578706[233] = 0;
   out_2568972817437578706[234] = 0;
   out_2568972817437578706[235] = 0;
   out_2568972817437578706[236] = 0;
   out_2568972817437578706[237] = 0;
   out_2568972817437578706[238] = 0;
   out_2568972817437578706[239] = 0;
   out_2568972817437578706[240] = 0;
   out_2568972817437578706[241] = 0;
   out_2568972817437578706[242] = 0;
   out_2568972817437578706[243] = 0;
   out_2568972817437578706[244] = 0;
   out_2568972817437578706[245] = 0;
   out_2568972817437578706[246] = 0;
   out_2568972817437578706[247] = 1;
   out_2568972817437578706[248] = 0;
   out_2568972817437578706[249] = 0;
   out_2568972817437578706[250] = 0;
   out_2568972817437578706[251] = 0;
   out_2568972817437578706[252] = 0;
   out_2568972817437578706[253] = 0;
   out_2568972817437578706[254] = 0;
   out_2568972817437578706[255] = 0;
   out_2568972817437578706[256] = 0;
   out_2568972817437578706[257] = 0;
   out_2568972817437578706[258] = 0;
   out_2568972817437578706[259] = 0;
   out_2568972817437578706[260] = 0;
   out_2568972817437578706[261] = 0;
   out_2568972817437578706[262] = 0;
   out_2568972817437578706[263] = 0;
   out_2568972817437578706[264] = 0;
   out_2568972817437578706[265] = 0;
   out_2568972817437578706[266] = 1;
   out_2568972817437578706[267] = 0;
   out_2568972817437578706[268] = 0;
   out_2568972817437578706[269] = 0;
   out_2568972817437578706[270] = 0;
   out_2568972817437578706[271] = 0;
   out_2568972817437578706[272] = 0;
   out_2568972817437578706[273] = 0;
   out_2568972817437578706[274] = 0;
   out_2568972817437578706[275] = 0;
   out_2568972817437578706[276] = 0;
   out_2568972817437578706[277] = 0;
   out_2568972817437578706[278] = 0;
   out_2568972817437578706[279] = 0;
   out_2568972817437578706[280] = 0;
   out_2568972817437578706[281] = 0;
   out_2568972817437578706[282] = 0;
   out_2568972817437578706[283] = 0;
   out_2568972817437578706[284] = 0;
   out_2568972817437578706[285] = 1;
   out_2568972817437578706[286] = 0;
   out_2568972817437578706[287] = 0;
   out_2568972817437578706[288] = 0;
   out_2568972817437578706[289] = 0;
   out_2568972817437578706[290] = 0;
   out_2568972817437578706[291] = 0;
   out_2568972817437578706[292] = 0;
   out_2568972817437578706[293] = 0;
   out_2568972817437578706[294] = 0;
   out_2568972817437578706[295] = 0;
   out_2568972817437578706[296] = 0;
   out_2568972817437578706[297] = 0;
   out_2568972817437578706[298] = 0;
   out_2568972817437578706[299] = 0;
   out_2568972817437578706[300] = 0;
   out_2568972817437578706[301] = 0;
   out_2568972817437578706[302] = 0;
   out_2568972817437578706[303] = 0;
   out_2568972817437578706[304] = 1;
   out_2568972817437578706[305] = 0;
   out_2568972817437578706[306] = 0;
   out_2568972817437578706[307] = 0;
   out_2568972817437578706[308] = 0;
   out_2568972817437578706[309] = 0;
   out_2568972817437578706[310] = 0;
   out_2568972817437578706[311] = 0;
   out_2568972817437578706[312] = 0;
   out_2568972817437578706[313] = 0;
   out_2568972817437578706[314] = 0;
   out_2568972817437578706[315] = 0;
   out_2568972817437578706[316] = 0;
   out_2568972817437578706[317] = 0;
   out_2568972817437578706[318] = 0;
   out_2568972817437578706[319] = 0;
   out_2568972817437578706[320] = 0;
   out_2568972817437578706[321] = 0;
   out_2568972817437578706[322] = 0;
   out_2568972817437578706[323] = 1;
}
void h_4(double *state, double *unused, double *out_2596008029261534560) {
   out_2596008029261534560[0] = state[6] + state[9];
   out_2596008029261534560[1] = state[7] + state[10];
   out_2596008029261534560[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_6377697350598801949) {
   out_6377697350598801949[0] = 0;
   out_6377697350598801949[1] = 0;
   out_6377697350598801949[2] = 0;
   out_6377697350598801949[3] = 0;
   out_6377697350598801949[4] = 0;
   out_6377697350598801949[5] = 0;
   out_6377697350598801949[6] = 1;
   out_6377697350598801949[7] = 0;
   out_6377697350598801949[8] = 0;
   out_6377697350598801949[9] = 1;
   out_6377697350598801949[10] = 0;
   out_6377697350598801949[11] = 0;
   out_6377697350598801949[12] = 0;
   out_6377697350598801949[13] = 0;
   out_6377697350598801949[14] = 0;
   out_6377697350598801949[15] = 0;
   out_6377697350598801949[16] = 0;
   out_6377697350598801949[17] = 0;
   out_6377697350598801949[18] = 0;
   out_6377697350598801949[19] = 0;
   out_6377697350598801949[20] = 0;
   out_6377697350598801949[21] = 0;
   out_6377697350598801949[22] = 0;
   out_6377697350598801949[23] = 0;
   out_6377697350598801949[24] = 0;
   out_6377697350598801949[25] = 1;
   out_6377697350598801949[26] = 0;
   out_6377697350598801949[27] = 0;
   out_6377697350598801949[28] = 1;
   out_6377697350598801949[29] = 0;
   out_6377697350598801949[30] = 0;
   out_6377697350598801949[31] = 0;
   out_6377697350598801949[32] = 0;
   out_6377697350598801949[33] = 0;
   out_6377697350598801949[34] = 0;
   out_6377697350598801949[35] = 0;
   out_6377697350598801949[36] = 0;
   out_6377697350598801949[37] = 0;
   out_6377697350598801949[38] = 0;
   out_6377697350598801949[39] = 0;
   out_6377697350598801949[40] = 0;
   out_6377697350598801949[41] = 0;
   out_6377697350598801949[42] = 0;
   out_6377697350598801949[43] = 0;
   out_6377697350598801949[44] = 1;
   out_6377697350598801949[45] = 0;
   out_6377697350598801949[46] = 0;
   out_6377697350598801949[47] = 1;
   out_6377697350598801949[48] = 0;
   out_6377697350598801949[49] = 0;
   out_6377697350598801949[50] = 0;
   out_6377697350598801949[51] = 0;
   out_6377697350598801949[52] = 0;
   out_6377697350598801949[53] = 0;
}
void h_10(double *state, double *unused, double *out_2165897035409663064) {
   out_2165897035409663064[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_2165897035409663064[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_2165897035409663064[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_3933408193679582807) {
   out_3933408193679582807[0] = 0;
   out_3933408193679582807[1] = 9.8100000000000005*cos(state[1]);
   out_3933408193679582807[2] = 0;
   out_3933408193679582807[3] = 0;
   out_3933408193679582807[4] = -state[8];
   out_3933408193679582807[5] = state[7];
   out_3933408193679582807[6] = 0;
   out_3933408193679582807[7] = state[5];
   out_3933408193679582807[8] = -state[4];
   out_3933408193679582807[9] = 0;
   out_3933408193679582807[10] = 0;
   out_3933408193679582807[11] = 0;
   out_3933408193679582807[12] = 1;
   out_3933408193679582807[13] = 0;
   out_3933408193679582807[14] = 0;
   out_3933408193679582807[15] = 1;
   out_3933408193679582807[16] = 0;
   out_3933408193679582807[17] = 0;
   out_3933408193679582807[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_3933408193679582807[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_3933408193679582807[20] = 0;
   out_3933408193679582807[21] = state[8];
   out_3933408193679582807[22] = 0;
   out_3933408193679582807[23] = -state[6];
   out_3933408193679582807[24] = -state[5];
   out_3933408193679582807[25] = 0;
   out_3933408193679582807[26] = state[3];
   out_3933408193679582807[27] = 0;
   out_3933408193679582807[28] = 0;
   out_3933408193679582807[29] = 0;
   out_3933408193679582807[30] = 0;
   out_3933408193679582807[31] = 1;
   out_3933408193679582807[32] = 0;
   out_3933408193679582807[33] = 0;
   out_3933408193679582807[34] = 1;
   out_3933408193679582807[35] = 0;
   out_3933408193679582807[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_3933408193679582807[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_3933408193679582807[38] = 0;
   out_3933408193679582807[39] = -state[7];
   out_3933408193679582807[40] = state[6];
   out_3933408193679582807[41] = 0;
   out_3933408193679582807[42] = state[4];
   out_3933408193679582807[43] = -state[3];
   out_3933408193679582807[44] = 0;
   out_3933408193679582807[45] = 0;
   out_3933408193679582807[46] = 0;
   out_3933408193679582807[47] = 0;
   out_3933408193679582807[48] = 0;
   out_3933408193679582807[49] = 0;
   out_3933408193679582807[50] = 1;
   out_3933408193679582807[51] = 0;
   out_3933408193679582807[52] = 0;
   out_3933408193679582807[53] = 1;
}
void h_13(double *state, double *unused, double *out_8398353702287059752) {
   out_8398353702287059752[0] = state[3];
   out_8398353702287059752[1] = state[4];
   out_8398353702287059752[2] = state[5];
}
void H_13(double *state, double *unused, double *out_3165423525266469148) {
   out_3165423525266469148[0] = 0;
   out_3165423525266469148[1] = 0;
   out_3165423525266469148[2] = 0;
   out_3165423525266469148[3] = 1;
   out_3165423525266469148[4] = 0;
   out_3165423525266469148[5] = 0;
   out_3165423525266469148[6] = 0;
   out_3165423525266469148[7] = 0;
   out_3165423525266469148[8] = 0;
   out_3165423525266469148[9] = 0;
   out_3165423525266469148[10] = 0;
   out_3165423525266469148[11] = 0;
   out_3165423525266469148[12] = 0;
   out_3165423525266469148[13] = 0;
   out_3165423525266469148[14] = 0;
   out_3165423525266469148[15] = 0;
   out_3165423525266469148[16] = 0;
   out_3165423525266469148[17] = 0;
   out_3165423525266469148[18] = 0;
   out_3165423525266469148[19] = 0;
   out_3165423525266469148[20] = 0;
   out_3165423525266469148[21] = 0;
   out_3165423525266469148[22] = 1;
   out_3165423525266469148[23] = 0;
   out_3165423525266469148[24] = 0;
   out_3165423525266469148[25] = 0;
   out_3165423525266469148[26] = 0;
   out_3165423525266469148[27] = 0;
   out_3165423525266469148[28] = 0;
   out_3165423525266469148[29] = 0;
   out_3165423525266469148[30] = 0;
   out_3165423525266469148[31] = 0;
   out_3165423525266469148[32] = 0;
   out_3165423525266469148[33] = 0;
   out_3165423525266469148[34] = 0;
   out_3165423525266469148[35] = 0;
   out_3165423525266469148[36] = 0;
   out_3165423525266469148[37] = 0;
   out_3165423525266469148[38] = 0;
   out_3165423525266469148[39] = 0;
   out_3165423525266469148[40] = 0;
   out_3165423525266469148[41] = 1;
   out_3165423525266469148[42] = 0;
   out_3165423525266469148[43] = 0;
   out_3165423525266469148[44] = 0;
   out_3165423525266469148[45] = 0;
   out_3165423525266469148[46] = 0;
   out_3165423525266469148[47] = 0;
   out_3165423525266469148[48] = 0;
   out_3165423525266469148[49] = 0;
   out_3165423525266469148[50] = 0;
   out_3165423525266469148[51] = 0;
   out_3165423525266469148[52] = 0;
   out_3165423525266469148[53] = 0;
}
void h_14(double *state, double *unused, double *out_961638628362298617) {
   out_961638628362298617[0] = state[6];
   out_961638628362298617[1] = state[7];
   out_961638628362298617[2] = state[8];
}
void H_14(double *state, double *unused, double *out_2414456494259317420) {
   out_2414456494259317420[0] = 0;
   out_2414456494259317420[1] = 0;
   out_2414456494259317420[2] = 0;
   out_2414456494259317420[3] = 0;
   out_2414456494259317420[4] = 0;
   out_2414456494259317420[5] = 0;
   out_2414456494259317420[6] = 1;
   out_2414456494259317420[7] = 0;
   out_2414456494259317420[8] = 0;
   out_2414456494259317420[9] = 0;
   out_2414456494259317420[10] = 0;
   out_2414456494259317420[11] = 0;
   out_2414456494259317420[12] = 0;
   out_2414456494259317420[13] = 0;
   out_2414456494259317420[14] = 0;
   out_2414456494259317420[15] = 0;
   out_2414456494259317420[16] = 0;
   out_2414456494259317420[17] = 0;
   out_2414456494259317420[18] = 0;
   out_2414456494259317420[19] = 0;
   out_2414456494259317420[20] = 0;
   out_2414456494259317420[21] = 0;
   out_2414456494259317420[22] = 0;
   out_2414456494259317420[23] = 0;
   out_2414456494259317420[24] = 0;
   out_2414456494259317420[25] = 1;
   out_2414456494259317420[26] = 0;
   out_2414456494259317420[27] = 0;
   out_2414456494259317420[28] = 0;
   out_2414456494259317420[29] = 0;
   out_2414456494259317420[30] = 0;
   out_2414456494259317420[31] = 0;
   out_2414456494259317420[32] = 0;
   out_2414456494259317420[33] = 0;
   out_2414456494259317420[34] = 0;
   out_2414456494259317420[35] = 0;
   out_2414456494259317420[36] = 0;
   out_2414456494259317420[37] = 0;
   out_2414456494259317420[38] = 0;
   out_2414456494259317420[39] = 0;
   out_2414456494259317420[40] = 0;
   out_2414456494259317420[41] = 0;
   out_2414456494259317420[42] = 0;
   out_2414456494259317420[43] = 0;
   out_2414456494259317420[44] = 1;
   out_2414456494259317420[45] = 0;
   out_2414456494259317420[46] = 0;
   out_2414456494259317420[47] = 0;
   out_2414456494259317420[48] = 0;
   out_2414456494259317420[49] = 0;
   out_2414456494259317420[50] = 0;
   out_2414456494259317420[51] = 0;
   out_2414456494259317420[52] = 0;
   out_2414456494259317420[53] = 0;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_7430843458597553837) {
  err_fun(nom_x, delta_x, out_7430843458597553837);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_8627048357023842126) {
  inv_err_fun(nom_x, true_x, out_8627048357023842126);
}
void pose_H_mod_fun(double *state, double *out_8834050469615675398) {
  H_mod_fun(state, out_8834050469615675398);
}
void pose_f_fun(double *state, double dt, double *out_6950331196878070812) {
  f_fun(state,  dt, out_6950331196878070812);
}
void pose_F_fun(double *state, double dt, double *out_2568972817437578706) {
  F_fun(state,  dt, out_2568972817437578706);
}
void pose_h_4(double *state, double *unused, double *out_2596008029261534560) {
  h_4(state, unused, out_2596008029261534560);
}
void pose_H_4(double *state, double *unused, double *out_6377697350598801949) {
  H_4(state, unused, out_6377697350598801949);
}
void pose_h_10(double *state, double *unused, double *out_2165897035409663064) {
  h_10(state, unused, out_2165897035409663064);
}
void pose_H_10(double *state, double *unused, double *out_3933408193679582807) {
  H_10(state, unused, out_3933408193679582807);
}
void pose_h_13(double *state, double *unused, double *out_8398353702287059752) {
  h_13(state, unused, out_8398353702287059752);
}
void pose_H_13(double *state, double *unused, double *out_3165423525266469148) {
  H_13(state, unused, out_3165423525266469148);
}
void pose_h_14(double *state, double *unused, double *out_961638628362298617) {
  h_14(state, unused, out_961638628362298617);
}
void pose_H_14(double *state, double *unused, double *out_2414456494259317420) {
  H_14(state, unused, out_2414456494259317420);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
