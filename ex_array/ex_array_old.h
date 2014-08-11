#ifndef EX_ARRAY_H
#define EX_ARRAY_H

#include <vld\vld.h>

#include <J_Error>
#include <j_iterator>
#include <J_Fwd_Decl>
//Algorithm
#include<algorithm>

#if EX_ARRAY_TEST
#include<iostream>

#endif



//Utility 
#include <cassert>
#include <utility>



namespace jomike{

template<typename St, class Alloc = std::allocator<St>>
class ex_array;

//reguires no space in class_name
#define EX_ARRAY_TYPEDEF_UPPER(class_name) typedef ex_array<class_name> class_name##_Arr_t;

//Requires no space in type name
#define EX_ARRAY_TYPEDEF_LOWER(class_name) typedef ex_array<class_name> class_name##_arr_t;


typedef unsigned long ul;
typedef long long ll;
EX_ARRAY_TYPEDEF_LOWER(int)
EX_ARRAY_TYPEDEF_LOWER(double)
EX_ARRAY_TYPEDEF_LOWER(char)
EX_ARRAY_TYPEDEF_LOWER(float)
EX_ARRAY_TYPEDEF_LOWER(unsigned)
EX_ARRAY_TYPEDEF_LOWER(ul)
EX_ARRAY_TYPEDEF_LOWER(long)
EX_ARRAY_TYPEDEF_LOWER(ll)


const int MAX_WASTE = 8;
const int DEFAULT_FRONT_BUFFER = 8;
/*Ex Array Class*/
template<typename St, class Alloc>
class ex_array{
public:
	typedef ex_array<St, Alloc> this_type;
	typedef  std::random_access_iterator_tag iterator_category;
	typedef  St  	value_type;
	typedef  ptrdiff_t 	difference_type;
	typedef  St& 	reference;
	typedef const St& const_reference;
	typedef  St*  	pointer;
	typedef  const St* const_pointer;
	typedef unsigned size_t;

	/*Iterator*/
	typedef ptr_iterator<pointer, ex_array> iterator;
	typedef ptr_iterator<const_pointer, ex_array> const_iterator;

	/*Size*/
	inline int size()const{
		return static_cast<int>(M_used - M_front);
	};
	/*Reserve*/
	inline void reserve(int i_size){
		if((M_front + i_size) > M_end)
			ex_expand(i_size);
	}
	inline void reserve(int i_size, int front_buf){
		M_front_buff = front_buf;
		if((M_front + i_size) > (M_end + front_buffer()))
			ex_expand(i_size);
	}
	/*Capacity*/
	inline const int capacity()const{
		return static_cast<int>(M_end - M_zero);
	}
	/*Resize*/
	inline void resize(int, const St&);
	inline void resize(int);
	/*Empty*/
	inline bool empty()const{return (M_front == M_used);}
	/*Access Operators*/
	inline const St& operator[](int _ik_iX)const{return get(_ik_iX);}
	inline St& operator[](int _ik_iX){return get(_ik_iX);}
	inline St& back(){return *(M_used-1);}
	inline const St& back()const{return *(M_used-1);}
	inline St& front(){return get(0);}
	inline const St& front()const{return get(0);}
	/*data()*/
	St* data(){return M_front;}
	const St* data()const{return M_used;}
	/*Push_back*/
	inline void push_back(const St&);//Push Back
	inline void push_back(St&&);
	/*Push_front*/
	inline void push_front(const St&);//Push Front
	inline void push_front(St&&);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
	template<typename... Args>
	inline void emplace_back(Args&& ... _i_args);
#endif
	inline void emplace_back(const St& );
	/*Insert*/
	template<typename iter>
	iterator insert(iterator, iter,iter);
	//void insert(iterator, int, const St& val);
	//void insert(iterator, int, St&& val);
	iterator insert(iterator, const St& val);
	iterator insert(iterator, St&& val);
	
	iterator erase(const_iterator);
	iterator erase(const_iterator, const_iterator);
	iterator erase(j_size_t);
	iterator erase(const_iterator, j_size_t);

	/*Deletion operator*/
	inline void pop_back(){
		M_Alloc.destroy(M_used--);
	}
	inline void pop_front(){
		M_Alloc.destroy(M_front++);
	}

	/*Iterators*/
	inline iterator begin(){
		return iterator(M_front);
	}
	inline const_iterator begin()const{
		return const_iterator(M_front);
	}
	inline const_iterator cbegin()const{
		return const_iterator(M_front);
	}
	inline iterator end(){
		return iterator(M_used);
	}
	inline const_iterator end()const{
		return const_iterator(M_used);
	}
	inline const_iterator cend()const{
		return const_iterator(M_used);
	}
	/*Maintenance*/
	void set_front_buffer(int _ik_front_buff){
		M_front_buff = _ik_front_buff;
		force_ex_expand(size());
	}
	/*Swap*/
	void swap(ex_array<St, Alloc>& _ir_src){
		std::swap(_ir_src.M_zero, M_zero);
		std::swap(_ir_src.M_front, M_front);
		std::swap(_ir_src.M_used, M_used);
		std::swap(_ir_src.M_end, M_end);
		std::swap(_ir_src.M_front_buff, M_front_buff);
	}
	/*Assignment*/
	ex_array<St, Alloc>& operator=(const ex_array<St, Alloc>&);
	ex_array<St, Alloc>& operator=(ex_array<St, Alloc>&&);
	void assign(int i_size, const St& val);
	void assign(int , St&& );
	template<class iter>
	void assign(iter i_first, iter i_last);

	/*Construction*/
	ex_array(){empty_init();}

	ex_array(int);

	template<class iter>
	ex_array(iter i_first, iter i_last){
		empty_init();
		assign(i_first, i_last);
	}

	ex_array(const ex_array<St, Alloc>& irk_src){
		empty_init();
		if(irk_src.empty()){
			return;
		}
		set_capacity(2*irk_src.capacity());
		copy_impl(irk_src);
	}
	ex_array(ex_array<St, Alloc>&& irk_src){
		empty_init();
		swap(irk_src);
	}
	
	/*Destruction*/
	ex_array(int i_size, const St& val);
	~ex_array(){destroy();}
	void reset(){destroy();}
	void clear(){destroy();}
#if EX_ARRAY_DEBUG
	void print_impl(std::ostream&);
#endif
private:
	void copy_impl(const ex_array<St, Alloc>& irk_src){
		assert((M_end - M_front) >= irk_src.size());
		M_front = M_zero + irk_src.front_buffer();
		M_used = M_front + irk_src.size();
		copy_vals(irk_src.cbegin(), irk_src.cend());
	}
	
	/*Data Accessors*/
	inline St& get(int _ik_iX){return *(M_front+_ik_iX);}
	inline const St& get(int _ik_iX)const{return *(M_front+_ik_iX);}
	/*Front Buffer*/
	inline int front_buffer()const{
		return static_cast<int>(M_front - M_zero);
	}
	/*Set Capacity*/
	void set_capacity(int i_size){
#if EX_ARRAY_DEBUG
		assert(i_size > capacity());
#endif
		expand(i_size);
	}
	/*Check_Expanions*/
	bool check_back(int);
	bool check_front();
	/*Ex Expand called to call expand*/
	void ex_expand(int);
	/*Force Expand*/
	void force_ex_expand(int);
	/*Expand and Insert*/
	void expand_and_insert(iterator, this_type&);
	/*Expand Front. (sets minimum front space to allow push_front operation)*/
	void expand_front();
	/* Inserts*/
	bool insert_left(iterator, this_type&);
	bool insert_right(iterator, this_type&);
	bool insert_slide(iterator, this_type&);
	/* Slides*/

	/*Decide Move*/
	bool decide_move(const St*);


	//Data Management Functions
	St* M_zero;
	St* M_front;
	St* M_used;
	St* M_end;
	unsigned M_front_buff;
	Alloc M_Alloc;
	void expand(int);
	void force_expand(int);
	/*Slides */
	void slide_left(ptrdiff_t, ptrdiff_t);
	void slide_right(ptrdiff_t, ptrdiff_t);
	/*Destruction*/
	void destroy();
	void destruct_data();


	template<class Iter>
	void copy_vals(Iter i_first, Iter i_last);
	void move_vals(St*);
	void slide_to_right(ptrdiff_t, ptrdiff_t);
	void slide_to_left(ptrdiff_t, ptrdiff_t);
	void empty_construct();



	template<class iter>
	void move_vals(St*, iter, iter);
	void do_fill(ptrdiff_t, const St&);
	void do_fill_end(ptrdiff_t , const St&);
	void init_size(int);
	void empty_init(){
		M_zero = nullptr; 
		M_front = nullptr; 
		M_used = nullptr; 
		M_end = nullptr;
		M_front_buff = DEFAULT_FRONT_BUFFER;
	}
};


/*Constructor*/
template<typename St, class Alloc>
ex_array<St, Alloc>::ex_array(int i_size){
	empty_init();
	init_size(i_size);
	do_fill(M_used-M_front, St());
}

/*Check Expand*/
template<typename St, class Alloc>
bool ex_array<St, Alloc>::check_back(int _ik_req){
	if((M_front + _ik_req) >= 
		M_end){
		return false;
	}else
		return true;
}

/*Ex Expand*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::ex_expand(int i_size){
	expand(std::max(2*i_size+i_size/M_front_buff
		,  unsigned(1)));
}

/*Force Ex Expand*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::force_ex_expand(int i_size){
	force_expand(std::max(2*i_size+i_size/M_front_buff
		, unsigned(1)));
}

/*Check Front*/
template<typename St, class Alloc>
bool ex_array<St, Alloc>::check_front(){
	if(M_front == M_zero){
		return false;
	}else
		return true;
}

/*Expand Front*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::expand_front(){
	expand(std::max(2*size()+size()/M_front_buff
		, unsigned(M_front_buff)));
#if EX_ARRAY_DEBUG
		assert(M_front > M_zero);
#endif
}

/*Push Back*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::push_back(const St& _ik_src){
	if(check_back(size())){
		M_Alloc.construct(M_used, _ik_src);
	}else{
		ex_expand(size()+1);
		M_Alloc.construct(M_used, _ik_src);
	}
	++M_used;
}

template<typename St, class Alloc>
void ex_array<St, Alloc>::push_back(St&& _ik_src){
	if(!check_back(size())){
		ex_expand(size()+1);
	}
	
	M_Alloc.construct(M_used, std::move(_ik_src));
	++M_used;
}

/*Push Front*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::push_front(const St& _ik_val){
	if(check_front()){
		--M_front;
		M_Alloc.construct(M_front, _ik_val);
	}else{
		St temp(_ik_val);
		expand_front();
		--M_front;
		M_Alloc.construct(M_front, temp);
	}

}

/*Push Front Move*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::push_front(St&& _ik_val){
	if(check_front()){
		--M_front;
		M_Alloc.construct(M_front, std::move(_ik_val));
	}else{
		expand_front();
		--M_front;
		M_Alloc.construct(M_front, std::move(_ik_val));
	}

}

/* Assignment*/
template<typename St, class Alloc>
template<class iter>
void ex_array<St, Alloc>::assign(iter i_first, iter i_last){
	//prevents self assignment issues
	ex_array<St,Alloc> temp;
	swap(temp);
	ex_expand(static_cast<int>(2+std::distance(i_first, i_last)));

	copy_vals(i_first, i_last);
	M_used = M_front + std::distance(i_first, i_last);
	//temp destroyed on exit
}

/* Assignment*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::assign(int i_size,  const St& val){
	ex_array<St,Alloc> temp;
	swap(temp);

	ex_expand(i_size*2);
	do_fill(i_size, val);
	
}

/* Assignment*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::assign(int i_size, St&& val){
	ex_array<St,Alloc> temp;
	swap(temp);

	ex_expand(i_size*2);
	do_fill(i_size, val);
	
}

/*Operator =*/
template<typename St, class Alloc>
ex_array<St, Alloc>& ex_array<St, Alloc>::operator=(const ex_array<St, Alloc>& _irk_src){
	this_type temp(_irk_src);
	swap(temp);
	return *this;
}

/*Operator =*/
template<typename St, class Alloc>
ex_array<St, Alloc>& ex_array<St, Alloc>::operator=(ex_array<St, Alloc>&& _irk_src){
	swap(_irk_src);
	return *this;
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
/*Emplace Back*/
template<typename St, class Alloc>
template<typename... Args>
inline void ex_array<St, Alloc>::emplace_back(Args&&... _i_args){
	if(check_back(size())){
		M_Alloc.construct(M_used
			, std::forward<Args>(_i_args)...);
	}else{
		St temp(std::forward<Args>(_i_args)...);
		ex_expand(size());
		M_Alloc.construct(M_used
			, std::move(temp));
	}
	++M_used;
}
#endif



/*Resize*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::resize(int i_size){

	resize(i_size, St());

}

/*Resize with Val*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::resize(int i_size, const St& val){

	if(check_back(i_size)){
		if(size() < i_size){
			do_fill_end(i_size, val);
		}
	}else{
		St temp(val);
		ex_expand(i_size);
		if(size() < i_size){
			do_fill_end(i_size, std::move(temp));
		}
	}
		M_used = M_front + i_size;
}

/*Insert Simple*/
template<typename St, class Alloc>
typename ex_array<St, Alloc>::iterator ex_array<St, Alloc>::insert(iterator pos, const St& val){
	this_type vals(1, val);
	return insert(pos, vals.begin(), vals.end());
}

/*Insert Move*/
template<typename St, class Alloc>
typename ex_array<St, Alloc>::iterator ex_array<St, Alloc>::insert(iterator i_pos, St&& i_val){
	this_type vals;
	vals.push_back(i_val);
	return insert(i_pos, vals.begin(), vals.end());
}

/*erasing functions*/

template<typename St, class Alloc>
typename ex_array<St, Alloc>::iterator ex_array<St, Alloc>::erase(j_size_t i_pos){
	return erase(begin() + i_pos);
}

template<typename St, class Alloc>
typename ex_array<St, Alloc>::iterator ex_array<St, Alloc>::erase(const_iterator i_pos, j_size_t i_size){
	return erase(i_pos, i_pos + i_size);
}

template<typename St, class Alloc>
typename ex_array<St, Alloc>::iterator ex_array<St, Alloc>::erase(const_iterator i_pos){
	return erase(i_pos, i_pos+1);
}


template<typename St, class Alloc>
typename ex_array<St, Alloc>::iterator ex_array<St, Alloc>::erase(const_iterator i_pos, const_iterator i_end_pos){
#if _DEBUG
	assert(i_pos.base() <= M_used);
	assert(i_pos.base() >= M_front);
#endif 

#if EX_ARRAY_DEBUG
	St* init_first = M_front;
	St* init_used = M_used;
#endif
	ptrdiff_t number_on_left = i_pos - begin();
	ptrdiff_t number_on_right = end()  - i_end_pos;
	ptrdiff_t number_to_delete = i_end_pos - i_pos;
	assert(size() == (number_on_left + number_on_right + number_to_delete));
	if(number_on_left < number_on_right){
		slide_to_right(number_to_delete, number_on_left);
#if EX_ARRAY_DEBUG
		assert(init_first == (M_front - number_to_delete));
#endif
	}else{
		slide_to_left(number_to_delete, number_on_right);
#if EX_ARRAY_DEBUG
		assert(init_used == (M_used + number_to_delete));
#endif
	}

	return begin() + number_on_left;
#if EX_ARRAY_DEBUG

#endif
}


/*Insertion Functions*/
template<typename St, class Alloc>
template<class iter>
typename ex_array<St, Alloc>::iterator ex_array<St, Alloc>::insert(iterator pos, iter i_first, iter i_last){
	this_type vals(i_first, i_last);
	ptrdiff_t index = pos - begin();
#if EX_ARRAY_DEBUG
	size_t init_size = size();
	ptrdiff_t dist = std::distance(i_first, i_last);
	assert(pos.base() <= M_used);
	assert(pos.base() >= M_front);
#endif
	if(decide_move(pos.base())){
		if(insert_left(pos, vals)){
			return begin() + index;
		}else if (insert_right(pos, vals)){
			return begin() + index;
		}
	}else{
		if(insert_right(pos, vals)){
			return begin() + index;
		}else if(insert_left(pos, vals)){
			return begin() + index;
		}
	}
	if(!insert_slide(pos, vals)){
		expand_and_insert(pos, vals);
		return begin() + index;
	}

#if EX_ARRAY_DEBUG
	assert(dist+init_size);
#endif
	return begin() + index;
}

/*Expand and Insert*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::expand_and_insert(iterator pos, this_type& vals){
	int dist = static_cast<int>(pos.M_pos - M_front);
	ex_expand(size() + vals.size());
	insert(iterator(M_front+dist), vals.begin(), vals.end());
}

/*Insert Slide*/
template<typename St, class Alloc>
bool ex_array<St, Alloc>::insert_slide(iterator pos, this_type& _r_vals){
	if((_r_vals.size()+size()) > capacity())
		return false;
#if EX_ARRAY_DEBUG
	assert(front_buffer() < _r_vals.size());
	assert((capacity()-size() - front_buffer()) <= _r_vals.size());
	assert((size() + _r_vals.size()) <= capacity());
#endif
	ptrdiff_t index = pos-begin();
	slide_left(front_buffer(), pos - begin());
	slide_right(_r_vals.size() - front_buffer(), end()-pos);
	move_vals(M_front + index, _r_vals.begin(), _r_vals.end());
	return true;
}

/*Insert left*/
template<typename St, class Alloc>
bool ex_array<St, Alloc>::insert_left(iterator pos
	, this_type& _r_vals){
	int val_size = _r_vals.size();
	
	if((M_front - val_size) > M_front){
		return false;
	}	
	if((M_front - val_size) < M_zero ){
		return false;
	}
#if EX_ARRAY_DEBUG
		assert(front_buffer() >= _r_vals.size());
#endif

	slide_left(val_size, pos - begin());


	move_vals(pos.M_pos - _r_vals.size(), _r_vals.begin(), _r_vals.end());

	return true;
}
/*Insert Right*/
template<typename St, class Alloc>
bool ex_array<St, Alloc>::insert_right(iterator pos
	, this_type& _r_vals){
	int val_size = _r_vals.size();
	if((M_used + val_size) >= M_end )
		return false;

#if EX_ARRAY_DEBUG
		assert((capacity()-front_buffer()-size()) >= _r_vals.size());
#endif
	slide_right(val_size, end()-pos);

	move_vals(pos.M_pos, _r_vals.begin(), _r_vals.end());

	return true;
}

/* Decide Move*/
template<typename St, class Alloc>
bool ex_array<St, Alloc>::decide_move(const St* _ik_pos){
#if EX_ARRAY_DEBUG
	assert(_ik_pos <= M_used);
	assert(_ik_pos >= M_front);
#endif
	// Returns true if the pointer is closer to the front than back
	return (_ik_pos - M_front) < (M_used - _ik_pos);
}

#if EX_ARRAY_DEBUG
/*Print impl*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::print_impl(std::ostream& oStr){
	for(unsigned i=0; i < capacity(); i++){
		oStr << M_zero[i] << ',';
	}
	oStr << std::endl;
}
#endif



/*Constructor*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::init_size(int i_size){
	assert(i_size >= 0);
	M_front_buff = DEFAULT_FRONT_BUFFER;
	M_zero = M_Alloc.allocate(i_size*2);
	M_front = (M_zero + i_size/M_front_buff);
	M_used = M_front + i_size;
	M_end = M_zero + i_size*2;
 }

/*Constructor*/
template<typename St, class Alloc>
ex_array<St,Alloc>::ex_array(int i_size, const St& irk_val){
	empty_init();
	init_size(i_size);
	do_fill(M_used-M_front, irk_val);
 }

/*Empty Construct*/
template<typename St, class Alloc>
void ex_array<St,Alloc>::empty_construct(){
	do_fill(M_used-M_front, St());
}
 /*do fill*/
 template<typename St, class Alloc>
 void ex_array<St, Alloc>::do_fill(ptrdiff_t i_size, const St& val){
	std::uninitialized_fill(M_front, M_front+i_size, val);
 }
 
 /* Do Fill End*/
 template<typename St, class Alloc>
 void ex_array<St, Alloc>::do_fill_end(ptrdiff_t i_size, const St& val){
	std::uninitialized_fill(M_used, M_front+i_size, val);

 }
 
 /*Destroy*/
 template<typename St, class Alloc>
 void ex_array<St, Alloc>::destroy(){
	destruct_data();
	M_Alloc.deallocate(M_zero, M_end-M_zero);
	M_zero = nullptr;
	M_front = nullptr;
	M_used = nullptr; 
	M_end = nullptr;
 }
 /*Destroy St*/
 template<typename St, class Alloc>
 void ex_array<St, Alloc>::destruct_data(){
	St* pos = M_front;
	while(pos != M_used){
		M_Alloc.destroy(pos++);
	}
 }

 /*Expand*/
 template<typename St, class Alloc>
void ex_array<St, Alloc>::expand(int i_size){
#if EX_ARRAY_DEBUG
	 assert(((M_front + i_size) >= M_end)
		 || (M_front == M_zero));
	 int old_size = size();
	 int old_capacity = capacity();
#endif 
	 std::unique_ptr<St> new_zero_ptr(M_Alloc.allocate(i_size));
	 St* new_zero = new_zero_ptr.get();
	 assert((new_zero + i_size/M_front_buff + size()) < (new_zero + i_size));
	 move_vals(new_zero + i_size/M_front_buff);
	 
	 destruct_data();
	 M_Alloc.deallocate(M_zero, M_end-M_zero);
	 const long long temp_size = M_used - M_front;
	 M_front = new_zero + i_size/M_front_buff;
	 M_used = M_front + temp_size;

	 M_zero = new_zero;
	 M_end = new_zero + i_size;
	 new_zero_ptr.release();
#if EX_ARRAY_DEBUG
	// std::cerr << "\nReallocation! Old size: " << old_size << " old capacity: " << old_capacity << " new capacity: " << capacity();
	 assert(size() == old_size);
	 assert(capacity() > old_capacity);
#endif 
}

 /*Expand*/
 template<typename St, class Alloc>
void ex_array<St, Alloc>::force_expand(int i_size){
#if EX_ARRAY_DEBUG

#endif 
	 St* new_zero = M_Alloc.allocate(i_size);
	 move_vals(new_zero + i_size/M_front_buff);
	 M_Alloc.deallocate(M_zero, M_end-M_zero);
	 const ptrdiff_t temp_size = M_used - M_front;
	 M_front = new_zero + i_size/M_front_buff;
	 M_used = M_front + temp_size;

	 M_zero = new_zero;
	 M_end = new_zero + i_size;
 }

/*Move Vals*/
 template<typename St, class Alloc>
 void ex_array<St, Alloc>::move_vals(St* _i_new_front){
	if(!M_zero)
		return;
	std::move_iterator<St*> pos = std::make_move_iterator<St*>(M_front);
	std::move_iterator<St*> end = std::make_move_iterator<St*>(M_used);
	while(pos != end){
		M_Alloc.construct(_i_new_front++, *pos++);
	}

 }

 /*Move Vals from Pos*/
 template<typename St, class Alloc>
 template<class Iter>
 void ex_array<St, Alloc>::move_vals(St* pos, Iter i_first, Iter i_last){
	if(!M_zero)
		return;
	std::move_iterator<Iter> start = std::make_move_iterator<Iter>(i_first);
	std::move_iterator<Iter> end = std::make_move_iterator<Iter>(i_last);
	while(start != end){
		M_Alloc.construct(pos++, *start++);
	}

 }



 /*Slide Left*/
 template<typename St, class Alloc>
 void ex_array<St, Alloc>::slide_left(ptrdiff_t i_slide_size,
	 ptrdiff_t i_slide_amount){
	
	if(!i_slide_size){
		return;
	}
	St* insert_pos = M_front;
	M_front -= i_slide_size;	

	for(int i=0; i < i_slide_amount; i++){
		M_Alloc.construct(M_front+i, std::move(*insert_pos));
		M_Alloc.destroy(insert_pos++);
	}
	
 }

  /*Slide Right*/
 template<typename St, class Alloc>
 void ex_array<St, Alloc>::slide_right(ptrdiff_t i_slide_size,
	 ptrdiff_t i_slide_amount){
	
	if(!i_slide_size){
		return;
	}

	St* insert_pos = M_used-1;
	M_used += (i_slide_size);

	for(int i=0; i < i_slide_amount; i++){
		M_Alloc.construct(M_used-i-1, std::move(*insert_pos));
		M_Alloc.destroy(insert_pos--);
		
	}
 }

 /*void slide_to_right(int size, int amount)*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::slide_to_right(ptrdiff_t i_slide_size, ptrdiff_t i_slide_amount){
	if(!i_slide_size){
		return;
	}
	St* cur_dest = M_front + i_slide_amount + i_slide_size - 1;

	for(int i=0; i < i_slide_size; i++){
		M_Alloc.destroy(cur_dest - i);
	}

	for(int i=0; i < i_slide_amount; i++){
		M_Alloc.construct(cur_dest, std::move(*(cur_dest-i_slide_size)));
		M_Alloc.destroy(cur_dest - i_slide_size);
		--cur_dest;
	}
	M_front += i_slide_size;
}

 /*void slight_to_left(int size, int amount)*/
template<typename St, class Alloc>
void ex_array<St, Alloc>::slide_to_left(ptrdiff_t i_slide_size, ptrdiff_t i_slide_amount){
	if(!i_slide_size){
		return;
	}
	St* cur_dest = M_used - i_slide_amount - i_slide_size;

	for(int i=0; i < i_slide_size; i++){
		M_Alloc.destroy(cur_dest + i);
	}

	for(int i=0; i < i_slide_amount; i++){
		M_Alloc.construct(cur_dest, std::move(*(cur_dest+i_slide_size)));
		M_Alloc.destroy(cur_dest + i_slide_size);
		++cur_dest;
	}
	M_used -= i_slide_size;
}


 /*Copy Vals*/
 template<typename St, class Alloc>
 template<class Iter>
 void ex_array<St, Alloc>::copy_vals(Iter i_first, Iter i_last){
	St* pos = M_front;
	while(i_first != i_last){
		M_Alloc.construct(pos++, *i_first++);
	}
}


/*bool operator<(const ex_array<St, Alloc>&, const ex_array<St, Alloc>&)*/
template<typename St, class Alloc>
bool operator<(const ex_array<St, Alloc>& irk_left, const ex_array<St, Alloc>& irk_right){
	return std::lexicographical_compare(irk_left.begin(), irk_left.end(), irk_right.begin(), irk_right.end());
}


}






#endif