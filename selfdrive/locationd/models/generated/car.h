#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_2042278345473927113);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5398886998709636999);
void car_H_mod_fun(double *state, double *out_3065330649401547536);
void car_f_fun(double *state, double dt, double *out_1469741008022123633);
void car_F_fun(double *state, double dt, double *out_1868155958597503509);
void car_h_25(double *state, double *unused, double *out_1506828409419385690);
void car_H_25(double *state, double *unused, double *out_457650000587944141);
void car_h_24(double *state, double *unused, double *out_3299976003260507657);
void car_H_24(double *state, double *unused, double *out_8761028887052412250);
void car_h_30(double *state, double *unused, double *out_6989832271844354302);
void car_H_30(double *state, double *unused, double *out_328311053444704071);
void car_h_26(double *state, double *unused, double *out_8181063989117740725);
void car_H_26(double *state, double *unused, double *out_3283853318286112083);
void car_h_27(double *state, double *unused, double *out_7149558280500342525);
void car_H_27(double *state, double *unused, double *out_1846452258355720840);
void car_h_29(double *state, double *unused, double *out_1988159004183508657);
void car_H_29(double *state, double *unused, double *out_3559814985225271873);
void car_h_28(double *state, double *unused, double *out_3077188681305826063);
void car_H_28(double *state, double *unused, double *out_8642214002294802447);
void car_h_31(double *state, double *unused, double *out_8535593804049382295);
void car_H_31(double *state, double *unused, double *out_488295962464904569);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}