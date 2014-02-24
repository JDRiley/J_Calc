#ifndef RB_TREE_H
#define RB_TREE_H

#include <J_Error.h>
#include <j_iterator.h>
#include <J_Utile.h>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include <utility>

namespace jomike{



const char* const UNDEFINED_OPERATION = "Undefined Operation: ";
class Bad_Tree_Op: public J_Error{
	std::string M_message;
public:
	Bad_Tree_Op(const char* const ik_message = ""):J_Error(ik_message)
		, M_message("###BAD_TREE_OPERATION###! ->" ){
		M_message.append(ik_message);
	};
};

template<typename St, typename Key = St, class Comp_t = std::less<Key>, class Alloc_t = std::allocator<St>>
class RB_Tree{
private:
	struct RB_Node__;
	typedef RB_Node__ Node_t;
public:
	typedef	St			value_type;
	typedef	ptrdiff_t	difference_type;
	typedef	St& 		reference;
	typedef const St&	const_reference;
	typedef	St*  		pointer;
	typedef	const St*	const_pointer;
	typedef Alloc_t allocator_type;

	typedef RB_Tree this_type;

	typedef node_iterator<typename this_type::Node_t*, St, this_type>
		iterator;

	typedef node_iterator<typename this_type::Node_t*, const St, this_type>
		const_iterator;

	friend class iterator;
	j_size_t erase(const Key&);
	iterator erase(const_iterator i_pos);
	iterator erase(const_iterator i_pos, const_iterator i_end);
	bool valid(){return false;}
	void in_order_walk(std::ostream&);
	/*Rotate Right Public*/
	void rotate_right_pub(iterator pos){
		try{
		rotate_right(pos.M_pos);
		}catch(Bad_Tree_Op& e){
			cerr << e.what() << std::endl;
		}
	}
	/*Rotate Left Public*/
	void rotate_left_pub(iterator pos){
		try{
		rotate_left(pos.M_pos);
		}catch(Bad_Tree_Op& e){
			cerr << e.what();
		}
	}
	void color_red(iterator pos){
		pos.M_pos->M_color = COLOR::RED;
	}
	void color_black(iterator pos){
		pos.M_pos->M_color = COLOR::BLACK;
	}
	int proper_rb_tree()const;

	void clear(){
		delete_nodes(M_root);
		M_size = 0;
		M_root = nullptr;
	}
	/*Insert*/
	std::pair<iterator, bool> insert(const St&);
	std::pair<iterator, bool> insert(St&&);
	/*Delete*/
	void remove(iterator);

	template<class Func_obj>
	Func_obj apply(Func_obj i_func_obj){
		return apply_internal(i_func_obj, M_root);
	}
	/*Element Access*/
	iterator minimum();
	const_iterator minimum()const;
	iterator maximum();
	iterator root(){return iterator(M_root);}
	iterator begin(){return minimum();}
	iterator end(){return iterator(nullptr);}

	const_iterator begin()const{return minimum();}
	const_iterator end()const{return const_iterator(nullptr);}
	const_iterator cbegin()const{ return const_iterator(minimum()); }
	const_iterator cend()const{ return const_iterator(nullptr); }

	iterator find(const Key& _ik_key);
	const_iterator find(const Key& _ik_key)const;
	iterator lower_bound(const Key& ik_key);
	/*Size*/
	const size_t size()const{return M_size;}
	/*Empty*/
	bool empty()const{return !M_root;}
	void set_messages(bool val){s_messages = val;}
	/*Reset Rotation*/
	static void reset_rotations(){num_rotations = 0;}
	static const int get_rotations(){return num_rotations;}

	/*Swap*/
	void swap(RB_Tree& ir_src){
		std::swap(M_root, ir_src.M_root);
		std::swap(s_messages, ir_src.s_messages);
		std::swap(M_size, ir_src.M_size);
	}

	bool count(const Key&)const;
	/*Constructors*/
	template<typename iter>
	RB_Tree(iter, iter);
	RB_Tree(const RB_Tree&);
	RB_Tree(RB_Tree&&);
	RB_Tree();
	/*Assignment*/
	RB_Tree& operator=(const RB_Tree& irk_src){
		RB_Tree temp(irk_src);
		RB_Tree::swap(temp);
		return *this;
	}
	/*Destructor*/
	~RB_Tree();
private:
	friend class iterator;
	friend class const_iterator;

	typename allocator_type::template rebind<Node_t*>::other M_alloc;

	static Node_t* previous_node(const Node_t* i_node);
	static Node_t* next_node(const Node_t* i_node);

	enum COLOR{RED, BLACK};
	struct RB_Node__{
		St data;
		COLOR M_color;
		RB_Node__* left;
		RB_Node__* right;
		RB_Node__* parent;
		bool is_leaf()const{return (!left && !right);};
		bool is_red()const{return this ? (M_color == COLOR::RED) : false;}
		bool is_black()const{return !is_red();}
		bool is_right()const{return (parent && (this == parent->right));}
		bool is_left()const{return (parent && (this == parent->left));}
		RB_Node__(const St&  ik_data):data(std::move(ik_data)), M_color(COLOR::RED)
			, left(nullptr), right(nullptr){}


	};
	/*Friends*/
	

	

	/*Private Types*/
	
	/*Nodes*/
	
	Node_t* M_root;
	size_t M_size;
	bool s_messages;
	std::ostringstream M_output_stream;

	Node_t* find_node(const Key& irk_key)const;
	void rb_transplant(Node_t* i_top_node, Node_t* i_bottom_node);
	void fix_up(Node_t* );
	void delete_fixup(Node_t* i_node);
	void delete_fixup(Node_t* i_node, bool left_branch);
	void in_order_walk(std::ostream& oos, Node_t*, size_t& ir_pos);
	std::pair<iterator, bool> insert_core(const St& irr_val);
	void rotate_left(Node_t*);
	void rotate_right(Node_t*);
	void sub_proper_rb_tree(Node_t*, bool&, int&, int&)const;
	static Node_t* minimum(Node_t*);
	static Node_t* maximum(Node_t*);

	iterator tree_search(Node_t* const, const Key&); 

	bool equal(const St&, const Key&)const;
	static int num_rotations;
	void copy_node(Node_t*& dest_node, const Node_t* const src, Node_t* );
	void print_output(){
		if(s_messages)
			std::cout << M_output_stream.str() << std::endl;
		M_output_stream.str(std::string());
	}
	/*Compare Func Object*/
	Comp_t M_comp;
	void delete_nodes(Node_t*);

	template<class Func_obj>
	Func_obj apply_internal(Func_obj i_func_obj, Node_t* i_node){
		if(!i_node)
			return i_func_obj;
		i_func_obj(i_node->data);
		i_func_obj = apply_internal(i_func_obj, i_node->left);
		return apply_internal(i_func_obj, i_node->right);
	}
};



/*Copy Node*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
void RB_Tree<St, Key, Comp_t, Alloc_t>::copy_node(Node_t*& dest_node, const Node_t* const irk_src, Node_t* i_parent_node){
	if(!irk_src){
		dest_node = nullptr;
		return;
	}

	dest_node = new Node_t(*irk_src);
	dest_node->parent = i_parent_node;

	copy_node(dest_node->left, irk_src->left, dest_node);
	copy_node(dest_node->right, irk_src->right, dest_node);
	
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_Tree<St, Key, Comp_t, Alloc_t>::RB_Tree(const RB_Tree<St, Key, Comp_t, Alloc_t>& irr_right): M_size(irr_right.size()), s_messages(false), M_output_stream(), M_comp(irr_right.M_comp){
	M_root = nullptr;
	copy_node(M_root, irr_right.M_root, nullptr);
}

/*Move Contstructor*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_Tree<St, Key, Comp_t, Alloc_t>::RB_Tree(RB_Tree<St, Key, Comp_t, Alloc_t>&& irr_right){
	M_size = 0; 
	M_root = nullptr; 
	s_messages = false;
	swap(irr_right);
}

/*static Node_t* next_node(const Node_t*)*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t* 
	RB_Tree<St, Key, Comp_t, Alloc_t>::next_node(const Node_t* i_node){
	assert(i_node);
	if(i_node->right){
		return minimum(i_node->right);
	}

	while(i_node->is_right()){
		i_node = i_node->parent;
	}
	
	return i_node->parent;
}

/*static Node_t* decrement(const Node_t*)*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t* 
	RB_Tree<St, Key, Comp_t, Alloc_t>::previous_node(const Node_t* i_node){
	assert(i_node);
	if(i_node->left){
		return maximum(i_node->left);
	}

	while(i_node->is_left()){
		i_node = i_node->parent;
	}
	
	return i_node->parent;
}

/*Proper RB Tree*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
int RB_Tree<St, Key, Comp_t, Alloc_t>::proper_rb_tree()const{
	bool return_val = true;
	int black_height = 0;
	if(!M_root)
		return true;

	if(M_root->is_red()){
		cerr << "Red Root! egad!" << std::endl;
		return false;
	}

	int found_black_height = -1;
	 sub_proper_rb_tree(M_root, return_val, black_height, found_black_height);
	 if(return_val)
		 return found_black_height;
	 return 0;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void RB_Tree<St, Key, Comp_t, Alloc_t>::sub_proper_rb_tree(Node_t* i_node, bool& valid, int& ir_black_height, int& found_black_height)const{
	if(!valid)
		return;

	if(!is_red(i_node)){
		++ir_black_height;
	}else{
		if(i_node->left->is_red() || i_node->right->is_red()){
			cerr << "Red on Red Action!!!" << std::endl;
			valid = false;
			return;
		}
	}

	if(!i_node){
		valid = ((found_black_height == -1) || (found_black_height == (ir_black_height)));
		if(!valid){
			assert(found_black_height != ir_black_height);
			cerr << "Differing Black Heights Calculated ! " << found_black_height << ' ' << ir_black_height << endl;
		}
		found_black_height = ir_black_height;
	}else{
		sub_proper_rb_tree(i_node->left, valid, ir_black_height, found_black_height);
		sub_proper_rb_tree(i_node->right, valid, ir_black_height, found_black_height);
	}
	if(!i_node->is_red()){
		--ir_black_height;
	}

}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_Tree<St, Key, Comp_t, Alloc_t>::RB_Tree():M_root(nullptr), M_size(0), s_messages(false){}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
RB_Tree<St, Key, Comp_t, Alloc_t>::~RB_Tree(){
	
	delete_nodes(M_root);
}

/*Delete Nodes*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
void RB_Tree<St, Key, Comp_t, Alloc_t>::delete_nodes(Node_t* i_node){
	if(!i_node)
		return;

	delete_nodes(i_node->left);
	delete_nodes(i_node->right);
	delete i_node;
		
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
std::pair<typename RB_Tree<St, Key, Comp_t, Alloc_t>::iterator, bool> 
	RB_Tree<St, Key, Comp_t, Alloc_t>::insert(St&& irr_val){

	return insert_core(irr_val);
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
std::pair<typename RB_Tree<St, Key, Comp_t, Alloc_t>::iterator, bool> 
	RB_Tree<St, Key, Comp_t, Alloc_t>::insert(const St& irk_val){

	return insert_core(irk_val);
}

/*Insert Core*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
std::pair<typename RB_Tree<St, Key, Comp_t, Alloc_t>::iterator, bool> RB_Tree<St, Key, Comp_t, Alloc_t>::insert_core(const St& i_val){
	/*if(s_messages){
		M_output_stream << "Inserting: " << i_val
			<< " Search Cost Incurred" << std::endl;
	}*/

	Node_t* prev_node = nullptr;
	Node_t* cur_node = M_root;
	Node_t* new_node = new Node_t(i_val);
	
	
	while(cur_node != nullptr){
		prev_node = cur_node;
		if(M_comp(i_val, cur_node->data)){
			cur_node = cur_node->left;
		}else{
			cur_node = cur_node->right;
		}
	}
	new_node->parent = prev_node;

	if(!prev_node){
		/*if(s_messages){
			M_output_stream << "New Root Node: " << new_node->data << std::endl;
		}*/
		M_root = new_node;
	}else if(M_comp(i_val, prev_node->data)){
		prev_node->left = new_node;
	}else if(M_comp(prev_node->data, i_val)){
		prev_node->right = new_node;
	}else{
		//Node already present
		delete new_node;
		return std::pair<iterator, bool>(end(), false);
	}


	new_node->right = new_node->left = nullptr;
	new_node->M_color = COLOR::RED;
	assert(M_root);
	fix_up(new_node);
	print_output();
	++M_size;
	return std::pair<iterator, bool>(iterator(new_node), true);
	//assert(proper_rb_tree());
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void RB_Tree<St, Key, Comp_t, Alloc_t>::fix_up(Node_t* i_node){
//	M_output_stream << "Fixing up node: " << i_node->data << std::endl;
	assert(COLOR::RED == i_node->M_color);
	Node_t* trace_node = nullptr;
	while(i_node->parent->is_red()){
		if(i_node->parent->is_left()){
			trace_node = i_node->parent->parent->right;
			if(trace_node && trace_node->is_red()){
				i_node->parent->M_color = COLOR::BLACK;
				trace_node->M_color = COLOR::BLACK;
				i_node->parent->parent->M_color = COLOR::RED;
				i_node = i_node->parent->parent;
			}else {
				if(i_node->is_right()){
					i_node = i_node->parent;
					rotate_left(i_node);
				}
				i_node->parent->M_color = COLOR::BLACK;
				i_node->parent->parent->M_color = COLOR::RED;
				rotate_right(i_node->parent->parent);
			}
		}else{
			trace_node = i_node->parent->parent->left;
			if(trace_node->is_red()){
				i_node->parent->M_color = COLOR::BLACK;
				trace_node->M_color = COLOR::BLACK;
				i_node->parent->parent->M_color = COLOR::RED;
				i_node = i_node->parent->parent;
			}else {
				if(i_node->is_left()){
					i_node = i_node->parent;
					rotate_right(i_node);
				}
				i_node->parent->M_color = COLOR::BLACK;
				i_node->parent->parent->M_color = COLOR::RED;
				rotate_left(i_node->parent->parent);
			}
		}
	}
	M_root->M_color = COLOR::BLACK;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void RB_Tree<St, Key, Comp_t, Alloc_t>::rb_transplant(Node_t* i_top_node, Node_t* i_bottom_node){
	/*M_output_stream << "Transplanting Nodes! Top Node: " << i_top_node->data;

	if(i_bottom_node){
		M_output_stream << " Bottom Node: " << i_bottom_node->data;
	}else{
		M_output_stream << " null bottom node...";
	}
	M_output_stream << std::endl;
	*/
	if(!i_top_node->parent){
		/*M_output_stream << "New Root Node: " ;
			if(i_bottom_node)
				M_output_stream << i_bottom_node->data << std::endl;
			else 
				M_output_stream << "null node" << std::endl;
				*/
		M_root = i_bottom_node;
	}else if(i_top_node->is_left()){
		i_top_node->parent->left = i_bottom_node;
	}else{
		i_top_node->parent->right = i_bottom_node;
	}
	if(i_bottom_node){
		i_bottom_node->parent = i_top_node->parent;
	}
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
j_size_t RB_Tree<St, Key, Comp_t, Alloc_t>::erase(const Key& i_key){
	const_iterator pos = find(i_key);
	if(end() == pos){
		return 0;
	}
	erase(pos);
	return 1;
}

/*Erase*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::iterator RB_Tree<St, Key, Comp_t, Alloc_t>::erase(const_iterator i_pos){

	Node_t* cur_node = i_pos.base();
	assert(cur_node);
	iterator iterator_to_return = ++iterator(cur_node);
	Node_t* node_to_delete = cur_node;
	Node_t* trailing_node = cur_node;
	assert(trailing_node);
	/*
	if(s_messages){
		M_output_stream << "Erasing " << trailing_node->data <<
			" Search Cost Incurred" << std::endl;
	}
	*/
	bool left_branch;
	bool is_y = false;
	bool right = false;
	Node_t* node_to_fixup;
	COLOR original_color = trailing_node->M_color;
	Node_t* old_parent = nullptr;
	if(!cur_node->left){
		node_to_fixup = cur_node->right;
		left_branch = cur_node->is_left();
		rb_transplant(cur_node, cur_node->right);
	}else if(!cur_node->right){
		node_to_fixup = cur_node->left;
		left_branch = cur_node->is_left();
		rb_transplant(cur_node, cur_node->left);
	}else{
		is_y = true;
		trailing_node = minimum(cur_node->right);
		original_color = trailing_node->M_color;
		node_to_fixup = trailing_node->right;

		left_branch = trailing_node->is_left();
		if((trailing_node->parent == cur_node)){
			if(node_to_fixup){
				node_to_fixup->parent = trailing_node;
			}else{
				right = true;
			}
		}else{
			rb_transplant(trailing_node, trailing_node->right);
			trailing_node->right = cur_node->right;
			trailing_node->right->parent = trailing_node;
		}

		if(!right)
			old_parent = trailing_node->parent;

		rb_transplant(cur_node, trailing_node);
		trailing_node->left = cur_node->left;
		trailing_node->left->parent = trailing_node;
		trailing_node->M_color = cur_node->M_color;
	}

	if(COLOR::BLACK == original_color){
		if(node_to_fixup)
			delete_fixup(node_to_fixup);
		else if(is_y){
			Node_t* node_to_use = right ? trailing_node : old_parent;
			delete_fixup(node_to_use, left_branch);
		}else{
			delete_fixup(cur_node->parent, left_branch);
		}
	}else if(s_messages){
		M_output_stream << "###BYPASSED DELETE FIXUP### " << std::endl;
	}

	delete node_to_delete;
	print_output();
	--M_size;
	//assert(proper_rb_tree());
	return iterator_to_return;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::iterator RB_Tree<St, Key, Comp_t, Alloc_t>::erase(
	const_iterator i_pos, const_iterator i_end_pos){
	iterator iterator_to_return
		= end() == i_end_pos ? end() : ++iterator(find_node(i_end_pos));

	while(i_pos != i_end_pos){
		i_pos = erase(i_pos);
	}


	return iterator_to_return;


}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void RB_Tree<St, Key, Comp_t, Alloc_t>::delete_fixup(Node_t* i_node){
	Node_t* trailing_node;
	if(!i_node){
		if(s_messages){
			M_output_stream<< "Delete Fixing up null node" << std::endl;
		}
		return;
	}
	/*if(s_messages){
		M_output_stream << "Delete Fixing up: " << i_node->data << std::endl;
	}*/
	while((i_node != M_root) && i_node->is_black()){
		if(i_node->is_left()){
			trailing_node = i_node->parent->right;
			if(trailing_node->is_red()){
				trailing_node->M_color = COLOR::BLACK;
				i_node->parent->M_color = COLOR::RED;
				rotate_left(i_node->parent);
				trailing_node = i_node->parent->right;
			}

			if(!trailing_node->left->is_red() && !trailing_node->right->is_red()){
				trailing_node->M_color = COLOR::RED;
				i_node = i_node->parent;
			}else {
				if(!trailing_node->right->is_red()){
					trailing_node->left->M_color = COLOR::BLACK;
					trailing_node->M_color = COLOR::RED;
					rotate_right(trailing_node);
					trailing_node = i_node->parent->right;
				}
				trailing_node->M_color = i_node->parent->M_color;
				i_node->parent->M_color = COLOR::BLACK;
				trailing_node->right->M_color = COLOR::BLACK;
				rotate_left(i_node->parent);
				i_node = M_root;
			}
		}else{
			trailing_node = i_node->parent->left;
			if(trailing_node->is_red()){
				trailing_node->M_color = COLOR::BLACK;
				i_node->parent->M_color = COLOR::RED;
				rotate_right(i_node->parent);
				trailing_node = i_node->parent->left;
			}
			if(!trailing_node->right->is_red() && !trailing_node->left->is_red()){
				trailing_node->M_color = COLOR::RED;
				i_node = i_node->parent;
			}else {
				if(!trailing_node->left->is_red()){
					trailing_node->right->M_color = COLOR::BLACK;
					trailing_node->M_color = COLOR::RED;
					rotate_left(trailing_node);
					trailing_node = i_node->parent->left;
				}
				trailing_node->M_color = i_node->parent->M_color;
				i_node->parent->M_color = COLOR::BLACK;
				trailing_node->left->M_color = COLOR::BLACK;
				rotate_right(i_node->parent);
				i_node = M_root;
			}
		}
	}
	i_node->M_color = COLOR::BLACK;
	
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
void RB_Tree<St, Key, Comp_t, Alloc_t>::delete_fixup(Node_t* i_node, bool left_branch){
	Node_t* trailing_node;
	if(!i_node){
		if(s_messages){
			M_output_stream<< "Special First Iteration, Delete Fixing up null node" << std::endl;
		}
		return;
	}
/*	if(s_messages){
		M_output_stream << "Special First Iteration Delete Fixing up: " << i_node->data << std::endl;
	}*/
	
		if(left_branch){
			trailing_node = i_node->right;
			if(trailing_node->is_red()){
				trailing_node->M_color = COLOR::BLACK;
				i_node->M_color = COLOR::RED;
				rotate_left(i_node);
				trailing_node = i_node->right;
			}
			if(!trailing_node->left->is_red() && !trailing_node->right->is_red()){
				trailing_node->M_color = COLOR::RED;
			}else {
				if(!trailing_node->right->is_red()){
					trailing_node->left->M_color = COLOR::BLACK;
					trailing_node->M_color = COLOR::RED;
					rotate_right(trailing_node);
					trailing_node = i_node->right;
				}
				trailing_node->M_color = i_node->M_color;
				i_node->M_color = COLOR::BLACK;
				trailing_node->right->M_color = COLOR::BLACK;
				rotate_left(i_node);
				i_node = M_root;
			}
		}else{
			trailing_node = i_node->left;
			if(trailing_node->is_red()){
				trailing_node->M_color = COLOR::BLACK;
				i_node->M_color = COLOR::RED;
				rotate_right(i_node);
				trailing_node = i_node->left;
			}
			if(trailing_node->right->is_black() && trailing_node->left->is_black()){
				trailing_node->M_color = COLOR::RED;
			}else {
				if(trailing_node->left->is_black()){
					trailing_node->right->M_color = COLOR::BLACK;
					trailing_node->M_color = COLOR::RED;
					rotate_left(trailing_node);
					trailing_node = i_node->left;
				}
				trailing_node->M_color = i_node->M_color;
				i_node->M_color = COLOR::BLACK;
				trailing_node->left->M_color = COLOR::BLACK;
				rotate_right(i_node);
				i_node = M_root;
			}
		}

	delete_fixup(i_node);
	
}

/*Rotate Left*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
void RB_Tree<St, Key, Comp_t, Alloc_t>::rotate_left(Node_t* i_node){
	/*if(s_messages){
		M_output_stream <<"Rotating Left Node: " << i_node->data << std::endl;
	}*/
	Node_t* trace_node = i_node->right;
	if(!trace_node){
		throw Bad_Tree_Op("No Right Node!");
	}
	i_node->right = trace_node->left;
	if(trace_node->left){
		trace_node->left->parent = i_node;
	}
	trace_node->parent = i_node->parent;
	if(!i_node->parent){
		/*if(s_messages){
			M_output_stream << "New Root Node: " << trace_node->data << std::endl;
		}*/
		M_root = trace_node;
	}else if(i_node->is_left()){
		i_node->parent->left = trace_node;
	}else{
		i_node->parent->right = trace_node;
	}
	trace_node->left = i_node;
	i_node->parent = trace_node;
	/*if(s_messages){
		M_output_stream << "Rotation " << num_rotations << std::endl;
	}*/
	++num_rotations;
}

/*Rotate Right*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
void RB_Tree<St, Key, Comp_t, Alloc_t>::rotate_right(Node_t* i_node){
	/*if(s_messages){
		M_output_stream <<"Rotating Right Node: " << i_node->data << std::endl;
	}*/
	Node_t* trace_node = i_node->left;

	if(!trace_node){
		throw Bad_Tree_Op("No Left Node!");
	}

	i_node->left = trace_node->right;
	if(trace_node->right){
		trace_node->right->parent = i_node;
	}
	trace_node->parent = i_node->parent;
	if(!i_node->parent){
		/*if(s_messages){
			M_output_stream << "New Root Node: " << trace_node->data << std::endl;
		}*/
		M_root = trace_node;
	}else if(i_node->is_right()){
		i_node->parent->right = trace_node;
	}else{
		i_node->parent->left = trace_node;
	}
	trace_node->right = i_node;
	i_node->parent = trace_node;
	if(s_messages){
		M_output_stream << "Rotation " << num_rotations << std::endl;
	}
	++num_rotations;
}

/*in order_walk*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
void RB_Tree<St, Key, Comp_t, Alloc_t>::in_order_walk(std::ostream& oos){
	size_t pos = 0;
	if(empty()){
		return;
	}
	if(M_root->left)
		in_order_walk(oos, M_root->left, pos);

	oos << M_root->data << ' ';
	++pos;
	if(pos == size()){
		oos << std::endl;
		return;
	}
	if(M_root->right)
		in_order_walk(oos, M_root->right, pos);
}

/*in order walk helper*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
void RB_Tree<St, Key, Comp_t, Alloc_t>::in_order_walk(std::ostream& oos, Node_t* i_node, size_t& ir_pos){
		if(i_node->left)
			in_order_walk(oos, i_node->left, ir_pos);
		oos << i_node->data << ' ';
		++ir_pos;
		if(ir_pos == size()){
			oos  <<std::endl;
			return;
		}
		if(i_node->right)
			in_order_walk(oos, i_node->right, ir_pos);
}

/*Num rotations*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
int RB_Tree<St, Key, Comp_t, Alloc_t>::num_rotations = 0;

/*Find*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::iterator RB_Tree<St, Key, Comp_t, Alloc_t>::find(const Key& irk_key){
	Node_t* found_node(find_node(irk_key));
	if(found_node){
		return iterator(find_node(irk_key));
	}else{
		return end();
	}
}


/*Find*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::const_iterator RB_Tree<St, Key, Comp_t, Alloc_t>::find(const Key& irk_key)const{
	Node_t* found_node(find_node(irk_key));
	if(found_node){
		return const_iterator(find_node(irk_key));
	}else{
		return end();
	}
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t* RB_Tree<St, Key, Comp_t, Alloc_t>
	::find_node(const Key& irk_key)const{
	Node_t* cur_node = M_root;
	while(cur_node && !equal(cur_node->data, irk_key)){
		if(M_comp(irk_key, cur_node->data)){
			cur_node = cur_node->left;
		}else{
			cur_node = cur_node->right;
		}
	}
	return cur_node;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::iterator RB_Tree<St, Key, Comp_t, Alloc_t>::lower_bound(const Key& irk_key){
	if(empty()){
		return end();
	}

	Node_t* cur_node = M_root;

	while(cur_node){
		if(M_comp(cur_node->data, irk_key)){
			cur_node = cur_node->right;
		}else if(M_comp(irk_key, cur_node->data)){
			cur_node = cur_node->left;
		}else{
			break;
		}
			
	}

	return iterator(cur_node);
}

/*Tree Search*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::iterator RB_Tree<St, Key, Comp_t, Alloc_t>::tree_search(Node_t* i_node, const Key& irk_key){
	if(!i_node || equal(i_node->data, irk_key)){
		return iterator(i_node);
	}
	if(M_comp(i_node->data, irk_key)){
		return tree_search(i_node->left, irk_key);
	}

	return tree_search(i_node->right, irk_key);
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
bool RB_Tree<St, Key, Comp_t, Alloc_t>::equal(const St& irk_data, const Key& irk_key)const{
	return (!M_comp(irk_data, irk_key) && !M_comp(irk_key, irk_data));
}

/*Minimum*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::iterator RB_Tree<St, Key, Comp_t, Alloc_t>::minimum(){
	if(empty()){
		return end();
	}
	return iterator(minimum(M_root));
}

/*Minimum*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::const_iterator RB_Tree<St, Key, Comp_t, Alloc_t>::minimum()const{
	if(empty()){
		return end();
	}
	return const_iterator(minimum(M_root));
}

/*Maximum*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::iterator RB_Tree<St, Key, Comp_t, Alloc_t>::maximum(){
	if(empty()){
		return end();
	}
	return iterator(maximum(M_root));
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t* RB_Tree<St, Key, Comp_t, Alloc_t>::minimum(Node_t* i_node){
	if(!i_node){
		return nullptr;
	}
	while( i_node->left){
		i_node = i_node->left;
	}

	return i_node;
}

template<typename St, typename Key, typename Comp_t, class Alloc_t>
typename RB_Tree<St, Key, Comp_t, Alloc_t>::Node_t* RB_Tree<St, Key, Comp_t, Alloc_t>::maximum(Node_t* i_node){
	if(!i_node){
		return nullptr;
	}
	while( i_node->right){
		i_node = i_node->right;
	}

	return i_node;
}


/*bool count(const Key&)const*/
template<typename St, typename Key, typename Comp_t, class Alloc_t>
bool RB_Tree<St, Key, Comp_t, Alloc_t>::count(const Key& irk_key)const{

	return end() != find(irk_key);
}

}


#endif