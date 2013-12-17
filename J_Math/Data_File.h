#ifndef DATA_FILE_H
#define DATA_FILE_H

#include <math_base.h>

#include <J_Fwd_Decl.h>

#include <iosfwd>

#include <ex_array.h>


namespace jomike{

J_FWD_DECL(Data_File)

typedef Data_File Data_t;
class j_function;

class Data_File{
public:
	Data_File():M_data(2){}
	Data_File(std::istream&);
	Data_File(const Dbl_data_t& irk_x_vals, const Dbl_data_t& irk_y_vals){
		M_data.push_back(irk_x_vals);
		M_data.push_back(irk_y_vals);
	}
	Data_File(const Dbl_data_t&, const j_function&);
	void set_data(std::istream& ir_os){get_data_from_file(ir_os);}
	const Dbl_data_t& operator[](size_t i_index)const{return M_data[i_index];}
	Dbl_data_t& operator[](size_t i_index){return M_data[i_index];}
	void operator+=(Dbl_t);
	void operator-=(Dbl_t i_val){this->operator+=(-i_val);}
	void operator*=(Dbl_t);
	void operator/=(Dbl_t);
	int size()const{return static_cast<int>(M_data[0].size());}
	bool in_bounds(int i_index)const{
		return ((i_index >= 0)&&(i_index < size()));
	}

	void print(std::ostream& ir_os)const;
	void swap(Data_File& ir_src){M_data.swap(ir_src.M_data);}
private:
	ex_array<Dbl_data_t> M_data;
	void get_data_from_file(std::istream&);
};

}





#endif