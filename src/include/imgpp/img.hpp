#ifndef IMGPP_IMG_HPP
#define IMGPP_IMG_HPP

#include <memory>
#include <cstdint>
#include <cstring>

namespace imgpp {

//! \brief ImgBuffer holds actual pixel data

//! ImgBuffer uses a smart pointer with reference counting to hold pixel data buffers.
//! The class destroys the smart pointer during destruction, hence reducing the reference count to the buffer by 1.
class ImgBuffer {
public:
  ImgBuffer() : length_(0) {}

  ImgBuffer(uint32_t length) {
    SetSize(length);
  }

  //! Allocates memory of the given length, and binds it to data_.
  //! If data_ is already bound to a buffer, its reference count is reduced by 1.
  //! \return true if succeeded, false if failed.
  void SetSize(uint32_t length) {
    if (length_ == length && data_.get()) {
      return;
    }

    uint8_t *data = new uint8_t[length];  // throws std::bad_alloc in case of failure
    data_.reset(data, [](uint8_t *p) { delete []p; });
    length_ = length;
  }

  uint32_t GetLength() const { return length_; }

  //! Get the C-style raw pointer to the buffer. Doesn't affect the reference count.
  uint8_t *GetBuffer() { return data_.get(); }
  const uint8_t *GetBuffer() const { return data_.get(); }

  //! Get a temporary shared copy of the buffer. Reference count += 1.
  std::shared_ptr<uint8_t> GetSharedBuffer() { return data_; }

  bool WriteData(const uint8_t* buffer, uint32_t length) {
    if (length != length_) {
      return false;
    } else {
      memcpy(data_.get(), buffer, length_ * sizeof(uint8_t));
      return true;
    }
  }

  //! Set all pixel value to zero
  void Zeros() {
    memset(data_.get(), 0, length_ * sizeof(uint8_t));
  }

  //! Copy data from another ImgBuffer of the same size
  void CopyFrom(const ImgBuffer &src) {
    if (length_ != src.length_) {
      SetSize(src.length_);
    }

    memcpy(data_.get(), src.data_.get(), src.length_ * sizeof(uint8_t));
  }

  //! Create a deep copy of this ImgBuffer
  ImgBuffer Clone() {
    ImgBuffer result;
    result.CopyFrom(*this);
    return result;
  }

protected:
  std::shared_ptr<uint8_t> data_;
  uint32_t length_;
};

//! Img holds a 2D or 3D image using an ImgBuffer and an ImgROI.
template <typename TROI>
class ImgBase {
public:
  ImgBase() {}
  ~ImgBase() {}

  //! Deep copy from another image
  void CopyFrom(const ImgBase& src) {
    buffer_.CopyFrom(src.buffer_);
    entire_img_ = src.entire_img_;
    entire_img_.data_ = buffer_.GetBuffer();
  }

  //! Create a deep copy of the current Img
  ImgBase<TROI> Clone() {
    ImgBase<TROI> result;
    result.CopyFrom(*this);
    return result;
  }

  //! Set all pixel value to zero
  void Zeros() {
    buffer_.Zeros();
  }

  //! Returns an ROI that covers the entire image.
  TROI &ROI() {
    return entire_img_;
  }
  const TROI &ROI() const {
    return entire_img_;
  }

  //! Returns the member ImgBuffer object. Don't mess with this unless you know what you are doing!
  ImgBuffer &Data() {
    return buffer_;
  }
  const ImgBuffer &Data() const {
    return buffer_;
  }

protected:
  ImgBuffer buffer_; //image buffer
  TROI entire_img_; //ROI covering entire image
};

}

#endif // IMGPP_IMG_HPP
