#ifndef HW4_H_
#define HW4_H_

// ! Place all includes here as I believe that redoing the include in the main or HW4 causes problems
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <regex>
#include <sstream>
#include <string>
#include <algorithm>
#include <queue>


//* created a template class so we dont have to recode for different maps
//* Afsheen Hatami - classTemplate and functions directly below it*/
template<class t1,class t2>
class classTemplate{
    
    private:
        std::map<t1,t2> _dummyMap;
        std::string _expression;
        std::string _var;

    public:
        
        //* use for inserting into non list maps         
        void insert_into_map(t1 _key,t2 _val){
            _dummyMap[_key] = _val;
        }

        //* checks if a key is in the map
        bool is_in_map(t1 _key){
            return (_dummyMap.find(_key) != _dummyMap.end()) ? true : false;
        }
        
        //* returns a single value string/int 
        t2 get_value(t1 _key){
            if(!is_in_map(_key)){
                std::cout << "The key, " << _key << ", is not in the map! (get_value function)\n"; 
                std::cout << "Exiting Program...\n";
                exit(1);
            }
            return _dummyMap[_key];
        }
        //* returns a slice given the start and stop conditions
        t2 get_slice(t1 _key,const int _start, const int _stop){
            if(!is_in_map(_key)){
                std::cout << "The key, " <<  _key << ", is not in the map! (get_slice function)\n";
                std::cout << "Exiting program...\n";
                exit(1);
            }

            int _iter = 0;
            std::string _slice = "";
            std::stringstream ss(_dummyMap[_key]);
            std::string _catchWord;

            while(ss >> _catchWord){
                if(_iter >= _start && _iter < _stop && _slice.empty()){
                    _slice += _catchWord;
                }
                else if(_iter >= _start && _iter < _stop){
                    _slice += " " + _catchWord;

                }
                _iter++;
            }
            return _slice;
        }

        t2 get_index(t1 _key,const int _index_val){
            if(!is_in_map(_key)){
                std::cout << "The key, " <<  _key << ", is not in the map! (get_index function)\n";
                std::cout << "Exiting program...\n";
                exit(1);
            }

            int _iter = 0;
            std::string _element;
            std::stringstream ss(_dummyMap[_key]);
            std::string _catch_word;
        
            while(ss >> _catch_word){
                if(_iter == _index_val){
                    _element = _catch_word;
                    break;
                }
                _iter++;
            }

            return _element;

        }



};

int type_of_operation(const std::string); // checks for type of operations
std::string process_operation_line(std::string); // pre-processing the line before we process the operands 
std::string process_operands(std::string,classTemplate<std::string,int> &, classTemplate<std::string,std::string> &, classTemplate<std::string,std::string> &,
                        classTemplate<std::string,std::string> &); // processes operands and returns newly formed list
void insert_after_operation(std::string,classTemplate<std::string,int> &, classTemplate<std::string,std::string> &, classTemplate<std::string,std::string> &,
                        classTemplate<std::string,std::string> &); // is a fork of charlies implementation in case 3 for assigning variables their values/datatypes
std::string process_print(std::string,classTemplate<std::string,int> &, classTemplate<std::string,std::string> &, classTemplate<std::string,std::string> &,
                        classTemplate<std::string,std::string> &);
std::string string_to_list(std::string);
void open_out_file(std::string,std::fstream&);
void open_read_file(std::string,std::fstream&);

/* Charlie Grealy */
std::string get_variable_name(std::string); //Returns the variable name
std::string get_datatype(std::string);//Returns the data type of the expression. Var = 1 would return integer
std::string get_string_value(std::string); //Returns the string
int get_integer_value(std::string); //Returns the integer
std::string get_var_name_in_expression(std::string);//Returns the name of the variable in order to search for it in the map
std::string get_list_values(std::string);//Returns the values that are in the list
void insert_to_list_map(classTemplate<std::string,int>, classTemplate<std::string,std::string> &, std::string, std::string); //Converts int to string
void help_insert_to_maps(classTemplate<std::string,int> &, classTemplate<std::string,std::string> &, classTemplate<std::string,std::string> &,
                        classTemplate<std::string,std::string> &, std::string, std::string, std::string); //Inserts everything into maps, to save space I wrote this in here.
std::string parse_compared_variable(std::string); //parses if statement for the variable in comparison. w > 0 for example would return w
std::string parse_operand_comparison(std::string); //returns the value being compared to. if w<0 would return 0
std::string parse_operand_comparison_strings(std::string); //returns value being compared to when dealing with strings
void evaluate_comparison(classTemplate<std::string,std::string> &, classTemplate<std::string,std::string> &,
    classTemplate<std::string,int> &, std::string, std::string, std::string); //evaluates the comparison for >, <, ==, <=, >= and inserts into map corresponding values
void evaluate_if_else_(classTemplate<std::string,std::string> &, classTemplate<std::string,std::string> &,
    classTemplate<std::string,int> &, std::string); //evaluates the if/else statement
std::string parse_if_else_values(std::string);
#endif // HW4_H_