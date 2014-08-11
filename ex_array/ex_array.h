#ifndef EX_ARRAY_H
#define EX_ARRAY_H

//
#include <J_Fwd_Decl.h>
//
#include <j_iterator.h>
//
#include <j_type.h>
//
#include <J_Utile.h>

//Algorithm	
#include<algorithm>

//Utility 
#include <cassert>		
//
#include <utility>



namespace jomike{

	///class ex_array
	/*																			*/
	/*		ex_array is an expandable array class. Similar to a vector. It has	*/
	/*	padding on both ends however for allowing speed insertions and removals */
	/*	near the ends of the ex_array. (Worst case is middle insertions)		*/
	/*																			*/
	/*	NOTE: size_type is an int and size() reserve() and many other methods	*/
	/*	take an long long you're welcome for not having to declare size_t		*/
	/*	in your for loops.														*/

	template<typename St, class Alloc = std::allocator<St>>
	class ex_array;

	//requires no space in class_name
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
		EX_ARRAY_TYPEDEF_LOWER(j_ubyte)

		const Dbl_t DEFAULT_FRONT_BUFFER_RATIO = 0.2;
	const Dbl_t OVERALLOCATION_MULTIPLIER = 2.01;

	/*class ex_array*/
	template<typename St, class Alloc>
	class ex_array{
	public:

		typedef ex_array<St, Alloc> M_type;
		//STL typedefs needed by container-------------------------------------------------------------------------------------
		typedef St					value_type;

		typedef St& 				reference;
		typedef const St&			const_reference;
		typedef	St*  				pointer;
		typedef	const St*			const_pointer;

		typedef std::ptrdiff_t			difference_type;

		typedef	j_size_t			size_type;	//NOTE: size_type is signed (break from STL conformality)

		///Iterator typedefs
		/*																			*/
		/*		Iterator functionality is provided via auxiliary ptr_iterator<>		*/
		/*	and reverse_ptr_iterator classes class.									*/

		typedef ptr_iterator<pointer, ex_array> iterator;
		typedef ptr_iterator<const_pointer, ex_array> const_iterator;

		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		//Allocator typedefs
		typedef Alloc				allocator_type;

		///allocator_type get_allocator()const
		/*																			*/
		/*		Returns an instance of the allocator type used by the container		*/
		allocator_type get_allocator()const;

		//Construction, Copy, and Derstruction Operations----------------------------------------------------------------------

		///ex_array(const allocator_type&)
		/*																			*/
		/*		Default constructor for ex_array class. Allocates Nothing			*/
		/*	and sets the space reserved for the front to DEFAULT_FRONT_BUFFER_RATIO	*/
		/*	Sets the allocator to i_allocator										*/

		explicit ex_array(const allocator_type& i_allocator = Alloc());


		///ex_array(size_type, const allocator_type& i_allocator)
		/*																			*/
		/*		Constructs an ex_array of size i_size copied from the				*/
		/*	default constructed values of St										*/
		/*	Sets the allocator to i_allocator										*/

		ex_array(size_type i_size, const allocator_type& i_allocator = Alloc());

		///ex_array(size_type, const St&, const allocator_type& i_allocator)
		/*																			*/
		/*		Constructs an ex_array of size i_size copied from the				*/
		/*	values i_val.															*/
		/*	Sets the allocator to i_allocator										*/

		ex_array(size_type i_size, const St& i_val
				 , const allocator_type& i_allocator = Alloc());

		///ex_array(std::initializer_list<St> , const allocator_type& = Alloc())
		/*																			*/
		/*		Initializer list constructor										*/
		/*																			*/
		ex_array(std::initializer_list<St> i_list
				 , const allocator_type& i_allocator = Alloc());

		///ex_array(Iter,Iter, const allocator_type& i_allocator)
		/*																			*/
		/*	Constructs an ex_array from the range [i_first, i_last)					*/
		/*	Sets the allocator to i_allocator										*/

		template<class Iter>
		ex_array(Iter i_first, Iter i_last, const allocator_type& i_allocator = Alloc());

		///ex_array(const ex_array<St, Alloc>&, const allocator_type& i_allocator)
		/*																			*/
		/*		Copy Constructor for ex_array										*/
		/*	Sets the allocator to i_allocator										*/

		ex_array(const ex_array<St, Alloc>& irk_src, const allocator_type& i_allocator = Alloc());

		///ex_array(ex_array<St, Alloc>&&, const allocator_type& i_allocator)
		/*																			*/
		/*		Move constructor for ex_array										*/
		/*	Sets the allocator to i_allocator										*/

		ex_array(ex_array<St, Alloc>&& irk_src, const allocator_type& i_allocator = Alloc());

		///~ex_array()
		/*																			*/
		/*		Deallocates all memory explicitly allocated by ex_array				*/
		/*	and also calls destructors on all given items							*/

		~ex_array();

		//Assignment-----------------------------------------------------------------------------------------

		///ex_array& operator=(const ex_array&)
		/*																			*/
		/*	Copy Assignment:														*/
		/*		Assigns contents of right hand side of '=' operator					*/
		/*	to the left hand side. containers will have equal size and lhs will		*/
		/*	have copies of rhs values. Original values of lhs are lost.				*/

		ex_array<St, Alloc>& operator=(const ex_array<St, Alloc>&);

		///ex_array& operator=(const ex_array&&)
		/*																			*/
		/*	Move Copy Assignment:													*/
		/*		"Moves" contents of right hand side of '=' operator					*/
		/*	to the left hand side. RHS will be left in an undefined but valid state.*/
		/*	Original values of lhs are lost.										*/

		ex_array<St, Alloc>& operator=(ex_array<St, Alloc>&&);

		///void swap(ex_array&)
		/*																			*/
		/*	Swaps the contents and settings of the ex_arrays						*/

		void swap(ex_array<St, Alloc>& _ir_src);

		///void assign(size_type, const St&)
		/*		Drops the current ex_arrays contents and recreates the vector with	*/
		/*	i_size copies of val													*/

		void assign(size_type i_size, const St& val);

		///void assign(Iter, Iter)
		/*		Drops the current ex_array's contents and constructs an ex_array	*/
		/*	from the range [i_first, i_last)										*/

		template<class iter>
		void assign(iter i_first, iter i_last);

		//Container Non-Modifying Operations-------------------------------------------------------------------------------

		///size_type size()const
		/*																			*/
		/*		Returns the number of items inserted into the array.				*/
		/*	Due to double ended padding allocated size likely exceeds				*/
		/*	the value returned by size()											*/

		size_type size()const;

		///size_type max_size()const
		/*																			*/
		/*		Returns the maximum amount of elements the container can			*/
		/*	theoretically contain													*/

		size_type max_size()const;

		///bool empty()const
		/*																			*/
		/*		Returns whether or not the container is storing any elements		*/

		bool empty()const;

		//Direct Access Functions-----------------------------------------------------------------------------------------------


		///St& at(size_type i_pos)
		/*																			*/
		/*      Returns a reference to the element at index i_pos with bounds		*/
		/*	throws std::out_of_range if it it is out of range.						*/
		St& at(size_type i_pos);


		///const St& at(size_type i_pos)
		/*																			*/
		/*      Returns a reference to the element at index i_pos with bounds		*/
		/*	throws std::out_of_range if it it is out of range.						*/
		const St& at(size_type i_pos)const;

		///St* data()
		/*																			*/
		/*      Returns cstyle array pointer to first element in the ex_array		*/

		St* data();

		///const St* data()const
		/*																			*/
		/*      Returns cstyle array pointer to first element in the ex_array		*/

		const St* data()const;

		///St& operator[](size_type i_index)												
		/*		Directly accesses element at index i_index. i_index must be greater	*/
		/*	zero and less than size()												*/

		St& operator[](size_type i_index);

		///const St& operator[](size_type i_index)const							
		/*		Directly accesses element at index i_index. i_index must be greater	*/
		/*	zero and less than size()												*/

		const St& operator[](size_type i_index)const;

		///St& back()
		/*																			*/
		/*		Access the last element of the ex_array. undefined if ex_array is	*/
		/*	empty.																	*/
		St& back();

		///St& back()
		/*																			*/
		/*		Access the last element of the ex_array. undefined if ex_array is	*/
		/*	empty.																	*/
		const St& back()const;

		///St& front()
		/*																			*/
		/*		Access the first element of the ex_array. undefined if ex_array is	*/
		/*	empty																	*/
		St& front();

		///const St& front()const
		/*																			*/
		/*		Access the first element of the ex_array. undefined if ex_array is	*/
		/*	empty																	*/
		const St& front()const;

		//Non Modifying Policy Operations--------------------------------------------------------------------------

		///const size_type capacity()const
		/*																			*/
		/*		Returns the amount of items that can be in the container before a	*/
		/*	reallocation is required. This is the total amount of allocated space	*/
		size_type capacity()const;

		///size_type front_buffer()const
		/*																			*/
		/*      Returns the unsused but allocated space resting in the front of the */
		/*	ex_array.																*/

		size_type front_buffer()const;

		///size_type back_buffer()const
		/*																			*/
		/*      Returns the unused but allocated space resting in the back of the	*/
		/*	ex_array																*/

		size_type back_buffer()const;

		///size_type buffer()const
		/*																			*/
		/*      Returns the unused but allocated space resting in the ex_array		*/

		size_type buffer()const;

		//Modifying Policy Operations------------------------------------------------------------------------------

		///void reserve(size_type i_size)
		/*																			*/
		/*		Reserves internal memory for at least i_size elements. This may		*/
		/*	cause a reallocation. Will not shrink size of allocation				*/
		void reserve(size_type i_size);

		///void reserve(size_type i_size, Dbl_t front_buf)
		/*																			*/
		/*		Reserves internal memory for at least i_size elements. This will	*/
		/*	cause a reallocation. Will not shrink size of allocation. Also sets		*/
		/* the front_buff attribute													*/
		void reserve(size_type i_size, Dbl_t i_front_buffer_ratio);

		///void shrink_to_fit()
		/*																			*/
		/*		Shrinks the allocation to fit the current size of the elements in	*/
		/*	container. This is a non-binding request								*/

		void shrink_to_fit();


		///void set_front_buffer(Dbl_t i_front_buffer_ratio)
		/*																			*/
		/*      Sets the front buffer ratio. i_front_buffer_ratio must be in the	*/
		/*	range [0.0,1.0]															*/

		void set_front_buffer(Dbl_t i_front_buffer_ratio);





		//Inserting and Removing elements-----------------------------------------------------------


		///iterator insert(const_iterator, const St& val)
		/*																			*/
		/*	Inserts the value val before the position pos. May cause reallocation	*/

		iterator insert(const_iterator pos, const St& val);

		///iterator insert(const_iterator, St&& val)
		/*																			*/
		/*		Inserts the value val before the position pos. May cause			*/
		/*	reallocation. it moves the val into the container						*/

		iterator insert(const_iterator poc, St&& val);


		///iterator insert(const_iterator, iter,iter)
		/*																			*/
		/*		Inserts the range [i_first, i_last) into the container before		*/
		/*	the position pos. Possibly causing a reallocation.						*/

		template<typename Iter>
		iterator insert(const_iterator pos, Iter i_first, Iter i_last);


		///iterator erase(size_type, size_type i_len =numeric_limits<size_type>::max())
		/*																			*/
		/*		Erase elements in the range [i_pos, i_pos + i_len). Up to the end of*/
		/*	the range. Default is to erase from pos until the end of the container	*/
		iterator erase(size_type, size_type i_len = std::numeric_limits<size_type>::max());

		///iterator erase(const_iterator, size_type i_len= 1)
		/*																			*/
		/*		Erase elements in the range [i_pos, i_pos + i_len). Up to the end of*/
		/*	the range. NOTE:: Default is to erase only the element at i_pos			*/

		iterator erase(const_iterator i_pos, size_type i_len = 1);


		///iterator erase(const_iterator, const_iterator)
		/*																			*/
		/*		Erase elements in the range [i_first, i_end).						*/
		/*	Requires [i_first, i_end) to be a subrange of the container				*/

		iterator erase(const_iterator, const_iterator);

		//Push and Pop, front and back---------------------------------------------------------------------------------------

		///void push_back(const St&)
		/*																			*/
		/*      Pushes the element val into the back of the ex_rray. Possibly		*/
		/*	causing a reallocation													*/

		void push_back(const St& val);


		///void push_back(St&&)
		/*																			*/
		/*      Pushes the element val into the back of the ex_array using move		*/
		/*	move semantics. may cause a reallocation								*/
		void push_back(St&& val);


		///void emplace_back(Args&& ...)
		/*																			*/
		/*      Pushes the element constructed from i_args into the back of the		*/
		/*	ex_rray. Possibly causing a reallocation								*/

		template<typename... Args>
		void emplace_back(Args&& ... _i_args);



		///void emplace_back(const St&)
		/*																			*/
		/*      Pushes the element val into the back of the ex_rray. Possibly		*/
		/*	causing a reallocation. Constructs the element in place. 				*/

		void emplace_back(const St& val);


		///void push_front(const St&)
		/*																			*/
		/*      Pushes the element val into the front of the ex_rray. Possibly		*/
		/*	causing a reallocation													*/

		void push_front(const St& val);


		///void push_front(St&&)
		/*																			*/
		/*      Pushes the element val into the front of the ex_array using move	*/
		/*	move semantics. may cause a reallocation								*/
		void push_front(St&& val);

		///void pop_back()
		/*																			*/
		/*      Removes the last element in the ex_array. Undefined if the array is	*/
		/*	empty.																	*/
		void pop_back();


		///void pop_front()
		/*																			*/
		/*      Removes the first element in the ex_array. Undefined if the array is*/
		/*	empty.																	*/
		void pop_front();

		///template<typename Pred_t>
		//void remove_if(const Pred_t& i_pred)
		/*																			*/
		/*		Removes Items under the given predicate								*/
		/*																			*/
		template<typename Pred_t>
		void remove_if(const Pred_t& i_pred);
		//Resizing---------------------------------------------------------------------------------------

		/// <summary> void resize(size_type)
		/*																			*/
		/*      Resizes the container to i_size. if i_size>size() new elements are	*/
		/*	created copy constructed from i_val. And placed in the back of the		*/
		/*	ex_array. i_val is by default the default default constructed version of*/
		/*	St. {St()} Deletes elements fromt the back to shrink container to		*/
		/*	i_size if size() > i_size. must be i_size >=0							*/
		///</summary>
		void resize(size_type i_size);

		///void resize(size_type, const St& i_val)
		/*																			*/
		/*      Resizes the container to i_size. if i_size>size() new elements are	*/
		/*	created copy constructed from i_val. And placed in the back of the		*/
		/*	ex_array. i_val is by default the default default constructed version of*/
		/*	St. {St()} Deletes elements fromt the back to shrink container to		*/
		/*	i_size if size() > i_size. must be i_size >=0							*/

		void resize(size_type i_size, const St& i_val);

		///void resize_front(size_type, const St& i_val = St())
		/*																			*/
		/*      Resizes the container to i_size. if i_size>size() new elements are	*/
		/*	created copy constructed from i_val. And placed in the front of the		*/
		/*	ex_array. i_val is by default the default default constructed version of*/
		/*	St. {St()} Deletes elements fromt the front to shrink container to		*/
		/*	i_size if size() > i_size. must be i_size >=0							*/

		void resize_front(size_type i_size, const St& i_val = St());


		///void clear()
		/*																			*/
		/*      Clears all elements in the ex_array and deallocates all memory		*/

		void clear();

		//Iterator Creation---------------------------------------------------------------------------------------


		///iterator begin()
		/*																			*/
		/*		Returns an iterator to the first element of the container			*/

		iterator begin();


		///const_iterator begin()const{
		/*																			*/
		/*      Returns a const_iterator to const items from the begining of the	*/
		/*	ex_array																*/

		const_iterator begin()const;


		///const_iterator cbegin()const
		/*																			*/
		/*      Returns a const_iterator to const elements from the beginning of the*/
		/*	ex_array.																*/

		const_iterator cbegin()const;

		///iterator end()
		/*																			*/
		/*      Returns an iterator to elements to one past the end element			*/
		/*	of the ex_array. Dereferencing this iterator at this pos is undefined	*/

		iterator end();

		///const_iterator end()const
		/*																			*/
		/*      Returns an iterator to const elements to one past the end element	*/
		/*	of the ex_array. dereferencing this iterator at this pos is undefined	*/

		const_iterator end()const;


		///const_iterator cend()const
		/*																			*/
		/*      Returns an iterator to const elements to one past the end element	*/
		/*	of the ex_array. dereferencing this iterator at this pos is undefined	*/

		const_iterator cend()const;


		///reverse_iterator rbegin()
		/*																			*/
		/*		Returns a reverse_iterator to the first element of the container	*/

		reverse_iterator rbegin();


		///const_reverse_iterator rbegin()const{
		/*																			*/
		/*      Returns a reverse_iterator to const items from the begining of the	*/
		/*ex_array																	*/

		const_reverse_iterator rbegin()const;


		///const_reverse_iterator crbegin()const
		/*																			*/
		/*      Returns a reverse_iterator to const elements from the beginning		*/
		/*	of the ex_array.														*/

		const_reverse_iterator crbegin()const;

		///reverse_iterator rend()
		/*																			*/
		/*      Return a reverse_iterator to one past the end element of the		*/
		/*	ex_array. dereferencing this reverse_iterator is undefined.				*/

		reverse_iterator rend();

		///const_reverse_iterator rend()const
		/*																			*/
		/*      Returns a const_reverse_iterator to const elements to one past the	*/
		/*	end element	of the ex_array. dereferencing this reverse_iterator at this*/
		/*	pos is undefined														*/

		const_reverse_iterator rend()const;


		///const_reverse_iterator crend()const
		/*																			*/
		/*      Returns a const_reverse_iterator to const elements to one past the	*/
		/*	end	element	of the ex_array. dereferencing this reverse_iterator at this*/
		/*	pos is undefined														*/

		const_reverse_iterator crend()const;

		//Operating On Elements---------------------------------------------------------------------------------------

		///Func_Obj_t apply(const Func_Obj_t& )
		/*																			*/
		/*      Applies the given function object to all elements in the container	*/
		/*																			*/
		template<typename Func_Obj_t>
		Func_Obj_t apply(const Func_Obj_t&);





	private: //Private      Private      Private      Private      Private      Private      Private      

		//Data Members---------------------------------------------------------------------------------------

		St* M_zero;					//Pointer to start of allocated space
		St* M_front;				//Pointer to start of occupied space
		St* M_used;					//Pointer to one past the end of occupied space
		St* M_end;					//Pointer to one past the end of allocated space

		Dbl_t M_front_buffer_ratio;	//The percentage of extra allocated space that is placed in the front of the exarray
		Dbl_t M_slide_overshoot_val;
		Alloc M_Alloc;				//Allocater for allocating memory and constructing objects


		//Construction Helpers---------------------------------------------------------------------------------------

		/*void make_pointers_null()*/
		void default_initialization();

		/*void init_allocation_with_no_buffer_space(size_type)*/
		void init_allocation_with_no_buffer_space(size_type);


		/*void copy_vals(Iter i_first, Iter i_last)*/
		template<typename Iter>
		void copy_vals(Iter i_first, Iter i_last);

		//Destruction Helpers---------------------------------------------------------------------------------------

		/*void destroy()*/
		void destroy();

		/*void destruct_data()*/
		void destruct_data(St*, St*);

		/*void clear_data()*/
		void clear_data();
		//Reallocation Helpers---------------------------------------------------------------------------------------

		/*void ex_expand(size_type)*/
		void ex_expand(size_type);

		/*void expand(size_type)*/
		void expand(size_type);

		/*void move_vals(Iter, Iter, St*)*/
		template<typename Iter>
		void move_construct_vals(Iter i_first, Iter i_last, St*);

		/*void construct_range(St*, St*, const St&)*/
		void construct_range(St* i_first, St* i_last, const St& irk_val);

		/*void construct_range(Iter, Iter, St*)*/
		template<typename Iter>
		void construct_range(Iter i_first, Iter i_last, St* i_dest);


		//Insertion Helpers---------------------------------------------------------------------------------------

		/*iterator insert_move(const_iterator pos, Iter i_first,Iter i_last)*/
		template<typename Iter>
		iterator insert_move(const_iterator pos, Iter i_first, Iter i_last);

		/*bool insert_left(const_iterator, Iter i_first, Iter i_last)*/
		template<typename Iter>
		bool insert_left(const_iterator, Iter i_first, Iter i_last);

		/*bool insert_right(const_iterator, Iter i_first, Iter i_last)*/
		template<typename Iter>
		bool insert_right(const_iterator, Iter i_first, Iter i_last);

		/*bool insert_slide(iterator, Iter i_first, Iter i_last)*/
		template<typename Iter>
		bool insert_slide(const_iterator, Iter i_first, Iter i_last);

		/*void expand_and_insert(const_iterator, Iter i_first, Iter i_last)*/
		template<typename Iter>
		void expand_and_insert(const_iterator, Iter i_first, Iter i_last);

		/*void slide_left(std::ptrdiff_t, std::ptrdiff_t)*/
		void slide_left(std::ptrdiff_t, std::ptrdiff_t);

		/*void slide_right(std::ptrdiff_t, std::ptrdiff_t)*/
		void slide_right(std::ptrdiff_t, std::ptrdiff_t);

		//Erasing Helpers---------------------------------------------------------------------------------------

		/*void slide_to_left(std::ptrdiff_t i_slide_size, std::ptrdiff_t i_slide_amount)*/
		void slide_to_left(std::ptrdiff_t i_slide_size, std::ptrdiff_t i_slide_amount);

		/*void slide_to_right(std::ptrdiff_t i_slide_size, std::ptrdiff_t i_slide_amount)*/
		void slide_to_right(std::ptrdiff_t i_slide_size, std::ptrdiff_t i_slide_amount);

		//Resizing, push_* , pop* helpers---------------------------------------------------------------------------------------

		/*void make_space_in_back(std::ptrdiff_t)*/
		void make_space_in_back(std::ptrdiff_t);

		/*void make_space_in_front(std::ptrdiff_t)*/
		void make_space_in_front(std::ptrdiff_t);

		/*void slide_contents(std::ptrdiff_t)*/
		void slide_contents(std::ptrdiff_t i_amount);

	};

	//Function Definitions***********************************************************************************************

	//______________________________________________________________________________________________________________


	//Allocator---------------------------------------------------------------------------------------

	/*allocator_type get_allocator()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::allocator_type ex_array<St, Alloc>::get_allocator()const{ return M_Alloc; }


	//Construction, Copy, and Destruction Operations--------------------------------------------------------

	/*ex_array(const allocator_type&)*/

	template<typename St, class Alloc>
	ex_array<St, Alloc>::ex_array(const allocator_type& i_alloc)
		:M_front_buffer_ratio(DEFAULT_FRONT_BUFFER_RATIO), M_Alloc(i_alloc){
		default_initialization();
	}


	///void make_pointers_null()
	/*																		*/
	/*		Sets all internal pointers to nullpre							*/

	template<typename St, class Alloc>
	void ex_array<St, Alloc>::default_initialization(){
		M_zero = M_front = M_used = M_end = nullptr;
		M_slide_overshoot_val = 0.2f;
	}

	/*ex_array(size_type, const St&, const allocator_type&)*/
	template<typename St, class Alloc>
	ex_array<St, Alloc>::ex_array(size_type i_size, const St& irk_val, const allocator_type& i_alloc)
		:M_front_buffer_ratio(DEFAULT_FRONT_BUFFER_RATIO), M_Alloc(i_alloc){
		init_allocation_with_no_buffer_space(i_size);

		construct_range(M_front, M_used, irk_val);
	}

	/*ex_array(size_type, const allocator_type&)*/
	template<typename St, class Alloc>
	ex_array<St, Alloc>::ex_array(size_type i_size, const allocator_type& i_alloc)
		:M_front_buffer_ratio(DEFAULT_FRONT_BUFFER_RATIO), M_Alloc(i_alloc){
		init_allocation_with_no_buffer_space(i_size);

		std::for_each(M_front, M_used, [&](St& y_datum){M_Alloc.construct(&y_datum); });
	}


	///void construct_range(St*, St*, const St&)
	/*																		*/
	/*		Constructs the values in range [i_first, i_last) with irk_val	*/
	/*	Requires memory already be allocated uninitialized in range			*/
	/*	[i_dest, i_dest + (i_last-i_first))									*/
	template<typename St, class Alloc>
	void ex_array<St, Alloc>::construct_range(St* i_first, St* i_last, const St& irk_val){
		std::for_each(i_first, i_last, [&](St& y_datum){M_Alloc.construct(&y_datum, irk_val); });
	}

	///void init_allocation_with_no_buffer_space(size_type i_size)
	/*																			*/
	/*      Initializes the ex_array with an allocation of size i_size and sets	*/
	/*	the M_front and M_used pointer to cover the entire range. so size() ==	*/
	/*	capacity																*/
	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::init_allocation_with_no_buffer_space(size_type i_size){
		default_initialization();
		expand(i_size);
		M_front = M_zero;
		M_used = M_end;
	}

	///void expand(size_type)
	/*																		*/
	/*		This performs a reallocation and expands the allocated space	*/
	/*	to i_size. It moves all values over from the original allocation	*/
	template<typename St, class Alloc>
	void ex_array<St, Alloc>::expand(size_type i_size){
#if EX_ARRAY_DEBUG
		assert(i_size >= capacity());
		size_type old_size = size();
		size_type old_capacity = capacity();
#endif	//EX_ARRAY_DEBUG

		//Performs automatic dallocation if something goes wrong.
		//Only place where memory is allocated
		std::unique_ptr<St> new_zero_ptr(M_Alloc.allocate(safe_ptrdiff_cast(i_size)));

		St* new_zero = new_zero_ptr.get();
		St* new_front = std::min(new_zero + static_cast<std::ptrdiff_t>(i_size*M_front_buffer_ratio)
								 , new_zero + i_size - size());

		assert((new_front + size()) <= (new_zero + i_size));

		//Move values over
		move_construct_vals(M_front, M_used, new_front);

		//get the size of the array before destruction
		size_type prev_size = size();

		//Destroy current data and deallocate.
		destroy();

		M_zero = new_zero;
		M_front = new_front;
		M_used = M_front + prev_size;
		M_end = new_zero + i_size;

		//Prevent unique_ptr deallocation
		new_zero_ptr.release();

#if EX_ARRAY_DEBUG
#ifdef EX_ARRAY_NOTIFY_REALLOCATION
		std::cerr << "\nReallocation! Old size: " << old_size << " old capacity: " << old_capacity << " new capacity: " << capacity();
#endif //EX_ARRAY_NOTIFY_REALLOCATION
		assert(size() == old_size);
		assert(!capacity() || (capacity() > old_capacity));
#endif  //EX_ARRAY_DEBUG
	}

	///void move_construct_vals(St*, Iter, Iter)
	/*																		*/
	/*		Moves the values from [i_first, i_last) over to i_pos			*/
	/*	Requires i_pos already have allocated memory. Requires the ranges	*/
	/*	not overlap															*/
	template<typename St, class Alloc>
	template<class Iter>
	void ex_array<St, Alloc>::move_construct_vals(Iter i_first, Iter i_last, St* i_pos){
		std::for_each(i_first, i_last
					  , [&](St& yr_val){M_Alloc.construct(i_pos++, std::move(yr_val)); });
	}

	template<typename St, class Alloc>
	ex_array<St, Alloc>::ex_array(std::initializer_list<St> i_list
								  , const allocator_type& i_allocator /*= Alloc()*/)
								  : ex_array<St, Alloc>(i_list.begin(), i_list.end(), i_allocator){}

	/*ex_array(Iter, Iter, const allocator_type&)*/
	template<typename St, class Alloc>
	template<typename Iter>
	ex_array<St, Alloc>::ex_array(Iter i_first, Iter i_last, const allocator_type& i_alloc)
		: M_front_buffer_ratio(DEFAULT_FRONT_BUFFER_RATIO), M_Alloc(i_alloc){
		if(j_size_t dist = std::distance(i_first, i_last)){
			init_allocation_with_no_buffer_space(dist);
			construct_range(i_first, i_last, M_front);
		} else{
			default_initialization();
		}
	}

	/*ex_array(const ex_array<St, Alloc>&, const allocator_type&)*/

	template<typename St, typename Alloc>
	ex_array<St, Alloc>::ex_array(const ex_array<St, Alloc>& irk_src, const allocator_type& i_alloc)
		:M_front_buffer_ratio(irk_src.M_front_buffer_ratio), M_Alloc(i_alloc){

		default_initialization();
		if(irk_src.empty()){
			return;
		}
		expand(irk_src.capacity());

		M_front = M_zero + irk_src.front_buffer();
		M_used = M_front;
		assert(back_buffer() >= irk_src.size());

		M_used = M_front + irk_src.size();



		construct_range(irk_src.cbegin(), irk_src.cend(), M_front);




	}

	///void construct_range(Iter, Iter, St*)
	/*																		*/
	/*		Constructs the values in range started by i_dest with values	*/
	/*	from the range [i_first, i_last). Requires memory already be		*/
	/*	allocated uninitialized in range [i_dest, i_dest + (i_last-i_first))*/

	template<typename St, class Alloc>
	template<class Iter>
	void ex_array<St, Alloc>::construct_range(Iter i_first, Iter i_last, St* i_dest){
		std::for_each(i_first, i_last, [&](const St& y_src){M_Alloc.construct(i_dest++, y_src); });
	}

	/*ex_array(ex_array<St, Alloc>&&, const allocator_type&)*/

	template<typename St, class Alloc>
	ex_array<St, Alloc>::ex_array(ex_array<St, Alloc>&& irk_src, const allocator_type& i_alloc)
		:M_front_buffer_ratio(DEFAULT_FRONT_BUFFER_RATIO), M_Alloc(i_alloc){
		default_initialization();
		swap(irk_src);
	}

	/*~ex_array()*/
	template<typename St, class Alloc>
	ex_array<St, Alloc>::~ex_array(){ destroy(); }

	///void destroy()
	/*																		*/
	/*		Deallocates all memory and runs destructor on objects in		*/
	/*	container. Returns ex_array to the empty state						*/

	template<typename St, class Alloc>
	void ex_array<St, Alloc>::destroy(){
		destruct_data(M_front, M_used);
		M_Alloc.deallocate(M_zero, M_end - M_zero);
		default_initialization();
	}

	///void destruct_data(St*, St*)
	/*																		*/
	/*		Calls the destructor on all data in the range [i_first,i_last)	*/
	/*	NOTE:: does not change any of the pointer values or the current		*/
	/*	allocation of the ex_array.											*/

	template<typename St, class Alloc>
	void ex_array<St, Alloc>::destruct_data(St* i_first, St* i_last){
		std::for_each(i_first, i_last, [&](St& y_datum){M_Alloc.destroy(std::addressof(y_datum)); });
	}

	/*ex_array& operator=(const ex_array&)*/
	template<typename St, class Alloc>
	ex_array<St, Alloc>& ex_array<St, Alloc>::operator=(const ex_array<St, Alloc>& irk_rhs){
		ex_array temp(irk_rhs);
		swap(temp);
		return *this;
	}

	/*ex_array& operator=(ex_array&&)*/
	template<typename St, class Alloc>
	ex_array<St, Alloc>& ex_array<St, Alloc>::operator=(ex_array<St, Alloc>&& irk_rhs){
		swap(irk_rhs);
		return *this;
	}

	/*void swap(ex_array&)*/
	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::swap(ex_array<St, Alloc>& _ir_src){
		std::swap(_ir_src.M_zero, M_zero);
		std::swap(_ir_src.M_front, M_front);
		std::swap(_ir_src.M_used, M_used);
		std::swap(_ir_src.M_end, M_end);
		std::swap(_ir_src.M_front_buffer_ratio, M_front_buffer_ratio);
	}

	/*void assign(size_type, const St&)*/
	template<typename St, class Alloc>
	void ex_array<St, Alloc>::assign(size_type i_size, const St& i_val){
		clear_data();


		if(i_size > capacity()){
			expand(i_size);
		}
#if EX_ARRAY_DEBUG
		St* initial_zero = M_zero;
#endif

		//Create Space
		make_space_in_back(i_size);


#if EX_ARRAY_DEBUG
		assert(initial_zero == M_zero);
		assert(M_front >= M_zero);
		assert((M_end - M_front) >= i_size);
#endif

		//Set ex_array size
		M_used = M_front + i_size;

		//Fill with value
		construct_range(M_front, M_used, i_val);


	}


	/*assign(Iter, Iter)*/
	template<typename St, class Alloc>
	template<typename Iter>
	void ex_array<St, Alloc>::assign(Iter i_first, Iter i_last){

		clear_data();

		std::ptrdiff_t range_size = std::distance(i_first, i_last);

		if(range_size > capacity()){
			expand(range_size);
		}

#if EX_ARRAY_DEBUG
		St* initial_zero = M_zero;
#endif

		//Create Space
		make_space_in_back(range_size);


#if EX_ARRAY_DEBUG
		assert(initial_zero == M_zero);
		assert(M_front >= M_zero);
		assert((M_end - M_front) >= range_size);
#endif
		//Set ex_array size
		M_used = M_front + range_size;

		//Fill with value
		construct_range(i_first, i_last, M_front);
	}

	///clear_data()
	/*																			*/
	/*      Clears all data and sets the ex_array's size to zero.				*/

	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::clear_data(){ destruct_data(M_front, M_used); M_used = M_front; }

	///void make_space_in_back(std::ptrdiff_t i_desired_back_buffer)
	/*																			*/
	/*		Reallocates or slides memory to allow there to be at least			*/
	/*	i_desired_back_buffer spots of unused allocated space in the back of the*/
	/*	ex_array																*/
	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::make_space_in_back(std::ptrdiff_t i_desired_back_buffer){
		if(capacity() < (i_desired_back_buffer + size())){
			ex_expand(size() + i_desired_back_buffer);
		}

		if(back_buffer() < i_desired_back_buffer){
			std::ptrdiff_t extra_space_needed_in_back = safe_cast<std::ptrdiff_t>(i_desired_back_buffer - back_buffer());

			assert(front_buffer() >= extra_space_needed_in_back);

			std::ptrdiff_t slide_size = safe_cast<std::ptrdiff_t>(buffer()*M_front_buffer_ratio - back_buffer());
			slide_size = slide_size > extra_space_needed_in_back ? slide_size : extra_space_needed_in_back;

			slide_size = std::max(slide_size
								  , static_cast<std::ptrdiff_t>(front_buffer()*M_slide_overshoot_val));
			assert(front_buffer() >= slide_size);
			slide_contents(-slide_size);
		}
	}


	///void slide_contents(std::ptrdiff_t)
	/*																			*/
	/*      Slides the contents of the array by slide_size. If slide_size if	*/
	/*	negative values are slid to the left. If they are positive they slide to*/
	/*	the right. Does nothing if slide_size == 0. Keeps pointers valid		*/

	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::slide_contents(std::ptrdiff_t i_slide_size){
		if(!i_slide_size){
			return;
		}

		if(0 > i_slide_size){
			//We are moving to the left
			i_slide_size *= -1;

			assert((M_front - i_slide_size) >= M_zero);
			slide_left(i_slide_size, safe_ptrdiff_cast(size()));
			M_used -= i_slide_size;
		} else{
			assert(0 < i_slide_size);
			//We have i_slide_size positive. Moving to right
			assert((M_used + i_slide_size) <= M_end);

			slide_right(i_slide_size, safe_ptrdiff_cast(size()));
			M_front += i_slide_size;
		}

	}

	///void slide_left(std::ptrdiff_t, std::ptrdiff_t)
	/*																		*/
	/*		Slides the first i_slide_amount elements to the left 			*/
	/*	i_slide_size positions. this is to create space for insertion		*/

	template<typename St, class Alloc>
	void ex_array<St, Alloc>::slide_left(std::ptrdiff_t i_slide_size
										 , std::ptrdiff_t i_slide_amount){

		if(!i_slide_size){
			return;
		}
		St* insert_pos = M_front;
		M_front -= i_slide_size;

		for(int i = 0; i < i_slide_amount; i++){
			M_Alloc.construct(M_front + i, std::move(*insert_pos));
			M_Alloc.destroy(insert_pos++);
		}

	}

	///void slide_right(std::ptrdiff_t, std::ptrdiff_t)
	/*																		*/
	/*		Slides the last i_slide_amount elements to the right 			*/
	/*	i_slide_size positions. this is to create space for insertion		*/

	template<typename St, class Alloc>
	void ex_array<St, Alloc>::slide_right(std::ptrdiff_t i_slide_size
										  , std::ptrdiff_t i_slide_amount){

		if(!i_slide_size){
			return;
		}

		St* insert_pos = M_used - 1;
		M_used += (i_slide_size);

		for(int i = 0; i < i_slide_amount; i++){
			M_Alloc.construct(M_used - i - 1, std::move(*insert_pos));
			M_Alloc.destroy(insert_pos--);
		}
	}


	///void ex_expand(size_type)
	/*																		*/
	/*		Expands when trying to accomadate i_size elements				*/
	/*	This results in an reallocation greater then i_size.				*/
	/*																		*/
	template<typename St, class Alloc>
	void ex_array<St, Alloc>::ex_expand(size_type i_size){
		assert(i_size > capacity());
		expand(static_cast<size_type>(i_size*OVERALLOCATION_MULTIPLIER));
	}

	//Container Non Modifying Operations-----------------------------------------------------------------------

	/*size_type size()const*/
	template<typename St, class Alloc>
	typename ex_array<St, Alloc>::size_type ex_array<St, Alloc>::size()const{ return M_used - M_front; };

	/*bool empty()const*/
	template<typename St, class Alloc>
	bool ex_array<St, Alloc>::empty()const{ return (M_front == M_used); } //Data is held between M_front and M_used

	/*size_type max_size()const*/
	template<typename St, class Alloc>
	typename ex_array<St, Alloc>::size_type ex_array<St, Alloc>::max_size()const{ return static_cast<size_type>(M_Alloc.max_size()); }

	//Non Modifying Policy Operations----------------------------------------------------------------------------------

	/*const size_type capacity()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::size_type ex_array<St, Alloc>::capacity()const{ return M_end - M_zero; }

	/*size_type front_buffer()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::size_type ex_array<St, Alloc>::front_buffer()const{ return M_front - M_zero; }

	/*size_type back_buffer()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::size_type ex_array<St, Alloc>::back_buffer()const{ return M_end - M_used; }

	/*size_type buffer()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::size_type ex_array<St, Alloc>::buffer()const{ return back_buffer() + front_buffer(); }

	//Modifying Policy Operations---------------------------------------------------------------------------------------

	/*void reserve(size_type)*/
	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::reserve(size_type i_size){
		if(i_size > capacity()){
			expand(i_size);
		}
	}

	/*void reserve(size_type)*/
	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::reserve(size_type i_size, Dbl_t i_front_buff){
		M_front_buffer_ratio = i_front_buff;
		reserve(i_size);
	}

	/*void shrink_to_fit()*/
	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::shrink_to_fit(){
		M_Alloc.deallocate(M_zero, front_buffer());
		M_Alloc.deallocate(M_used, back_buffer());
		M_zero = M_front;
		M_end = M_used;
		//Sanity Check
		assert(capacity() == size());
	}

	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::set_front_buffer(Dbl_t i_front_buffer_ratio){
		assert(between_inclusive(i_front_buffer_ratio, 0.0, 1.0));
		M_front_buffer_ratio = i_front_buffer_ratio;
	}

	//Direct Element Access------------------------------------------------------------------------

	/*St& at(size_type i_pos)*/
	template<typename St, typename Alloc>
	St& ex_array<St, Alloc>::at(size_type i_pos){
		if(!between_inclusive(i_pos, 0ll, size() - 1)){
			throw std::out_of_range("out o range");
		}
		return *(M_front + i_pos);
	}

	/*St& at(size_type i_pos)*/
	template<typename St, typename Alloc>
	const St& ex_array<St, Alloc>::at(size_type i_pos)const{
		if(!between_inclusive(i_pos, 0ll, size() - 1)){
			throw std::out_of_range("out o range");
		}
		return *(M_front + i_pos);
	}

	/*St* data()*/
	template<typename St, typename Alloc>
	St* ex_array<St, Alloc>::data(){ return M_front; }

	/*const St* data()const*/
	template<typename St, typename Alloc>
	const St* ex_array<St, Alloc>::data()const{ return M_front; }

	/*St& operator[](size_type)*/
	template<typename St, typename Alloc>
	St& ex_array<St, Alloc>::operator[](size_type i_index){
#if _DEBUG
		assert(between_inclusive(i_index, J_SIZE_T_ZERO, size() - 1));
#endif
		return M_front[i_index];
	}

	/*const St& operator[](size_type)const*/
	template<typename St, typename Alloc>
	const St& ex_array<St, Alloc>::operator[](size_type i_index)const{
#if _DEBUG
		assert(between_inclusive(i_index, J_SIZE_T_ZERO, size() - 1));
#endif
		return M_front[i_index];
	}

	/*St& back();*/
	template<typename St, typename Alloc>
	St& ex_array<St, Alloc>::back(){ return *(end() - 1); }

	/*const St& back();const*/
	template<typename St, typename Alloc>
	const St& ex_array<St, Alloc>::back()const{ return *(end() - 1); }

	/*St& front()*/
	template<typename St, typename Alloc>
	St& ex_array<St, Alloc>::front(){ return *begin(); }

	/*const St& front();const*/
	template<typename St, typename Alloc>
	const St& ex_array<St, Alloc>::front()const{ return *begin(); }


	//Insertion Functions---------------------------------------------------------------------------------------

	/*iterator insert(iterator, const St&)*/
	template<typename St, class Alloc>
	typename ex_array<St, Alloc>::iterator ex_array<St, Alloc>::insert(const_iterator pos, const St& i_val){
		return insert(pos, &i_val, &i_val + 1);
	}

	/*iterator insert(iterator, St&&)*/
	template<typename St, class Alloc>
	typename ex_array<St, Alloc>::iterator ex_array<St, Alloc>::insert(const_iterator i_pos, St&& i_val){
		return insert(i_pos, std::make_move_iterator(&i_val), std::make_move_iterator(&i_val + 1));
	}

	/*iterator insert(iterator, Iter, Iter)*/
	template<typename St, class Alloc>
	template<class Iter>
	typename ex_array<St, Alloc>::iterator
		ex_array<St, Alloc>::insert(const_iterator i_pos, Iter i_first, Iter i_end){
			std::ptrdiff_t index = i_pos - begin();
#if EX_ARRAY_DEBUG
			size_t init_size = size();
			std::ptrdiff_t dist = std::distance(i_first, i_end);
			assert(i_pos.base() <= M_used);
			assert(i_pos.base() >= M_front);
#endif
			if((i_pos.M_pos - M_front) < (M_used - i_pos.M_pos)){
				if(insert_left(i_pos, i_first, i_end)){
					return begin() + index;
				} else if(insert_right(i_pos, i_first, i_end)){
					return begin() + index;
				}
			} else{
				if(insert_right(i_pos, i_first, i_end)){
					return begin() + index;
				} else if(insert_left(i_pos, i_first, i_end)){
					return begin() + index;
				}
			}
			if(!insert_slide(i_pos, i_first, i_end)){
				expand_and_insert(i_pos, i_first, i_end);
				return begin() + index;
			}

#if EX_ARRAY_DEBUG
			assert(dist + init_size);
#endif
			return begin() + index;
	}

	///bool insert_left(iterator, Iter, Iter)
	/*																		*/
	/*		inserts the range [i_first, i_end) by sliding values to the left*/
	/*	from i_pos. Returns true on success									*/

	template<typename St, class Alloc>
	template<typename Iter>
	bool ex_array<St, Alloc>::insert_left(const_iterator i_pos, Iter i_first
										  , Iter i_last){
		size_type val_size = std::distance(i_first, i_last);

		if(empty() && val_size){
			return false;
		}
		
		if((M_front - val_size) < M_zero){
			return false;
		}
#if EX_ARRAY_DEBUG
		assert(front_buffer() >= val_size);
#endif

		slide_left(val_size, i_pos - begin());


		construct_range(i_first, i_last, M_front + (i_pos - begin()) - val_size);

		return true;
	}

	/*bool insert_right(iterator, Iter, Iter)*/
	/*																		*/
	/*		Inserts the range [i_first, i_end) by sliding values to the		*/
	/*	right from i_pos. Returns true on success							*/

	template<typename St, class Alloc>
	template<typename Iter>
	bool ex_array<St, Alloc>::insert_right(const_iterator i_pos
										   , Iter i_first, Iter i_last){
		size_type val_size = std::distance(i_first, i_last);
		if((M_used + val_size) >= M_end)
			return false;

#if EX_ARRAY_DEBUG
		assert((capacity() - front_buffer() - size()) >= val_size);
#endif
		slide_right(val_size, end() - i_pos);

		construct_range(i_first, i_last, M_front + (i_pos - begin()));

		return true;
	}

	///bool insert_slide(iterator, Iter, Iter)
	/*																		*/
	/*		Inserts the range [i_first, i_end) by sliding values to the		*/
	/*	right and left from i_pos. Returns true on success					*/
	template<typename St, class Alloc>
	template<typename Iter>
	bool ex_array<St, Alloc>::insert_slide(const_iterator pos
										   , Iter i_first, Iter i_last){
		size_type val_size = std::distance(i_first, i_last);
		if((val_size + size()) > capacity()){
			return false;
		}
#if EX_ARRAY_DEBUG
		assert(front_buffer() < val_size);
		assert((capacity() - size() - front_buffer()) <= val_size);
		assert((size() + val_size) <= capacity());
#endif
		std::ptrdiff_t index = pos - begin();
		size_type old_front_buffer = front_buffer();
		slide_left(old_front_buffer, pos - begin());
		slide_right(val_size - old_front_buffer, end() - pos);
		construct_range(i_first, i_last, M_front + index);

		return true;
	}


	///void expand_and_insert(iterator, Iter, Iter)
	/*																		*/
	/*		Inserts the range [i_first, i_end) by expanding the allocated	*/
	/*	space.																*/
	template<typename St, class Alloc>
	template<typename Iter>
	void ex_array<St, Alloc>::expand_and_insert(const_iterator pos
												, Iter i_first, Iter i_last){
		std::ptrdiff_t dist = pos.M_pos - M_front;
		ex_expand(size() + std::distance(i_first, i_last));
		insert(iterator(M_front + dist), i_first, i_last);
	}



	//Erasing Functions-----------------------------------------------------------------------------

	/*iterator erase(j_size_t i_pos, j_size_t i_len = j_npos)*/
	template<typename St, class Alloc>
	typename ex_array<St, Alloc>::iterator
		ex_array<St, Alloc>::erase(size_type i_pos, size_type i_len){
			return erase(begin() + i_pos, begin() + std::min(i_pos + i_len, size()));
	}

	/*iterator erase(const_iterator i_pos, j_size_t i_size = 1)*/
	template<typename St, class Alloc>
	typename ex_array<St, Alloc>::iterator
		ex_array<St, Alloc>::erase(const_iterator i_pos, size_type i_size){
			return erase(i_pos, i_pos + i_size);
	}

	/*iterator erase(const_iterator, const_iterator)*/
	template<typename St, class Alloc>
	typename ex_array<St, Alloc>::iterator
		ex_array<St, Alloc>::erase(const_iterator i_pos
		, const_iterator i_end_pos){
#if _DEBUG
			assert(i_pos.base() <= M_used);
			assert(i_pos.base() >= M_front);
#endif 

#if EX_ARRAY_DEBUG
			St* init_first = M_front;
			St* init_used = M_used;
#endif
			std::ptrdiff_t number_on_left = i_pos - begin();
			std::ptrdiff_t number_on_right = end() - i_end_pos;
			std::ptrdiff_t number_to_delete = i_end_pos - i_pos;
			assert(size() == (number_on_left + number_on_right + number_to_delete));
			if(number_on_left < number_on_right){
				slide_to_right(number_to_delete, number_on_left);
#if EX_ARRAY_DEBUG
				assert(init_first == (M_front - number_to_delete));
#endif
			} else{
				slide_to_left(number_to_delete, number_on_right);
#if EX_ARRAY_DEBUG
				assert(init_used == (M_used + number_to_delete));
#endif
			}

			return begin() + number_on_left;
#if EX_ARRAY_DEBUG

#endif
	}


	///void slide_to_left(std::ptrdiff_t size, std::ptrdiff_t amount)
	/*																		*/
	/*		Slides the last i_slide_amount elements to the left				*/
	/*	i_slide_size positions. Destroying and overwriting the data in		*/
	/* [size()-i_slide_amount-i_slide_size, size() - i_slide_size			*/

	template<typename St, class Alloc>
	void ex_array<St, Alloc>::slide_to_left(std::ptrdiff_t i_slide_size
											, std::ptrdiff_t i_slide_amount){
		if(!i_slide_size){
			return;
		}
		St* cur_dest = M_used - i_slide_amount - i_slide_size;

		for(int i = 0; i < i_slide_size; i++){
			M_Alloc.destroy(cur_dest + i);
		}

		for(int i = 0; i < i_slide_amount; i++){
			M_Alloc.construct(cur_dest, std::move(*(cur_dest + i_slide_size)));
			M_Alloc.destroy(cur_dest + i_slide_size);
			++cur_dest;
		}
		M_used -= i_slide_size;
	}

	///void slide_to_right(std::ptrdiff_t size, std::ptrdiff_t amount)
	/*																		*/
	/*		Slides the first i_slide_amount elements to the right			*/
	/*	i_slide_size positions. Destroying and overwriting the data in		*/
	/* [i_slide_size, i_slide_size + i_slide_amount)						*/

	template<typename St, class Alloc>
	void ex_array<St, Alloc>::slide_to_right(std::ptrdiff_t i_slide_size
											 , std::ptrdiff_t i_slide_amount){
		if(!i_slide_size){
			return;
		}
		St* cur_dest = M_front + i_slide_amount + i_slide_size - 1;

		for(int i = 0; i < i_slide_size; i++){
			M_Alloc.destroy(cur_dest - i);
		}

		for(int i = 0; i < i_slide_amount; i++){
			M_Alloc.construct(cur_dest, std::move(*(cur_dest - i_slide_size)));
			M_Alloc.destroy(cur_dest - i_slide_size);
			--cur_dest;
		}
		M_front += i_slide_size;
	}





	//Push and Pop, back and front---------------------------------------------------------------------------------------

	/*void push_back(const St&)*/
	template<typename St, class Alloc>
	void ex_array<St, Alloc>::push_back(const St& _ik_src){
		assert(capacity() >= size());
		make_space_in_back(1);
		M_Alloc.construct(M_used++, _ik_src);

	}

	/*void push_back(const St&&)*/
	template<typename St, class Alloc>
	void ex_array<St, Alloc>::push_back(St&& _ik_src){
		assert(capacity() >= size());
		make_space_in_back(1);
		M_Alloc.construct(M_used++, std::move(_ik_src));

	}

	/*void emplace_back(const St&)*/
	template<typename St, class Alloc>
	void ex_array<St, Alloc>::emplace_back(const St& _ik_src){
		assert(capacity() >= size());
		make_space_in_back(1);
		M_Alloc.construct(M_used++, _ik_src);

	}


	/*void emplace_back(Args&&...)*/
	template<typename St, class Alloc>
	template<typename... Args>
	void ex_array<St, Alloc>::emplace_back(Args&&... _i_args){
		assert(capacity() >= size());
		make_space_in_back(1);

		M_Alloc.construct(M_used++, std::forward<Args>(_i_args)...);
	}


	/*void push_front(const St&)*/
	template<typename St, class Alloc>
	void ex_array<St, Alloc>::push_front(const St& irk_val){
		assert(capacity() >= size());
		make_space_in_front(1);
		M_Alloc.construct(--M_front, irk_val);
	}

	/*void push_front(const St&&)*/
	template<typename St, class Alloc>
	void ex_array<St, Alloc>::push_front(St&& irr_val){
		assert(capacity() >= size());
		make_space_in_front(1);
		M_Alloc.construct(--M_front, std::move(irr_val));
	}


	///void make_space_in_front(std::ptrdiff_t i_desired_front_buffer)
	/*																			*/
	/*		Reallocates or slides memory to allow there to be at least			*/
	/*	i_desired_front_buffer spots of unused allocated space in the back of	*/
	/*	the	ex_array.															*/
	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::make_space_in_front(std::ptrdiff_t i_desired_front_buffer){
		if(capacity() == size()){
			ex_expand(size() + i_desired_front_buffer);
		}

		if(front_buffer() < i_desired_front_buffer){
			std::ptrdiff_t extra_space_needed_in_front
				= safe_ptrdiff_cast(i_desired_front_buffer - front_buffer());

			assert(back_buffer() >= extra_space_needed_in_front);

			std::ptrdiff_t slide_size = safe_ptrdiff_cast(buffer()*(1 - M_front_buffer_ratio)
														  - back_buffer());

			slide_size = slide_size > extra_space_needed_in_front ? slide_size : extra_space_needed_in_front;
			slide_size = std::max(slide_size
								  , static_cast<std::ptrdiff_t>(back_buffer()*M_slide_overshoot_val));

			assert(back_buffer() >= slide_size);
			slide_contents(slide_size);
		}
	}


	/*void pop_back()*/
	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::pop_back(){ assert(!empty()); M_Alloc.destroy(--M_used); }

	/*void pop_front()*/
	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::pop_front(){
		assert(!empty());
		M_Alloc.destroy(M_front++);
	}


	//Resizing---------------------------------------------------------------------------------------

	/*void resize(size_type i_size, const St& i_val = St())*/
	template<typename St, class Alloc>
	void ex_array<St, Alloc>::resize(size_type i_size, const St& irk_val){

		if(size() == i_size){
			return;
		}
		if(capacity() < i_size){
			expand(i_size);
		}

		if(size() > i_size){
			size_type reduction_amount = (size() - i_size);
			destruct_data(M_used - reduction_amount, M_used);
			assert((M_used - reduction_amount) == (M_front + i_size));
			M_used -= reduction_amount;
		} else{
			make_space_in_back(i_size - size());
			assert(M_front + i_size <= M_end);
			construct_range(M_used, M_used + (i_size - size()), irk_val);
			M_used = M_front + i_size;
		}
	}
	template<typename St, class Alloc>
	template<typename Pred_t>
	void ex_array<St, Alloc>::remove_if(const Pred_t& irk_pred_fo){
		auto iter_pos = begin();
		while(iter_pos != end()){
			if(irk_pred_fo(*iter_pos)){
				iter_pos = erase(iter_pos);
			} else{
				++iter_pos;
			}
		}
	}

	/*void resize(size_type i_size, const St& i_val */
	template<typename St, class Alloc>
	void ex_array<St, Alloc>::resize(size_type i_size){

		if(size() == i_size){
			return;
		}
		if(capacity() < i_size){
			expand(i_size);
		}

		if(size() > i_size){
			size_type reduction_amount = (size() - i_size);
			destruct_data(M_used - reduction_amount, M_used);
			assert((M_used - reduction_amount) == (M_front + i_size));
			M_used -= reduction_amount;
		} else{
			make_space_in_back(safe_ptrdiff_cast(i_size - size()));
			assert(M_front + i_size <= M_end);
			std::for_each(M_used, M_used + (i_size - size())
						  , [&](St& y_thing){M_Alloc.construct(std::addressof(y_thing)); }
			);
			M_used = M_front + i_size;
		}
	}


	/*void resize_front(size_type i_size, const St& i_val = St())*/
	template<typename St, class Alloc>
	void ex_array<St, Alloc>::resize_front(size_type i_size
										   , const St& irk_val){

		if(size() == i_size){
			return;
		}
		if(capacity() < i_size){
			expand(i_size);
		}

		if(size() > i_size){
			size_type reduction_amount = (size() - i_size);
			destruct_data(M_front, M_front + reduction_amount);
			assert((M_used) == (M_front + reduction_amount + i_size));
			M_front += reduction_amount;
		} else{
			size_type increase_amount = (i_size - size());
			make_space_in_front(increase_amount);
			assert(M_front - increase_amount >= M_zero);
			M_front -= increase_amount;
			construct_range(M_front, M_front + increase_amount, irk_val);
		}
	}

	/*void clear()*/
	template<typename St, typename Alloc>
	void ex_array<St, Alloc>::clear(){ destroy(); }

	//Iterator---------------------------------------------------------------------------------------

	/*iterator begin()*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::iterator ex_array<St, Alloc>::begin(){ return iterator(M_front); }

	/*const_iterator begin()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::const_iterator ex_array<St, Alloc>::begin()const{
		return const_iterator(M_front);
	}

	/*const_iterator cbegin()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::const_iterator ex_array<St, Alloc>::cbegin()const{
		return const_iterator(M_front);
	}

	/*iteraor end()*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::iterator ex_array<St, Alloc>::end(){
		return iterator(M_used);
	}

	/*const_iteraor end()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::const_iterator ex_array<St, Alloc>::end()const{
		return const_iterator(M_used);
	}

	/*const_iteraor cend()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::const_iterator ex_array<St, Alloc>::cend()const{
		return const_iterator(M_used);
	}

	/*reverse_iterator begin()*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::reverse_iterator ex_array<St, Alloc>::rbegin(){ return reverse_iterator(end()); }

	/*const_reverse_iterator begin()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::const_reverse_iterator ex_array<St, Alloc>::rbegin()const{
		return const_reverse_iterator(end());
	}

	/*const_reverse_iterator cbegin()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::const_reverse_iterator ex_array<St, Alloc>::crbegin()const{
		return const_reverse_iterator(cend());
	}

	/*iteraor end()*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::reverse_iterator ex_array<St, Alloc>::rend(){
		return reverse_iterator(begin());
	}

	/*const_iteraor end()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::const_reverse_iterator ex_array<St, Alloc>::rend()const{
		return const_reverse_iterator(begin());
	}

	/*const_iteraor cend()const*/
	template<typename St, typename Alloc>
	typename ex_array<St, Alloc>::const_reverse_iterator ex_array<St, Alloc>::crend()const{
		return const_reverse_iterator(cbegin());
	}

	//Lexographical Comparison---------------------------------------------------------------------------------------


	/*bool operator==(const ex_array&, const ex_array&)*/
	template<typename St, class Alloc>
	bool operator==(const ex_array<St, Alloc>& irk_left, const ex_array<St, Alloc>& irk_right){
		if(irk_left.size() != irk_right.size()){
			return false;
		}
		return std::equal(irk_left.begin(), irk_left.end(), irk_right.begin());
	}

	/*bool operator!=(const ex_array&, const ex_array&)*/
	template<typename St, class Alloc>
	bool operator!=(const ex_array<St, Alloc>& irk_left, const ex_array<St, Alloc>& irk_right){
		return !(irk_left == irk_right);
	}

	/*bool operator<(const ex_array&, const ex_array&)*/
	template<typename St, class Alloc>
	bool operator<(const ex_array<St, Alloc>& irk_left, const ex_array<St, Alloc>& irk_right){
		return std::lexicographical_compare(irk_left.begin(), irk_left.end(), irk_right.begin(), irk_right.end());
	}

	/*bool operator>(const ex_array&, const ex_array&)*/
	template<typename St, class Alloc>
	bool operator>(const ex_array<St, Alloc>& irk_left, const ex_array<St, Alloc>& irk_right){
		return std::lexicographical_compare(irk_right.begin(), irk_right.end(), irk_left.begin(), irk_left.end());
	}

	/*bool operator>=(const ex_array&, const ex_array&)*/
	template<typename St, class Alloc>
	bool operator>=(const ex_array<St, Alloc>& irk_left, const ex_array<St, Alloc>& irk_right){
		return !(irk_left < irk_right);
	}

	/*bool operator<=(const ex_array&, const ex_array&)*/
	template<typename St, class Alloc>
	bool operator<=(const ex_array<St, Alloc>& irk_left, const ex_array<St, Alloc>& irk_right){
		return !(irk_left > irk_right);
	}

	//template<typename St, class Alloc>
	//void swap(ex_array<St, Alloc>& ir_left, ex_array<St, Alloc>& ir_right){ir_left.swap(ir_right);}

	template<typename St, class Alloc>
	template<typename Func_Obj_t>
	Func_Obj_t ex_array<St, Alloc>::apply(const Func_Obj_t& i_func_obj){
		return std::for_each(begin(), end(), i_func_obj);
	}

} // namespace jomike

namespace std{
	template<typename St, class Alloc>
	void swap(jtl::ex_array<St, Alloc>& ir_left, jtl::ex_array<St, Alloc>& ir_right){ ir_left.swap(ir_right); }
} // namespace std

//Operating On Elements---------------------------------------------------------------------------------------




#endif //EX_ARRAY_H