/** \file NDArray.h
\brief Abstract base class for all Gadgetron host and device arrays
*/

#ifndef NDARRAY_H
#define NDARRAY_H
#pragma once

#include "GadgetronException.h"
#include "GadgetronCommon.h"

#include <new>
#include <vector>
#include <iostream>
#include <stdexcept>

#include <boost/shared_ptr.hpp>
#include <boost/cast.hpp>

namespace Gadgetron{

    template <typename T> class NDArray
    {
    public:

        typedef T element_type;
        typedef T value_type;

        void* operator new (size_t bytes)
        {
            return ::new char[bytes];
        }

        void operator delete (void *ptr)
        {
            delete [] static_cast <char *> (ptr);
        } 

        void * operator new(size_t s, void * p)
        {
            return p;
        }

        NDArray () : data_(0), elements_(0), delete_data_on_destruct_(true)
        {
            dimensions_ = boost::shared_ptr< std::vector<size_t> >( new std::vector<size_t> );
        }

        virtual ~NDArray() {}

        virtual void create(std::vector<size_t> &dimensions);
        virtual void create(std::vector<size_t> *dimensions);
        virtual void create(boost::shared_ptr< std::vector<size_t> > dimensions);

        virtual void create(std::vector<size_t> &dimensions, T* data, bool delete_data_on_destruct = false);
        virtual void create(std::vector<size_t> *dimensions, T* data, bool delete_data_on_destruct = false);
        virtual void create(boost::shared_ptr< std::vector<size_t> > dimensions, T* data, bool delete_data_on_destruct = false);

        void squeeze();

        void reshape(std::vector<size_t> *dims);
        void reshape(boost::shared_ptr< std::vector<size_t> > dims);

        bool dimensions_equal(std::vector<size_t> *d) const;

        template<class S> bool dimensions_equal(const NDArray<S> *a) const
        {
            std::vector<size_t>* dim;
            a->get_dimensions(dim);

            if ( this->dimensions_->size() != dim->size() ) return false;

            size_t NDim = this->dimensions_->size();
            for ( size_t d=0; d<NDim; d++ )
            {
                if ( (*this->dimensions_)[d] != (*dim)[d] ) return false;
            }

            return true;
        }

        size_t get_number_of_dimensions() const;

        size_t get_size(size_t dimension) const;

        boost::shared_ptr< std::vector<size_t> > get_dimensions() const;
        void get_dimensions(std::vector<size_t>*& dim) const;

        T* get_data_ptr() const;

        size_t get_number_of_elements() const;

        size_t get_number_of_bytes() const;

        bool delete_data_on_destruct() const;
        void delete_data_on_destruct(bool d);

        size_t calculate_offset(const std::vector<size_t>& ind) const;

        size_t calculate_offset(size_t x, size_t y) const;
        size_t calculate_offset(size_t x, size_t y, size_t z) const;
        size_t calculate_offset(size_t x, size_t y, size_t z, size_t s) const;
        size_t calculate_offset(size_t x, size_t y, size_t z, size_t s, size_t p) const;
        size_t calculate_offset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r) const;
        size_t calculate_offset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a) const;
        size_t calculate_offset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q) const;
        size_t calculate_offset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q, size_t u) const;

        size_t get_offset_factor(size_t dim) const;
        boost::shared_ptr< std::vector<size_t> > get_offset_factor() const;

        size_t get_offset_factor_lastdim() const;

        void calculate_offset_factors(const std::vector<size_t>& dimensions);

        std::vector<size_t> calculate_index( size_t offset ) const;
        void calculate_index( size_t offset, std::vector<size_t>& index ) const;

        void clear();

        T& operator()( const std::vector<size_t>& ind );
        const T& operator()( const std::vector<size_t>& ind ) const;

        T& operator()( size_t x );
        const T& operator()( size_t x ) const;

        T& operator()( size_t x, size_t y );
        const T& operator()( size_t x, size_t y ) const;

        T& operator()( size_t x, size_t y, size_t z );
        const T& operator()( size_t x, size_t y, size_t z ) const;

        T& operator()( size_t x, size_t y, size_t z, size_t s );
        const T& operator()( size_t x, size_t y, size_t z, size_t s ) const;

        T& operator()( size_t x, size_t y, size_t z, size_t s, size_t p );
        const T& operator()( size_t x, size_t y, size_t z, size_t s, size_t p ) const;

        T& operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r );
        const T& operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r ) const;

        T& operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a );
        const T& operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a ) const;

        T& operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q );
        const T& operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q ) const;

        T& operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q, size_t u );
        const T& operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q, size_t u ) const;

    protected:

        virtual void allocate_memory() = 0;
        virtual void deallocate_memory() = 0;

    protected:

        boost::shared_ptr< std::vector<size_t> > dimensions_;
        boost::shared_ptr< std::vector<size_t> > offsetFactors_;
        T* data_;
        size_t elements_;
        bool delete_data_on_destruct_;
    };

    template <typename T> 
    inline void NDArray<T>::create(std::vector<size_t> *dimensions) 
    {
        if(!dimensions) throw std::runtime_error("NDArray<T>::create(): 0x0 pointer provided");
        std::vector<size_t> *tmp = new std::vector<size_t>;
        *tmp = *dimensions;
        dimensions_ = boost::shared_ptr< std::vector<size_t> >(tmp);
        allocate_memory();
        calculate_offset_factors(*dimensions_);
    }

    template <typename T> 
    inline void NDArray<T>::create(std::vector<size_t>& dimensions) 
    {
        std::vector<size_t> *tmp = new std::vector<size_t>;
        *tmp = dimensions;
        dimensions_ = boost::shared_ptr< std::vector<size_t> >(tmp);
        allocate_memory();
        calculate_offset_factors(*dimensions_);
    }

    template <typename T> 
    inline void NDArray<T>::create(boost::shared_ptr< std::vector<size_t> > dimensions)
    {
        this->create(dimensions.get());
    }

    template <typename T> 
    void NDArray<T>::create(std::vector<size_t> *dimensions, T* data, bool delete_data_on_destruct) 
    {
        if (!dimensions) throw std::runtime_error("NDArray<T>::create(): 0x0 pointer provided");
        if (!data) throw std::runtime_error("NDArray<T>::create(): 0x0 pointer provided");    
        std::vector<size_t> *tmp = new std::vector<size_t>;
        *tmp = *dimensions;
        dimensions_ = boost::shared_ptr< std::vector<size_t> >(tmp);
        this->data_ = data;
        this->delete_data_on_destruct_ = delete_data_on_destruct;
        this->elements_ = 1;
        for (size_t i = 0; i < this->dimensions_->size(); i++){
            this->elements_ *= (*this->dimensions_)[i];
        }
        calculate_offset_factors(*dimensions_);
    }

    template <typename T> 
    void NDArray<T>::create(std::vector<size_t> &dimensions, T* data, bool delete_data_on_destruct) 
    {
        if (!data) throw std::runtime_error("NDArray<T>::create(): 0x0 pointer provided");    
        std::vector<size_t> *tmp = new std::vector<size_t>;
        *tmp = dimensions;
        dimensions_ = boost::shared_ptr< std::vector<size_t> >(tmp);
        this->data_ = data;
        this->delete_data_on_destruct_ = delete_data_on_destruct;
        this->elements_ = 1;
        for (size_t i = 0; i < this->dimensions_->size(); i++){
            this->elements_ *= (*this->dimensions_)[i];
        }
        calculate_offset_factors(*dimensions_);
    }

    template <typename T> 
    inline void NDArray<T>::create(boost::shared_ptr<std::vector<size_t>  > dimensions, 
        T* data, bool delete_data_on_destruct)
    {
        this->create(dimensions.get(), data, delete_data_on_destruct);
    }

    template <typename T> 
    inline void NDArray<T>::squeeze()
    {
        boost::shared_ptr< std::vector<size_t> > new_dimensions( new std::vector<size_t> ); 
        for (size_t i = 0; i < dimensions_->size(); i++){
            if ((*dimensions_)[i] != 1){
                new_dimensions->push_back((*dimensions_)[i]);
            }
        }    
        dimensions_ = new_dimensions;
        this->calculate_offset_factors(*dimensions_);
    }

    template <typename T> 
    inline void NDArray<T>::reshape(std::vector<size_t> *dims)
    {
        size_t new_elements = 1;
        for (size_t i = 0; i < dims->size(); i++){
            new_elements *= (*dims)[i];
        }    

        if (new_elements != elements_)
            throw std::runtime_error("NDArray<T>::reshape : Number of elements cannot change during reshape");    

        // Copy the input dimensions array
        std::vector<size_t> *tmp = new std::vector<size_t>;
        *tmp = *dims;
        dimensions_ = boost::shared_ptr< std::vector<size_t> >(tmp);
        this->calculate_offset_factors(*dimensions_);
    }

    template <typename T> 
    inline void NDArray<T>::reshape( boost::shared_ptr< std::vector<size_t> > dims )
    {
        this->reshape(dims.get());
    }

    template <typename T> 
    inline bool NDArray<T>::dimensions_equal(std::vector<size_t> *d) const
    {
        if ( this->dimensions_->size() != d->size() ) return false;

        size_t NDim = this->dimensions_->size();
        for ( size_t ii=0; ii<NDim; ii++ )
        {
            if ( (*this->dimensions_)[ii] != (*d)[ii] ) return false;
        }

        return true;
    }

    template <typename T> 
    inline size_t NDArray<T>::get_number_of_dimensions() const
    {
        return (size_t)dimensions_->size();
    }

    template <typename T> 
    inline size_t NDArray<T>::get_size(size_t dimension) const
    {
        if (dimension >= dimensions_->size()){
            return 1;
        }
        else{
            return (*dimensions_)[dimension];
        }
    }

    template <typename T> 
    inline boost::shared_ptr< std::vector<size_t> > NDArray<T>::get_dimensions() const
    {
        // Make copy to ensure that the receiver cannot alter the array dimensions
        std::vector<size_t> *tmp = new std::vector<size_t>;
        *tmp=*dimensions_;
        return boost::shared_ptr< std::vector<size_t> >(tmp); 
    }

     template <typename T> 
    inline void NDArray<T>::get_dimensions(std::vector<size_t>*& dim) const
    {
        dim = dimensions_.get();
    }

    template <typename T> 
    inline T* NDArray<T>::get_data_ptr() const
    { 
        return data_;
    }

    template <typename T> 
    inline size_t NDArray<T>::get_number_of_elements() const
    {
        return elements_;
    }

    template <typename T> 
    inline size_t NDArray<T>::get_number_of_bytes() const
    {
        return elements_*sizeof(T);
    }

    template <typename T> 
    inline bool NDArray<T>::delete_data_on_destruct() const
    {
        return delete_data_on_destruct_;
    }

    template <typename T> 
    inline void NDArray<T>::delete_data_on_destruct(bool d)
    {
        delete_data_on_destruct_ = d;
    }

    template <typename T> 
    inline size_t NDArray<T>::calculate_offset(const std::vector<size_t>& ind) const
    {
        size_t offset = ind[0];
        for( size_t i = 1; i < dimensions_->size(); i++ )
            offset += ind[i] * (*offsetFactors_)[i];
        return offset;
    }

    template <typename T> 
    inline size_t NDArray<T>::calculate_offset(size_t x, size_t y) const
    {
        GADGET_DEBUG_CHECK_THROW(dimensions_->size()==2);
        return x + y * (*offsetFactors_)[1];
    }

    template <typename T> 
    inline size_t NDArray<T>::calculate_offset(size_t x, size_t y, size_t z) const
    {
        GADGET_DEBUG_CHECK_THROW(dimensions_->size()==3);
        return x + y * (*offsetFactors_)[1] + z * (*offsetFactors_)[2];
    }

    template <typename T> 
    inline size_t NDArray<T>::calculate_offset(size_t x, size_t y, size_t z, size_t s) const
    {
        GADGET_DEBUG_CHECK_THROW(dimensions_->size()==4);
        return x + y * (*offsetFactors_)[1] + z * (*offsetFactors_)[2] + s * (*offsetFactors_)[3];
    }

    template <typename T> 
    inline size_t NDArray<T>::calculate_offset(size_t x, size_t y, size_t z, size_t s, size_t p) const
    {
        GADGET_DEBUG_CHECK_THROW(dimensions_->size()==5);
        return x + y * (*offsetFactors_)[1] + z * (*offsetFactors_)[2] + s * (*offsetFactors_)[3] + p * (*offsetFactors_)[4];
    }

    template <typename T> 
    inline size_t NDArray<T>::calculate_offset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r) const
    {
        GADGET_DEBUG_CHECK_THROW(dimensions_->size()==6);
        return x + y * (*offsetFactors_)[1] + z * (*offsetFactors_)[2] + s * (*offsetFactors_)[3] + p * (*offsetFactors_)[4] + r * (*offsetFactors_)[5];
    }

    template <typename T> 
    inline size_t NDArray<T>::calculate_offset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a) const
    {
        GADGET_DEBUG_CHECK_THROW(dimensions_->size()==7);
        return x + y * (*offsetFactors_)[1] + z * (*offsetFactors_)[2] + s * (*offsetFactors_)[3] + p * (*offsetFactors_)[4] + r * (*offsetFactors_)[5] + a * (*offsetFactors_)[6];
    }

    template <typename T> 
    inline size_t NDArray<T>::calculate_offset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q) const
    {
        GADGET_DEBUG_CHECK_THROW(dimensions_->size()==8);
        return x + y * (*offsetFactors_)[1] + z * (*offsetFactors_)[2] + s * (*offsetFactors_)[3] + p * (*offsetFactors_)[4] + r * (*offsetFactors_)[5] + a * (*offsetFactors_)[6] + q * (*offsetFactors_)[7];
    }

    template <typename T> 
    inline size_t NDArray<T>::calculate_offset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q, size_t u) const
    {
        GADGET_DEBUG_CHECK_THROW(dimensions_->size()==9);
        return x + y * (*offsetFactors_)[1] + z * (*offsetFactors_)[2] + s * (*offsetFactors_)[3] + p * (*offsetFactors_)[4] + r * (*offsetFactors_)[5] + a * (*offsetFactors_)[6] + q * (*offsetFactors_)[7]+ u * (*offsetFactors_)[8];
    }

    template <typename T> 
    inline size_t NDArray<T>::get_offset_factor(size_t dim) const
    {
        if ( dim >= (*dimensions_).size() )
            throw std::runtime_error("NDArray<T>::get_offset_factor : index out of range");
        return (*offsetFactors_)[dim];
    }

    template <typename T> 
    inline size_t NDArray<T>::get_offset_factor_lastdim() const
    {
        if( dimensions_->size() == 0 )
            throw std::runtime_error("NDArray<T>::get_offset_factor_lastdim : array is empty");

        return get_offset_factor(dimensions_->size()-1);
    }

    template <typename T> 
    inline boost::shared_ptr< std::vector<size_t> > NDArray<T>::get_offset_factor() const
    {
        std::vector<size_t> *tmp = new std::vector<size_t>;
        *tmp=*offsetFactors_;
        return boost::shared_ptr< std::vector<size_t> >(tmp); 
    }

    template <typename T> 
    inline void NDArray<T>::calculate_offset_factors(const std::vector<size_t>& dimensions)
    {
        if ( offsetFactors_.get() == NULL ){
            std::vector<size_t> *tmp = new std::vector<size_t>;
            offsetFactors_ = boost::shared_ptr< std::vector<size_t> >(tmp);
        }
        offsetFactors_->resize(dimensions.size());
        for( size_t i = 0; i < dimensions.size(); i++ ){
            size_t k = 1;
            for( size_t j = 0; j < i; j++ )
                k *= dimensions[j];
            (*offsetFactors_)[i] = k;
        }
    }

    template <typename T> 
    inline std::vector<size_t> NDArray<T>::calculate_index( size_t offset ) const
    {
        if( dimensions_->size() == 0 )
            throw std::runtime_error("NDArray<T>::calculate_index : array is empty");

        std::vector<size_t> index(dimensions_->size());
        for( long long i = dimensions_->size()-1; i>=0; i-- ){
            index[i] = offset / (*offsetFactors_)[i];
            offset %= (*offsetFactors_)[i];
        }
        return index;
    }

    template <typename T> 
    inline void NDArray<T>::calculate_index( size_t offset, std::vector<size_t>& index ) const
    {
        if( dimensions_->size() == 0 )
            throw std::runtime_error("NDArray<T>::calculate_index : array is empty");

        index.resize(dimensions_->size(), 0);
        for( long long i = dimensions_->size()-1; i>=0; i-- ){
            index[i] = offset / (*offsetFactors_)[i];
            offset %= (*offsetFactors_)[i];
        }
    }

    template <typename T> 
    void NDArray<T>::clear()
    {
        if ( this->delete_data_on_destruct_ ){
            this->deallocate_memory();
        }
        this->data_ = 0;
        this->elements_ = 0;
        this->delete_data_on_destruct_ = true;

        if ( !this->dimensions_ ){
            this->dimensions_->clear();
            this->offsetFactors_->clear();
        }
        else{
            this->dimensions_ = boost::shared_ptr< std::vector<size_t> >( new std::vector<size_t> );
            this->offsetFactors_ = boost::shared_ptr< std::vector<size_t> >( new std::vector<size_t> );
        }
    } 

    template <typename T> 
    inline T& NDArray<T>::operator()( const std::vector<size_t>& ind )
    {
        size_t idx = this->calculate_offset(ind);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline const T& NDArray<T>::operator()( const std::vector<size_t>& ind ) const
    {
        size_t idx = this->calculate_offset(ind);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline T& NDArray<T>::operator()( size_t x )
    {
        GADGET_DEBUG_CHECK_THROW(x < this->get_number_of_elements());
        return this->get_data_ptr()[x];
    }

    template <typename T> 
    inline const T& NDArray<T>::operator()( size_t x ) const
    {
        GADGET_DEBUG_CHECK_THROW(x < this->get_number_of_elements());
        return this->get_data_ptr()[x];
    }

    template <typename T> 
    inline T& NDArray<T>::operator()( size_t x, size_t y )
    {
        size_t idx = this->calculate_offset(x, y);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline const T& NDArray<T>::operator()( size_t x, size_t y ) const
    {
        size_t idx = this->calculate_offset(x, y);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline T& NDArray<T>::operator()( size_t x, size_t y, size_t z )
    {
        size_t idx = this->calculate_offset(x, y, z);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline const T& NDArray<T>::operator()( size_t x, size_t y, size_t z ) const
    {
        size_t idx = this->calculate_offset(x, y, z);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline T& NDArray<T>::operator()( size_t x, size_t y, size_t z, size_t s )
    {
        size_t idx = this->calculate_offset(x, y, z, s);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline const T& NDArray<T>::operator()( size_t x, size_t y, size_t z, size_t s ) const
    {
        size_t idx = this->calculate_offset(x, y, z, s);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline T& NDArray<T>::operator()( size_t x, size_t y, size_t z, size_t s, size_t p )
    {
        size_t idx = this->calculate_offset(x, y, z, s, p);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline const T& NDArray<T>::operator()( size_t x, size_t y, size_t z, size_t s, size_t p ) const
    {
        size_t idx = this->calculate_offset(x, y, z, s, p);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline T& NDArray<T>::operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r )
    {
        size_t idx = this->calculate_offset(x, y, z, s, p, r);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline const T& NDArray<T>::operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r ) const
    {
        size_t idx = this->calculate_offset(x, y, z, s, p, r);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline T& NDArray<T>::operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a )
    {
        size_t idx = this->calculate_offset(x, y, z, s, p, r, a);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline const T& NDArray<T>::operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a ) const
    {
        size_t idx = this->calculate_offset(x, y, z, s, p, r, a);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline T& NDArray<T>::operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q )
    {
        size_t idx = this->calculate_offset(x, y, z, s, p, r, a, q);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline const T& NDArray<T>::operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q ) const
    {
        size_t idx = this->calculate_offset(x, y, z, s, p, r, a, q);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline T& NDArray<T>::operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q, size_t u )
    {
        size_t idx = this->calculate_offset(x, y, z, s, p, r, a, q, u);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }

    template <typename T> 
    inline const T& NDArray<T>::operator()( size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q, size_t u ) const
    {
        size_t idx = this->calculate_offset(x, y, z, s, p, r, a, q, u);
        GADGET_DEBUG_CHECK_THROW(idx < this->get_number_of_elements());
        return this->get_data_ptr()[idx];
    }
}

#endif //NDARRAY_H