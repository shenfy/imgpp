#ifndef IMGPP_COMPOSITEIMG_HPP
#define IMGPP_COMPOSITEIMG_HPP

/*! \file compositeimg.hpp */

#include <vector>
#include <variant>
#include <imgpp/imgpp.hpp>
#include <imgpp/blockimg.hpp>
#include <imgpp/texturedesc.hpp>
#include <imgpp/texturehelper.hpp>

namespace imgpp {

//! CompositeImg holds images data of a texture
class CompositeImg {
public:
  CompositeImg() {
  }

  ~CompositeImg() {
  }

  const TextureDesc &TexDesc() const {
    return tex_desc_;
  }

  TextureDesc &TexDesc() {
    return tex_desc_;
  }

  //! Specifies texture description, dimensions and alignment of a uncompressed texture
  void SetSize(const TextureDesc &desc, uint32_t levels, uint32_t layers, uint32_t faces,
    uint32_t width, uint32_t height, uint32_t depth, uint8_t alignment) {
    if (desc.format == FORMAT_UNDEFINED) {
      return;
    }
    const auto& pixel_desc = GetPixelDesc(desc.format);
    if (alignment % std::get<2>(pixel_desc) != 0) {
      return;
    }
    tex_desc_ = desc;
    uint32_t bpc = std::get<1>(pixel_desc);
    uint32_t c = std::get<0>(pixel_desc);
    levels_ = levels;
    layers_ = layers;
    faces_ = faces;
    alignment_ = alignment;
    std::vector<ImgROI> rois(levels * layers * faces);
    uint32_t pitch = ImgROI::CalcPitch(width, c, bpc, alignment);
    rois[0] = ImgROI(nullptr, width, height, depth, c, bpc,
      pitch, pitch * height, std::get<3>(pixel_desc), std::get<4>(pixel_desc));
    rois_ = std::move(rois);
  }

  //! Specifies texture description, dimensions and alignment of a compressed texture
  void SetBCSize(TextureDesc desc, uint32_t levels, uint32_t layers, uint32_t faces,
    uint32_t width, uint32_t height, uint32_t depth) {
    if (desc.format != FORMAT_UNDEFINED) {
      tex_desc_ = std::move(desc);
      levels_ = levels;
      layers_ = layers;
      faces_ = faces;
      std::vector<BlockImgROI> rois(levels * layers * faces);
      const BlockSize &block_size = GetBlockSize(tex_desc_.format);
      rois[0] = BlockImgROI(nullptr, block_size, width, height, depth);
      rois_ = std::move(rois);
    }
  }


  //! Create roi for specified dimension
  void SetData(uint8_t *data, uint32_t level, uint32_t layer, uint32_t face) {
    if (tex_desc_.format == FORMAT_UNDEFINED) {
      return;
    }
    if (IsCompressed()) {
      std::vector<BlockImgROI> &rois = std::get<std::vector<BlockImgROI>>(rois_);
      uint32_t width = std::max(rois[0].Width() >> level, 1u);
      uint32_t height = std::max(rois[0].Height() >> level, 1u);
      uint32_t depth = std::max(rois[0].Depth() >> level, 1u);
      rois[level * layers_ * faces_ + layer * faces_ + face] =
        BlockImgROI(data, rois[0].BlkSize(), width, height, depth);
    } else {
      std::vector<ImgROI> &rois = std::get<std::vector<ImgROI>>(rois_);
      uint32_t width = std::max(rois[0].Width() >> level, 1u);
      uint32_t height = std::max(rois[0].Height() >> level, 1u);
      uint32_t depth = std::max(rois[0].Depth() >> level, 1u);
      uint32_t pitch = ImgROI::CalcPitch(width, rois[0].Channel(), rois[0].BPC(), alignment_);
      rois[level * layers_ * faces_ + layer * faces_ + face] =
        ImgROI(data, width, height, depth,
          rois[0].Channel(), rois[0].BPC(), pitch, pitch * height,
          rois[0].IsFloat(), rois[0].IsSigned());
    }
  }

  void AddBuffer(imgpp::ImgBuffer buffer) {
    buffers_.push_back(std::move(buffer));
  }

  const std::vector<ImgBuffer> &Buffers() const {
    return buffers_;
  }

  const ImgROI &ROI(uint32_t level, uint32_t layer, uint32_t face) const {
    return std::get<std::vector<ImgROI>>(rois_)[level * layers_ * faces_ + layer * faces_ + face];
  }

  ImgROI &ROI(uint32_t level, uint32_t layer, uint32_t face) {
    return std::get<std::vector<ImgROI>>(rois_)[level * layers_ * faces_ + layer * faces_ + face];
  }

  const BlockImgROI &BlockROI(uint32_t level, uint32_t layer, uint32_t face) const {
    return std::get<std::vector<BlockImgROI>>(rois_)[level * layers_ * faces_ + layer * faces_ + face];
  }

  BlockImgROI &BlockROI(uint32_t level, uint32_t layer, uint32_t face) {
    return std::get<std::vector<BlockImgROI>>(rois_)[level * layers_ * faces_ + layer * faces_ + face];
  }

  bool IsCompressed() const {
    return IsCompressedFormat(tex_desc_.format);
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

  uint32_t Alignment() const {
    return alignment_;
  }

private:
  std::vector<ImgBuffer> buffers_; /*!< vector of ImgBuffers who holds img data */
  std::variant<std::vector<ImgROI>, std::vector<BlockImgROI>> rois_; /*!< Either vector of ImgROI or BlockImgROI denpens on texture format is compressed or not */
  TextureDesc tex_desc_;
  uint32_t levels_{0}; /*!< mipmap levels */
  uint32_t layers_{0}; /*!< number of textures in texture array, if not an texture array, is 1 */
  uint32_t faces_{0}; /*!< for cubemap faces is 6, otherwise 1 */
  uint8_t alignment_{1};
};
}
#endif
