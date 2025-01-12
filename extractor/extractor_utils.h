/*
 * Copyright 2012 Michael Chen <omxcodec@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CODEC_UTILS_H_

#define CODEC_UTILS_H_

#include <unistd.h>
#include <stdlib.h>

#include <media/NdkMediaError.h>
#include <media/NdkMediaFormat.h>
#include <media/stagefright/foundation/MediaDefs.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

namespace android {

//////////////////////////////////////////////////////////////////////////////////
// video
//////////////////////////////////////////////////////////////////////////////////

media_status_t setAVCFormat(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setH264Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setMPEG4Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setH263Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setMPEG2VIDEOFormat(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setVC1Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setWMV1Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setWMV2Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setWMV3Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setRV20Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setRV30Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setRV40Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setFLV1Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setHEVCFormat(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setVP8Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setVP9Format(AVCodecParameters *avpar, AMediaFormat *meta);

//////////////////////////////////////////////////////////////////////////////////
// audio
//////////////////////////////////////////////////////////////////////////////////

media_status_t setMP2Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setMP3Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setVORBISFormat(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setAC3Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setAACFormat(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setWMAV1Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setWMAV2Format(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setWMAProFormat(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setWMALossLessFormat(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setRAFormat(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setAPEFormat(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setDTSFormat(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setFLACFormat(AVCodecParameters *avpar, AMediaFormat *meta);
media_status_t setALACFormat(AVCodecParameters *avpar, AMediaFormat *meta);

//////////////////////////////////////////////////////////////////////////////////
// parser
//////////////////////////////////////////////////////////////////////////////////

//Convert H.264 NAL format to annex b
media_status_t convertNal2AnnexB(uint8_t *dst, size_t dst_size,
        uint8_t *src, size_t src_size, size_t nal_len_size);

int getDivXVersion(AVCodecParameters *avpar);

media_status_t parseMetadataTags(AVFormatContext *ctx, AMediaFormat *meta);

AudioEncoding sampleFormatToEncoding(AVSampleFormat fmt);

double get_rotation(AVStream *st);
AVDictionary **setup_find_stream_info_opts(AVFormatContext *, AVDictionary *);

int is_extradata_compatible_with_android(AVCodecParameters *avpar);
int parser_split(AVCodecParameters *avpar, const uint8_t *buf, int buf_size);

//////////////////////////////////////////////////////////////////////////////////
// packet queue
//////////////////////////////////////////////////////////////////////////////////

typedef struct PacketQueue PacketQueue;

PacketQueue* packet_queue_alloc();
void packet_queue_free(PacketQueue **q);
void packet_queue_flush(PacketQueue *q);
void packet_queue_start(PacketQueue *q);
void packet_queue_abort(PacketQueue *q);
int packet_queue_is_wait_for_data(PacketQueue *q);
int packet_queue_put(PacketQueue *q, AVPacket *pkt);
int packet_queue_put_nullpacket(PacketQueue *q, int stream_index);
int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block);

}  // namespace android

#endif  // CODEC_UTILS_H_
