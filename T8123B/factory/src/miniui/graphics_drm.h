/*
 * Copyright (C) 2017 The Android Open Source Project
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

#pragma once

#include <stddef.h>
#include <stdint.h>

#include <memory>

#include <xf86drmMode.h>

#include "graphics_drm.h"
#include "minui.h"

static PixelFormat pixel_format = PixelFormat::UNKNOWN;




class MinuiBackend {
 public:
  // Initializes the backend and returns a GRSurface* to draw into.
  virtual GRSurface* Init() = 0;

  // Causes the current drawing surface (returned by the most recent call to Flip() or Init()) to
  // be displayed, and returns a new drawing surface.
  virtual GRSurface* Flip() = 0;
  virtual GRSurface* Flip_setCrtc() = 0;
  

  // Blank (or unblank) the screen.
  virtual void Blank(bool) = 0;

  // For camera
  virtual int getDisplayWidth() = 0;
  virtual int getDisplayHeight() = 0;
  virtual int getDrmFd() = 0;
  virtual unsigned int getDrmConnectId() = 0;
  virtual unsigned int getDrmCrtcId() = 0;
  virtual drmModeModeInfo* getDrmCrtcMode() = 0;

  // Device cleanup when drawing is done.
  virtual ~MinuiBackend() {};
};

/*
struct GRFont {
  GRSurface* texture;
  int char_width;
  int char_height;
};

enum class GRRotation : int {
  NONE = 0,
  RIGHT = 1,
  DOWN = 2,
  LEFT = 3,
};

enum class PixelFormat : int {
  UNKNOWN = 0,
  ABGR = 1,
  RGBX = 2,
  BGRA = 3,
  ARGB = 4,
};

*/

class GRSurfaceDrm : public GRSurface {
 public:
  ~GRSurfaceDrm() override;

  // Creates a GRSurfaceDrm instance.
  static std::unique_ptr<GRSurfaceDrm> Create(int drm_fd, int width, int height);

  uint8_t* data() override {
    return mmapped_buffer_;
  }

 private:
  friend class MinuiBackendDrm;

  GRSurfaceDrm(size_t width, size_t height, size_t row_bytes, size_t pixel_bytes, int drm_fd,
               uint32_t handle)
      : GRSurface(width, height, row_bytes, pixel_bytes), drm_fd_(drm_fd), handle(handle) {}

  const int drm_fd_;

  uint32_t fb_id{ 0 };
  uint32_t handle{ 0 };
  uint8_t* mmapped_buffer_{ nullptr };
};

class MinuiBackendDrm : public MinuiBackend {
 public:
  MinuiBackendDrm() = default;
  ~MinuiBackendDrm() override;

  GRSurface* Init() override;
  GRSurface* Flip() override;
  GRSurface* Flip_setCrtc() override;
  void Blank(bool) override;

  // For camera
  int getDisplayWidth() override;
  int getDisplayHeight() override;
  int getDrmFd() override;
  unsigned int getDrmConnectId() override;
  unsigned int getDrmCrtcId() override;
  drmModeModeInfo* getDrmCrtcMode() override;


 private:
  void DrmDisableCrtc(int drm_fd, drmModeCrtc* crtc);
  bool DrmEnableCrtc(int drm_fd, drmModeCrtc* crtc, const std::unique_ptr<GRSurfaceDrm>& surface);
  void DisableNonMainCrtcs(int fd, drmModeRes* resources, drmModeCrtc* main_crtc);
  drmModeConnector* FindMainMonitor(int fd, drmModeRes* resources, uint32_t* mode_index);

  std::unique_ptr<GRSurfaceDrm> GRSurfaceDrms[2];
  int current_buffer{ 0 };
  drmModeCrtc* main_monitor_crtc{ nullptr };
  drmModeConnector* main_monitor_connector{ nullptr };
  int drm_fd{ -1 };
};
