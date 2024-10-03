#include "cdm.h"

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <memory>


void hexdump(uint8_t *data, int len) {
  for (int i = 0; i < len; i++) { if (i!=0&&i%0x10==0) printf("\n"); printf("%02X ", data[i]); } printf("\n");
  for (int i = 0; i < len; i++) printf("\\x%02X", data[i]);
  printf("\n");
}

int write_cont(uint8_t *dst, uint32_t reg, std::vector<uint32_t> vals) {
  struct cdm_regcontinuous_cmd *cmd = (struct cdm_regcontinuous_cmd*)dst;
  cmd->cmd = CAM_CDM_CMD_REG_CONT;
  cmd->count = vals.size();
  cmd->offset = reg;
  cmd->reserved0 = 0;
  cmd->reserved1 = 0;

  uint32_t *vd = (uint32_t*)(dst + sizeof(struct cdm_regcontinuous_cmd));
  for (int i = 0; i < vals.size(); i++) {
    *vd = vals[i];
    vd++;
  }

  return sizeof(struct cdm_regcontinuous_cmd) + vals.size()*sizeof(uint32_t);
}

int write_random(uint8_t *dst, std::vector<uint32_t> vals) {
  struct cdm_regrandom_cmd *cmd = (struct cdm_regrandom_cmd*)dst;
  cmd->cmd = CAM_CDM_CMD_REG_RANDOM;
  cmd->count = vals.size() / 2;
  cmd->reserved = 0;

  uint32_t *vd = (uint32_t*)(dst + sizeof(struct cdm_regrandom_cmd));
  for (int i = 0; i < vals.size(); i++) {
    *vd = vals[i];
    vd++;
  }

  return sizeof(struct cdm_regrandom_cmd) + vals.size()*sizeof(uint32_t);
}

int build_initial_config(uint8_t *dst) {
  uint8_t *start = dst;

  // constants, some kind of HW quirk?
  dst += write_random(dst, {
    0x2c, 0xffffffff,   // IFE_0_VFE_MODULE_LENS_CGC_OVERRIDE
    0x30, 0xffffffff,   // IFE_0_VFE_MODULE_STATS_CGC_OVERRIDE
    0x34, 0xffffffff,   // IFE_0_VFE_MODULE_COLOR_CGC_OVERRIDE
    0x38, 0xffffffff,   // IFE_0_VFE_MODULE_ZOOM_CGC_OVERRIDE
    0x3c, 0xffffffff,   // IFE_0_VFE_MODULE_BUS_CGC_OVERRIDE
  });

  // *** ifecamif ***
  // IFE_0_VFE_CAMIF_CMD
  dst += write_cont(dst, 0x478, {
    0x00000004,   // IFE_0_VFE_CAMIF_CMD
    0x004000c0,   // IFE_0_VFE_CAMIF_CFG
  });
  // IFE_0_VFE_CAMIF_LINE_SKIP_PATTERN
  dst += write_cont(dst, 0x488, {
    0x00000000,   // IFE_0_VFE_CAMIF_LINE_SKIP_PATTERN
    0x00000000,   // IFE_0_VFE_CAMIF_PIXEL_SKIP_PATTERN
    0x00000f0f,   // IFE_0_VFE_CAMIF_SKIP_PERIOD
  });
  // IFE_0_VFE_CAMIF_IRQ_SUBSAMPLE_PATTERN
  dst += write_cont(dst, 0x49c, {
    0x00000001,   // IFE_0_VFE_CAMIF_IRQ_SUBSAMPLE_PATTERN
  });
  // IFE_0_VFE_CAMIF_RAW_CROP_WIDTH_CFG
  dst += write_cont(dst, 0xce4, {
    0x00000000,   // IFE_0_VFE_CAMIF_RAW_CROP_WIDTH_CFG
    0x00000000,   // IFE_0_VFE_CAMIF_RAW_CROP_HEIGHT_CFG
  });

  // *** ifelinearization33 IQ module ***
  // IFE_0_VFE_BLACK_CFG
  dst += write_cont(dst, 0x4dc, {
    0x00000000,   // IFE_0_VFE_BLACK_CFG
    0x04050b84,   // IFE_0_VFE_BLACK_INTERP_R_0
    0x13031a82,   // IFE_0_VFE_BLACK_INTERP_R_1
    0x22022981,   // IFE_0_VFE_BLACK_INTERP_R_2
    0x3100387f,   // IFE_0_VFE_BLACK_INTERP_R_3
    0x04010b80,   // IFE_0_VFE_BLACK_INTERP_GB_0
    0x13001a80,   // IFE_0_VFE_BLACK_INTERP_GB_1
    0x2200297f,   // IFE_0_VFE_BLACK_INTERP_GB_2
    0x30ff387f,   // IFE_0_VFE_BLACK_INTERP_GB_3
    0x04050b84,   // IFE_0_VFE_BLACK_INTERP_B_0
    0x13031a82,   // IFE_0_VFE_BLACK_INTERP_B_1
    0x22022981,   // IFE_0_VFE_BLACK_INTERP_B_2
    0x3100387f,   // IFE_0_VFE_BLACK_INTERP_B_3
    0x04010b80,   // IFE_0_VFE_BLACK_INTERP_GR_0
    0x13001a80,   // IFE_0_VFE_BLACK_INTERP_GR_1
    0x2200297f,   // IFE_0_VFE_BLACK_INTERP_GR_2
    0x30ff387f,   // IFE_0_VFE_BLACK_INTERP_GR_3
    0x04050b84,   // IFE_0_VFE_BLACK_RIGHT_INTERP_R_0
    0x13031a82,   // IFE_0_VFE_BLACK_RIGHT_INTERP_R_1
    0x22022981,   // IFE_0_VFE_BLACK_RIGHT_INTERP_R_2
    0x3100387f,   // IFE_0_VFE_BLACK_RIGHT_INTERP_R_3
    0x04010b80,   // IFE_0_VFE_BLACK_RIGHT_INTERP_GB_0
    0x13001a80,   // IFE_0_VFE_BLACK_RIGHT_INTERP_GB_1
    0x2200297f,   // IFE_0_VFE_BLACK_RIGHT_INTERP_GB_2
    0x30ff387f,   // IFE_0_VFE_BLACK_RIGHT_INTERP_GB_3
    0x04050b84,   // IFE_0_VFE_BLACK_RIGHT_INTERP_B_0
    0x13031a82,   // IFE_0_VFE_BLACK_RIGHT_INTERP_B_1
    0x22022981,   // IFE_0_VFE_BLACK_RIGHT_INTERP_B_2
    0x3100387f,   // IFE_0_VFE_BLACK_RIGHT_INTERP_B_3
    0x04010b80,   // IFE_0_VFE_BLACK_RIGHT_INTERP_GR_0
    0x13001a80,   // IFE_0_VFE_BLACK_RIGHT_INTERP_GR_1
    0x2200297f,   // IFE_0_VFE_BLACK_RIGHT_INTERP_GR_2
    0x30ff387f,   // IFE_0_VFE_BLACK_RIGHT_INTERP_GR_3
  });
  /* TODO
  cdm_dmi_cmd_t 248
    .length = 287
    .reserved = 33
    .cmd = 11
    .addr = 0
    .DMIAddr = 3108
    .DMISel = 9
  */

  // *** ifedemux13 IQ module ***
  // IFE_0_VFE_DEMUX_CFG
  dst += write_cont(dst, 0x560, {
    0x00000001,   // IFE_0_VFE_DEMUX_CFG
    0x04440444,   // IFE_0_VFE_DEMUX_GAIN_0
    0x04450445,   // IFE_0_VFE_DEMUX_GAIN_1
    0x04440444,   // IFE_0_VFE_DEMUX_RIGHT_GAIN_0
    0x04450445,   // IFE_0_VFE_DEMUX_RIGHT_GAIN_1
    0x000000ca,   // IFE_0_VFE_DEMUX_EVEN_CFG
    0x0000009c,   // IFE_0_VFE_DEMUX_ODD_CFG
  });

  // *** ifeabf34 IQ module ***
  // IFE_0_VFE_ABF_CFG
  dst += write_cont(dst, 0x5e8, {
    0x06363005,   // IFE_0_VFE_ABF_CFG
  });
  // IFE_0_VFE_ABF_GR_CFG
  dst += write_cont(dst, 0x5f4, {
    0x00000000,   // IFE_0_VFE_ABF_GR_CFG
    0x00000000,   // IFE_0_VFE_ABF_GB_CFG
    0x00000000,   // IFE_0_VFE_ABF_R_CFG
    0x00000000,   // IFE_0_VFE_ABF_B_CFG
    0x3b3839a0,   // IFE_0_VFE_ABF_RNR_CFG_0
    0x003f8040,   // IFE_0_VFE_ABF_RNR_CFG_1
    0x00000000,   // IFE_0_VFE_ABF_RNR_CFG_2
    0x00000000,   // IFE_0_VFE_ABF_RNR_CFG_3
    0x00078000,   // IFE_0_VFE_ABF_RNR_CFG_4
    0x00078000,   // IFE_0_VFE_ABF_RNR_CFG_5
    0x00078000,   // IFE_0_VFE_ABF_RNR_CFG_6
    0x00078000,   // IFE_0_VFE_ABF_RNR_CFG_7
    0x00078000,   // IFE_0_VFE_ABF_RNR_CFG_8
    0x00078000,   // IFE_0_VFE_ABF_RNR_CFG_9
    0x00078000,   // IFE_0_VFE_ABF_RNR_CFG_10
    0x00078000,   // IFE_0_VFE_ABF_RNR_CFG_11
    0x00000009,   // IFE_0_VFE_ABF_RNR_CFG_12
    0x00400808,   // IFE_0_VFE_ABF_BPC_CFG_0
    0x00000044,   // IFE_0_VFE_ABF_BPC_CFG_1
    0x004000a0,   // IFE_0_VFE_ABF_NOISE_PRESERVE_CFG_0
    0x0a0d00a6,   // IFE_0_VFE_ABF_NOISE_PRESERVE_CFG_1
    0x0a0d00a6,   // IFE_0_VFE_ABF_NOISE_PRESERVE_CFG_2
  });
  /* TODO
  cdm_dmi_cmd_t 392
    .length = 255
    .reserved = 33
    .cmd = 10
    .addr = 0
    .DMIAddr = 3108
    .DMISel = 12
  */

  // *** ifelsc34 IQ module ***
  // IFE_0_VFE_ROLLOFF_CFG
  dst += write_cont(dst, 0x6bc, {
    0x0b3c0000,   // IFE_0_VFE_ROLLOFF_CFG
    0x00670067,   // IFE_0_VFE_ROLLOFF_GRID_CFG_0
    0xd3b1300c,   // IFE_0_VFE_ROLLOFF_GRID_CFG_1
    0x13b1300c,   // IFE_0_VFE_ROLLOFF_GRID_CFG_2
    0x00670067,   // IFE_0_VFE_ROLLOFF_RIGHT_GRID_CFG_0
    0xd3b1300c,   // IFE_0_VFE_ROLLOFF_RIGHT_GRID_CFG_1
    0x13b1300c,   // IFE_0_VFE_ROLLOFF_RIGHT_GRID_CFG_2
    0xec4e4000,   // IFE_0_VFE_ROLLOFF_STRIPE_CFG_0
    0x0100c003,   // IFE_0_VFE_ROLLOFF_STRIPE_CFG_1
    0xec4e4000,   // IFE_0_VFE_ROLLOFF_RIGHT_STRIPE_CFG_0
    0x0100c003,   // IFE_0_VFE_ROLLOFF_RIGHT_STRIPE_CFG_1
  });
  /* TODO
  cdm_dmi_cmd_t 444
    .length = 883
    .reserved = 33
    .cmd = 10
    .addr = 0
    .DMIAddr = 3108
    .DMISel = 14
  */
  /* TODO
  cdm_dmi_cmd_t 444
    .length = 883
    .reserved = 33
    .cmd = 10
    .addr = 0
    .DMIAddr = 3108
    .DMISel = 15
  */

  // *** ifewb12 IQ module ***
  // IFE_0_VFE_DEMO_WB_LEFT_CFG_0
  dst += write_cont(dst, 0x6fc, {
    0x00bf0080,   // IFE_0_VFE_DEMO_WB_LEFT_CFG_0
    0x00000106,   // IFE_0_VFE_DEMO_WB_LEFT_CFG_1
    0x00000000,   // IFE_0_VFE_DEMO_WB_LEFT_OFFSET_CFG_0
    0x00000000,   // IFE_0_VFE_DEMO_WB_LEFT_OFFSET_CFG_1
  });

  // *** ifedemosaic36 IQ module ***
  // IFE_0_VFE_DEMO_CFG
  dst += write_cont(dst, 0x6f8, {
    0x00000100,   // IFE_0_VFE_DEMO_CFG
  });
  // IFE_0_VFE_DEMO_INTERP_COEFF_CFG
  dst += write_cont(dst, 0x71c, {
    0x00008000,   // IFE_0_VFE_DEMO_INTERP_COEFF_CFG
    0x08000066,   // IFE_0_VFE_DEMO_INTERP_CLASSIFIER_0
  });

  // *** ifecc12 IQ module ***
  // IFE_0_VFE_COLOR_CORRECT_COEFF_0
  dst += write_cont(dst, 0x760, {
    0x00800080,   // IFE_0_VFE_COLOR_CORRECT_COEFF_0
    0x00000000,   // IFE_0_VFE_COLOR_CORRECT_COEFF_1
    0x00000000,   // IFE_0_VFE_COLOR_CORRECT_COEFF_2
    0x00000000,   // IFE_0_VFE_COLOR_CORRECT_COEFF_3
    0x00800080,   // IFE_0_VFE_COLOR_CORRECT_COEFF_4
    0x00000000,   // IFE_0_VFE_COLOR_CORRECT_COEFF_5
    0x00000000,   // IFE_0_VFE_COLOR_CORRECT_COEFF_6
    0x00000000,   // IFE_0_VFE_COLOR_CORRECT_COEFF_7
    0x00800080,   // IFE_0_VFE_COLOR_CORRECT_COEFF_8
    0x00000000,   // IFE_0_VFE_COLOR_CORRECT_OFFSET_0
    0x00000000,   // IFE_0_VFE_COLOR_CORRECT_OFFSET_1
    0x00000000,   // IFE_0_VFE_COLOR_CORRECT_OFFSET_2
    0x00000000,   // IFE_0_VFE_COLOR_CORRECT_COEFF_Q
  });

  // *** ifegtm10 IQ module ***
  // IFE_0_VFE_GTM_CFG
  dst += write_cont(dst, 0x794, {
    0x00000000,   // IFE_0_VFE_GTM_CFG
  });
  /* TODO
  cdm_dmi_cmd_t 568
    .length = 511
    .reserved = 33
    .cmd = 11
    .addr = 0
    .DMIAddr = 3108
    .DMISel = 24
  */

  // *** ifegamma16 IQ module ***
  // IFE_0_VFE_RGB_LUT_CFG
  dst += write_cont(dst,
  });
  /* TODO
  cdm_dmi_cmd_t 580
    .length = 255
    .reserved = 33
    .cmd = 10
    .addr = 0
    .DMIAddr = 3108
    .DMISel = 26
  */
  /* TODO
  cdm_dmi_cmd_t 580
    .length = 255
    .reserved = 33
    .cmd = 10
    .addr = 0
    .DMIAddr = 3108
    .DMISel = 28
  */
  /* TODO
  cdm_dmi_cmd_t 580
    .length = 255
    .reserved = 33
    .cmd = 10
    .addr = 0
    .DMIAddr = 3108
    .DMISel = 30
  */

  // *** ifecst12 IQ module ***
  // IFE_0_VFE_COLOR_XFORM_CH0_COEFF_CFG_0
  dst += write_cont(dst, 0xf30, {
    0x00750259,   // IFE_0_VFE_COLOR_XFORM_CH0_COEFF_CFG_0
    0x00000132,   // IFE_0_VFE_COLOR_XFORM_CH0_COEFF_CFG_1
    0x00000000,   // IFE_0_VFE_COLOR_XFORM_CH0_OFFSET_CFG
    0x03ff0000,   // IFE_0_VFE_COLOR_XFORM_CH0_CLAMP_CFG
    0x01fe1eae,   // IFE_0_VFE_COLOR_XFORM_CH1_COEFF_CFG_0
    0x00001f54,   // IFE_0_VFE_COLOR_XFORM_CH1_COEFF_CFG_1
    0x02000000,   // IFE_0_VFE_COLOR_XFORM_CH1_OFFSET_CFG
    0x03ff0000,   // IFE_0_VFE_COLOR_XFORM_CH1_CLAMP_CFG
    0x1fad1e55,   // IFE_0_VFE_COLOR_XFORM_CH2_COEFF_CFG_0
    0x000001fe,   // IFE_0_VFE_COLOR_XFORM_CH2_COEFF_CFG_1
    0x02000000,   // IFE_0_VFE_COLOR_XFORM_CH2_OFFSET_CFG
    0x03ff0000,   // IFE_0_VFE_COLOR_XFORM_CH2_CLAMP_CFG
  });

  // *** ifemnds16 IQ module ***
  // IFE_0_VFE_SCALE_VID_Y_CFG
  dst += write_cont(dst, 0xa3c, {
    0x00000003,   // IFE_0_VFE_SCALE_VID_Y_CFG
    0x077f0cbf,   // IFE_0_VFE_SCALE_VID_Y_H_IMAGE_SIZE_CFG
    0x30036666,   // IFE_0_VFE_SCALE_VID_Y_H_PHASE_CFG
    0x00000000,   // IFE_0_VFE_SCALE_VID_Y_H_STRIPE_CFG_0
    0x00000000,   // IFE_0_VFE_SCALE_VID_Y_H_STRIPE_CFG_1
    0x00000cbf,   // IFE_0_VFE_SCALE_VID_Y_H_PAD_CFG
    0x059f098f,   // IFE_0_VFE_SCALE_VID_Y_V_IMAGE_SIZE_CFG
    0x30036666,   // IFE_0_VFE_SCALE_VID_Y_V_PHASE_CFG
    0x00000000,   // IFE_0_VFE_SCALE_VID_Y_V_STRIPE_CFG_0
    0x00000000,   // IFE_0_VFE_SCALE_VID_Y_V_STRIPE_CFG_1
    0x0000098f,   // IFE_0_VFE_SCALE_VID_Y_V_PAD_CFG
  });
  // IFE_0_VFE_SCALE_VID_CBCR_CFG
  dst += write_cont(dst, 0xa68, {
    0x00000003,   // IFE_0_VFE_SCALE_VID_CBCR_CFG
    0x03bf0cbf,   // IFE_0_VFE_SCALE_VID_CBCR_H_IMAGE_SIZE_CFG
    0x3006cccc,   // IFE_0_VFE_SCALE_VID_CBCR_H_PHASE_CFG
    0x00000000,   // IFE_0_VFE_SCALE_VID_CBCR_H_STRIPE_CFG_0
    0x00000000,   // IFE_0_VFE_SCALE_VID_CBCR_H_STRIPE_CFG_1
    0x00000cbf,   // IFE_0_VFE_SCALE_VID_CBCR_H_PAD_CFG
    0x02cf098f,   // IFE_0_VFE_SCALE_VID_CBCR_V_IMAGE_SIZE_CFG
    0x3006cccc,   // IFE_0_VFE_SCALE_VID_CBCR_V_PHASE_CFG
    0x00000000,   // IFE_0_VFE_SCALE_VID_CBCR_V_STRIPE_CFG_0
    0x00000000,   // IFE_0_VFE_SCALE_VID_CBCR_V_STRIPE_CFG_1
    0x0000098f,   // IFE_0_VFE_SCALE_VID_CBCR_V_PAD_CFG
  });


  // *** ifecrop10 IQ module ***
  // IFE_0_VFE_FULL_OUT_Y_CROP_LINE_CFG
  dst += write_cont(dst, 0xe10, {
    0x00b404eb,   // IFE_0_VFE_FULL_OUT_Y_CROP_LINE_CFG
    0x0000077f,   // IFE_0_VFE_FULL_OUT_Y_CROP_PIXEL_CFG
  });
  // IFE_0_VFE_FULL_OUT_C_CROP_LINE_CFG
  dst += write_cont(dst, 0xe30, {
    0x005a0275,   // IFE_0_VFE_FULL_OUT_C_CROP_LINE_CFG
    0x0000077f,   // IFE_0_VFE_FULL_OUT_C_CROP_PIXEL_CFG
  });
  // IFE_0_VFE_FULL_OUT_Y_CH0_CLAMP_CFG
  dst += write_cont(dst, 0xe18, {
    0x0ff00000,   // IFE_0_VFE_FULL_OUT_Y_CH0_CLAMP_CFG
    0x00000016,   // IFE_0_VFE_FULL_OUT_Y_CH0_ROUNDING_CFG
  });
  // IFE_0_VFE_FULL_OUT_C_CH0_CLAMP_CFG
  dst += write_cont(dst, 0xe38, {
    0x0ff00000,   // IFE_0_VFE_FULL_OUT_C_CH0_CLAMP_CFG
    0x00000017,   // IFE_0_VFE_FULL_OUT_C_CH0_ROUNDING_CFG
  });

  // *** ifeds410 IQ module ***
  // IFE_0_VFE_DS4_Y_PRE_CROP_LINE_CFG
  dst += write_cont(dst, 0xd84, {
    0x00b404eb,   // IFE_0_VFE_DS4_Y_PRE_CROP_LINE_CFG
    0x0000077f,   // IFE_0_VFE_DS4_Y_PRE_CROP_PIXEL_CFG
  });
  // IFE_0_VFE_DS4_C_PRE_CROP_LINE_CFG
  dst += write_cont(dst, 0xda4, {
    0x005a0275,   // IFE_0_VFE_DS4_C_PRE_CROP_LINE_CFG
    0x0000077f,   // IFE_0_VFE_DS4_C_PRE_CROP_PIXEL_CFG
  });
  // IFE_0_VFE_DS_4TO1_Y_1ST_CFG
  dst += write_cont(dst, 0xd60, {
    0x04380300,   // IFE_0_VFE_DS_4TO1_Y_1ST_CFG
    0x09016c7d,   // IFE_0_VFE_DS_4TO1_Y_1ST_COEFF
    0x021c0300,   // IFE_0_VFE_DS_4TO1_C_1ST_CFG
  });
  // IFE_0_VFE_DS4_Y_CH0_CLAMP_CFG
  dst += write_cont(dst, 0xd98, {
    0x0ff00000,   // IFE_0_VFE_DS4_Y_CH0_CLAMP_CFG
    0x00000016,   // IFE_0_VFE_DS4_Y_CH0_ROUNDING_CFG
  });
  // IFE_0_VFE_DS4_C_CH0_CLAMP_CFG
  dst += write_cont(dst, 0xdb8, {
    0x0ff00000,   // IFE_0_VFE_DS4_C_CH0_CLAMP_CFG
    0x00000017,   // IFE_0_VFE_DS4_C_CH0_ROUNDING_CFG
  });

  // *** ifer2pd10 IQ module ***
  // IFE_0_VFE_R2PD_1ST_CFG
  dst += write_cont(dst, 0xd6c, {
    0x00000300,   // IFE_0_VFE_R2PD_1ST_CFG
  });

  // *** ifeds410 IQ module ***
  // IFE_0_VFE_DS_4TO1_Y_2ND_CFG
  dst += write_cont(dst, 0xd70, {
    0x010e0f00,   // IFE_0_VFE_DS_4TO1_Y_2ND_CFG
    0x09016c7d,   // IFE_0_VFE_DS_4TO1_Y_2ND_COEFF
    0x00870f00,   // IFE_0_VFE_DS_4TO1_C_2ND_CFG
  });

  // *** ifer2pd10 IQ module ***
  // IFE_0_VFE_R2PD_2ND_CFG
  dst += write_cont(dst, 0xd7c, {
    0x00000f00,   // IFE_0_VFE_R2PD_2ND_CFG
  });

  // *** ifenode ***
  // IFE_0_VFE_MODULE_LENS_EN
  dst += write_cont(dst, 0x40, {
    0x00000586,   // IFE_0_VFE_MODULE_LENS_EN
  });
  // IFE_0_VFE_MODULE_COLOR_EN
  dst += write_cont(dst, 0x48, {
    0x0000000e,   // IFE_0_VFE_MODULE_COLOR_EN
  });
  // IFE_0_VFE_MODULE_ZOOM_EN
  dst += write_cont(dst, 0x4c, {
    0x00000019,   // IFE_0_VFE_MODULE_ZOOM_EN
  });
  // IFE_0_VFE_FD_OUT_Y_CROP_RND_CLAMP_CFG
  dst += write_cont(dst, 0xe4c, {
    0x00000000,   // IFE_0_VFE_FD_OUT_Y_CROP_RND_CLAMP_CFG
  });
  // IFE_0_VFE_FD_OUT_C_CROP_RND_CLAMP_CFG
  dst += write_cont(dst, 0xe6c, {
    0x00000000,   // IFE_0_VFE_FD_OUT_C_CROP_RND_CLAMP_CFG
  });
  // IFE_0_VFE_FULL_OUT_Y_CROP_RND_CLAMP_CFG
  dst += write_cont(dst, 0xe0c, {
    0x00000e00,   // IFE_0_VFE_FULL_OUT_Y_CROP_RND_CLAMP_CFG
  });
  // IFE_0_VFE_FULL_OUT_C_CROP_RND_CLAMP_CFG
  dst += write_cont(dst, 0xe2c, {
    0x00000e00,   // IFE_0_VFE_FULL_OUT_C_CROP_RND_CLAMP_CFG
  });
  // IFE_0_VFE_DS4_Y_CROP_RND_CLAMP_CFG
  dst += write_cont(dst, 0xd8c, {
    0x00000000,   // IFE_0_VFE_DS4_Y_CROP_RND_CLAMP_CFG
  });
  // IFE_0_VFE_DS4_C_CROP_RND_CLAMP_CFG
  dst += write_cont(dst, 0xdac, {
    0x00000000,   // IFE_0_VFE_DS4_C_CROP_RND_CLAMP_CFG
  });
  // IFE_0_VFE_DS16_Y_CROP_RND_CLAMP_CFG
  dst += write_cont(dst, 0xdcc, {
    0x00000000,   // IFE_0_VFE_DS16_Y_CROP_RND_CLAMP_CFG
  });
  // IFE_0_VFE_DS16_C_CROP_RND_CLAMP_CFG
  dst += write_cont(dst, 0xdec, {
    0x00000000,   // IFE_0_VFE_DS16_C_CROP_RND_CLAMP_CFG
  });
  // IFE_0_VFE_MODULE_STATS_EN
  dst += write_cont(dst, 0x44, {
    0x00000000,   // IFE_0_VFE_MODULE_STATS_EN
  });
  // IFE_0_VFE_STATS_CFG
  dst += write_cont(dst, 0xaac, {
    0x00000040,   // IFE_0_VFE_STATS_CFG
  });
  // IFE_0_VFE_DSP_TO_SEL
  dst += write_cont(dst, 0xf00, {
    0x00000000,   // IFE_0_VFE_DSP_TO_SEL
  });

  hexdump(start, dst - start);
  printf("\n***** wrote out %ld bytes, togo %ld\n", dst - start, 1100 - (dst-start));
  return dst - start;
}

int build_first_update(uint8_t *dst) {
  return 0;
}

int build_update(uint8_t *dst) {
  return 0;
}

/*
int main(void) {
  uint8_t buf[2048];
  memset(buf, 0xee, sizeof(buf));

  buint ild_init(buf);
};
*/
