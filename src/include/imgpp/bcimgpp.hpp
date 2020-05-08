#ifndef IMGPP_BCIMGPP_HPP
#define IMGPP_BCIMGPP_HPP

#include "img.hpp"
#include "bcdesc.hpp"

namespace imgpp {

//! \Block based compression img, etc. ASTC, BC3, BC7, ETC1, ETC2
class BCImgROI {
public:
  friend class BCImg;

  BCImgROI() {}

  BCImgROI(uint8_t *src, BCFormat format, uint32_t w, uint32_t h) :
    BCImgROI(src, format, w, h, 1) {}

  BCImgROI(uint8_t *src, BCFormat format, uint32_t w, uint32_t h, uint32_t depth) :
    data_(src), bc_desc_(GetBCDesc(format)), width_(w), height_(h), depth_(depth),
    horizontal_block_num_(width_ / bc_desc_.block_width),
    vertical_block_num(height_ / bc_desc_.block_height),
    block_pitch_(horizontal_block_num_ * bc_desc_.block_bytes),
    slice_pitch_(block_pitch_ * vertical_block_num) {
    Validate();
  }

  BCImgROI(const BCImgROI &src,
    uint32_t left, uint32_t top,
    uint32_t front, uint32_t right,
    uint32_t bottom, uint32_t back) :
    data_((uint8_t*)(src.BlockAt(left / bc_desc_.block_width, right / bc_desc_.block_height, front))),
    bc_desc_(src.bc_desc_), width_(right - left + 1), height_(bottom - top + 1), depth_(back - front + 1),
    block_pitch_(src.block_pitch_), slice_pitch_(src.slice_pitch_) {
    Validate();
  }

  BCImgROI SubRegion(
    uint32_t left, uint32_t top, uint32_t front, uint32_t right, uint32_t bottom, uint32_t back) {
    BCImgROI new_roi(*this, left, top, front, right, bottom, back);
    return new_roi;
  }

  void *BlockAt(uint32_t block_x, uint32_t block_y) {
    return data_ + block_y * block_pitch_ + block_x * bc_desc_.block_bytes;
  }

  const void *BlockAt(uint32_t block_x, uint32_t block_y) const {
    return data_ + block_y * block_pitch_ + block_x * bc_desc_.block_bytes;
  }

  void *BlockAt(uint32_t block_x, uint32_t block_y, uint32_t z) {
    return data_ + z * slice_pitch_ + block_y * block_pitch_ + block_x * bc_desc_.block_bytes;
  }

  const void *BlockAt(uint32_t block_x, uint32_t block_y, uint32_t z) const {
    return data_ + z * slice_pitch_ + block_y * block_pitch_ + block_x * bc_desc_.block_bytes;
  }

  BCFormat Format() const {
    return bc_desc_.format;
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
    return bc_desc_.block_width;
  }

  uint32_t BlockHeight() const {
    return bc_desc_.block_height;
  }

  uint32_t BlockBytes() const {
    return bc_desc_.block_bytes;
  }

  uint32_t BlockPitch() const {
    return block_pitch_;
  }

  uint32_t SlicePitch() const {
    return slice_pitch_;
  }

  uint32_t HorizontalBlockNum() const {
    return horizontal_block_num_;
  }

  uint32_t VerticalBlockNum() const {
    return vertical_block_num;
  }

  const uint32_t *Dimensions () const {
    return dimensions_;
  }

  const uint8_t *GetData() const {
    return data_;
  }

  uint8_t *GetData() {
    return data_;
  }

  static const uint32_t CalcBlockPitch(
    BCFormat format, uint32_t w, uint8_t align_bytes = 1) {
    auto &bc_desc = GetBCDesc(format);
    if (format == Unknown || w % bc_desc.block_width != 0) {
      return 0;
    }
    uint32_t horizontal_block_num = w / bc_desc.block_width;
    if (align_bytes) {
      return (((horizontal_block_num * bc_desc.block_bytes) >> 3) + (align_bytes - 1)) / align_bytes * align_bytes;
    } else {
      return (horizontal_block_num * bc_desc.block_bytes) >> 3;
    }
  }

  static const uint32_t CalcHorizontalBlockNum(BCFormat format, uint32_t w) {
    auto &bc_desc = GetBCDesc(format);
    return w / bc_desc.block_width;
  }

  static const uint32_t CalcVerticalBlockNum(BCFormat format, uint32_t h) {
    auto &bc_desc = GetBCDesc(format);
    return h / bc_desc.block_height;
  }


private:
  void Validate() {
    if (bc_desc_.format == Unknown || width_ % bc_desc_.block_width != 0 ||
      height_ % bc_desc_.block_height != 0) {
      Reset();
    }
  }

  void Reset() {
    data_ = nullptr;
    width_ = height_ = depth_ = horizontal_block_num_ = vertical_block_num = block_pitch_ = slice_pitch_ = 0;
  }

  uint8_t *data_{nullptr}; //!<Data pointer. NOT a smart pointer hence NOT responsible the buffer!
  BCDesc bc_desc_;

  union {
    struct {
      uint32_t width_{0};
      uint32_t height_{0};
      uint32_t depth_{0};
      uint32_t horizontal_block_num_{0};
      uint32_t vertical_block_num{0};
    };
    uint32_t dimensions_[5];
  };

  uint32_t block_pitch_{0};
  uint32_t slice_pitch_{0};
};

inline bool CopyData(BCImgROI &dst, const BCImgROI &src) {
  if (src.Format() != dst.Format() || src.Width() > dst.Width() || src.Height() > dst.Height()
    || src.Depth() > dst.Depth()) {
    return false;
  }

  uint32_t src_length = BCImgROI::CalcBlockPitch(src.Format(), src.Width());
  for (uint32_t layer = 0; layer < src.Depth(); ++layer) {
    for (uint32_t y = 0; y < src.VerticalBlockNum(); ++y) {
      memcpy(dst.BlockAt(0, y, layer), src.BlockAt(0, y, layer), src_length);
    }
  }
  return true;
}

class BCImg: public ImgBase<BCImgROI> {
public:
  BCImg() {}
  ~BCImg() {}

  BCImg(BCFormat format, uint32_t w, uint32_t h, uint32_t depth = 1, uint8_t align_bytes = 1) {
    SetSize(format, w, h, depth, align_bytes);
  }

  void SetSize(BCFormat format, uint32_t w, uint32_t h, uint32_t depth, uint32_t align_bytes = 1) {
    entire_img_ = BCImgROI(nullptr, format, w, h, depth);
    if (entire_img_.width_ == 0) {
      return;
    }
    buffer_.SetSize(entire_img_.slice_pitch_ * entire_img_.depth_);
    entire_img_.data_ = buffer_.GetBuffer();
  }

  void SetSizeLike(const BCImgROI &src_roi, uint8_t align_bytes = 1) {
    SetSize(src_roi.bc_desc_.format, src_roi.width_, src_roi.height_, src_roi.depth_, align_bytes);
  }

  bool CopyFrom(const BCImgROI &src_roi, uint8_t align_bytes = 1) {
    SetSizeLike(src_roi, align_bytes);
    return CopyData(entire_img_, src_roi);
  }
};

}

#endif
