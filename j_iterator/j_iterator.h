#ifndef JITERATOR_H
#define JITERATOR_H
//
#include <J_Fwd_Decl.h>
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
	typedef typename ptrdiff_t				 	difference_type;
	typedef typename traits_type::reference 	reference;
	typedef typename traits_type::pointer   	pointer;

	//From Pointer
	ptr_iterator(const Iter& _i_pos): M_pos(_i_pos){}


	//From Iterator
	ptr_iterator(const ptr_iterator
		<typename std::remove_const<typename std::remove_pointer<Iter>::type>::type*
		, Container>& irk_src)
		:M_pos(irk_src.base()){}

	//Empty Constructor
	ptr_iterator():M_pos(){}

	reference operator*()const{return *M_pos;}
	pointer operator->()const{return M_pos;}

	ptr_iterator& operator++(){++M_pos; return *this;}
	ptr_iterator operator++(int){return ptr_iterator(M_pos++);}

	ptr_iterator operator--(){--M_pos; return *this;}
	ptr_iterator operator--(int){return ptr_iterator(M_pos--);}

	reference operator[](const difference_type& _ik_iX)const{return *(M_pos+_ik_iX);}

	ptr_iterator& operator+=(const difference_type& _ik_iX){M_pos += _ik_iX; return *this;}

	ptr_iterator operator+(const difference_type& _irk_iX){return ptr_iterator(M_pos+_irk_iX);}

	ptr_iterator& operator-=(const difference_type& _irk_iX){M_pos -= _irk_iX; return *this;}

	ptr_iterator operator-(const difference_type& _irk_iX){return ptr_iterator(M_pos-_irk_iX);}

	const Iter& base()const{return M_pos;}
protected:
	Iter get_base_iter(){return M_pos;}
private:
	Iter M_pos;
};

template<typename IterL, typename IterR>
struct points_to_same_non_qualified_type
	: public std::integral_constant<bool, 
		std::is_pointer<typename IterR>::value &&
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
			,typename ptr_iterator<IterL, Container>::difference_type>::value
			,"Difference types Different in Iterator minus operation");

		return 
			static_cast<typename ptr_iterator<IterL, Container>::difference_type>(_i_lhs.base() - _i_rhs.base());
}

template<typename Iter, class Container, class Val_Type = J_Val_Type<typename Container::value_type>>
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

	typedef	node_iterator<Iter, Val_t, Container>	this_type;

	typedef std::bidirectional_iterator_tag		iterator_category;
	
	typedef typename Val_t						value_type;
	typedef typename Val_Types::difference_type	difference_type;

	typedef typename Val_t&						reference;
	typedef typename Val_Types::const_reference	const_reference;

	typedef typename Val_t*						pointer;
	typedef typename Val_Types::const_pointer	const_pointer;

	node_iterator(Iter i_pos):M_node(i_pos){}


	node_iterator(const node_iterator<Iter, typename std::remove_const<Val_t>::type, Container>& irk_src)
		:M_node(irk_src.base()){}

	node_iterator& operator++(){M_node = Container::next_node(M_node); return *this;}
	node_iterator operator++(int){node_iterator temp(*this); ++*this; return temp;}
	node_iterator& operator--(){M_node = Container::previous_node(M_node); return *this;}
	node_iterator operator--(int){node_iterator temp(*this); --*this; return temp;}

	reference operator*()const{return M_node->data;}
	pointer operator->()const{return &M_node->data;}
private:
	
	Iter M_node;
	friend class node_iterator<Iter, typename std::add_const<Val_t>::type, Container>;
	friend class node_iterator<Iter, typename std::remove_const<Val_t>::type, Container>;

	template<typename Iter, class Val_TypeL, class Val_TypeR, class Container>
	friend typename std::enable_if<std::is_same<typename std::remove_cv<Val_TypeL>::type
	, typename std::remove_cv<Val_TypeR>::type>::value, bool>::type 
		operator!=(const node_iterator<Iter, Val_TypeL, Container>& _lhs
	, const node_iterator<Iter, Val_TypeR, Container>& _rhs);

	template<typename Iter, class Val_TypeL, class Val_TypeR, class Container>
	friend typename std::enable_if<std::is_same<typename std::remove_cv<Val_TypeL>::type
	, typename std::remove_cv<Val_TypeR>::type>::value, bool>::type
	operator==(const node_iterator<Iter, Val_TypeL, Container>& _lhs
				, const node_iterator<Iter, Val_TypeR, Container>& _rhs);

	template<typename IterL, class Val_TypeL, typename IterR, class Val_TypeR, class Container>
	friend bool operator<(const node_iterator<IterL, Val_TypeL, Container>& _lhs
	, const node_iterator<IterR, Val_TypeR, Container>& _rhs);

	template<typename IterL, class Val_TypeL, typename IterR, class Val_TypeR, class Container>
	friend bool operator<=(const node_iterator<IterL, Val_TypeL, Container>& _lhs
	, const node_iterator<IterR, Val_TypeR, Container>& _rhs);

	template<typename IterL, class Val_TypeL, typename IterR, class Val_TypeR, class Container>
	friend bool operator<(const node_iterator<IterL, Val_TypeL, Container>& _lhs
	, const node_iterator<IterR, Val_TypeR, Container>& _rhs);

	template<typename IterL, class Val_TypeL, typename IterR, class Val_TypeR, class Container>
	friend bool operator<=(const node_iterator<IterL, Val_TypeL, Container>& _lhs
	, const node_iterator<IterR, Val_TypeR, Container>& _rhs);

	Iter base(){return M_node;}
	const Iter base()const{return M_node;}
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
	typedef typename PairIter_t::iterator_category iterator_category;
	typedef typename PairIter_t::value_type::second_type value_type;
	typedef typename PairIter_t::difference_type 	difference_type;
	typedef const typename PairIter_t::value_type::second_type& reference;
	typedef const typename PairIter_t::value_type::second_type* pointer;



	PairIterator(PairIter_t i_iter):M_iter(i_iter){}

	PairIterator& operator++(){++M_iter; return *this;}
	PairIterator operator++(int){PairIterator temp(*this); ++M_iter; return temp;}

	PairIterator& operator--(){--M_iter; return *this;}
	PairIterator operator--(int){PairIterator temp(*this); --M_iter; return temp;}

	reference operator*(){return M_iter->second;}
	pointer operator->(){return &M_iter->second;}

	bool operator==(const PairIterator& irk_second){
		return (M_iter == irk_second.M_iter);
	}

	bool operator!=(const PairIterator& irk_second){
		return (M_iter != irk_second.M_iter);
	}


private:
	PairIter_t M_iter;
};

template<typename Iter_t>
PairIterator<Iter_t> make_pair_iter(Iter_t i_iter){
	return PairIterator<Iter_t>(i_iter);
}


template<typename Ret_t, typename Iter, typename Val_t, typename Container, typename Func_t>
class value_ptr_iterator : public ptr_iterator<Iter, Container>{
public:
	friend Container;
	typedef Iter iterator_type;
	typedef typename Ret_t						value_type;
	typedef typename long long				 	difference_type;
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

	pointer operator->()const{return &M_func(*base());}

	value_ptr_iterator operator+(difference_type i_pos){
		return value_ptr_iterator(get_base_iter() + i_pos, M_func);
	}

private:
	const Func_t M_func;
};


}

#endif