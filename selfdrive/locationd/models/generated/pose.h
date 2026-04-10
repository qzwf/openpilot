#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_7430843458597553837);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_8627048357023842126);
void pose_H_mod_fun(double *state, double *out_8834050469615675398);
void pose_f_fun(double *state, double dt, double *out_6950331196878070812);
void pose_F_fun(double *state, double dt, double *out_2568972817437578706);
void pose_h_4(double *state, double *unused, double *out_2596008029261534560);
void pose_H_4(double *state, double *unused, double *out_6377697350598801949);
void pose_h_10(double *state, double *unused, double *out_2165897035409663064);
void pose_H_10(double *state, double *unused, double *out_3933408193679582807);
void pose_h_13(double *state, double *unused, double *out_8398353702287059752);
void pose_H_13(double *state, double *unused, double *out_3165423525266469148);
void pose_h_14(double *state, double *unused, double *out_961638628362298617);
void pose_H_14(double *state, double *unused, double *out_2414456494259317420);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}