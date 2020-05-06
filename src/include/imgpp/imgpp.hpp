#ifndef IMGPP_IMGPP_HPP
#define IMGPP_IMGPP_HPP

/*! \file imgpp.hpp */

#include <memory>
#include <cstdint>
#include <cstring>

namespace imgpp {

  //! ImgROI is a view into a ImgBuffer or a plain C-style buffer.

  //! ImgROI doesn't "own" the buffer memory, hence the user must make sure the pointer buffer_ is valid before accesing data.
  class ImgROI {
  public:
    //! Give access to member variables only to the class Img for operations like Reshape()
    friend class Img;

    //! Default constructor creating an empty region with buffer_ pointing to NULL.
    ImgROI() :
      data_(0), width_(0), height_(0), channel_(0), depth_(0), bpc_(0),
      is_signed_(true), is_float_(false), pitch_(0), slice_pitch_(0) {}

    //! Constructor for a 2D image with known dimension.
    ImgROI(uint8_t *src, uint32_t w, uint32_t h, uint32_t c,
      uint32_t bpc, uint32_t pitch, bool is_float, bool is_signed) :
      data_(src), width_(w), height_(h), channel_(c), depth_(1), bpc_(bpc),
      is_signed_(is_signed), is_float_(is_float), pitch_(pitch), slice_pitch_(pitch * h) {}

    //! Constructor for a 3D image with known dimension.
    ImgROI(uint8_t *src, uint32_t w, uint32_t h, uint32_t depth, uint32_t c,
      uint32_t bpc, uint32_t pitch, uint32_t slice_pitch, bool is_float, bool is_signed) :
      data_(src), width_(w), height_(h), channel_(c), depth_(depth), bpc_(bpc),
      is_signed_(is_signed), is_float_(is_float), pitch_(pitch), slice_pitch_(slice_pitch) {}

    //! \brief Constructor from a sub-region of an existing ROI.

    //! Given an existing ROI, this method sets the current ROI as a new view into a sub-region of the original ROI,
    //! without modifying the content in the buffer.
    //! This is achieved by inheriting the original pitch and slice pitch and using padding to access pixels.
    //! \param src constant reference to source ROI
    //! \param left left-most pixel of the sub-region, in range [0, width-1]
    //! \param top top-most pixel of the sub-region, in range [0, height-1]
    //! \param front front layer of the sub-region, in range [0, depth-1]
    //! \param right right-most pixel of the sub-region, in range [left, width-1]
    //! \param bottom bottom pixel of the sub-region, in range [top, height-1]
    //! \param back back layer of the sub-region, in range [front, depth-1]
    ImgROI(const ImgROI &src,
      uint32_t left, uint32_t top,
      uint32_t front, uint32_t right,
      uint32_t bottom, uint32_t back) :
      data_((uint8_t*)(src.PtrAt(left, top, front, 0))),
      width_(right - left + 1), height_(bottom - top + 1),
      channel_(src.channel_),
      depth_(back - front + 1), bpc_(src.bpc_),
      is_signed_(src.is_signed_), is_float_(src.is_float_),
      pitch_(src.pitch_), slice_pitch_(src.slice_pitch_) {}

    //! \brief Destructor.

    //! Create a new ROI of a subregion of the current ROI.
    //! Range specified by closed intervals, sizes = (right-left+1, bottom-top+1, back-front+1)
    ImgROI SubRegion(
      uint32_t left, uint32_t top, uint32_t front,
      uint32_t right, uint32_t bottom, uint32_t back) {
      ImgROI new_roi(*this, left, top, front, right, bottom, back);
      return new_roi;
    }

    //! Pitch calculator.
    //! \param w the width of the region.
    //! \param c the number of channels the image has
    //! \param bpc bit-depth (bits per channel, NOT bytes!)
    //! \param align_bytes memory alignment of the starts of each row (e.g. BMPs are aligned to 4 bytes).
    //! 0 or 1 means no alignment (or aligned to bytes).
    static const uint32_t CalcPitch(
      uint32_t w, uint32_t c, uint32_t bpc,
      uint8_t align_bytes = 1) {
      if (align_bytes) {
        return (((c * bpc * w) >> 3) + (align_bytes - 1)) / align_bytes * align_bytes;
      } else {
        return (c * bpc * w) >> 3;
      }
    }

    //! \brief 2D Accessor. Returns a reference to the pixel channel.

    //! If used on a 3D volume, accesses the layer 0.
    //! The template parameter is only used to determine the type of the returned value,
    //! and doesn't affect the location it accesses.
    //! \sa PtrAt()
    template<typename T> T &At(uint32_t x, uint32_t y, uint32_t c) {
      return *(T*)(data_ + y * pitch_ + x * ((bpc_ * channel_) >> 3) + c * (bpc_ >> 3));
    }

    template<typename T> const T &At(uint32_t x, uint32_t y, uint32_t c) const {
      return *(T*)(data_ + y * pitch_ + x * ((bpc_ * channel_) >> 3) + c * (bpc_ >> 3));
    }

    template<typename T> T &At(uint32_t x, uint32_t y) {
      return *(T*)(data_ + y * pitch_ + x * ((bpc_ * channel_) >> 3));
    }

    template<typename T> const T &At(uint32_t x, uint32_t y) const {
      return *(T*)(data_ + y * pitch_ + x * ((bpc_ * channel_) >> 3));
    }

    //! \brief 2D Accessor. Returns a void * pointer to the pixel channel.

    //! These pointer accessors don't require a type template parameter.
    //! \sa Ptr()
    void *PtrAt(uint32_t x, uint32_t y, uint32_t c) {
      return data_ + y * pitch_ + x * ((bpc_ * channel_) >> 3) + c * (bpc_ >> 3);
    }

    const void *PtrAt(uint32_t x, uint32_t y, uint32_t c) const {
      return data_ + y * pitch_ + x * ((bpc_ * channel_) >> 3) + c * (bpc_ >> 3);
    }

    void *PtrAt(uint32_t x, uint32_t y) {
      return data_ + y * pitch_ + x * ((bpc_ * channel_) >> 3);
    }

    const void *PtrAt(uint32_t x, uint32_t y) const {
      return data_ + y * pitch_ + x * ((bpc_ * channel_) >> 3);
    }

    //! \brief 2D Pixel Accessor. Returns all channels of a pixel.

    //! Convenient function for getting values of an entire pixel.
    template<typename TPt, typename TValue = typename TPt::value_type>
    TPt Pixel(uint32_t x, uint32_t y) const {
      auto base = data_ + y * pitch_ + x * ((bpc_ * channel_) >> 3);
      TPt pt;
      for (uint32_t c = 0; c < channel_; c++) {
        pt[c] = static_cast<typename TPt::value_type>(*(TValue *)(base + c * (bpc_ >> 3)));
      }
      return pt;
    }

    //! \brief 3D Accessor. Returns a reference to the pixel channel.

    //! If used on a 2D image, z must be 0.
    //! The template parameter is only used to determine the type of the returned value,
    //! and doesn't affect the location it accesses.
    //! \sa PtrAt()
    template<typename T>
    T &At(uint32_t x, uint32_t y, uint32_t z, uint32_t c) {
      return *(T*)(data_ + z * slice_pitch_ +
        y * pitch_ + x * ((bpc_ * channel_) >> 3) + c * (bpc_ >> 3));
    }

    template<typename T>
    const T &At(uint32_t x, uint32_t y, uint32_t z, uint32_t c) const {
      return *(T*)(data_ + z * slice_pitch_
        + y * pitch_ + x * ((bpc_ * channel_) >> 3) + c * (bpc_ >> 3));
    }

    //! \brief 3D Accessor. Returns a void * pointer to the pixel channel.

    //! These pointer accessors don't require a type template parameter.
    //! \sa Ptr()
    void *PtrAt(uint32_t x, uint32_t y, uint32_t z, uint32_t c) {
      return data_ + z * slice_pitch_
        + y * pitch_ + x * ((bpc_ * channel_) >> 3) + c * (bpc_ >> 3);
    }

    const void *PtrAt(uint32_t x, uint32_t y, uint32_t z, uint32_t c) const {
      return data_ + z * slice_pitch_
        + y * pitch_ + x * ((bpc_ * channel_) >> 3) + c * (bpc_ >> 3);
    }

    //! \brief 3D Pixel Accessor. Returns all channels of a pixel.

    //! Convenient function for getting values of an entire pixel.
    template<typename TPt, typename TValue = typename TPt::value_type>
    TPt Pixel(uint32_t x, uint32_t y, uint32_t z) const {
      auto base = data_ + z * slice_pitch_
      + y * pitch_ + x * ((bpc_ * channel_) >> 3);
      TPt pt;
      for (uint32_t c = 0; c < channel_; c++) {
        pt[c] = static_cast<typename TPt::value_type>(*(TValue *)(base + c * (bpc_ >> 3)));
      }
      return pt;
    }

    //! \brief Accessor to image size metrics, in [width, height, depth, channel] order
    const uint32_t *Dimensions() {
      return dimensions_;
    }

    //! \brief return current image width
    uint32_t Width() const { return width_; }

    //! \brief return current image height
    uint32_t Height() const { return height_; }

    //! \brief return current image depth
    uint32_t Depth() const { return depth_; }

    //! \brief return current image's channel count
    uint32_t Channel() const { return channel_; }

    //! \brief return the BIT-per-channel number of the current image
    uint8_t BPC() const { return bpc_; }

    //! \brief return current image pitch (i.e. bytes from one row to the next)
    uint32_t Pitch() const { return pitch_; }

    //! \brief return current image slice pitch (i.e. bytes from one frame to the next)
    uint32_t SlicePitch() const { return slice_pitch_; }

    //! \brief return if the data is signed
    bool IsSigned() const { return is_signed_; }

    //! \brief return if the data is in floating point format
    bool IsFloat() const { return is_float_; }

    //! \brief return pointer to the memory containing image data
    const uint8_t *GetData() const { return data_; }
    uint8_t *GetData() { return data_; }

  private:
    uint8_t *data_; //!<Data pointer. NOT a smart pointer hence NOT responsible the buffer!

    //meta data
    union {
      struct{ uint32_t width_, height_, channel_, depth_; };
      uint32_t dimensions_[4];
    };

    uint8_t bpc_; //!<bit-depth (bits per channel, NOT bytes)
    bool is_signed_; //!<Signed/unsigned flag for integer types. Is true for floats.
    bool is_float_; //!<Flag for float types.
    uint32_t pitch_; //!<Distance in bytes between consecutive lines
    uint32_t slice_pitch_; //!<Distance in bytes between consecutive slices
  };

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

  /*! \fn bool CopyData(ImgROI &dst, const ImgROI &src)
      \brief Copy rows from source ROI to destination ROI.
      Padding and pitch of the destination ROI is kept.
      \param dst Target ROI.
      \param src Source ROI.
      \return true if succeeded, false if dimension doesn't match.
  */
  inline bool CopyData(ImgROI &dst, const ImgROI &src) {
    if (src.Width() > dst.Width() || src.Height() > dst.Height()
      || src.Depth() > dst.Depth() || src.BPC() != dst.BPC()
      || src.Channel() != dst.Channel()) {
      return false;
    }

    uint32_t src_length = ImgROI::CalcPitch(src.Width(), src.Channel(), src.BPC());
    for (uint32_t layer = 0; layer < src.Depth(); ++layer) {
      for (uint32_t y = 0; y < src.Height(); ++y) {
        memcpy(dst.PtrAt(0, y, layer, 0), src.PtrAt(0, y, layer, 0), src_length);
      }
    }
    return true;
  }

  //! Img holds a 2D or 3D image using an ImgBuffer and an ImgROI.
  class Img {
  public:
    Img() {}
    Img(uint32_t w, uint32_t h, uint32_t depth, uint32_t c, uint32_t bpc,
      bool is_float = false, bool is_signed = false, uint8_t align_bytes = 1) {
      SetSize(w, h, depth, c, bpc, is_float, is_signed, align_bytes);
    }

    Img(uint32_t w, uint32_t h, uint32_t c, uint32_t bpc,
      bool is_float = false, bool is_signed = false, uint8_t align_bytes = 1)
      : Img(w, h, 1, c, bpc, is_float, is_signed, align_bytes) {}

    ~Img() {}

    //! Allocates memory and creates an ROI for the entire image.
    void SetSize(uint32_t w, uint32_t h, uint32_t depth,
      uint32_t c, uint32_t bpc,
      bool is_float = false, bool is_signed = false, uint8_t align_bytes = 1) {
      uint32_t pitch = ImgROI::CalcPitch(w, c, bpc, align_bytes);
      uint32_t slice_pitch = pitch * h;
      buffer_.SetSize(slice_pitch * depth);  // may throw
      entire_img_ = ImgROI(buffer_.GetBuffer(), w, h,
        depth, c, bpc, pitch, slice_pitch, is_float, is_signed);
    }

    //! Allocates memory and creates an ROI for the entire image,
    //! based on the size of a source ROI, ignoring the source's pitch and alignment
    void SetSizeLike(const ImgROI &src_roi, uint8_t align_bytes = 1) {
      uint32_t pitch = ImgROI::CalcPitch(
        src_roi.width_, src_roi.channel_, src_roi.bpc_, align_bytes);
      uint32_t slice_pitch = pitch * src_roi.height_;
      buffer_.SetSize(slice_pitch * src_roi.depth_);  // may throw
      entire_img_ = ImgROI(buffer_.GetBuffer(), src_roi.width_, src_roi.height_,
        src_roi.depth_, src_roi.channel_, src_roi.bpc_, pitch, slice_pitch,
        src_roi.is_float_, src_roi.is_signed_);
    }

    //! Change the shape of the image without modifying the pixel data.
    //! The new dimension must match the old dimension of the entire image.
    //! \param w the new width.
    //! \param h the new height.
    //! \param depth the new depth.
    //! \param c the new channel number.
    //! \param bpc the new bit-depth.
    //! \param align_bytes the new memory align bytes of scan-lines. Defaults to 1 (no alignment).
    //! \return true if succeeded, false if dimension don't match.
    bool ReShape(uint32_t w, uint32_t h, uint32_t depth,
      uint32_t c, uint32_t bpc, uint8_t align_bytes = 1) {
      uint32_t new_pitch = ImgROI::CalcPitch(w, c, bpc, align_bytes);
      if (new_pitch * h * depth != buffer_.GetLength()) {  //doesn't match original size
        return false;
      }

      entire_img_.width_ = w;
      entire_img_.height_ = h;
      entire_img_.channel_ = c;
      entire_img_.depth_ = depth;
      entire_img_.bpc_ = bpc;
      entire_img_.pitch_ = new_pitch;
      entire_img_.slice_pitch_ = new_pitch * h;
      return true;
    }

    //! Deep copy from another image
    void CopyFrom(const Img& src) {
      buffer_.CopyFrom(src.buffer_);
      entire_img_ = src.entire_img_;
      entire_img_.data_ = buffer_.GetBuffer();
    }

    //! Deep copy from an ROI, ignoring the original pitch and alignment
    void CopyFrom(const ImgROI &src_roi, uint8_t align_bytes = 1) {
      uint32_t pitch = ImgROI::CalcPitch(
        src_roi.width_, src_roi.channel_, src_roi.bpc_, align_bytes);
      uint32_t slice_pitch = pitch * src_roi.height_;
      buffer_.SetSize(slice_pitch * src_roi.depth_);
      entire_img_ = ImgROI(buffer_.GetBuffer(), src_roi.width_, src_roi.height_,
        src_roi.depth_, src_roi.channel_, src_roi.bpc_, pitch, slice_pitch,
        src_roi.is_float_, src_roi.is_signed_);

      CopyData(entire_img_, src_roi);
    }

    //! Create a deep copy of the current Img
    Img Clone() {
      Img result;
      result.CopyFrom(*this);
      return result;
    }

    //! Set all pixel value to zero
    void Zeros() {
      buffer_.Zeros();
    }

    //! Returns an ROI that covers the entire image.
    ImgROI &ROI(void) { return entire_img_; }
    const ImgROI &ROI(void) const { return entire_img_; }

    //! Returns the member ImgBuffer object. Don't mess with this unless you know what you are doing!
    ImgBuffer &Data() { return buffer_; }
    const ImgBuffer &Data() const { return buffer_; }

    protected:
    ImgBuffer buffer_; //image buffer
    ImgROI entire_img_; //ROI covering entire image
  };

  inline Img Zeros(uint32_t w, uint32_t h, uint32_t depth, uint32_t c, uint32_t bpc,
    bool is_float = false, bool is_signed = false, uint8_t align_bytes = 1) {
    Img result(w, h, depth, c, bpc, is_float, is_signed, align_bytes);
    result.Zeros();
    return result;
  }

  inline Img ZerosLike(const ImgROI &src_roi) {
    Img result;
    result.SetSizeLike(src_roi);
    result.Zeros();
    return result;
  }


  /*! \fn bool Add2DBorder(Img &dst, const ImgROI &src, uint32_t border_size, uint8_t align_byte = 1)
      \brief Create a new image with a border of size "border_size" to each layer around the source ROI.
  */
  inline bool Add2DBorder(Img &dst, const ImgROI &src,
    uint32_t border_size, uint8_t align_byte = 1) {

    dst.SetSize(src.Width() + 2 * border_size, src.Height() + 2 * border_size, src.Depth(),
      src.Channel(), src.BPC(), src.IsFloat(), src.IsSigned(), align_byte);
    ImgROI dst_roi = dst.ROI().SubRegion(
      border_size, border_size, 0,
      src.Width() + border_size - 1, src.Height() + border_size - 1, src.Depth() - 1);
    return CopyData(dst_roi, src);
  }

} //namespace imgpp

#endif //IMGPP_IMGPP_HPP
