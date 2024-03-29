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

#define LOG_NIDEBUG 0
#define LOG_TAG "QCamera2Factory"

#include <stdlib.h>
#include <utils/Log.h>
#include <utils/Errors.h>
#include <hardware/camera.h>

#include "QCamera2Factory.h"

namespace android {

QCamera2Factory gQCamera2Factory;

QCamera2Factory::QCamera2Factory()
{
    mNumOfCameras = get_num_of_cameras();
}

QCamera2Factory::~QCamera2Factory()
{
}

int QCamera2Factory::get_number_of_cameras()
{
    return gQCamera2Factory.getNumberOfCameras();
}

int QCamera2Factory::get_camera_info(int camera_id, struct camera_info *info)
{
    return gQCamera2Factory.getCameraInfo(camera_id, info);
}

int QCamera2Factory::getNumberOfCameras()
{
    return mNumOfCameras;
}

int QCamera2Factory::getCameraInfo(int camera_id, struct camera_info *info)
{
    int rc;
    ALOGE("%s: E, camera_id = %d", __func__, camera_id);

    if (!mNumOfCameras || camera_id >= mNumOfCameras || !info)
        return INVALID_OPERATION;

    rc = QCamera2HardwareInterface::getCapabilities(camera_id, info);
    ALOGV("%s: X", __func__);
    return rc;
}

int QCamera2Factory::cameraDeviceOpen(int camera_id,
                    struct hw_device_t **hw_device)
{
    int rc = NO_ERROR;
    if (camera_id < 0 || camera_id >= mNumOfCameras)
        return BAD_VALUE;

    QCamera2HardwareInterface *hw = new QCamera2HardwareInterface(camera_id);
    if (!hw) {
        ALOGE("Allocation of hardware interface failed");
        return NO_MEMORY;
    }
    rc = hw->openCamera(hw_device);
    if (rc != NO_ERROR) {
        delete hw;
    }
    return rc;
}

int QCamera2Factory::camera_device_open(
    const struct hw_module_t *module, const char *id,
    struct hw_device_t **hw_device)
{
    if (module != &HAL_MODULE_INFO_SYM.common) {
        ALOGE("Invalid module. Trying to open %p, expect %p",
            module, &HAL_MODULE_INFO_SYM.common);
        return INVALID_OPERATION;
    }
    if (!id) {
        ALOGE("Invalid camera id");
        return BAD_VALUE;
    }
    return gQCamera2Factory.cameraDeviceOpen(atoi(id), hw_device);
}

struct hw_module_methods_t QCamera2Factory::mModuleMethods = {
    open: QCamera2Factory::camera_device_open,
};

}; // namespace android
