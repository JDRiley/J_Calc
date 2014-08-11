#ifndef J_ITERATOR_H
#define J_ITERATOR_H
#include <J_Fwd_Decl>
#include <iterator>
#include<type_traits>

namespace jomike{

template<typename _Iter, class _Container>
class ptr_iterator: public std::random_access_iterator_tag{
protected:
	
	typedef std::iterator_traits<_Iter> traits_type;
public:
	friend _Container;
	typedef _Iter iterator_type;
	typedef typename traits_type::iterator_category iterator_category;
	typedef typename traits_type::value_type  	value_type;
	typedef typename long					 	difference_type;
	typedef typename traits_type::reference 	reference;
	typedef typename traits_type::pointer   	pointer;

	//From Pointer
	ptr_iterator(const _Iter& _i_pos): M_pos(_i_pos){}


	//From Iterator
	ptr_iterator(const ptr_iterator
		<typename std::remove_const<typename std::remove_pointer<_Iter>::type>::type*
		, _Container>& irk_src)
		:M_pos(irk_src.base()){}

	//Empty Constructor
	ptr_iterator():M_pos(){}

	inline reference operator*()const{
		return *M_pos;
	}
	inline pointer operator->()const{
		return M_pos;
	}
	inline ptr_iterator& operator++(){
		++M_pos;
		return *this;
	}
	inline ptr_iterator operator++(int){
		return ptr_iterator(M_pos++);
	}
	inline ptr_iterator operator--(){
		--M_pos;
		return *this;
	}
	inline ptr_iterator operator--(int){
		return ptr_iterator(M_pos--);
	}
	inline reference operator[](const difference_type& _ik_iX)const{
		return *(M_pos+_ik_iX);
	}
	inline ptr_iterator& operator+=(const difference_type& _ik_iX){
		M_pos += _ik_iX; return *this;
	}
	inline ptr_iterator operator+(const difference_type& _irk_iX){
		return ptr_iterator(M_pos+_irk_iX);
	}
	inline ptr_iterator& operator-=(const difference_type& _irk_iX){
		M_pos -= _irk_iX; return *this;
	}
	inline ptr_iterator operator-(const difference_type& _irk_iX){
		return ptr_iterator(M_pos-_irk_iX);
	}
	inline const _Iter& base()const{return M_pos;}
private:
	_Iter M_pos;
};

template<typename _IterL, typename _IterR, class Container>
inline bool
	operator!=(const ptr_iterator<_IterL, Container>& _lhs
	, const ptr_iterator<_IterR, Container>& _rhs){
			   return _lhs.base() != _rhs.base();
}

template<typename _IterL, typename _IterR, class Container>
inline bool
	operator<(const ptr_iterator<_IterL, Container>& _lhs
	, const ptr_iterator<_IterR, Container>& _rhs){
			   return _lhs.base() < _rhs.base();
}

template<typename _IterL, typename _IterR, class Container>
inline bool
	operator>(const ptr_iterator<_IterL, Container>& _lhs
	, const ptr_iterator<_IterR, Container>& _rhs){
			   return _lhs.base() > _rhs.base();
}

template<typename _IterL, typename _IterR, class Container>
inline bool
	operator<=(const ptr_iterator<_IterL, Container>& _lhs
	, const ptr_iterator<_IterR, Container>& _rhs){
			   return _lhs.base() <= _rhs.base();
}

template<typename _IterL, typename _IterR, class Container>
inline bool
	operator>=(const ptr_iterator<_IterL, Container>& _lhs
	, const ptr_iterator<_IterR, Container>& _rhs){
		return _lhs.base() >= _rhs.base();
}

template<typename _IteratorL, typename _IteratorR, class _Container>
inline bool
	operator==(const ptr_iterator<_IteratorL, _Container>& _lhs
	, const ptr_iterator<_IteratorR, _Container>& _rhs){
		return _lhs.base() == _rhs.base();
}

template<typename _Iterator, class _Container>
inline bool
operator==(const ptr_iterator<_Iterator, _Container>& _lhs,
	const ptr_iterator<_Iterator, _Container>& _rhs){
	return _lhs.base() == _rhs.base();
}

template<typename _IterL, typename _IterR, class _Container>
inline typename ptr_iterator<_IterL, _Container>::difference_type
	operator-(const ptr_iterator<_IterL, _Container>& _i_lhs
	, const ptr_iterator<_IterR, _Container>& _i_rhs){
		static_assert(std::is_same<typename ptr_iterator<_IterL, _Container>::difference_type
			,typename ptr_iterator<_IterL, _Container>::difference_type>::value
			,"Difference types Different in Iterator minus operation");

		return 
			static_cast<typename ptr_iterator<_IterL, _Container>::difference_type>(_i_lhs.base() - _i_rhs.base());
}

template<typename _Iter, class _Container>
inline typename ptr_iterator<_Iter, _Container>::difference_type
    operator+(const ptr_iterator<_Iter, _Container>& _i_lhs,
	      const ptr_iterator<_Iter, _Container>& _i_rhs){
			  return _i_lhs.base() + _i_rhs.base();
}

template<typename St, class _Container>
class binary_node_iterator;

template<typename _IteratorL, typename _IteratorR, class _Container>
 bool
	operator==(const binary_node_iterator<_IteratorL, _Container>& _lhs
	, const binary_node_iterator<_IteratorR, _Container>& _rhs){
		return _lhs.base() == _rhs.base();
}

template<typename _IteratorL, typename _IteratorR, class _Container>
 bool
	operator!=(const binary_node_iterator<_IteratorL, _Container>& _lhs
	, const binary_node_iterator<_IteratorR, _Container>& _rhs){
		return _lhs.base() != _rhs.base();
}

template<typename Iter, class Container>
class node_iterator : public std::bidirectional_iterator_tag{
public:
	typedef  typename Container::value_type  value_type;
	typedef  j_size_t 	difference_type;
	typedef  typename Container::reference 	reference;
	typedef  typename Container::pointer  	pointer;
	typedef typename Container::const_pointer const_pointer;

	node_iterator& operator++();
	node_iterator operator++(int);
	node_iterator& operator--();
	node_iterator operator--(int);

	reference operator*();
	pointer operator->();

private:
	Iter M_node;
};



template<typename Pair_Iter_t>
class Pair_Iterator{
protected:
	typedef std::iterator_traits<Pair_Iter_t> traits_type;
public:
	typedef typename traits_type::iterator_category iterator_category;
	typedef typename traits_type::value_type::second_type value_type;
	typedef typename traits_type::difference_type 	difference_type;
	typedef typename traits_type::value_type::second_type& reference;
	typedef typename traits_type::value_type::second_type* 	pointer;

	Pair_Iterator(Pair_Iter_t i_iter):M_iter(i_iter){}

	Pair_Iterator& operator++(){++M_iter; return *this;}
	Pair_Iterator operator++(int){Pair_Iterator temp(*this); ++M_iter; return temp;}

	Pair_Iterator& operator--(){--M_iter; return *this;}
	Pair_Iterator operator--(int){Pair_Iterator temp(*this); --M_iter; return temp;}

	reference operator*(){return M_iter->second;}
	pointer operator->(){return &M_iter->second;}

	bool operator==(const Pair_Iterator& irk_second){
		return (M_iter == irk_second.M_iter);
	}

	bool operator!=(const Pair_Iterator& irk_second){
		return (M_iter != irk_second.M_iter);
	}


private:
	Pair_Iter_t M_iter;
};

template<typename Iter_t>
Pair_Iterator<Iter_t> make_pair_iter(Iter_t i_iter){
	return Pair_Iterator<Iter_t>(i_iter);
}


template<typename Ret_t, typename Iter, typename Val_t, typename Container, typename Func_t>
class value_ptr_iterator : public ptr_iterator<Iter, Container>{
public:
	friend Container;
	typedef Iter iterator_type;
	typedef typename Ret_t						value_type;
	typedef typename int					 	difference_type;
	typedef typename Ret_t&	reference;
	typedef typename Ret_t*  	pointer;

	value_ptr_iterator(const Iter& i_pos, const Func_t& irk_func)
		:ptr_iterator<Iter, Container>(i_pos), M_func(irk_func){}


	value_ptr_iterator(const ptr_iterator<Iter, Container>& ir_src, const Func_t& i_func_obj)
		:ptr_iterator<Iter, Container>(ir_src), M_func(i_func_obj){}

	value_ptr_iterator(const Func_t& irk_func):M_func(irk_func){}

	Ret_t operator*()const{
		return M_func(ptr_iterator<Iter, Container>::operator*());
	}
	inline pointer operator->()const{
		return &M_func(*base());
	}
private:
	const Func_t M_func;
};


}

#endif