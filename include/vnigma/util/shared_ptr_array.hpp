#if !defined(VNIGMA_SHARED_PTR_ARRAY)
#define VNIGMA_SHARED_PTR_ARRAY

#include <memory>

namespace vnigma {
using shared_ptr_array = std::shared_ptr<char []>;
using const_shared_ptr_array = std::shared_ptr<char const []>;

inline shared_ptr_array make_shared_ptr_array(std::size_t size) {
#if __cpp_lib_shared_ptr_arrays >= 201707L
    return std::make_shared<char[]>(size);
#else  // __cpp_lib_shared_ptr_arrays >= 201707L
    return std::shared_ptr<char[]>(new char[size]);
#endif // __cpp_lib_shared_ptr_arrays >= 201707L
}

}  // namespace vnigma

#endif  // VNIGMA_SHARED_PTR_ARRAY
