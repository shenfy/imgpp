#ifndef IMGPP_COMPOSITEIMG_HPP
#define IMGPP_COMPOSITEIMG_HPP

#include <vector>
#include <imgpp/imgpp.hpp>
#include <imgpp/imgpp_bc.hpp>
#include <imgpp/texturedesc.hpp>

namespace imgpp {

class CompositeImg {
public:
  CompositeImg() {
  };
  CompositeImg(TextureDesc desc, uint32_t levels, uint32_t layers, uint32_t faces)
    : tex_desc(desc), levels_(levels), layers_(layers), faces_(faces) {
  }
  ~CompositeImg() {
  };

  const TextureDesc &TexDesc() const {
    return tex_desc;
  }

  TextureDesc &TexDesc() {
    return tex_desc;
  }

  void SetSize(uint32_t levels, uint32_t layers, uint32_t faces,
    uint32_t width, uint32_t height, uint32_t depth, uint32_t c, uint32_t bpc,
    bool is_float, bool is_signed, uint8_t align_bytes) {
    if (tex_desc.format != FORMAT_UNDEFINED) {
      levels_ = levels;
      layers_ = layers;
      faces_ = faces;
      align_bytes_ = align_bytes;
      img_rois_.resize(levels * layers * faces);
      uint32_t pitch = ImgROI::CalcPitch(width, c, bpc, align_bytes);
      ImgROI roi(nullptr, width, height, depth, c, bpc,
        pitch, pitch * height, is_float, is_signed);
      img_rois_[0] = std::move(roi);
    }
  }

  void SetSize(uint32_t levels, uint32_t layers, uint32_t faces,
    uint32_t width, uint32_t height, uint32_t depth) {
    if (tex_desc.format != FORMAT_UNDEFINED) {
      levels_ = levels;
      layers_ = layers;
      faces_ = faces;
      bcimg_rois_.resize(levels * layers * faces);
      BCImgROI bc_roi(nullptr, tex_desc.format, width, height, depth);
      bcimg_rois_[0] = std::move(bc_roi);
    }
  }

  void SetData(uint8_t *data, uint32_t level, uint32_t layer, uint32_t face) {
    if (IsCompressed()) {
      uint32_t width = std::max(bcimg_rois_[0].Width() >> level, 1u);
      uint32_t height = std::max(bcimg_rois_[0].Height() >> level, 1u);
      uint32_t depth = std::max(bcimg_rois_[0].Depth() >> level, 1u);
      BCImgROI bcimg_roi(data, tex_desc.format, width, height, depth);
      bcimg_rois_[level * layers_ * faces_ + layer * faces_ + face] = bcimg_roi;
    } else {
      uint32_t width = std::max(img_rois_[0].Width() >> level, 1u);
      uint32_t height = std::max(img_rois_[0].Height() >> level, 1u);
      uint32_t depth = std::max(img_rois_[0].Depth() >> level, 1u);
      uint32_t pitch = ImgROI::CalcPitch(width, img_rois_[0].Channel(), img_rois_[0].BPC(), align_bytes_);
      ImgROI roi = ImgROI(data, width, height, depth,
          img_rois_[0].Channel(), img_rois_[0].BPC(), pitch, pitch * height,
          img_rois_[0].IsFloat(), img_rois_[0].IsSigned());
      img_rois_[level * layers_ * faces_ + layer * faces_ + face] = roi;
    }
  }

  void AddBuffer(imgpp::ImgBuffer buffer) {
    buffers_.push_back(std::move(buffer));
  }

  const ImgROI &ROI(uint32_t level, uint32_t layer, uint32_t face) const {
    return img_rois_[level * layers_ * faces_ + layer * faces_ + face];
  }


  const BCImgROI &BCROI(uint32_t level, uint32_t layer, uint32_t face) const {
    return bcimg_rois_[level * layers_ * faces_ + layer * faces_ + face];
  }

  bool IsCompressed() const {
    return IsCompressedFormat(tex_desc.format);
  }

  uint32_t Levels() const {
    return levels_;
  }

  uint32_t Layers() const {
    return layers_;
  };

  uint32_t Faces() const {
    return faces_;
  }

private:
  std::vector<ImgBuffer> buffers_;
  std::vector<ImgROI> img_rois_;
  std::vector<BCImgROI> bcimg_rois_;
  TextureDesc tex_desc;
  // mipmap levels
  uint32_t levels_{0};
  uint32_t layers_{0};
  // for cubemap faces is 6, otherwise 1
  uint32_t faces_{0};
  uint32_t align_bytes_{1};
};
}
#endif
