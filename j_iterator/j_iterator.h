#ifndef JITERATOR_H
#define JITERATOR_H
//
#include "J_Fwd_Decl.h"
//
#include <iterator>
//
#include<type_traits>

namespace jomike{

template<typename Iter, class Container>
class ptr_iterator{
protected:

	typedef std::iterator_traits<Iter> traits_type;
public:
	friend Container;
	typedef Iter iterator_type;
	typedef typename traits_type::iterator_category iterator_category;
	typedef typename traits_type::value_type  	value_type;
	typedef std::ptrdiff_t				 	difference_type;
	typedef typename traits_type::reference 	reference;
	typedef typename traits_type::pointer   	pointer;
	typedef Container container_type;
	//From Pointer
	ptr_iterator(const Iter& _i_pos): M_pos(_i_pos){}


	//From Iterator
	ptr_iterator(const ptr_iterator
					<typename std::remove_const<typename std::remove_pointer<Iter>::type>::type*
					, Container>& irk_src)
					:M_pos(irk_src.base()){}

	//Empty Constructor
	ptr_iterator():M_pos(){}

	reference operator*()const{ return *M_pos; }
	pointer operator->()const{ return M_pos; }

	ptr_iterator& operator++(){ ++M_pos; return *this; }
	ptr_iterator operator++(int){ return ptr_iterator(M_pos++); }

	ptr_iterator operator--(){ --M_pos; return *this; }
	ptr_iterator operator--(int){ return ptr_iterator(M_pos--); }

	reference operator[](const difference_type& _ik_iX)const{ return *(M_pos + _ik_iX); }

	ptr_iterator& operator+=(const difference_type& _ik_iX){ M_pos += _ik_iX; return *this; }

	ptr_iterator operator+(const difference_type& _irk_iX){ return ptr_iterator(M_pos + _irk_iX); }

	ptr_iterator& operator-=(const difference_type& _irk_iX){ M_pos -= _irk_iX; return *this; }

	ptr_iterator operator-(const difference_type& _irk_iX){ return ptr_iterator(M_pos - _irk_iX); }

	const Iter& base()const{ return M_pos; }
protected:
	Iter get_base_iter(){ return M_pos; }
private:
	Iter M_pos;
};

template <class Iterator>
class ptr_wrapper_iterator : public Iterator{
public:
	ptr_wrapper_iterator(const Iterator& i_iterator): Iterator(i_iterator){}
	ptr_wrapper_iterator(const ptr_wrapper_iterator<Iterator>&) = default;

	typename std::remove_pointer<typename Iterator::value_type>::type&
		operator*(){ return *(Iterator::operator*()); }

	typename Iterator::value_type operator->(){ return *(Iterator::operator->()); }


	bool operator!=(const ptr_wrapper_iterator& irk_left){
		return this != static_cast<Iterator>(irk_left);
	}
private:
};

template<typename IterL, typename IterR>
struct points_to_same_non_qualified_type
	: public std::integral_constant<bool,
	std::is_pointer<IterR>::value &&
	std::is_same<typename std::remove_const<typename std::remove_pointer<IterL>::type>::type
	, typename std::remove_const<typename std::remove_pointer<IterR>::type>::type>::value>{};

template<typename Val_TypeL, typename Val_TypeR, typename St>
struct enable_if_same_non_qualified_type{
	typedef typename std::enable_if<
	std::is_same<typename std::remove_cv<St>::type, typename std::remove_cv<St>::type>::value
	, St>::type type;
};

template<typename IterL, typename IterR, class Container>
typename std::enable_if<points_to_same_non_qualified_type<IterL, IterR>::value, bool>::type
	operator!=(const ptr_iterator<IterL, Container>& _lhs
	, const ptr_iterator<IterR, Container>& _rhs){
		return _lhs.base() != _rhs.base();
	}

template<typename IterL, typename IterR, class Container>
typename std::enable_if<points_to_same_non_qualified_type<IterL, IterR>::value, bool>::type
	operator<(const ptr_iterator<IterL, Container>& _lhs
	, const ptr_iterator<IterR, Container>& _rhs){
		return _lhs.base() < _rhs.base();
	}

template<typename IterL, typename IterR, class Container>
typename std::enable_if<points_to_same_non_qualified_type<IterL, IterR>::value, bool>::type
	operator>(const ptr_iterator<IterL, Container>& _lhs
	, const ptr_iterator<IterR, Container>& _rhs){
		return _lhs.base() > _rhs.base();
	}

template<typename IterL, typename IterR, class Container>
typename std::enable_if<points_to_same_non_qualified_type<IterL, IterR>::value, bool>::type
	operator<=(const ptr_iterator<IterL, Container>& _lhs
	, const ptr_iterator<IterR, Container>& _rhs){
		return _lhs.base() <= _rhs.base();
	}

template<typename IterL, typename IterR, class Container>
typename std::enable_if<points_to_same_non_qualified_type<IterL, IterR>::value, bool>::type
	operator>=(const ptr_iterator<IterL, Container>& _lhs
	, const ptr_iterator<IterR, Container>& _rhs){
		return _lhs.base() >= _rhs.base();
	}

template<typename IterL, typename IterR, class Container>
typename std::enable_if<points_to_same_non_qualified_type<IterL, IterR>::value, bool>::type
	operator==(const ptr_iterator<IterL, Container>& _lhs
	, const ptr_iterator<IterR, Container>& _rhs){
		return _lhs.base() == _rhs.base();
	}

template<typename IterL, typename IterR, class Container>
typename std::enable_if<points_to_same_non_qualified_type<IterL, IterR>::value
	, typename ptr_iterator<IterL, Container>::difference_type>::type
	operator-(const ptr_iterator<IterL, Container>& _i_lhs
	, const ptr_iterator<IterR, Container>& _i_rhs){
		static_assert(std::is_same<typename ptr_iterator<IterL, Container>::difference_type
						, typename ptr_iterator<IterL, Container>::difference_type>::value
						, "Difference types Different in Iterator minus operation");

		return
			static_cast<typename ptr_iterator<IterL, Container>::difference_type>(_i_lhs.base() - _i_rhs.base());
	}

template<typename Iter, class Val_Type, class Container>
class node_iterator;

template<typename Iter, class Val_TypeL, class Val_TypeR, class Container>
typename std::enable_if<std::is_same<typename std::remove_cv<Val_TypeL>::type
	, typename std::remove_cv<Val_TypeR>::type>::value, bool>::type
	operator!=(const node_iterator<Iter, Val_TypeL, Container>& _lhs
	, const node_iterator<Iter, Val_TypeR, Container>& _rhs);

template<typename Iter, class Val_TypeL, class Val_TypeR, class Container>
typename std::enable_if<std::is_same<typename std::remove_cv<Val_TypeL>::type
	, typename std::remove_cv<Val_TypeR>::type>::value, bool>::type
	operator==(const node_iterator<Iter, Val_TypeL, Container>& _lhs
	, const node_iterator<Iter, Val_TypeR, Container>& _rhs);

template<typename Iter, typename Val_t, class Container>
class node_iterator{
public:
	typedef typename Container::allocator_type::template rebind<Val_t>::other Val_Types;
	typedef	node_iterator	this_type;

	typedef std::bidirectional_iterator_tag		iterator_category;

	typedef Val_t						value_type;
	typedef typename Container::difference_type	difference_type;

	typedef value_type&						reference;
	typedef const value_type&	const_reference;

	typedef value_type*						pointer;
	typedef const value_type*	const_pointer;

	
	node_iterator():M_node(){}

	node_iterator(const node_iterator<Iter, typename std::remove_const<Val_t>::type, Container>& irk_src)
		:M_node(irk_src.base()){}

	node_iterator& operator++(){ M_node = Container::next_node(M_node); return *this; }
	node_iterator operator++(int){ node_iterator temp(*this); ++*this; return temp; }
	node_iterator& operator--(){ M_node = Container::previous_node(M_node); return *this; }
	node_iterator operator--(int){ node_iterator temp(*this); --*this; return temp; }

	reference operator*()const{ assert(M_node);  return M_node->data(); }
	pointer operator->()const{ assert(M_node);  return &M_node->data(); }
private:
	node_iterator(Iter i_pos):M_node(i_pos){}



	Iter M_node;
	//For allowing access to base() for copy construction
	friend class node_iterator<Iter, typename std::add_const<Val_t>::type, Container>;
	friend class node_iterator<Iter, typename std::remove_const<Val_t>::type, Container>;

	/*For allowing access to .base() for the container to go from an iterator to a node	*/
	/* Also for allowing access to the private node_iterator(Iter) constructor			*/
	friend Container;


	
	template<typename Iter_t, typename Val_TypeL, typename Val_TypeR, typename Cont_t>
	friend typename std::enable_if<std::is_same<typename std::remove_cv<Val_TypeL>::type
		, typename std::remove_cv<Val_TypeR>::type>::value, bool>::type
		operator!=(const node_iterator<Iter_t, Val_TypeL, Cont_t>& _lhs
		, const node_iterator<Iter_t, Val_TypeR, Cont_t>& _rhs);

	template<typename Iter_t, typename Val_TypeL, typename Val_TypeR, typename Cont_t>
	friend typename std::enable_if<std::is_same<typename std::remove_cv<Val_TypeL>::type
		, typename std::remove_cv<Val_TypeR>::type>::value, bool>::type
		operator==(const node_iterator<Iter_t, Val_TypeL, Cont_t>& _lhs
		, const node_iterator<Iter_t, Val_TypeR, Cont_t>& _rhs);

	Iter base(){ return M_node; }
	const Iter base()const{ return M_node; }
};


template<typename Iter, class Val_TypeL, class Val_TypeR, class Container>
typename std::enable_if<std::is_same<typename std::remove_cv<Val_TypeL>::type
	, typename std::remove_cv<Val_TypeR>::type>::value, bool>::type
	operator!=(const node_iterator<Iter, Val_TypeL, Container>& _lhs
	, const node_iterator<Iter, Val_TypeR, Container>& _rhs){
		return _lhs.base() != _rhs.base();
	}

template<typename Iter, class Val_TypeL, class Val_TypeR, class Container>
typename std::enable_if<std::is_same<typename std::remove_cv<Val_TypeL>::type
	, typename std::remove_cv<Val_TypeR>::type>::value, bool>::type
	operator==(const node_iterator<Iter, Val_TypeL, Container>& _lhs
	, const node_iterator<Iter, Val_TypeR, Container>& _rhs){
		return _lhs.base() == _rhs.base();
}



template<typename PairIter_t>
class PairIterator{

public:
	typedef PairIter_t iter_type;
	typedef typename std::bidirectional_iterator_tag iterator_category;
	typedef typename PairIter_t::value_type::second_type value_type;
	typedef typename PairIter_t::difference_type 	difference_type;
	typedef const typename PairIter_t::value_type::second_type& reference;
	typedef const typename PairIter_t::value_type::second_type* pointer;

	PairIterator():M_iter(){}

	PairIterator(PairIter_t i_iter):M_iter(i_iter){}

	PairIterator& operator++(){ ++M_iter; return *this; }
	PairIterator operator++(int){ PairIterator temp(*this); ++M_iter; return temp; }

	PairIterator& operator--(){ --M_iter; return *this; }
	PairIterator operator--(int){ PairIterator temp(*this); --M_iter; return temp; }

	reference operator*(){ return M_iter->second; }
	pointer operator->(){ return &M_iter->second; }



	const PairIter_t & base()const{ return M_iter; }
private:
	PairIter_t M_iter;
};


template<typename IterL, typename IterR>
bool operator!=(const PairIterator<IterL>& irk_left, const PairIterator<IterR>& irk_right){
	return !(irk_left.base() == irk_right.base());
}

template<typename IterL, typename IterR>
bool operator==(const PairIterator<IterL>& irk_left, const PairIterator<IterR>& irk_right){
	return (irk_left.base() == irk_right.base());
}

template<typename Iter_t>
PairIterator<Iter_t> make_pair_iter(Iter_t i_iter){
	return PairIterator<Iter_t>(i_iter);
}


template<typename Ret_t, typename Iter, typename Container, typename Func_t>
class value_ptr_iterator : public ptr_iterator<Iter, Container>{
public:
	friend Container;
	typedef Iter iterator_type;
	typedef Ret_t						value_type;
	typedef long long				 	difference_type;
	typedef Ret_t&	reference;
	typedef Ret_t*  	pointer;
	typedef ptr_iterator<Iter, Container> M_base_t;

	using M_base_t::base;

	value_ptr_iterator(const Iter& i_pos, const Func_t& irk_func)
		:ptr_iterator<Iter, Container>(i_pos), M_func(irk_func){}


	value_ptr_iterator(const ptr_iterator<Iter, Container>& irk_src, const Func_t& i_func_obj)
		:ptr_iterator<Iter, Container>(irk_src), M_func(i_func_obj){}

	value_ptr_iterator(const Func_t& irk_func):M_func(irk_func){}

	Ret_t operator*()const{
		return M_func(*base());
	}

	pointer operator->()const{ return &M_func(M_base_t::operator*()); }

	value_ptr_iterator operator+(difference_type i_pos){
		return value_ptr_iterator(get_base_iter() + i_pos, M_func);
	}

	value_ptr_iterator operator++(int){
		value_ptr_iterator temp(*this);
		++*this;
		return temp;
	}

	value_ptr_iterator& operator++(){
		M_base_t::operator++();
		return *this;
	}
private:
	using M_base_t::get_base_iter;
	const Func_t M_func;
};

template<typename Ret_t, typename Ptr_Iterator_t, typename Func_t>
value_ptr_iterator<Ret_t
	, typename Ptr_Iterator_t::iterator_type, typename Ptr_Iterator_t::container_type
	, Func_t> make_value_ptr_iterator(Ptr_Iterator_t i_iterator, Func_t i_func_obj){
		return value_ptr_iterator<Ret_t//decltype(i_func_obj(Ptr_Iterator_t::value_type()))
			, typename Ptr_Iterator_t::iterator_type, typename Ptr_Iterator_t::container_type
			, Func_t>(i_iterator, i_func_obj);
	}
}

#endif