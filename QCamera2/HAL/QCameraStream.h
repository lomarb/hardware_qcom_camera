/* Copyright (c) 2012, The Linux Foundataion. All rights reserved.
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
 *     * Neither the name of The Linux Foundation nor the names of its
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

#ifndef __QCAMERA_STREAM_H__
#define __QCAMERA_STREAM_H__

#include <hardware/camera.h>
#include "QCameraCmdThread.h"
#include "QCameraMem.h"
#include "QCameraAllocator.h"

extern "C" {
#include <mm_camera_interface.h>
}

namespace android {

class QCameraStream;
typedef void (*stream_cb_routine)(mm_camera_super_buf_t *frame,
                                  QCameraStream *stream,
                                  void *userdata);

class QCameraStream
{
public:
    QCameraStream(QCameraAllocator &allocator,
                  uint32_t camHandle,
                  uint32_t chId,
                  mm_camera_ops_t *camOps,
                  cam_padding_info_t *paddingInfo);
    virtual ~QCameraStream();
    virtual int32_t init(cam_stream_type_t stream_type,
                stream_cb_routine stream_cb, void *userdata);
    virtual int32_t processZoomDone(preview_stream_ops_t *previewWindow);
    virtual int32_t bufDone(int index);
    virtual int32_t bufDone(const void *opaque, bool isMetaData);
    virtual int32_t processDataNotify(mm_camera_super_buf_t *bufs);
    virtual int32_t start();
    virtual int32_t stop();

    static void dataNotifyCB(mm_camera_super_buf_t *recvd_frame, void *userdata);
    static void *dataProcRoutine(void *data);
    uint32_t getMyHandle() const {return mHandle;}
    bool isTypeOf(cam_stream_type_t type);
    int32_t getFrameOffset(cam_frame_len_offset_t &offset);
    int32_t getCropInfo(cam_rect_t &crop);
    int32_t getFrameDimension(cam_dimension_t &dim);
    int32_t getFormat(cam_format_t &fmt);

private:
    uint32_t mCamHandle;
    uint32_t mChannelHandle;
    uint32_t mHandle; // stream handle from mm-camera-interface
    mm_camera_ops_t *mCamOps;
    cam_stream_info_t *mStreamInfo; // ptr to stream info buf
    mm_camera_stream_mem_vtbl_t mMemVtbl;
    int mNumBufs;
    stream_cb_routine mDataCB;
    void *mUserData;

    QCameraQueue     mDataQ;
    QCameraCmdThread mProcTh; // thread for dataCB

    QCameraHeapMemory *mStreamInfoBuf;
    QCameraMemory *mStreamBufs;
    QCameraAllocator &mAllocator;
    mm_camera_buf_def_t mBufDef[MM_CAMERA_MAX_NUM_FRAMES];
    cam_frame_len_offset_t mFrameLenOffset;
    cam_padding_info_t mPaddingInfo;

    static int32_t get_bufs(
                     cam_frame_len_offset_t *offset,
                     uint8_t *num_bufs,
                     uint8_t **initial_reg_flag,
                     mm_camera_buf_def_t **bufs,
                     mm_camera_map_unmap_ops_tbl_t *ops_tbl,
                     void *user_data);
    static int32_t put_bufs(
                     mm_camera_map_unmap_ops_tbl_t *ops_tbl,
                     void *user_data);

    int32_t getBufs(cam_frame_len_offset_t *offset,
                     uint8_t *num_bufs,
                     uint8_t **initial_reg_flag,
                     mm_camera_buf_def_t **bufs,
                     mm_camera_map_unmap_ops_tbl_t *ops_tbl);
    int32_t putBufs(mm_camera_map_unmap_ops_tbl_t *ops_tbl);

};

}; // namespace android

#endif /* __QCAMERA_STREAM_H__ */
