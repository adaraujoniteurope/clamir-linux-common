#ifndef _components_processing_linear_correction_filter_hpp_
#define _components_processing_linear_correction_filter_hpp_

// this dependency should be private within the module context
// pimpl is be the best approach? absolutely, then I should create
// a proxy in the legacy router to control it.

#include <Eigen>

namespace components::processing {
template <typename type> class linear_correction_filter {
public:
  using pointer_type = std::shared_ptr<linear_correction_filter<type>>;

  linear_correction_filter(type scale, type offset)
      : scale(scale), offset(offset) {}

  static pointer_type create(type scale, type offset) {
    return std::make_shared<linear_correction_filter<type>>();
  }

  int capture_scale(type matrix_type) {

  };

  int apply(type matrix_type) {}

private:
  type scale;
  type offset;
};
}; // namespace components::processing

#endif