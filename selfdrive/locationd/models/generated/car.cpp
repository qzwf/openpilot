#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_2042278345473927113) {
   out_2042278345473927113[0] = delta_x[0] + nom_x[0];
   out_2042278345473927113[1] = delta_x[1] + nom_x[1];
   out_2042278345473927113[2] = delta_x[2] + nom_x[2];
   out_2042278345473927113[3] = delta_x[3] + nom_x[3];
   out_2042278345473927113[4] = delta_x[4] + nom_x[4];
   out_2042278345473927113[5] = delta_x[5] + nom_x[5];
   out_2042278345473927113[6] = delta_x[6] + nom_x[6];
   out_2042278345473927113[7] = delta_x[7] + nom_x[7];
   out_2042278345473927113[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5398886998709636999) {
   out_5398886998709636999[0] = -nom_x[0] + true_x[0];
   out_5398886998709636999[1] = -nom_x[1] + true_x[1];
   out_5398886998709636999[2] = -nom_x[2] + true_x[2];
   out_5398886998709636999[3] = -nom_x[3] + true_x[3];
   out_5398886998709636999[4] = -nom_x[4] + true_x[4];
   out_5398886998709636999[5] = -nom_x[5] + true_x[5];
   out_5398886998709636999[6] = -nom_x[6] + true_x[6];
   out_5398886998709636999[7] = -nom_x[7] + true_x[7];
   out_5398886998709636999[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_3065330649401547536) {
   out_3065330649401547536[0] = 1.0;
   out_3065330649401547536[1] = 0.0;
   out_3065330649401547536[2] = 0.0;
   out_3065330649401547536[3] = 0.0;
   out_3065330649401547536[4] = 0.0;
   out_3065330649401547536[5] = 0.0;
   out_3065330649401547536[6] = 0.0;
   out_3065330649401547536[7] = 0.0;
   out_3065330649401547536[8] = 0.0;
   out_3065330649401547536[9] = 0.0;
   out_3065330649401547536[10] = 1.0;
   out_3065330649401547536[11] = 0.0;
   out_3065330649401547536[12] = 0.0;
   out_3065330649401547536[13] = 0.0;
   out_3065330649401547536[14] = 0.0;
   out_3065330649401547536[15] = 0.0;
   out_3065330649401547536[16] = 0.0;
   out_3065330649401547536[17] = 0.0;
   out_3065330649401547536[18] = 0.0;
   out_3065330649401547536[19] = 0.0;
   out_3065330649401547536[20] = 1.0;
   out_3065330649401547536[21] = 0.0;
   out_3065330649401547536[22] = 0.0;
   out_3065330649401547536[23] = 0.0;
   out_3065330649401547536[24] = 0.0;
   out_3065330649401547536[25] = 0.0;
   out_3065330649401547536[26] = 0.0;
   out_3065330649401547536[27] = 0.0;
   out_3065330649401547536[28] = 0.0;
   out_3065330649401547536[29] = 0.0;
   out_3065330649401547536[30] = 1.0;
   out_3065330649401547536[31] = 0.0;
   out_3065330649401547536[32] = 0.0;
   out_3065330649401547536[33] = 0.0;
   out_3065330649401547536[34] = 0.0;
   out_3065330649401547536[35] = 0.0;
   out_3065330649401547536[36] = 0.0;
   out_3065330649401547536[37] = 0.0;
   out_3065330649401547536[38] = 0.0;
   out_3065330649401547536[39] = 0.0;
   out_3065330649401547536[40] = 1.0;
   out_3065330649401547536[41] = 0.0;
   out_3065330649401547536[42] = 0.0;
   out_3065330649401547536[43] = 0.0;
   out_3065330649401547536[44] = 0.0;
   out_3065330649401547536[45] = 0.0;
   out_3065330649401547536[46] = 0.0;
   out_3065330649401547536[47] = 0.0;
   out_3065330649401547536[48] = 0.0;
   out_3065330649401547536[49] = 0.0;
   out_3065330649401547536[50] = 1.0;
   out_3065330649401547536[51] = 0.0;
   out_3065330649401547536[52] = 0.0;
   out_3065330649401547536[53] = 0.0;
   out_3065330649401547536[54] = 0.0;
   out_3065330649401547536[55] = 0.0;
   out_3065330649401547536[56] = 0.0;
   out_3065330649401547536[57] = 0.0;
   out_3065330649401547536[58] = 0.0;
   out_3065330649401547536[59] = 0.0;
   out_3065330649401547536[60] = 1.0;
   out_3065330649401547536[61] = 0.0;
   out_3065330649401547536[62] = 0.0;
   out_3065330649401547536[63] = 0.0;
   out_3065330649401547536[64] = 0.0;
   out_3065330649401547536[65] = 0.0;
   out_3065330649401547536[66] = 0.0;
   out_3065330649401547536[67] = 0.0;
   out_3065330649401547536[68] = 0.0;
   out_3065330649401547536[69] = 0.0;
   out_3065330649401547536[70] = 1.0;
   out_3065330649401547536[71] = 0.0;
   out_3065330649401547536[72] = 0.0;
   out_3065330649401547536[73] = 0.0;
   out_3065330649401547536[74] = 0.0;
   out_3065330649401547536[75] = 0.0;
   out_3065330649401547536[76] = 0.0;
   out_3065330649401547536[77] = 0.0;
   out_3065330649401547536[78] = 0.0;
   out_3065330649401547536[79] = 0.0;
   out_3065330649401547536[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_1469741008022123633) {
   out_1469741008022123633[0] = state[0];
   out_1469741008022123633[1] = state[1];
   out_1469741008022123633[2] = state[2];
   out_1469741008022123633[3] = state[3];
   out_1469741008022123633[4] = state[4];
   out_1469741008022123633[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_1469741008022123633[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_1469741008022123633[7] = state[7];
   out_1469741008022123633[8] = state[8];
}
void F_fun(double *state, double dt, double *out_1868155958597503509) {
   out_1868155958597503509[0] = 1;
   out_1868155958597503509[1] = 0;
   out_1868155958597503509[2] = 0;
   out_1868155958597503509[3] = 0;
   out_1868155958597503509[4] = 0;
   out_1868155958597503509[5] = 0;
   out_1868155958597503509[6] = 0;
   out_1868155958597503509[7] = 0;
   out_1868155958597503509[8] = 0;
   out_1868155958597503509[9] = 0;
   out_1868155958597503509[10] = 1;
   out_1868155958597503509[11] = 0;
   out_1868155958597503509[12] = 0;
   out_1868155958597503509[13] = 0;
   out_1868155958597503509[14] = 0;
   out_1868155958597503509[15] = 0;
   out_1868155958597503509[16] = 0;
   out_1868155958597503509[17] = 0;
   out_1868155958597503509[18] = 0;
   out_1868155958597503509[19] = 0;
   out_1868155958597503509[20] = 1;
   out_1868155958597503509[21] = 0;
   out_1868155958597503509[22] = 0;
   out_1868155958597503509[23] = 0;
   out_1868155958597503509[24] = 0;
   out_1868155958597503509[25] = 0;
   out_1868155958597503509[26] = 0;
   out_1868155958597503509[27] = 0;
   out_1868155958597503509[28] = 0;
   out_1868155958597503509[29] = 0;
   out_1868155958597503509[30] = 1;
   out_1868155958597503509[31] = 0;
   out_1868155958597503509[32] = 0;
   out_1868155958597503509[33] = 0;
   out_1868155958597503509[34] = 0;
   out_1868155958597503509[35] = 0;
   out_1868155958597503509[36] = 0;
   out_1868155958597503509[37] = 0;
   out_1868155958597503509[38] = 0;
   out_1868155958597503509[39] = 0;
   out_1868155958597503509[40] = 1;
   out_1868155958597503509[41] = 0;
   out_1868155958597503509[42] = 0;
   out_1868155958597503509[43] = 0;
   out_1868155958597503509[44] = 0;
   out_1868155958597503509[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_1868155958597503509[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_1868155958597503509[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1868155958597503509[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1868155958597503509[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_1868155958597503509[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_1868155958597503509[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_1868155958597503509[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_1868155958597503509[53] = -9.8100000000000005*dt;
   out_1868155958597503509[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_1868155958597503509[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_1868155958597503509[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1868155958597503509[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1868155958597503509[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_1868155958597503509[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_1868155958597503509[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_1868155958597503509[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1868155958597503509[62] = 0;
   out_1868155958597503509[63] = 0;
   out_1868155958597503509[64] = 0;
   out_1868155958597503509[65] = 0;
   out_1868155958597503509[66] = 0;
   out_1868155958597503509[67] = 0;
   out_1868155958597503509[68] = 0;
   out_1868155958597503509[69] = 0;
   out_1868155958597503509[70] = 1;
   out_1868155958597503509[71] = 0;
   out_1868155958597503509[72] = 0;
   out_1868155958597503509[73] = 0;
   out_1868155958597503509[74] = 0;
   out_1868155958597503509[75] = 0;
   out_1868155958597503509[76] = 0;
   out_1868155958597503509[77] = 0;
   out_1868155958597503509[78] = 0;
   out_1868155958597503509[79] = 0;
   out_1868155958597503509[80] = 1;
}
void h_25(double *state, double *unused, double *out_1506828409419385690) {
   out_1506828409419385690[0] = state[6];
}
void H_25(double *state, double *unused, double *out_457650000587944141) {
   out_457650000587944141[0] = 0;
   out_457650000587944141[1] = 0;
   out_457650000587944141[2] = 0;
   out_457650000587944141[3] = 0;
   out_457650000587944141[4] = 0;
   out_457650000587944141[5] = 0;
   out_457650000587944141[6] = 1;
   out_457650000587944141[7] = 0;
   out_457650000587944141[8] = 0;
}
void h_24(double *state, double *unused, double *out_3299976003260507657) {
   out_3299976003260507657[0] = state[4];
   out_3299976003260507657[1] = state[5];
}
void H_24(double *state, double *unused, double *out_8761028887052412250) {
   out_8761028887052412250[0] = 0;
   out_8761028887052412250[1] = 0;
   out_8761028887052412250[2] = 0;
   out_8761028887052412250[3] = 0;
   out_8761028887052412250[4] = 1;
   out_8761028887052412250[5] = 0;
   out_8761028887052412250[6] = 0;
   out_8761028887052412250[7] = 0;
   out_8761028887052412250[8] = 0;
   out_8761028887052412250[9] = 0;
   out_8761028887052412250[10] = 0;
   out_8761028887052412250[11] = 0;
   out_8761028887052412250[12] = 0;
   out_8761028887052412250[13] = 0;
   out_8761028887052412250[14] = 1;
   out_8761028887052412250[15] = 0;
   out_8761028887052412250[16] = 0;
   out_8761028887052412250[17] = 0;
}
void h_30(double *state, double *unused, double *out_6989832271844354302) {
   out_6989832271844354302[0] = state[4];
}
void H_30(double *state, double *unused, double *out_328311053444704071) {
   out_328311053444704071[0] = 0;
   out_328311053444704071[1] = 0;
   out_328311053444704071[2] = 0;
   out_328311053444704071[3] = 0;
   out_328311053444704071[4] = 1;
   out_328311053444704071[5] = 0;
   out_328311053444704071[6] = 0;
   out_328311053444704071[7] = 0;
   out_328311053444704071[8] = 0;
}
void h_26(double *state, double *unused, double *out_8181063989117740725) {
   out_8181063989117740725[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3283853318286112083) {
   out_3283853318286112083[0] = 0;
   out_3283853318286112083[1] = 0;
   out_3283853318286112083[2] = 0;
   out_3283853318286112083[3] = 0;
   out_3283853318286112083[4] = 0;
   out_3283853318286112083[5] = 0;
   out_3283853318286112083[6] = 0;
   out_3283853318286112083[7] = 1;
   out_3283853318286112083[8] = 0;
}
void h_27(double *state, double *unused, double *out_7149558280500342525) {
   out_7149558280500342525[0] = state[3];
}
void H_27(double *state, double *unused, double *out_1846452258355720840) {
   out_1846452258355720840[0] = 0;
   out_1846452258355720840[1] = 0;
   out_1846452258355720840[2] = 0;
   out_1846452258355720840[3] = 1;
   out_1846452258355720840[4] = 0;
   out_1846452258355720840[5] = 0;
   out_1846452258355720840[6] = 0;
   out_1846452258355720840[7] = 0;
   out_1846452258355720840[8] = 0;
}
void h_29(double *state, double *unused, double *out_1988159004183508657) {
   out_1988159004183508657[0] = state[1];
}
void H_29(double *state, double *unused, double *out_3559814985225271873) {
   out_3559814985225271873[0] = 0;
   out_3559814985225271873[1] = 1;
   out_3559814985225271873[2] = 0;
   out_3559814985225271873[3] = 0;
   out_3559814985225271873[4] = 0;
   out_3559814985225271873[5] = 0;
   out_3559814985225271873[6] = 0;
   out_3559814985225271873[7] = 0;
   out_3559814985225271873[8] = 0;
}
void h_28(double *state, double *unused, double *out_3077188681305826063) {
   out_3077188681305826063[0] = state[0];
}
void H_28(double *state, double *unused, double *out_8642214002294802447) {
   out_8642214002294802447[0] = 1;
   out_8642214002294802447[1] = 0;
   out_8642214002294802447[2] = 0;
   out_8642214002294802447[3] = 0;
   out_8642214002294802447[4] = 0;
   out_8642214002294802447[5] = 0;
   out_8642214002294802447[6] = 0;
   out_8642214002294802447[7] = 0;
   out_8642214002294802447[8] = 0;
}
void h_31(double *state, double *unused, double *out_8535593804049382295) {
   out_8535593804049382295[0] = state[8];
}
void H_31(double *state, double *unused, double *out_488295962464904569) {
   out_488295962464904569[0] = 0;
   out_488295962464904569[1] = 0;
   out_488295962464904569[2] = 0;
   out_488295962464904569[3] = 0;
   out_488295962464904569[4] = 0;
   out_488295962464904569[5] = 0;
   out_488295962464904569[6] = 0;
   out_488295962464904569[7] = 0;
   out_488295962464904569[8] = 1;
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

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_2042278345473927113) {
  err_fun(nom_x, delta_x, out_2042278345473927113);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5398886998709636999) {
  inv_err_fun(nom_x, true_x, out_5398886998709636999);
}
void car_H_mod_fun(double *state, double *out_3065330649401547536) {
  H_mod_fun(state, out_3065330649401547536);
}
void car_f_fun(double *state, double dt, double *out_1469741008022123633) {
  f_fun(state,  dt, out_1469741008022123633);
}
void car_F_fun(double *state, double dt, double *out_1868155958597503509) {
  F_fun(state,  dt, out_1868155958597503509);
}
void car_h_25(double *state, double *unused, double *out_1506828409419385690) {
  h_25(state, unused, out_1506828409419385690);
}
void car_H_25(double *state, double *unused, double *out_457650000587944141) {
  H_25(state, unused, out_457650000587944141);
}
void car_h_24(double *state, double *unused, double *out_3299976003260507657) {
  h_24(state, unused, out_3299976003260507657);
}
void car_H_24(double *state, double *unused, double *out_8761028887052412250) {
  H_24(state, unused, out_8761028887052412250);
}
void car_h_30(double *state, double *unused, double *out_6989832271844354302) {
  h_30(state, unused, out_6989832271844354302);
}
void car_H_30(double *state, double *unused, double *out_328311053444704071) {
  H_30(state, unused, out_328311053444704071);
}
void car_h_26(double *state, double *unused, double *out_8181063989117740725) {
  h_26(state, unused, out_8181063989117740725);
}
void car_H_26(double *state, double *unused, double *out_3283853318286112083) {
  H_26(state, unused, out_3283853318286112083);
}
void car_h_27(double *state, double *unused, double *out_7149558280500342525) {
  h_27(state, unused, out_7149558280500342525);
}
void car_H_27(double *state, double *unused, double *out_1846452258355720840) {
  H_27(state, unused, out_1846452258355720840);
}
void car_h_29(double *state, double *unused, double *out_1988159004183508657) {
  h_29(state, unused, out_1988159004183508657);
}
void car_H_29(double *state, double *unused, double *out_3559814985225271873) {
  H_29(state, unused, out_3559814985225271873);
}
void car_h_28(double *state, double *unused, double *out_3077188681305826063) {
  h_28(state, unused, out_3077188681305826063);
}
void car_H_28(double *state, double *unused, double *out_8642214002294802447) {
  H_28(state, unused, out_8642214002294802447);
}
void car_h_31(double *state, double *unused, double *out_8535593804049382295) {
  h_31(state, unused, out_8535593804049382295);
}
void car_H_31(double *state, double *unused, double *out_488295962464904569) {
  H_31(state, unused, out_488295962464904569);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
