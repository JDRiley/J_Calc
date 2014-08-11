J_Calc
======
This is a calculator that allows for the using of variables. It is written in C++ and uses OpenGL glew and glfw to perform input/output.

Text library FreeType is used to get font bitmaps for displaying text. 

J_Calc_Data is the model.

J_Calc_Controller is the controller. 

J_Calc_View is the view.

ex_array - an implementation of a std::vector like class that also has extra space in the beginning and the end for fast insertion and removal at both ends of the array. Unlike deque this gaurantees continuous memory. 

j_function- a module for function and symbol abstraction. j_functions operate on double values. j_symbols operate on other symbols and jomike::Arguments classes and return j_values.

j_iterator - Header for commonly used iterator and iterator-wrappers used by the ex_array and j_tree, and j_map classes.

J_Math - Module for various mathematical operations. 

J_OpenGL - A module for interfacing with OpenGL commands.

J_String - A module for basic string functions

J_UI - Module which loads GUI elements to be modified by the program. 

J_Utile - A Module with various commonly used functions.

RB_Tree. A module which contains the headers for various Tree based classes. j_map which is analogous to std::map.

j_tree which is analagous to std::set. j_multi_map analogous to std::multi_map.
