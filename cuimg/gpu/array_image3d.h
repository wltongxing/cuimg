// -*- mode: cuda-mode -*-

#ifndef CUIMG_ARRAY_IMAGE3D_H_
# define CUIMG_ARRAY_IMAGE3D_H_

# include <boost/shared_ptr.hpp>
# include <cuimg/point3d.h>
# include <cuimg/simple_ptr.h>
# include <cuimg/obox3d.h>

namespace cuimg
{
  template <typename V>
  class array_image3d
  {
  public:
    typedef V value_type;
    typedef point3d<int> point;
    typedef obox3d<point> domain_type;

    inline array_image3d(unsigned nslices, unsigned nrows, unsigned ncols);
    inline array_image3d(const domain_type& d);

    __host__ __device__ inline array_image3d(const array_image3d<V>& d);
    __host__ __device__ inline array_image3d<V>& operator=(const array_image3d<V>& d);

    __host__ __device__ inline const domain_type& domain() const;
    __host__ __device__ inline unsigned nrows() const;
    __host__ __device__ inline unsigned ncols() const;
    __host__ __device__ inline unsigned nslices() const;
    __host__ __device__ inline size_t pitch() const;

    __host__ __device__ inline V* data();
    __host__ __device__ inline const V* data() const;

    __host__ __device__ inline device_image2d<V> slice(unsigned s);
    __host__ __device__ inline const device_image2d<V> slice(unsigned s) const;

    __host__ __device__ inline V* slice_data(unsigned s);
    __host__ __device__ inline const V* slice_data(unsigned s) const;

  private:
    domain_type domain_;
    size_t pitch_;
    boost::shared_ptr<V> data_;
    V* data_ptr_;
  };

}

# include <cuimg/gpu/array_image3d.hpp>

#endif
