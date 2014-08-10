#include "J_UI_Multi_String.h"
//
#include "../J_Font_Face.h"
//
#include <numeric>
//
#include <utility>

using std::accumulate; using std::back_inserter; using std::pair;
using std::string; using std::min; using std::max_element;

//
#include <algorithm>
//
#include <functional>

using std::mem_fn; using std::bind; using std::for_each; using std::transform;
using namespace std::placeholders;

namespace jomike{

J_UI_Multi_String::J_UI_Multi_String(){}

J_UI_Multi_String::J_UI_Multi_String(const J_UI_String& irk_string):M_strings(1, irk_string){}

J_UI_Multi_String& J_UI_Multi_String::operator=(J_UI_Multi_String&& irr_right){
	M_strings.swap(irr_right.M_strings);
	return *this;
}

J_UI_Multi_String::J_UI_Multi_String(J_UI_Multi_String&& irr_source){
	M_strings.swap(irr_source.M_strings);
}


J_UI_Multi_String::const_iterator J_UI_Multi_String::get_string_holding_index(j_size_t i_index)const{
	j_size_t cur_size = 0;
	auto string_it = M_strings.begin();

	assert(i_index <= size());
	assert(!M_strings.empty());

	auto string_end_it = M_strings.end();
	while((string_it != string_end_it) && ((cur_size + string_it->size()) <= i_index)){
		cur_size += string_it->size();
		++string_it;
	}
	return string_it;
}

J_UI_Multi_String::iterator J_UI_Multi_String::get_string_holding_index(j_size_t i_index){
	j_size_t cur_size = 0;
	auto string_it = M_strings.begin();

	assert(i_index <= size());
	assert(!M_strings.empty());

	auto string_end_it = M_strings.end();
	while((string_it != string_end_it) && ((cur_size + string_it->size()) <= i_index)){
		cur_size += string_it->size();
		++string_it;
	}


	return string_it;
}


j_size_t J_UI_Multi_String::size()const{
	return accumulate(M_strings.begin(), M_strings.end()
		, static_cast<j_size_t>(0), [](j_size_t i_val, const J_UI_String& yrk_string){
		return yrk_string.size() + i_val;
	});

}

bool J_UI_Multi_String::empty()const{
	return M_strings.empty();
}

J_UI_String::const_iterator J_UI_Multi_String::at_pos(j_size_t i_index)const{
	j_size_t cur_size = 0;
	auto string_it = M_strings.begin();

	assert(i_index < size());
	assert(!empty());

	auto string_end_it = M_strings.end();
	while((string_it != string_end_it) && ((cur_size + string_it->size()) <= i_index)){
		cur_size += string_it->size();
		++string_it;
	}

	return string_it->begin() + (i_index - cur_size);
}

J_UI_String::iterator J_UI_Multi_String::at_pos(j_size_t i_index){
	j_size_t cur_size = 0;
	auto string_it = M_strings.begin();

	assert(i_index <= size());
	assert(!M_strings.empty());

	auto string_end_it = M_strings.end();
	while((string_it != string_end_it) && ((cur_size + string_it->size()) <= i_index)){
		cur_size += string_it->size();
		++string_it;
	}

	return string_it->begin() + (i_index - cur_size);
}


pair<J_UI_String::const_iterator, J_UI_Multi_String::const_iterator> 
	J_UI_Multi_String::get_insert_pos(j_size_t i_index)const{
	j_size_t cur_size = 0;
	auto string_it = M_strings.begin();

	assert(i_index <= size());
	assert(!M_strings.empty());

	auto string_end_it = M_strings.end();
	while((string_it != string_end_it) && ((cur_size + string_it->size()) < i_index)){
		cur_size += string_it->size();
		++string_it;
	}

	return make_pair(string_it->begin() + (i_index - cur_size), string_it);
}


pair<J_UI_String::iterator, J_UI_Multi_String::iterator>
J_UI_Multi_String::get_insert_pos(j_size_t i_index){
	j_size_t cur_size = 0;
	auto string_it = M_strings.begin();

	assert(i_index <= size());
	assert(!M_strings.empty());

	auto string_end_it = M_strings.end();
	while((string_it != string_end_it) && ((cur_size + string_it->size()) < i_index)){
		cur_size += string_it->size();
		++string_it;
	}

	return make_pair(string_it->begin() + (i_index - cur_size), string_it);
}


J_UI_Multi_String::j_ui_char_iterator J_UI_Multi_String::insert
	(j_size_t i_pos, J_UI_Char i_char){
	assert(!M_strings.empty());


	pair<J_UI_String::iterator, J_UI_Multi_String::iterator>
			insert_pos = get_insert_pos(i_pos);

	auto string_it = get_string_holding_index(i_pos);
	insert_pos.second->insert(insert_pos.first, i_char);

	return j_ui_char_iterator(i_pos + 1 , *this);
}

void J_UI_Multi_String::insert(j_size_t i_pos, const J_UI_String& irk_string){
	
	if(M_strings.empty()){
		M_strings.push_back(irk_string);
	}
	auto insert_pos = get_insert_pos(i_pos);

	auto indices = get_string_indices(insert_pos.second);

	if(i_pos == indices.first){
		M_strings.insert(insert_pos.second, irk_string);
	} else if(i_pos == indices.second){
		M_strings.insert(insert_pos.second + 1, irk_string);
	} else{
		J_UI_String remaining_string = insert_pos.second->substr(i_pos - indices.first);
		insert_pos.second->resize(i_pos - indices.first);
		auto new_insert_pos = M_strings.insert(insert_pos.second + 1, irk_string);
		M_strings.insert(new_insert_pos + 1, remaining_string);
	}


}

J_UI_Multi_String::iterator 
	J_UI_Multi_String::insert(j_size_t i_pos, const J_UI_Multi_String& irk_string){
	
	if(M_strings.empty()){
		assert(i_pos == 0);
		M_strings = irk_string.M_strings;
		return begin();
	}

	auto insert_pos_pair = get_insert_pos(i_pos);
	

	auto indices = get_string_indices(insert_pos_pair.second);

	auto insert_pos = insert_pos_pair.second + 1;
	J_UI_Multi_String::iterator return_iterator;

	if(i_pos == indices.second){
		return insert(insert_pos, irk_string);

	} else if(i_pos == indices.first){

		return insert(insert_pos - 1, irk_string);
	} else{
		J_UI_String remaining_string = insert_pos->substr(i_pos - indices.first);
		insert_pos->resize(i_pos - indices.first);
		auto new_insert_pos = insert(insert_pos, irk_string);
		insert(new_insert_pos + irk_string.num_strings() , remaining_string);
		return new_insert_pos;
	}

	
}

J_UI_Multi_String::iterator
	J_UI_Multi_String::insert_into(const_iterator i_ins_pos, const J_UI_String& irk_string){
		J_UI_Multi_String::iterator pos = begin() + (i_ins_pos - begin());
	if((pos != end()) && pos->is_same_type(irk_string)){

		pos = M_strings.insert(pos, irk_string);
		auto delete_pos = pos + 1;
		assert(end() != delete_pos);
		assert(pos->is_same_type(*delete_pos));

		pos->append(*delete_pos);
		pos = M_strings.erase(delete_pos);

	} else{
		pos = ++M_strings.insert(pos, irk_string);
	}
	return pos;
};


J_UI_Multi_String::iterator  J_UI_Multi_String
	::insert(const_iterator i_pos, const J_UI_Multi_String& irk_string){
		assert(irk_string.no_same_adjacent_types());
		assert(no_same_adjacent_types());
		J_UI_Multi_String::iterator pos = begin() + (i_pos - begin());
		J_UI_Multi_String::iterator return_iterator;
		

		
		if(!irk_string.num_strings()){
			return pos;
		}

		if((i_pos != begin()) && (pos -1)->is_same_type(irk_string.front())){
			return_iterator = pos - 1;

			(pos - 1)->append(irk_string.front());

		} else {
			pos = insert_into(pos, irk_string.front());
			return_iterator = (pos - 1);
		}


		if(irk_string.num_strings() == 1){
			return return_iterator;
		}

		auto src_string_pos = irk_string.begin() + 1;
		auto end_string_pos = irk_string.end();
		while(src_string_pos != end_string_pos){
			pos = insert_into(pos, *src_string_pos++);
		}
		return pos - 1;
}

J_UI_Multi_String::iterator J_UI_Multi_String::insert(
	const_iterator i_pos, const J_UI_String& irk_string){
	auto pos = begin() + (i_pos - begin());
	J_UI_Multi_String::iterator return_iterator;
	if((pos != begin()) && (pos - 1)->is_same_type(irk_string)){
		return_iterator = pos - 1;

		(pos - 1)->append(irk_string);

	} else {
		pos = insert_into(pos, irk_string);
		return_iterator = (pos - 1);
	}
	return return_iterator;
}

template<typename Value_Type>
J_UI_Multi_String::multi_string_iterator<Value_Type>::multi_string_iterator(j_size_t i_pos
									, const J_UI_Multi_String& irk_cont): M_pos(i_pos)
									, M_cont(irk_cont){}

void J_UI_Multi_String::erase(j_size_t i_pos, j_size_t i_size){
	
	J_UI_String::iterator char_pos = at_pos(i_pos);

	auto string_it = get_string_holding_index(i_pos);
	assert(string_it != end());
	j_size_t total_erased = 0;
	while(total_erased < i_size){
		j_size_t offset = char_pos - string_it->begin();

		j_size_t amount_to_erase = std::min(i_size - total_erased, string_it->size() - offset);
		

		string_it->erase(char_pos, char_pos + amount_to_erase);
		total_erased += amount_to_erase;
		++string_it;
		if(string_it != end()){
			char_pos = string_it->begin();
		}
	}
	

}

void J_UI_Multi_String::erase(j_size_t i_pos){
	J_UI_String::iterator char_pos = at_pos(i_pos);

	auto string_it = get_string_holding_index(i_pos);
	string_it->erase(char_pos);
}

void J_UI_Multi_String::resize(j_size_t i_size){
	if(size() == i_size){
		return;
	}
	if(size() > i_size){
		auto insert_pos = get_insert_pos(i_size);
		M_strings.erase(insert_pos.second+1, M_strings.end());
		insert_pos.second->erase(insert_pos.first, insert_pos.second->end());
	}else{
		auto string_it = get_string_holding_index(i_size);
		M_strings.erase(string_it+1);
		auto& back_string = M_strings.back();
		j_size_t amount_to_delete = (size() - i_size);
		back_string.erase(back_string.end() - amount_to_delete
			, back_string.end());
	}
}



J_UI_Char& J_UI_Multi_String::operator[](j_size_t i_index){
	return *at_pos(i_index);
}

const J_UI_Char& J_UI_Multi_String::operator[](j_size_t i_index)const{
	return *at_pos(i_index);
}

J_UI_String& J_UI_Multi_String::back(){
	return M_strings.back();
}

const J_UI_String& J_UI_Multi_String::back()const{
	return M_strings.back();
}

void J_UI_Multi_String::push_back(const J_UI_Char& irk_char){
	assert(M_strings.size());
	M_strings.back().push_back(irk_char);
}

J_UI_Multi_String J_UI_Multi_String::substr(j_size_t i_pos, j_size_t i_size)const{
	auto string_it = get_string_holding_index(i_pos);
	if(end() == string_it){
		return num_strings() 
			? J_UI_String(M_strings.back().font_face(), M_strings.back().color()) 
			: J_UI_Multi_String();
	}
	auto char_pos = at_pos(i_pos);
	J_UI_String part_string = string_it->substr(char_pos - string_it->begin());
	J_UI_Multi_String string_to_return;
	if(!part_string.empty()){
		string_to_return.push_back(part_string);
	}


	copy(string_it + 1, end(), back_inserter(string_to_return));
	string_to_return.resize(min(string_to_return.size(), i_size));
	return string_to_return;
}

void J_UI_Multi_String::push_back(const J_UI_String& irk_string){
	if(!M_strings.empty() && M_strings.back().is_same_type(irk_string)){
		M_strings.back().append(irk_string);
	}else{
		M_strings.push_back(irk_string);
	}

}
J_UI_Multi_String::iterator J_UI_Multi_String::begin(){return M_strings.begin();}
J_UI_Multi_String::const_iterator J_UI_Multi_String::begin()const{return M_strings.begin();}

J_UI_Multi_String::const_iterator J_UI_Multi_String::cbegin()const{return M_strings.begin();}

J_UI_Multi_String::iterator J_UI_Multi_String::end(){return M_strings.end();}
J_UI_Multi_String::const_iterator J_UI_Multi_String::end()const{return M_strings.end();}

J_UI_String& J_UI_Multi_String::front(){
	return M_strings.front();
}

const J_UI_String& J_UI_Multi_String::front()const{
	return M_strings.front();
}

void J_UI_Multi_String::clear(){
	M_strings.clear();
}

J_UI_String to_display_string(const string& irk_string){
	return J_UI_String(irk_string.begin(), irk_string.end());
}

void J_UI_String::white_space_normalize(){
	bool copy_whitespace = false;
	J_UI_String normalized_str(font_face(), color());


	std::for_each(begin(), end()
		, [&normalized_str, &copy_whitespace](const J_UI_Char& ik_char){

		if(!ik_char.is_space()){
			copy_whitespace = true;
			normalized_str.push_back(ik_char);
		} else if(copy_whitespace){
			copy_whitespace = false;
			normalized_str.push_back(' ');
		}
	});

	if(!normalized_str.empty() && normalized_str.back().is_space()){
		normalized_str.pop_back();
	}
	swap(normalized_str);
}

J_UI_String& J_UI_String::operator+=(const char* irk_string){
	while(char c = *irk_string++){
		push_back(c);
	}
	return *this;
}

ex_array<Bitmap_Metrics*> J_UI_String::metrics_array()const{
	ex_array<Bitmap_Metrics*> the_metrics_array;
	for(auto f_char : *this){
		the_metrics_array.push_back(&font_face()->bitmap_metric(f_char.charcode()));
	}
	return the_metrics_array;
}

ex_array<const j_ubyte*> J_UI_String::bitmap_data_array()const{
	ex_array<const j_ubyte*> the_bitmap_data_array;
	for(auto f_char : *this){
		the_bitmap_data_array.emplace_back(font_face()->get_data(f_char.charcode()));
	}
	return the_bitmap_data_array;
}


j_dbl read_double_and_advance(J_UI_String::const_iterator* i_string_pos, ptrdiff_t i_max_len){
	auto string_pos = *i_string_pos;
	auto end_pos = *i_string_pos + i_max_len;

	string new_string;
	bool decimals_found = false;
	bool is_first = true;
	for(; string_pos != end_pos; ++string_pos){
		new_string.push_back(safe_char_cast(string_pos->charcode()));

		if(string_pos->is_number() || (is_first && (*string_pos == '-'))){
			is_first = false;
			continue;
		}
		if(!decimals_found && ('.' == string_pos->charcode())){
			decimals_found = true;
			continue;
		}
		new_string.pop_back();

		break;
	}
	char* end_char_pos;
	Dbl_t return_val = strtod(new_string.c_str(), &end_char_pos);
	*i_string_pos += safe_int_cast(end_char_pos - &new_string[0]);

	return return_val;
}

string J_UI_Multi_String::std_str()const{
	return accumulate(begin(), end(), string()
		, [](const string& irk_return_string, const J_UI_String& irk_string){
		return irk_return_string + irk_string.std_str();
	});
}

J_UI_Multi_String J_UI_Multi_String::operator+(const char* irk_string)const{
	assert(!M_strings.empty());
	J_UI_Multi_String new_string(*this);
	new_string.M_strings.back() += irk_string;
	return new_string;
}

J_UI_Multi_String J_UI_Multi_String::operator+(const J_UI_Multi_String& irk_string)const{
	J_UI_Multi_String new_string(*this);
	for(const auto& f_string : irk_string){
		new_string.push_back(f_string);
	}
	return new_string;
}

int J_UI_Multi_String::new_line_size()const{
	if(M_strings.empty()){
		return 0;
	}
	return max_element(begin(), end()
					   , [](const J_UI_String& yrk_left, const J_UI_String& yrk_right){
		return (yrk_left.font_face()->new_line_size()
				< yrk_right.font_face()->new_line_size());
	})->font_face()->new_line_size();
}

const Bitmap_Metrics& J_UI_Multi_String::bitmap_metric(j_size_t i_index)const{
	auto found_pos = get_insert_pos(i_index);
	return found_pos.second->font_face()->bitmap_metric(found_pos.first->charcode());
}

Pen_Pos_t J_UI_Multi_String::get_string_indices(const_iterator i_pos)const{
	int index = 0;
	auto string_pos = begin();

	while(i_pos != string_pos){
		index += safe_int_cast(string_pos->size());
		++string_pos;
	}

	int str_size = i_pos == end() ? 0 : safe_int_cast(index + i_pos->size());
	return Pen_Pos_t(index, str_size);
}

void J_UI_Multi_String::set_color(const J_UI_Color& irk_color){
	M_strings.apply(bind(&J_UI_String::set_color, _1, std::cref(irk_color)));
}

void J_UI_Multi_String::push_front(const J_UI_Char& i_char){
	assert(!M_strings.empty());
	M_strings.front().push_front(i_char);
}

jomike::j_size_t J_UI_Multi_String::num_strings()const{
	return M_strings.size();
}

bool J_UI_Multi_String::no_same_adjacent_types()const{
	if(num_strings() <= 1){
		return true;
	}

	auto pos = begin();
	auto end_pos = end() - 1;
	
	while(pos != end_pos){
		if(pos->is_same_type(*(pos+1))){
			return false;
		}
		++pos;
	}
	return true;

}

void J_UI_Multi_String::pop_back(){
	M_strings.pop_back();
}

std::ostream& operator<<(std::ostream& ir_os, const J_UI_Multi_String& irk_string){
	for(const auto& f_string : irk_string){
		ir_os << f_string;
	}
	return ir_os;
}


}



