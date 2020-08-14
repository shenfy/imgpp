#ifndef IMGPP_BCIMG_HPP
#define IMGPP_BCIMG_HPP

/*! \file imgpp.hpp */

#include <imgpp/imgbase.hpp>
#include <imgpp/textureformat.hpp>

namespace imgpp {

//! Wrapper for block-compressed images (e.g. ASTC, BC7, ETC2)
class BCImgROI {
public:
  friend class BCImg;

  BCImgROI() {}

  BCImgROI(uint8_t *src, TextureFormat format,
    uint32_t w, uint32_t h, uint32_t depth, uint32_t pitch, uint32_t slice_pitch) {
    if (IsCompressedFormat(format) && w != 0 && h != 0 && depth != 0) {
      Init(src, format, w, h, depth, pitch, slice_pitch);
    }
  }

  BCImgROI(uint8_t *src, TextureFormat format, uint32_t w, uint32_t h, uint32_t depth) {
    if (IsCompressedFormat(format) && w != 0 && h != 0 && depth != 0) {
      auto &desc = GetBCDesc(format);
      uint32_t horizontal_block_num = (w + desc.block_width - 1) / desc.block_width;
      uint32_t vertical_block_num = (h + desc.block_height - 1) / desc.block_height;
      uint32_t pitch = horizontal_block_num * desc.block_bytes;
      uint32_t slice_pitch = pitch * vertical_block_num;
      Init(src, format, w, h, depth, pitch, slice_pitch);
    }
  }

  BCImgROI(uint8_t *src, TextureFormat format, uint32_t w, uint32_t h) :
    BCImgROI(src, format, w, h, 1) {}

  BCImgROI(const BCImgROI &src,
    uint32_t left, uint32_t top, uint32_t front,
    uint32_t right, uint32_t bottom, uint32_t back) {
    if (right < src.width_ && bottom < src.height_ && back < src.depth_
      && right >= left && bottom >= top && back >= front && IsCompressedFormat(src.format_)) {
      auto &desc = GetBCDesc(src.format_);
      // 2D blocks only!!!
      Init((uint8_t*)(src.BlockAt(left / desc.block_width, top / desc.block_height, front)),
        src.format_, right - left + 1, bottom - top + 1, back - front + 1, src.pitch_, src.slice_pitch_);
    }
  }

  BCImgROI SubRegion(
    uint32_t left, uint32_t top, uint32_t front,
    uint32_t right, uint32_t bottom, uint32_t back) {
    BCImgROI new_roi(*this, left, top, front, right, bottom, back);
    return new_roi;
  }

  void *BlockAt(uint32_t block_x, uint32_t block_y) {
    auto &desc = GetBCDesc(format_);
    return data_ + block_y * pitch_ + block_x * desc.block_bytes;
  }

  const void *BlockAt(uint32_t block_x, uint32_t block_y) const {
    auto &desc = GetBCDesc(format_);
    return data_ + block_y * pitch_ + block_x * desc.block_bytes;
  }

  void *BlockAt(uint32_t block_x, uint32_t block_y, uint32_t z) {
    auto &desc = GetBCDesc(format_);
    return data_ + z * slice_pitch_ + block_y * pitch_ + block_x * desc.block_bytes;
  }

  const void *BlockAt(uint32_t block_x, uint32_t block_y, uint32_t z) const {
    auto &desc = GetBCDesc(format_);
    return data_ + z * slice_pitch_ + block_y * pitch_ + block_x * desc.block_bytes;
  }

  TextureFormat Format() const {
    return format_;
  }

  uint32_t Width() const {
    return width_;
  }

  uint32_t Height() const {
    return height_;
  }

  uint32_t Depth() const {
    return depth_;
  }

  uint32_t BlockWidth() const {
    return GetBCDesc(format_).block_width;
  }

  uint32_t BlockHeight() const {
    return GetBCDesc(format_).block_height;
  }

  uint32_t BlockBytes() const {
    return GetBCDesc(format_).block_bytes;
  }

  uint32_t Pitch() const {
    return pitch_;
  }

  uint32_t SlicePitch() const {
    return slice_pitch_;
  }

  uint32_t HorizontalBlockNum() const {
    return horizontal_block_num_;
  }

  uint32_t VerticalBlockNum() const {
    return vertical_block_num_;
  }

  const uint32_t *Dimensions() const {
    return dimensions_;
  }

  const uint8_t *GetData() const {
    return data_;
  }

  uint8_t *GetData() {
    return data_;
  }

  static const uint32_t CalcPitch(TextureFormat format, uint32_t w) {
    auto &desc = GetBCDesc(format);
    if (!IsCompressedFormat(format)) {
      return 0;
    }
    auto horizontal_block_num = (w + desc.block_width - 1) / desc.block_width;
    return horizontal_block_num * desc.block_bytes;
  }

private:
  void Init(uint8_t *src, TextureFormat format,
    uint32_t w, uint32_t h, uint32_t depth,
    uint32_t pitch, uint32_t slice_pitch) {
    data_ = src;
    format_ = format;
    width_ = w;
    height_ = h;
    depth_ = depth;
    pitch_ = pitch;
    slice_pitch_ = slice_pitch;
    auto &desc = GetBCDesc(format);
    horizontal_block_num_ = (width_ + desc.block_width - 1) / desc.block_width;
    vertical_block_num_ = (height_ + desc.block_height - 1) / desc.block_height;
  }

  uint8_t *data_{nullptr}; //!<Data pointer. NOT a smart pointer hence NOT responsible the buffer!
  TextureFormat format_{FORMAT_UNDEFINED};

  union {
    struct {
      uint32_t width_;
      uint32_t height_;
      uint32_t depth_;
      uint32_t horizontal_block_num_;
      uint32_t vertical_block_num_;
    };
    uint32_t dimensions_[5] = {0, 0, 0, 0, 0};
  };

  uint32_t pitch_{0};
  uint32_t slice_pitch_{0};
};

inline bool CopyData(BCImgROI &dst, const BCImgROI &src) {
  if (src.Format() != dst.Format() || src.Width() > dst.Width() || src.Height() > dst.Height()
    || src.Depth() > dst.Depth()) {
    return false;
  }

  uint32_t src_pitch = BCImgROI::CalcPitch(src.Format(), src.Width());
  for (uint32_t z = 0; z < src.Depth(); ++z) {
    for (uint32_t y = 0; y < src.VerticalBlockNum(); ++y) {
      memcpy(dst.BlockAt(0, y, z), src.BlockAt(0, y, z), src_pitch);
    }
  }
  return true;
}

class BCImg: public ImgBase<BCImgROI> {
public:
  BCImg() {}
  ~BCImg() {}

  BCImg(TextureFormat format, uint32_t w, uint32_t h, uint32_t depth = 1) {
    SetSize(format, w, h, depth);
  }

  void SetSize(TextureFormat format, uint32_t w, uint32_t h, uint32_t depth) {
    entire_img_ = BCImgROI(nullptr, format, w, h, depth);
    if (entire_img_.width_ == 0) {
      return;
    }
    buffer_.SetSize(entire_img_.slice_pitch_ * entire_img_.depth_);
    entire_img_.data_ = buffer_.GetBuffer();
  }

  void SetSizeLike(const BCImgROI &src_roi) {
    SetSize(src_roi.format_, src_roi.width_, src_roi.height_, src_roi.depth_);
  }

  bool CopyFrom(const BCImgROI &src_roi) {
    SetSizeLike(src_roi);
    return CopyData(entire_img_, src_roi);
  }
};

}

#endif
