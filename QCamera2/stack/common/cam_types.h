/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of Code Aurora Forum, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __QCAMERA_TYPES_H__
#define __QCAMERA_TYPES_H__

#include <stdint.h>
#include <pthread.h>
#include <inttypes.h>
#include <media/msmb_camera.h>

#define CEILING32(X) (((X) + 0x0001F) & 0xFFFFFFE0)
#define CEILING16(X) (((X) + 0x000F) & 0xFFF0)
#define CEILING4(X)  (((X) + 0x0003) & 0xFFFC)
#define CEILING2(X)  (((X) + 0x0001) & 0xFFFE)

#define MAX_ZOOMS_CNT 64
#define MAX_SIZES_CNT 12
#define MAX_EXP_BRACKETING_LENGTH 32
#define MAX_ROI 5

typedef enum {
    CAM_STATUS_SUCCESS,       /* Operation Succeded */
    CAM_STATUS_FAILED,        /* Failure in doing operation */
    CAM_STATUS_INVALID_PARM,  /* Inavlid parameter provided */
    CAM_STATUS_NOT_SUPPORTED, /* Parameter/operation not supported */
    CAM_STATUS_ACCEPTED,      /* Parameter accepted */
    CAM_STATUS_MAX,
} cam_status_t;

typedef enum {
    CAM_POSITION_BACK,
    CAM_POSITION_FRONT
} cam_position_t;

typedef enum {
    CAM_FORMAT_YUV_420_NV12 = 1,
    CAM_FORMAT_YUV_420_NV21,
    CAM_FORMAT_YUV_420_NV21_ADRENO,
    CAM_FORMAT_YUV_420_YV12,
    CAM_FORMAT_YUV_422_NV16,
    CAM_FORMAT_YUV_422_NV61,

    /* Packed YUV/YVU raw format, 16 bpp: 8 bits Y and 8 bits UV.
     * U and V are interleaved with Y: YUYV or YVYV */
    CAM_FORMAT_YUV_RAW_8BIT,
    /* QCOM RAW formats where data is packed into 64bit word.
     * 8BPP: 1 64-bit word contains 8 pixels p0 - p7, where p0 is
     *       stored at LSB.
     * 10BPP: 1 64-bit word contains 6 pixels p0 - p5, where most
     *       significant 4 bits are set to 0. P0 is stored at LSB.
     * 12BPP: 1 64-bit word contains 5 pixels p0 - p4, where most
     *       significant 4 bits are set to 0. P0 is stored at LSB. */
    CAM_FORMAT_BAYER_QCOM_RAW_8BPP_GBRG,
    CAM_FORMAT_BAYER_QCOM_RAW_8BPP_GRBG,
    CAM_FORMAT_BAYER_QCOM_RAW_8BPP_RGGB,
    CAM_FORMAT_BAYER_QCOM_RAW_8BPP_BGGR,
    CAM_FORMAT_BAYER_QCOM_RAW_10BPP_GBRG,
    CAM_FORMAT_BAYER_QCOM_RAW_10BPP_GRBG,
    CAM_FORMAT_BAYER_QCOM_RAW_10BPP_RGGB,
    CAM_FORMAT_BAYER_QCOM_RAW_10BPP_BGGR,
    CAM_FORMAT_BAYER_QCOM_RAW_12BPP_GBRG,
    CAM_FORMAT_BAYER_QCOM_RAW_12BPP_GRBG,
    CAM_FORMAT_BAYER_QCOM_RAW_12BPP_RGGB,
    CAM_FORMAT_BAYER_QCOM_RAW_12BPP_BGGR,
    /* MIPI RAW formats based on MIPI CSI-2 specifiction.
     * 8BPP: Each pixel occupies one bytes, starting at LSB.
     *       Output with of image has no restrictons.
     * 10BPP: Four pixels are held in every 5 bytes. The output
     *       with of image must be a multiple of 4 pixels.
     * 12BPP: Two pixels are held in every 3 bytes. The output
     *       width of image must be a multiple of 2 pixels. */
    CAM_FORMAT_BAYER_MIPI_RAW_8BPP_GBRG,
    CAM_FORMAT_BAYER_MIPI_RAW_8BPP_GRBG,
    CAM_FORMAT_BAYER_MIPI_RAW_8BPP_RGGB,
    CAM_FORMAT_BAYER_MIPI_RAW_8BPP_BGGR,
    CAM_FORMAT_BAYER_MIPI_RAW_10BPP_GBRG,
    CAM_FORMAT_BAYER_MIPI_RAW_10BPP_GRBG,
    CAM_FORMAT_BAYER_MIPI_RAW_10BPP_RGGB,
    CAM_FORMAT_BAYER_MIPI_RAW_10BPP_BGGR,
    CAM_FORMAT_BAYER_MIPI_RAW_12BPP_GBRG,
    CAM_FORMAT_BAYER_MIPI_RAW_12BPP_GRBG,
    CAM_FORMAT_BAYER_MIPI_RAW_12BPP_RGGB,
    CAM_FORMAT_BAYER_MIPI_RAW_12BPP_BGGR,
    /* Ideal raw formats where image data has gone through black
     * correction, lens rolloff, demux/channel gain, bad pixel
     * correction, and ABF.
     * Ideal raw formats could output any of QCOM_RAW and MIPI_RAW
     * formats, plus plain8 8bbp, plain16 800, plain16 10bpp, and
     * plain 16 12bpp */
    CAM_FORMAT_BAYER_IDEAL_RAW_QCOM_8BPP_GBRG,
    CAM_FORMAT_BAYER_IDEAL_RAW_QCOM_8BPP_GRBG,
    CAM_FORMAT_BAYER_IDEAL_RAW_QCOM_8BPP_RGGB,
    CAM_FORMAT_BAYER_IDEAL_RAW_QCOM_8BPP_BGGR,
    CAM_FORMAT_BAYER_IDEAL_RAW_QCOM_10BPP_GBRG,
    CAM_FORMAT_BAYER_IDEAL_RAW_QCOM_10BPP_GRBG,
    CAM_FORMAT_BAYER_IDEAL_RAW_QCOM_10BPP_RGGB,
    CAM_FORMAT_BAYER_IDEAL_RAW_QCOM_10BPP_BGGR,
    CAM_FORMAT_BAYER_IDEAL_RAW_QCOM_12BPP_GBRG,
    CAM_FORMAT_BAYER_IDEAL_RAW_QCOM_12BPP_GRBG,
    CAM_FORMAT_BAYER_IDEAL_RAW_QCOM_12BPP_RGGB,
    CAM_FORMAT_BAYER_IDEAL_RAW_QCOM_12BPP_BGGR,
    CAM_FORMAT_BAYER_IDEAL_RAW_MIPI_8BPP_GBRG,
    CAM_FORMAT_BAYER_IDEAL_RAW_MIPI_8BPP_GRBG,
    CAM_FORMAT_BAYER_IDEAL_RAW_MIPI_8BPP_RGGB,
    CAM_FORMAT_BAYER_IDEAL_RAW_MIPI_8BPP_BGGR,
    CAM_FORMAT_BAYER_IDEAL_RAW_MIPI_10BPP_GBRG,
    CAM_FORMAT_BAYER_IDEAL_RAW_MIPI_10BPP_GRBG,
    CAM_FORMAT_BAYER_IDEAL_RAW_MIPI_10BPP_RGGB,
    CAM_FORMAT_BAYER_IDEAL_RAW_MIPI_10BPP_BGGR,
    CAM_FORMAT_BAYER_IDEAL_RAW_MIPI_12BPP_GBRG,
    CAM_FORMAT_BAYER_IDEAL_RAW_MIPI_12BPP_GRBG,
    CAM_FORMAT_BAYER_IDEAL_RAW_MIPI_12BPP_RGGB,
    CAM_FORMAT_BAYER_IDEAL_RAW_MIPI_12BPP_BGGR,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN8_8BPP_GBRG,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN8_8BPP_GRBG,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN8_8BPP_RGGB,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN8_8BPP_BGGR,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN16_8BPP_GBRG,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN16_8BPP_GRBG,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN16_8BPP_RGGB,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN16_8BPP_BGGR,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN16_10BPP_GBRG,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN16_10BPP_GRBG,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN16_10BPP_RGGB,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN16_10BPP_BGGR,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN16_12BPP_GBRG,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN16_12BPP_GRBG,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN16_12BPP_RGGB,
    CAM_FORMAT_BAYER_IDEAL_RAW_PLAIN16_12BPP_BGGR,

    CAM_FORMAT_MAX
} cam_format_t;

typedef enum {
    CAM_STREAM_TYPE_DEFAULT,       /* default stream type */
    CAM_STREAM_TYPE_PREVIEW,       /* preview */
    CAM_STREAM_TYPE_POSTVIEW,      /* postview */
    CAM_STREAM_TYPE_SNAPSHOT,      /* snapshot */
    CAM_STREAM_TYPE_VIDEO,         /* video */
    CAM_STREAM_TYPE_RAW,           /* raw dump from camif */
    CAM_STREAM_TYPE_METADATA,      /* meta data */
    CAM_STREAM_TYPE_OFFLINE_PROC,  /* offline process */
    CAM_STREAM_TYPE_MAX,
} cam_stream_type_t;

typedef enum {
    CAM_PAD_NONE = 1,
    CAM_PAD_TO_2 = 2,
    CAM_PAD_TO_4 = 4,
    CAM_PAD_TO_WORD = CAM_PAD_TO_4,
    CAM_PAD_TO_8 = 8,
    CAM_PAD_TO_16 = 16,
    CAM_PAD_TO_32 = 32,
    CAM_PAD_TO_64 = 64,
    CAM_PAD_TO_1K = 1024,
    CAM_PAD_TO_2K = 2048,
    CAM_PAD_TO_4K = 4096,
    CAM_PAD_TO_8K = 8192
} cam_pad_format_t;

typedef enum {
    /* followings are per camera */
    CAM_MAPPING_BUF_TYPE_CAPABILITY,  /* mapping camera capability buffer */
    CAM_MAPPING_BUF_TYPE_PARM_BUF,    /* mapping parameters buffer */

    /* followings are per stream */
    CAM_MAPPING_BUF_TYPE_STREAM_BUF,        /* mapping stream buffers */
    CAM_MAPPING_BUF_TYPE_STREAM_INFO,       /* mapping stream information buffer */
    CAM_MAPPING_BUF_TYPE_OFFLINE_INPUT_BUF, /* mapping offline process input buffer */
    CAM_MAPPING_BUF_TYPE_MAX
} cam_mapping_buf_type;

typedef struct {
    cam_mapping_buf_type type;
    uint32_t stream_id;   /* stream id: valid if STREAM_BUF */
    uint32_t frame_idx;   /* frame index: valid if type is STREAM_BUF */
    int32_t plane_idx;    /* planner index. valid if type is STREAM_BUF.
                           * -1 means all planners shanre the same fd;
                           * otherwise, each planner has its own fd */
    unsigned long cookie; /* could be job_id(uint32_t) to identify mapping job */
    int fd;               /* origin fd */
    uint32_t size;        /* size of the buffer */
} cam_buf_map_type;

typedef struct {
    cam_mapping_buf_type type;
    uint32_t stream_id;   /* stream id: valid if STREAM_BUF */
    uint32_t frame_idx;   /* frame index: valid if STREAM_BUF or HIST_BUF */
    int32_t plane_idx;    /* planner index. valid if type is STREAM_BUF.
                           * -1 means all planners shanre the same fd;
                           * otherwise, each planner has its own fd */
    unsigned long cookie; /* could be job_id(uint32_t) to identify unmapping job */
} cam_buf_unmap_type;

typedef enum {
    CAM_MAPPING_TYPE_FD_MAPPING,
    CAM_MAPPING_TYPE_FD_UNMAPPING,
    CAM_MAPPING_TYPE_MAX
} cam_mapping_type;

typedef struct {
    cam_mapping_type msg_type;
    union {
        cam_buf_map_type buf_map;
        cam_buf_unmap_type buf_unmap;
    } payload;
} cam_sock_packet_t;

typedef enum {
    CAM_MODE_2D = (1<<0),
    CAM_MODE_3D = (1<<1)
} cam_mode_t;

typedef struct {
    uint32_t len;
    uint32_t y_offset;
    uint32_t cbcr_offset;
} cam_sp_len_offset_t;

typedef struct{
    uint32_t len;
    uint32_t offset;
} cam_mp_len_offset_t;

typedef struct {
    uint32_t width_padding;
    uint32_t height_padding;
    uint32_t plane_padding;
} cam_padding_info_t;

typedef struct {
    int num_planes;
    union {
        cam_sp_len_offset_t sp;
        cam_mp_len_offset_t mp[VIDEO_MAX_PLANES];
    };
    uint32_t frame_len;
} cam_frame_len_offset_t;

typedef struct {
    int32_t width;
    int32_t height;
} cam_dimension_t;

typedef struct {
    int32_t offset_x;
    int32_t offset_y;
    int32_t stride;
    int32_t scanline;
    cam_frame_len_offset_t plane_info;
} cam_stream_buf_plane_info_t;

typedef struct {
    float min_fps;
    float max_fps;
} cam_fps_range_t;

typedef enum {
    CAM_HFR_MODE_OFF,
    CAM_HFR_MODE_60FPS,
    CAM_HFR_MODE_90FPS,
    CAM_HFR_MODE_120FPS,
    CAM_HFR_MODE_150FPS,
} cam_hfr_mode_t;

typedef struct {
    cam_hfr_mode_t mode;
    cam_dimension_t dim;
    uint8_t frame_skip;
} cam_hfr_info_t;

typedef enum {
    CAM_WB_MODE_AUTO,
    CAM_WB_MODE_CUSTOM,
    CAM_WB_MODE_INCANDESCENT,
    CAM_WB_MODE_FLUORESCENT,
    CAM_WB_MODE_WARM_FLUORESCENT,
    CAM_WB_MODE_DAYLIGHT,
    CAM_WB_MODE_CLOUDY_DAYLIGHT,
    CAM_WB_MODE_TWILIGHT,
    CAM_WB_MODE_SHADE,
    CAM_WB_MODE_MAX
} cam_wb_mode_type;

typedef enum {
    CAM_ANTIBANDING_MODE_OFF,
    CAM_ANTIBANDING_MODE_60HZ,
    CAM_ANTIBANDING_MODE_50HZ,
    CAM_ANTIBANDING_MODE_AUTO,
    CAM_ANTIBANDING_MODE_AUTO_50HZ,
    CAM_ANTIBANDING_MODE_AUTO_60HZ,
    CAM_ANTIBANDING_MODE_MAX,
} cam_antibanding_mode_type;

/* Enum Type for different ISO Mode supported */
typedef enum {
    CAM_ISO_MODE_AUTO,
    CAM_ISO_MODE_DEBLUR,
    CAM_ISO_MODE_100,
    CAM_ISO_MODE_200,
    CAM_ISO_MODE_400,
    CAM_ISO_MODE_800,
    CAM_ISO_MODE_1600,
    CAM_ISO_MODE_MAX
} cam_iso_mode_type;

typedef enum {
    CAM_AEC_MODE_FRAME_AVERAGE,
    CAM_AEC_MODE_CENTER_WEIGHTED,
    CAM_AEC_MODE_SPOT_METERING,
    CAM_AEC_MODE_SMART_METERING,
    CAM_AEC_MODE_USER_METERING,
    CAM_AEC_MODE_SPOT_METERING_ADV,
    CAM_AEC_MODE_CENTER_WEIGHTED_ADV,
    CAM_AEC_MODE_MAX
} cam_auto_exposure_mode_type;

typedef enum {
    CAM_FOCUS_ALGO_AUTO,
    CAM_FOCUS_ALGO_SPOT,
    CAM_FOCUS_ALGO_CENTER_WEIGHTED,
    CAM_FOCUS_ALGO_AVERAGE,
    CAM_FOCUS_ALGO_MAX
} cam_focus_algorithm_type;

/* Auto focus mode */
typedef enum {
    CAM_FOCUS_MODE_AUTO,
    CAM_FOCUS_MODE_INFINITY,
    CAM_FOCUS_MODE_MACRO,
    CAM_FOCUS_MODE_FIXED,
    CAM_FOCUS_MODE_EDOF,
    CAM_FOCUS_MODE_CONTINOUS_VIDEO,
    CAM_FOCUS_MODE_CONTINOUS_PICTURE,
    CAM_FOCUS_MODE_MAX
} cam_focus_mode_type;

typedef enum {
    CAM_SCENE_MODE_OFF,
    CAM_SCENE_MODE_AUTO,
    CAM_SCENE_MODE_LANDSCAPE,
    CAM_SCENE_MODE_SNOW,
    CAM_SCENE_MODE_BEACH,
    CAM_SCENE_MODE_SUNSET,
    CAM_SCENE_MODE_NIGHT,
    CAM_SCENE_MODE_PORTRAIT,
    CAM_SCENE_MODE_BACKLIGHT,
    CAM_SCENE_MODE_SPORTS,
    CAM_SCENE_MODE_ANTISHAKE,
    CAM_SCENE_MODE_FLOWERS,
    CAM_SCENE_MODE_CANDLELIGHT,
    CAM_SCENE_MODE_FIREWORKS,
    CAM_SCENE_MODE_PARTY,
    CAM_SCENE_MODE_NIGHT_PORTRAIT,
    CAM_SCENE_MODE_THEATRE,
    CAM_SCENE_MODE_ACTION,
    CAM_SCENE_MODE_AR,
    CAM_SCENE_MODE_MAX
} cam_scene_mode_type;

typedef enum {
    CAM_EFFECT_MODE_OFF,
    CAM_EFFECT_MODE_MONO,
    CAM_EFFECT_MODE_NEGATIVE,
    CAM_EFFECT_MODE_SOLARIZE,
    CAM_EFFECT_MODE_SEPIA,
    CAM_EFFECT_MODE_POSTERIZE,
    CAM_EFFECT_MODE_WHITEBOARD,
    CAM_EFFECT_MODE_BLACKBOARD,
    CAM_EFFECT_MODE_AQUA,
    CAM_EFFECT_MODE_EMBOSS,
    CAM_EFFECT_MODE_SKETCH,
    CAM_EFFECT_MODE_NEON,
    CAM_EFFECT_MODE_MAX
} cam_effect_mode_type;

typedef enum {
    CAM_FLASH_MODE_OFF,
    CAM_FLASH_MODE_AUTO,
    CAM_FLASH_MODE_ON,
    CAM_FLASH_MODE_TORCH,
    CAM_FLASH_MODE_MAX
} cam_flash_mode_t;

typedef struct  {
    int32_t left;
    int32_t top;
    int32_t width;
    int32_t height;
} cam_rect_t;

typedef enum {
    CAM_STREAMING_MODE_CONTINUOUS, /* continous streaming */
    CAM_STREAMING_MODE_BURST,      /* burst streaming */
    CAM_STREAMING_MODE_MAX
} cam_streaming_mode_t;

#define CAM_REPROCESS_MASK_TYPE_WNR (1<<0)

/* event from server */
typedef enum {
    CAM_EVENT_TYPE_MAP_UNMAP_DONE  = (1<<0),
    CAM_EVENT_TYPE_AUTO_FOCUS_DONE = (1<<1),
    CAM_EVENT_TYPE_ZOOM_DONE       = (1<<2),
    CAM_EVENT_TYPE_MAX
} cam_event_type_t;

typedef enum {
    CAM_HDR_BRACKETING_OFF,
    CAM_HDR_MODE,
    CAM_EXP_BRACKETING_MODE
} cam_hdr_mode;

typedef struct {
    cam_hdr_mode mode;
    uint32_t total_frames;
    uint32_t total_hal_frames;
    char values[MAX_EXP_BRACKETING_LENGTH];  /* user defined values */
} cam_exp_bracketing_t;

typedef enum {
    CAM_AEC_ROI_OFF,
    CAM_AEC_ROI_ON
} cam_aec_roi_ctrl_t;

typedef enum {
    CAM_AEC_ROI_BY_INDEX,
    CAM_AEC_ROI_BY_COORDINATE,
} cam_aec_roi_type_t;

typedef struct {
    uint32_t x;
    uint32_t y;
} cam_coordinate_type_t;

typedef struct {
    cam_aec_roi_ctrl_t aec_roi_enable;
    cam_aec_roi_type_t aec_roi_type;
    union {
        cam_coordinate_type_t coordinate;
        uint32_t aec_roi_idx;
    } cam_aec_roi_position;
} cam_set_aec_roi_t;

typedef struct {
    uint32_t frm_id;
    uint8_t num_roi;
    cam_rect_t roi[MAX_ROI];
    uint8_t is_multiwindow;
} cam_roi_info_t;

typedef struct {
    int denoise_enable;
    int process_plates;
} cam_denoise_param_t;

typedef struct {
    int fd_mode;
    int num_fd;
} cam_fd_set_parm_t;

typedef struct {
    int8_t face_id;            /* unique id for face tracking within view unless view changes */
    int8_t score;              /* score of confidence (0, -100) */
    cam_rect_t face_boundary;  /* boundary of face detected */
    cam_coordinate_type_t left_eye_center;  /* coordinate of center of left eye */
    cam_coordinate_type_t right_eye_center; /* coordinate of center of right eye */
    cam_coordinate_type_t mouth_center;     /* coordinate of center of mouth */
    uint8_t smile_degree;      /* smile degree (0, -100) */
    uint8_t smile_confidence;  /* smile confidence (0, 100) */
    uint8_t face_recognised;   /* if face is recognised */
    int8_t gaze_angle;         /* -90 -45 0 45 90 for head left to rigth tilt */
    int8_t updown_dir;         /* up down direction (-90, 90) */
    int8_t leftright_dir;      /* left right direction (-90, 90) */
    int8_t roll_dir;           /* roll direction (-90, 90) */
    int8_t left_right_gaze;    /* left right gaze degree (-50, 50) */
    int8_t top_bottom_gaze;    /* up down gaze degree (-50, 50) */
    uint8_t blink_detected;    /* if blink is detected */
    uint8_t left_blink;        /* left eye blink degeree (0, -100) */
    uint8_t right_blink;       /* right eye blink degree (0, - 100) */
} cam_face_detection_info_t;

typedef struct {
    uint32_t frame_id;                         /* frame index of which faces are detected */
    uint8_t num_faces_detected;                /* number of faces detected */
    cam_face_detection_info_t faces[MAX_ROI];  /* detailed information of faces detected */
} cam_face_detection_data_t;

#define CAM_HISTOGRAM_STATS_SIZE 256
typedef struct {
    uint32_t max_hist_value;
    uint32_t hist_buf[CAM_HISTOGRAM_STATS_SIZE]; /* buf holding histogram stats data */
} cam_histogram_data_t;

enum cam_focus_distance_index{
  CAM_FOCUS_DISTANCE_NEAR_INDEX,  /* 0 */
  CAM_FOCUS_DISTANCE_OPTIMAL_INDEX,
  CAM_FOCUS_DISTANCE_FAR_INDEX,
  CAM_FOCUS_DISTANCE_MAX_INDEX
};

typedef struct {
  float focus_distance[CAM_FOCUS_DISTANCE_MAX_INDEX];
} cam_focus_distances_info_t;

/* Different autofocus cycle when calling do_autoFocus
 * CAM_AF_COMPLETE_EXISTING_SWEEP: Complete existing sweep
 * if one is ongoing, and lock.
 * CAM_AF_DO_ONE_FULL_SWEEP: Do one full sweep, regardless
 * of the current state, and lock.
 * CAM_AF_START_CONTINUOUS_SWEEP: Start continous sweep.
 * After do_autoFocus, HAL receives an event: CAM_AF_FOCUSED,
 * or CAM_AF_NOT_FOCUSED.
 * cancel_autoFocus stops any lens movement.
 * Each do_autoFocus call only produces 1 FOCUSED/NOT_FOCUSED
 * event, not both.
 */
typedef enum {
    CAM_AF_COMPLETE_EXISTING_SWEEP,
    CAM_AF_DO_ONE_FULL_SWEEP,
    CAM_AF_START_CONTINUOUS_SWEEP
} cam_autofocus_cycle_t;

typedef enum {
    CAM_AF_SCANNING,
    CAM_AF_FOCUSED,
    CAM_AF_NOT_FOCUSED
} cam_autofocus_state_t;

typedef struct {
    cam_autofocus_state_t focus_state;           /* state of focus */
    cam_focus_distances_info_t focus_dist;       /* focus distance */
} cam_auto_focus_data_t;

typedef  struct {
    uint8_t is_hist_valid;                /* if histgram data is valid */
    cam_histogram_data_t hist_data;       /* histogram data */

    uint8_t is_faces_valid;               /* if face detection data is valid */
    cam_face_detection_data_t faces_data; /* face detection result */

    uint8_t is_focus_valid;               /* if focus data is valid */
    cam_auto_focus_data_t focus_data;     /* focus data */
} cam_metadata_info_t;

typedef enum {
    CAM_INTF_PARM_QUERY_FLASH4SNAP,
    CAM_INTF_PARM_EXPOSURE,
    CAM_INTF_PARM_SHARPNESS,
    CAM_INTF_PARM_CONTRAST,
    CAM_INTF_PARM_SATURATION,
    CAM_INTF_PARM_BRIGHTNESS,
    CAM_INTF_PARM_WHITE_BALANCE,
    CAM_INTF_PARM_ISO,
    CAM_INTF_PARM_ZOOM,
    CAM_INTF_PARM_ANTIBANDING,
    CAM_INTF_PARM_EFFECT,
    CAM_INTF_PARM_FPS_RANGE,
    CAM_INTF_PARM_EXPOSURE_COMPENSATION,
    CAM_INTF_PARM_LED_MODE,
    CAM_INTF_PARM_ROLLOFF,
    CAM_INTF_PARM_MODE,             /* camera mode */
    CAM_INTF_PARM_AEC_ALGO_TYPE,    /* auto exposure algorithm */
    CAM_INTF_PARM_FOCUS_ALGO_TYPE,  /* focus algorithm */
    CAM_INTF_PARM_AEC_ROI,
    CAM_INTF_PARM_AF_ROI,
    CAM_INTF_PARM_FOCUS_MODE,
    CAM_INTF_PARM_BESTSHOT_MODE,
    CAM_INTF_PARM_SCE_FACTOR,
    CAM_INTF_PARM_FD,
    CAM_INTF_PARM_AEC_LOCK,
    CAM_INTF_PARM_AWB_LOCK,
    CAM_INTF_PARM_MCE,
    CAM_INTF_PARM_HFR,
    CAM_INTF_PARM_REDEYE_REDUCTION,
    CAM_INTF_PARM_WAVELET_DENOISE,
    CAM_INTF_PARM_HISTOGRAM,
    CAM_INTF_PARM_ASD_ENABLE,
    CAM_INTF_PARM_RECORDING_HINT,
    CAM_INTF_PARM_DIS_ENABLE,
    CAM_INTF_PARM_HDR,
    CAM_INTF_PARM_MAX
} cam_intf_parm_type_t;

typedef struct {
    int32_t min_value;
    int32_t max_value;
    int32_t def_value;
    int32_t step;
} cam_control_range_t;

#define CAM_QCOM_FEATURE_SUPPORTED_FACE_DETECTION 0x00000001

#endif /* __QCAMERA_TYPES_H__ */
