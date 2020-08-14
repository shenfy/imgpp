#ifndef IMGPP_COMPOSITEIMG_HPP
#define IMGPP_COMPOSITEIMG_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <imgpp/imgpp.hpp>
#include <imgpp/imgpp_bc.hpp>
#include <imgpp/textureformat.hpp>

namespace imgpp {

class CompositeImg {
public:
  CompositeImg() {
  };
  CompositeImg(TextureFormat format, TextureTarget target,
    uint32_t levels, uint32_t layers, uint32_t faces)
    : texture_format_(format), texture_target_(target),
    levels_(levels), layers_(layers), faces_(faces) {
  }
  ~CompositeImg() {
  };

  void SetTarget(TextureTarget target) {
    texture_target_ = target;
  }

  TextureTarget Target() const {
    return texture_target_;
  }

  void SetFormat(TextureFormat format) {
    texture_format_ = format;
  }

  TextureFormat Format() const {
    return texture_format_;
  }

  void AutoGenerateMipmaps() {
    need_generate_mimaps_ = true;
  }

  bool NeedGenerateMipmaps() const {
    return need_generate_mimaps_;
  }

  void SetSize(uint32_t levels, uint32_t layers, uint32_t faces, uint32_t data_size) {
    levels_ = levels;
    layers_ = layers;
    faces_ = faces;
    buffer_.SetSize(data_size);
  }

  // Make sure all rois is 4 bytes alignment
  void AddROI(std::vector<ImgROI> img_rois) {
    img_rois_ = std::move(img_rois);
  }

  const ImgROI &ROI(uint32_t level, uint32_t layer, uint32_t face) const {
    return img_rois_[level * layers_ * faces_ + layer * faces_ + face];
  }

  // Make sure all rois is 4 bytes alignment
  void AddBCROI(std::vector<BCImgROI> bcimg_rois) {
    bcimg_rois_ = std::move(bcimg_rois);
  }

  const BCImgROI &BCROI(uint32_t level, uint32_t layer, uint32_t face) const {
    return bcimg_rois_[level * layers_ * faces_ + layer * faces_ + face];
  }

  void SetInfo(std::string &&key, std::string &&value) {
    customized_info_.insert_or_assign(key, value);
  }

  void SetInfo(const std::string &key, const std::string &value) {
    customized_info_.insert_or_assign(key, value);
  }

  const std::string &GetInfo(const std::string &key) const {
    if (auto it = customized_info_.find(key); it != customized_info_.end()) {
      return it->second;
    } else {
      return "";
    }
  }

  const auto &Info() const {
    return customized_info_;
  }

  ImgBuffer &Data() {
    return buffer_;
  }

  const ImgBuffer &Data() const {
    return buffer_;
  }

  bool IsCompressed() const {
    return IsCompressedFormat(texture_format_);
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
  ImgBuffer buffer_;
  std::vector<ImgROI> img_rois_;
  std::vector<BCImgROI> bcimg_rois_;
  std::unordered_map<std::string, std::string> customized_info_;
  TextureFormat texture_format_{FORMAT_UNDEFINED};
  TextureTarget texture_target_;
  bool need_generate_mimaps_{false};
  // mipmap levels
  uint32_t levels_{0};
  uint32_t layers_{0};
  // for cubemap faces is 6, otherwise 1
  uint32_t faces_{0};
};
}
#endif
