#ifndef CUIMG_IMAGE3D_HPP_
# define CUIMG_IMAGE3D_HPP_

# include <cuimg/gpu/device_image3d.h>
# include <cuimg/error.h>

namespace cuimg
{
  template <typename V>
  device_image3d<V>::device_image3d(unsigned nslices, unsigned nrows, unsigned ncols)
    : domain_(nslices, nrows, ncols)
  {
#ifndef NO_CUDA
    cudaExtent extent;
    extent.width  = ncols * sizeof(V);
    extent.height = nrows;
    extent.depth  = nslices;

    cudaPitchedPtr pp;
    cudaMalloc3D(&pp, extent);
    check_cuda_error();
    assert(pp.ptr);
    data_ = boost::shared_ptr<V>((V*)pp.ptr, cudaFree);
    pitch_ = pp.pitch;
    data_ptr_ = data_.get();
#endif
  }

  template <typename V>
  device_image3d<V>::device_image3d(const domain_type& d)
    : domain_(d)
  {
#ifndef NO_CUDA
    cudaExtent extent;
    extent.width  = domain_.ncols() * sizeof(V);
    extent.height = domain_.nrows();
    extent.depth  = domain_.nslices();

    cudaPitchedPtr pp;
    cudaMalloc3D(&pp, extent);
    check_cuda_error();
    assert(pp.ptr);
    data_ = boost::shared_ptr<V>((V*)pp.ptr, cudaFree);
    pitch_ = pp.pitch;
    data_ptr_ = data_.get();
#endif
  }

  template <typename V>
  device_image3d<V>::device_image3d(const device_image3d<V>& img)
    : domain_(img.domain()),
      pitch_(img.pitch()),
      data_(img.data_),
      data_ptr_(img.data_ptr_)
  {
  }

  template <typename V>
  device_image3d<V>&
  device_image3d<V>::operator=(const device_image3d<V>& img)
  {
    domain_ = img.domain();
    pitch_ = img.pitch();
    data_ = img.data_;
    data_ptr_ = img.data_ptr_;
    return *this;
  }

  template <typename V>
  const typename device_image3d<V>::domain_type& device_image3d<V>::domain() const
  {
    return domain_;
  }

  template <typename V>
  unsigned device_image3d<V>::nrows() const
  {
    return domain_.nrows();
  }

  template <typename V>
  unsigned device_image3d<V>::nslices() const
  {
    return domain_.nslices();
  }
  template <typename V>
  unsigned device_image3d<V>::ncols() const
  {
    return domain_.ncols();
  }

  template <typename V>
  size_t device_image3d<V>::pitch() const
  {
    return pitch_;
  }

  template <typename V>
  V* device_image3d<V>::data()
  {
    return data_ptr_;
  }

  template <typename V>
  const V* device_image3d<V>::data() const
  {
    return data_ptr_;
  }

  template <typename V>
  device_image2d<V>
  device_image3d<V>::slice(unsigned s)
  {
    return device_image2d<V>((V*)(((char*)data_ptr_) + s * pitch_ * nrows()),
                                  nrows(), ncols(), pitch());
  }

  template <typename V>
  const device_image2d<V>
  device_image3d<V>::slice(unsigned s) const
  {
    return device_image2d<V>((V*)(((char*)data_ptr_) + s * pitch_ * nrows()),
                                  nrows(), ncols(), pitch());
  }

  template <typename V>
  V*
  device_image3d<V>::slice_data(unsigned s)
  {
    return (V*)(((char*)data_ptr_) + s * pitch_ * nrows());
  }

  template <typename V>
  const V*
  device_image3d<V>::slice_data(unsigned s) const
  {
    return (V*)(((char*)data_ptr_) + s * pitch_ * nrows());
  }

}

#endif
