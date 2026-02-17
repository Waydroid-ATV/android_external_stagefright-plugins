/*
 * Copyright 2025 BlissLabs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "C2FFMPEGVideoUtils.h"
#include <android-base/properties.h>
#include <system/graphics.h>

namespace android {

C2FFMPEGVideoUtils::C2FFMPEGVideoUtils()
    : mSwapVAColorRGB(base::GetBoolProperty("persist.ffmpeg-codec2.vaapi_rgb_swap_color", false)),
      mOverridePixelFormat(base::GetProperty("debug.ffmpeg-codec2.pixel_format", "YUV_420")) {
}

bool C2FFMPEGVideoUtils::shouldEnableCodec(const std::string codec, bool hwonly) const {
    using namespace ::android;

    const std::string supported_codecs = base::GetProperty("ro.waydroid.hwcodecs", "");

    // For MPEG-2/MPEG-4/H.263, always enable them unless hwonly is true
    if (codec.find("mpeg2") != std::string::npos) {
        return hwonly ? supported_codecs.find("MPG2D") != std::string::npos : true;
    } else if (codec.find("mpeg4") != std::string::npos) {
        return hwonly ? supported_codecs.find("MPG4D") != std::string::npos : true;
    } else if (codec.find("h263") != std::string::npos) {
        return hwonly ? supported_codecs.find("H263D") != std::string::npos : true;
    } else if (codec.find("h264") != std::string::npos) {
        return supported_codecs.find("H264D") != std::string::npos;
    } else if (codec.find("hevc") != std::string::npos) {
        return supported_codecs.find("HEVCD") != std::string::npos;
    } else if (codec.find("vp8") != std::string::npos) {
        return supported_codecs.find("VP80D") != std::string::npos;
    } else if (codec.find("vp9") != std::string::npos) {
        return supported_codecs.find("VP90D") != std::string::npos;
    } else if (codec.find("av1") != std::string::npos) {
        return supported_codecs.find("AV10D") != std::string::npos;
    } else {
        // We don't care about other codecs
        return !hwonly;
    }
}

PixelFormatType C2FFMPEGVideoUtils::getPixelFormatType() const {
    if (mOverridePixelFormat == "YUV_420") { return PixelFormatType::YUV_420;
    } else if (mOverridePixelFormat == "RGB_565") { return PixelFormatType::RGB_565;
    } else if (mOverridePixelFormat == "RGBX_8888") { return PixelFormatType::RGBX_8888;
    } else if (mOverridePixelFormat == "BGRA_8888") { return PixelFormatType::BGRA_8888;}
    return PixelFormatType::UNKNOWN;
}

uint32_t C2FFMPEGVideoUtils::getPixelFormat(bool flexible) const {
    switch (getPixelFormatType()) { // Now switching on an enum
        case PixelFormatType::YUV_420:
            if (flexible) { // Corrected comparison here
                return HAL_PIXEL_FORMAT_YCbCr_420_888;
            } else {
                return HAL_PIXEL_FORMAT_YV12;}
        case PixelFormatType::RGB_565:
            return HAL_PIXEL_FORMAT_RGB_565;
        case PixelFormatType::RGBX_8888:
            return HAL_PIXEL_FORMAT_RGBX_8888;
        case PixelFormatType::BGRA_8888:
            return HAL_PIXEL_FORMAT_BGRA_8888;
        case PixelFormatType::UNKNOWN:
        default:
            break;
    }
    return HAL_PIXEL_FORMAT_YV12;
}

#if CONFIG_VAAPI
uint32_t C2FFMPEGVideoUtils::getVAFormat() const {
    switch (getPixelFormatType()) {
        case PixelFormatType::YUV_420:
            return VA_RT_FORMAT_YUV420;
        case PixelFormatType::RGB_565:
            return VA_RT_FORMAT_RGB16;
        case PixelFormatType::BGRA_8888:
        case PixelFormatType::RGBX_8888:
            return VA_RT_FORMAT_RGB32;
        case PixelFormatType::UNKNOWN:
        default:
            break;
    }
    return VA_RT_FORMAT_YUV420;
}

uint32_t C2FFMPEGVideoUtils::getVAFOURCCFormat() const {
    switch (getPixelFormatType()) {
        case PixelFormatType::YUV_420:
            return VA_FOURCC_NV12;
        case PixelFormatType::RGB_565:
            return VA_FOURCC_RGB565;
        case PixelFormatType::RGBX_8888:
            return VA_FOURCC_RGBX;
        case PixelFormatType::BGRA_8888:
            return VA_FOURCC_BGRA;
        case PixelFormatType::UNKNOWN:
        default:
            break;
    }
    return VA_FOURCC_NV12;
}
#endif

enum AVPixelFormat C2FFMPEGVideoUtils::getAVFormat() const {
    switch (getPixelFormatType()) {
        case PixelFormatType::YUV_420:
            return AV_PIX_FMT_YUV420P;
        case PixelFormatType::RGB_565:
            return AV_PIX_FMT_RGB565;
        case PixelFormatType::RGBX_8888:
            return AV_PIX_FMT_RGB0;
        case PixelFormatType::BGRA_8888:
            return AV_PIX_FMT_BGRA;
        case PixelFormatType::UNKNOWN:
        default:
            break;
    }
    return AV_PIX_FMT_YUV420P;
}

} // namespace android
