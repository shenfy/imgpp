#ifndef IMGPP_BCIMG_HPP
#define IMGPP_BCIMG_HPP

/*! \file imgpp.hpp */

#include <imgpp/imgbase.hpp>
#include <imgpp/bcdesc.hpp>

namespace imgpp {

//! Wrapper for block-compressed images (e.g. ASTC, BC7, ETC2)
class BCImgROI {
public:
  friend class BCImg;

  BCImgROI() {}

  BCImgROI(uint8_t *src, BCFormat format, uint32_t w, uint32_t h, uint32_t depth) {
    if (format != UNKNOWN_BC && w != 0 && h != 0 && depth != 0) {
      data_ = src;
      format_ = format;
      width_ = w;
      height_ = h;
      depth_ = depth;
      auto &desc = GetBCDesc(format);
      horizontal_block_num_ = width_ / desc.block_width;
      vertical_block_num_ = height_ / desc.block_height;
      pitch_ = horizontal_block_num_ * desc.block_bytes;
      slice_pitch_ = pitch_ * vertical_block_num_;
    }
  }

  BCImgROI(uint8_t *src, BCFormat format, uint32_t w, uint32_t h) :
    BCImgROI(src, format, w, h, 1) {}

  BCImgROI(const BCImgROI &src,
    uint32_t left, uint32_t top,
    uint32_t front, uint32_t right,
    uint32_t bottom, uint32_t back) {

    if (right > left && bottom > top && back > front && src.format_ != UNKNOWN_BC) {
      auto &desc = GetBCDesc(src.format_);
      // 2D blocks only!!!
      data_ = (uint8_t*)(src.BlockAt(left / desc.block_width, right / desc.block_height, front));
      format_ = src.format_;
      width_ = right - left + 1;
      height_ = bottom - top + 1;
      depth_ = back - front + 1;
      horizontal_block_num_ = width_ / desc.block_width;
      vertical_block_num_ = height_ / desc.block_height;
      pitch_ = src.pitch_;
      slice_pitch_ = src.slice_pitch_;
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

  BCFormat Format() const {
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

  static const uint32_t CalcPitch(BCFormat format, uint32_t w) {
    auto &desc = GetBCDesc(format);
    if (format == UNKNOWN_BC || w % desc.block_width != 0) {
      return 0;
    }
    uint32_t horizontal_block_num = w / desc.block_width;
    return horizontal_block_num * desc.block_bytes;
  }

  static const uint32_t CalcHorizontalBlockNum(BCFormat format, uint32_t w) {
    auto &desc = GetBCDesc(format);
    return w / desc.block_width;
  }

  static const uint32_t CalcVerticalBlockNum(BCFormat format, uint32_t h) {
    auto &desc = GetBCDesc(format);
    return h / desc.block_height;
  }


private:
  uint8_t *data_{nullptr}; //!<Data pointer. NOT a smart pointer hence NOT responsible the buffer!
  BCFormat format_{UNKNOWN_BC};

  union {
    struct {
      uint32_t width_{0};
      uint32_t height_{0};
      uint32_t depth_{0};
      uint32_t horizontal_block_num_{0};
      uint32_t vertical_block_num_{0};
    };
    uint32_t dimensions_[5];
  };

  uint32_t pitch_{0};
  uint32_t slice_pitch_{0};
};

inline bool CopyData(BCImgROI &dst, const BCImgROI &src) {
  if (src.Format() != dst.Format() || src.Width() > dst.Width() || src.Height() > dst.Height()
    || src.Depth() > dst.Depth()) {
    return false;
  }

  uint32_t src_length = BCImgROI::CalcPitch(src.Format(), src.Width());
  for (uint32_t z = 0; z < src.Depth(); ++z) {
    for (uint32_t y = 0; y < src.VerticalBlockNum(); ++y) {
      memcpy(dst.BlockAt(0, y, z), src.BlockAt(0, y, z), src_length);
    }
  }
  return true;
}

class BCImg: public ImgBase<BCImgROI> {
public:
  BCImg() {}
  ~BCImg() {}

  BCImg(BCFormat format, uint32_t w, uint32_t h, uint32_t depth = 1) {
    SetSize(format, w, h, depth);
  }

  void SetSize(BCFormat format, uint32_t w, uint32_t h, uint32_t depth) {
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
