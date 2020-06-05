#include "HW4.h"

// * takes in line and looks for key words/characters and returns an int for the switch statement
int type_of_operation(const std::string line){
    if(line.find('+') != std::string::npos || line.find('*') != std::string::npos){
        return 0;
    }
    else if(line.find("print") != std::string::npos){
        return 1;
    }
    else if(line.find("if") != std::string::npos && line.find("else") != std::string::npos){
        return 2;
    }
    else{
        return 3; // * just a regular assignment
    }
}

std::string process_operation_line(std::string _raw_line){
    std::string _raw_operands = _raw_line.substr(_raw_line.find('=')+1,_raw_line.length());
    std::replace(_raw_operands.begin(),_raw_operands.end(),'+',' ');
    _raw_operands.erase(remove(_raw_operands.begin(),_raw_operands.end(),'('),_raw_operands.end());
    _raw_operands.erase(remove(_raw_operands.begin(),_raw_operands.end(),')'),_raw_operands.end());
    return _raw_operands;
}

//* goes through and concatenates lists and returns the newly formed list
//* Input: l1=["str1"]+["str2"]+["str3]
//* Output: ["str1","str2","str3"] <- (exact form)
//* after ouput create the newly formed line -> l1=["str1","str2","str3"]
std::string process_operands(std::string _operands,classTemplate<std::string,int> &_intVars, classTemplate<std::string,std::string> &_listVars, 
                        classTemplate<std::string,std::string> &_strVars, classTemplate<std::string,std::string> &_dataTypes){
    std::stringstream ss(_operands);
    std::string _final_value = "";
    std::string _word;
    std::string _inside_brackets;
    std::string _data_types = "";
    std::string _map_data_type;
    std::string _initial_data_type;
    while(ss>>_word){
        _inside_brackets = get_list_values(_word);
        if(_dataTypes.is_in_map(_inside_brackets)){
            // which map is it in
            _map_data_type = _dataTypes.get_value(_inside_brackets);
            
            if(_data_types.empty()){
                if(_map_data_type.find("string") != std::string::npos){
                    _data_types += "string";
                    _initial_data_type = "string";
                }
                else if(_map_data_type.find("integer") != std::string::npos){
                    _data_types += "integer";
                    _initial_data_type = "integer";
                }
            }
            else{

                if(_map_data_type.find("string") != std::string::npos){
                    _data_types = _data_types + " " + "string";
                }
                else if(_map_data_type.find("integer") != std::string::npos){
                    _data_types = _data_types + " " + "integer";
                }
            }  

            if(_map_data_type == "string"){
                if(_final_value.empty()){
                    _final_value += _strVars.get_value(_inside_brackets);
                }
                else{
                    _final_value += " " + _strVars.get_value(_inside_brackets);
                }
            }
            else if(_map_data_type == "integer"){
                if(_final_value.empty()){
                    _final_value += std::to_string(_intVars.get_value(_inside_brackets));
                }
                else{
                    _final_value += " " + std::to_string(_intVars.get_value(_inside_brackets));
                }

            }
            else if(_map_data_type.find("list") != std::string::npos){
                if(_final_value.empty()){
                    _final_value += _listVars.get_value(_inside_brackets);
                }
                else{
                    _final_value += " " + _listVars.get_value(_inside_brackets);
                }

            }
        }
        else{
            if(_data_types.empty()){
                _data_types += get_datatype(_inside_brackets);
                _initial_data_type = get_datatype(_inside_brackets);
            }
            else{
                _data_types += " " + get_datatype(_inside_brackets);
            }  
            
            if(_final_value.empty()){
                _final_value += _inside_brackets;
            }
            else{
                _final_value += " " + _inside_brackets;
            }
        }
    }


    std::stringstream ss2(_data_types);
    std::string _catch_data_type;

    while(ss2 >> _catch_data_type){
        if(_initial_data_type.find("integer") != std::string::npos){
            if(_catch_data_type.find("integer") == std::string::npos){
                return "";
            }
        }
        else if(_initial_data_type.find("string") != std::string::npos){
            if(_catch_data_type.find("string") == std::string::npos){
                return "";
            }
        }
    }

   std::string _list_notation = string_to_list(_final_value); 

   return _list_notation;
}

//* Takes in the newly formed line from process_operands -> l1=["str1","str2","str3"]
//* Is a fork of charlies help_insert_into_maps functions
void insert_after_operation(std::string _currLine,classTemplate<std::string,int> &_intVars, classTemplate<std::string,std::string> &_listVars, 
                        classTemplate<std::string,std::string> &_strVars, classTemplate<std::string,std::string> &_dataTypes){

                std::string _var_name;
                std::string _data_type;
                
                _var_name = get_variable_name(_currLine);
                _data_type = get_datatype(_currLine);
                
                if(_dataTypes.is_in_map(_var_name)){
                    if(_dataTypes.get_value(_var_name) != _data_type){
                        std::cout << "The variable is being reassigned as a different data type" << std::endl;
                        std::cout << "May need to output #Warning" << std::endl;
                    }
                }
                _dataTypes.insert_into_map(_var_name,_data_type);
                help_insert_to_maps(_intVars, _listVars, _strVars, _dataTypes, _var_name, _data_type, _currLine);
                //std::cout << "We succesfuly added variable: " << _var_name << " into the map. The data type is currently: " << _dataTypes.get_value(_var_name) << std::endl;
                
                if(_listVars.is_in_map(_var_name)){
                    //std::cout << "The items in the List map are: " << _listVars.get_value(_var_name) << "\n" <<std::endl;
                }

}


std::string process_print(std::string _currLine,classTemplate<std::string,int> &_intVars, classTemplate<std::string,std::string> &_listVars, 
                        classTemplate<std::string,std::string> &_strVars, classTemplate<std::string,std::string> &_dataTypes){
    

    unsigned left = _currLine.find('(');
    unsigned right = _currLine.find(')');
    std::string _print_argument = _currLine.substr(left+1,right-left-1);
    
    std::string _print_output = ">>>";
    std::string _index_string;
    std::string _start_index_string;
    std::string _end_index_string;
    std::string _var_name;
    std::string _type_of_map;

    int _start_index_int;
    int _end_index_int;
    int _index_int;

    if(_print_argument.find('[') != std::string::npos){
        left = _print_argument.find('[');
        right = _print_argument.find(']');
        _index_string = _print_argument.substr(left+1,right-left-1);

        if(_index_string.find(':') != std::string::npos){
            
            if(_index_string.length() == 3){
                
                _start_index_int = std::stoi(_index_string.substr(0,_index_string.find(':')));
                _end_index_int = std::stoi(_index_string.substr(_index_string.find(':')+1,_index_string.length()));
                _var_name = _print_argument.substr(0,_print_argument.find('['));

                _print_output += string_to_list(_listVars.get_slice(_var_name,_start_index_int,_end_index_int));

            }
            else if(_index_string.length() == 2){
                if(_index_string[0] == ':'){
                    _start_index_int = 0;
                    _end_index_int = _index_string[1] - '0';
                    _var_name = _print_argument.substr(0,_print_argument.find('['));
                    
                    _print_output += string_to_list(_listVars.get_slice(_var_name,_start_index_int,_end_index_int));
                }
                else{
                    _start_index_int = _index_string[0] - '0';
                    _end_index_int = 50; //! arbitrary number
                    _var_name = _print_argument.substr(0,_print_argument.find('['));

                    _print_output += string_to_list(_listVars.get_slice(_var_name,_start_index_int,_end_index_int));

                }
            }
        }
        else{
            left = _print_argument.find('[');
            right = _print_argument.find(']');
            _index_string = _print_argument.substr(left+1,right-left-1);
            _index_int = std::stoi(_index_string);
            _print_argument = _print_argument.substr(0,_print_argument.find('['));

            _print_output += _listVars.get_index(_print_argument,_index_int);
        }
    }
    else if(_dataTypes.is_in_map(_print_argument)){
        _type_of_map = _dataTypes.get_value(_print_argument);

        if(_type_of_map.find("list") != std::string::npos){
            _print_output += string_to_list(_listVars.get_value(_print_argument));
        }
        else if(_type_of_map.find("string") != std::string::npos){
            _print_output += _strVars.get_value(_print_argument);
        }
        else if(_type_of_map.find("integer") != std::string::npos){
            //_print_output += std::to_string(_intVars.get_value(_print_argument));
        }
    }
    else{
        _print_output += _print_argument;
    }

    if(_print_output == ">>>"){
        return "";
    }

    return _print_output;
}

std::string string_to_list(std::string string_of_values){
    std::string _list_notation = "[";

    std::stringstream ss(string_of_values);
    std::string _catch_val;

    while(ss >> _catch_val){
        _list_notation += _catch_val + ",";
    }

    int _last_index = _list_notation.length()-1;

    _list_notation[_last_index] = ']';

    return _list_notation;
}

void open_read_file(std::string arg_var,std::fstream &my_file){
    std::string delimiter = "=";
    std::string filename = arg_var.substr(arg_var.find(delimiter)+1,arg_var.length()-1);
    my_file.open(filename,std::ios::in);
    
    if(!my_file.is_open()){
        std::cout << "read in file not found!\n";
        exit(1);
    }
}
void open_out_file(std::string arg_var,std::fstream &out_file){
    std::string delimiter = "=";
    std::string filename = arg_var.substr(arg_var.find(delimiter)+1,arg_var.length()-1);
    filename = filename.substr(0,filename.find('.'));
    filename = filename + ".out";
    out_file.open(filename,std::ios::out | std::ios::trunc);
    
    if(!out_file.is_open()){
        std::cout << "outfile not opened!\n";
        exit(1);
    }
}







//Returns the variable name
std::string get_variable_name(std::string _curr_line){
    _curr_line = _curr_line.substr(0, _curr_line.find("="));
    _curr_line.erase(remove_if(_curr_line.begin(), _curr_line.end(), ::isspace), _curr_line.end());
    return _curr_line;
}

//Returns the data type of the expression. Var = 1 would return integer
std::string get_datatype(std::string _curr_line){
    std::string _data_type = _curr_line.substr(_curr_line.find("=")+1, _curr_line.length());
    if(_data_type.find('[') != std::string::npos){
        if(_data_type.find("\"") != std::string::npos || _data_type.find('\'') != std::string::npos) return "list string";
        else if(_data_type.find_first_of("0123456789") != std::string::npos && (_data_type.find("\"") == std::string::npos || _data_type.find('\'') == std::string::npos))return "list integer"; //if finds a number and there is no " then it's an integer.. in case we have "this 3412 kind of string"
        else return "list variable"; //the list contains a variable that should be another list else error
    }
    else if(_data_type.find('\"') != std::string::npos || _data_type.find('\'') != std::string::npos){ //we have var = "string"
        return "string";
    }
    else if(_data_type.find_first_of("0123456789") != std::string::npos && (_data_type.find_first_of("\"") == std::string::npos || _data_type.find('\'') == std::string::npos)){
        return "integer";
    }
    else return "variable"; //trying to assign a variable to a variable, ie var = var
return "Error, data input is not an integer, string, list, or variable";
}

//Returns the string
std::string get_string_value(std::string _datavalue){
    std::string _data_ = _datavalue.substr(_datavalue.find('\"'), _datavalue.length());
    return _data_;
}

//Returns the integer
int get_integer_value(std::string _datavalue){
    std::string _data_ = _datavalue.substr(_datavalue.find('=')+1, _datavalue.length());
    int _data_integer = stoi(_data_);
    return _data_integer;
}

//Returns the name of the variable in order to search for it in the map
std::string get_var_name_in_expression(std::string _curr_line_){
    std::string varName = _curr_line_.substr(_curr_line_.find('=')+1, _curr_line_.length());
    if(varName.find('[') != std::string::npos){//the variable is enclosed in a list
        unsigned begin = varName.find_first_of("[");
        unsigned end = varName.find_first_of("]");
        varName = varName.substr(begin+1, end-begin+1);
    }
    return varName;
}

//Returns the values that are in the list
//! NOTE THAT I HAD TO MODIFY THIS SINCE THIS FUNCTION DID NOT ACCOUNT FOR 
//! SCENARIOS SUCH AS l2=['s1','s2','s3'] WHICH IS CONFIRMED TO BE POSSIBLE
std::string get_list_values(std::string _curr_line_){
    std::string list_value;
    unsigned begin = _curr_line_.find_first_of("[");
    unsigned end = _curr_line_.find_first_of("]");
    list_value = _curr_line_.substr(begin+1, end-begin-1);
    
    //! MODIFICATION
    if(list_value.find(',') != std::string::npos){
        std::replace(list_value.begin(),list_value.end(),',',' '); 
    }
    
    return list_value;
}

//I used stringstream cause stoi function was giving errors, didn't want to risk anything. Converts int to string
// TODO: LOOK INTO WHAT THIS FUNCTION IS DOING
void insert_to_list_map(classTemplate<std::string,int> _intVars, classTemplate<std::string,std::string> &_listVars, std::string _currLine, std::string _var_name){
    std::string list_var;
    std::stringstream ss;
    ss << _intVars.get_value(get_list_values(_currLine));
    list_var = ss.str();
    _listVars.insert_into_map(_var_name, list_var);
}

//TODO: LOOK INTO WHAT THIS FUNCTION IS DOING
//Inserts everything into maps to save space I wrote this in here.
void help_insert_to_maps(classTemplate<std::string,int> &_intVars, classTemplate<std::string,std::string> &_listVars, 
                        classTemplate<std::string,std::string> &_strVars, classTemplate<std::string,std::string> &_dataTypes,
                        std::string _var_name, std::string datatype, std::string _currLine){
    if(datatype == "string"){
        _strVars.insert_into_map(_var_name, get_string_value(_currLine));
    }
    else if(datatype == "integer"){
        _intVars.insert_into_map(_var_name, get_integer_value(_currLine));
    }
    else if(datatype == "list string" || datatype == "list integer"){
        _listVars.insert_into_map(_var_name, get_list_values(_currLine));
    }
    else if(datatype == "list variable"){
        if(_dataTypes.get_value(get_list_values(_currLine)) == "integer"){
            insert_to_list_map(_intVars, _listVars, _currLine, _var_name); //function adds integer value to list map, but converts int to string (main reason for the function)
            _dataTypes.insert_into_map(_var_name, "list integer");
        }
        else{
            _listVars.insert_into_map(_var_name, _strVars.get_value(get_list_values(_currLine)));
            _dataTypes.insert_into_map(_var_name, "list string");
        }
    }
    else if(datatype == "variable"){
        _currLine = _currLine.substr(_currLine.find('=')+1, _currLine.length());
        _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
        if(_intVars.is_in_map(_currLine)){
            _intVars.insert_into_map(_var_name, _intVars.get_value(_currLine));
            _dataTypes.insert_into_map(_var_name, "integer");
        }
        else{
            _strVars.insert_into_map(_var_name, _strVars.get_value(_currLine));
            _dataTypes.insert_into_map(_var_name, "string");
        }
    }
}

//Returns variable that is being compared to. In y=0 if w<0 else -1 would return w
std::string parse_compared_variable(std::string _currLine){
    if(_currLine.find('>') != std::string::npos && _currLine.find(">=") == std::string::npos){
        if(_currLine.find("\'") == std::string::npos && _currLine.find("\"") == std::string::npos){
            _currLine = _currLine.substr(_currLine.find("if")+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find(">"));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
            return _currLine;
        }
        else{
            _currLine = _currLine.substr(_currLine.find("if")+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('>'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
            return _currLine;
        }
    }
    else if(_currLine.find('<') != std::string::npos && _currLine.find("<=") == std::string::npos){
         if(_currLine.find("\'") == std::string::npos && _currLine.find("\"") == std::string::npos){
            _currLine = _currLine.substr(_currLine.find("if")+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('<'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
            //std::cout << "NEW CUR LINE IS____" << _currLine << std::endl;
            return _currLine;
        }
        else{
            _currLine = _currLine.substr(_currLine.find("if")+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('<'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
            return _currLine;
        }
    }
    else if(_currLine.find("==") != std::string::npos){
         if(_currLine.find("\'") == std::string::npos && _currLine.find("\"") == std::string::npos){
            _currLine = _currLine.substr(_currLine.find("if")+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('='));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
           // std::cout << "current line is " << _currLine << std::endl;
            return _currLine;
        }
        else{
            _currLine = _currLine.substr(_currLine.find("if")+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('='));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
            return _currLine;
        }
    }
    else if(_currLine.find(">=") != std::string::npos){
         if(_currLine.find("\'") == std::string::npos && _currLine.find("\"") == std::string::npos){
            _currLine = _currLine.substr(_currLine.find("if")+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('>'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
           // std::cout << "NEW CUR LINE IS____" << _currLine << std::endl;
            return _currLine;
        }
        else{
            _currLine = _currLine.substr(_currLine.find("if")+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('>'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
           // std::cout << "NEW CUR LINE IS____" << _currLine << std::endl;
            return _currLine;
        }
    }
    else if(_currLine.find("<=") != std::string::npos){
        if(_currLine.find("\'") == std::string::npos && _currLine.find("\"") == std::string::npos){
            _currLine = _currLine.substr(_currLine.find("if")+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('<'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
           // std::cout << "NEW CUR LINE IS____" << _currLine << std::endl;
            return _currLine;
        }
        else{
            _currLine = _currLine.substr(_currLine.find("if")+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('<'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
            //std::cout << "NEW CUR LINE IS____" << _currLine << std::endl;
            return _currLine;
        }
    }
    return "No variable";

}

std::string parse_operand_comparison(std::string _currLine){
    if(_currLine.find('>') != std::string::npos && _currLine.find(">=") == std::string::npos){
        if(_currLine.find("\'") == std::string::npos && _currLine.find("\"") == std::string::npos){
            _currLine = _currLine.substr(_currLine.find('>')+1, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('e'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
           // std::cout << "NEW CUR LINE IS" << _currLine << std::endl;
            return _currLine;
        }
        else {
            _currLine = _currLine.substr(_currLine.find('>')+1, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('e'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
           // std::cout << "IN HERE" << std::endl;
           // std::cout << "curr line is " << _currLine << std::endl;
            return _currLine;
        }
    }
    else if(_currLine.find('<') != std::string::npos && _currLine.find("<=") == std::string::npos){
        if(_currLine.find("\'") == std::string::npos && _currLine.find("\"") == std::string::npos){
            _currLine = _currLine.substr(_currLine.find('<')+1, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('e'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
           // std::cout << "NEW CUR LINE IS" << _currLine << std::endl;
            return _currLine;
        }
        else {
            _currLine = _currLine.substr(_currLine.find('<')+1, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('e'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
          //  std::cout << "IN HERE" << std::endl;
           // std::cout << "curr line is " << _currLine << std::endl;
            return _currLine;
        }
    }
    else if(_currLine.find("==") != std::string::npos){
        if(_currLine.find("\'") == std::string::npos && _currLine.find("\"") == std::string::npos){
            _currLine = _currLine.substr(_currLine.find_last_of('=')+1, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('e'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
           // std::cout << "NEW CUR LINE IS" << _currLine << std::endl;
            return _currLine;
        }
        else {
            _currLine = _currLine.substr(_currLine.find_last_of('=')+1, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('e'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
           // std::cout << "IN HERE!!!" << std::endl;
            //std::cout << "curr line is " << _currLine << std::endl;
            return _currLine;
        }
    }
    else if(_currLine.find(">=") != std::string::npos){
        if(_currLine.find("\'") == std::string::npos && _currLine.find("\"") == std::string::npos){
            _currLine = _currLine.substr(_currLine.find('>')+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('e'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
            //std::cout << "NEW CUR LINE IS " << _currLine << std::endl;
            return _currLine;
        }
        else {
            _currLine = _currLine.substr(_currLine.find('>')+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('e'));
           // std::cout << "IN HERE" << std::endl;
            //std::cout << "curr line is " << _currLine << std::endl;
            return _currLine;
        }
    }
    else if(_currLine.find("<=") != std::string::npos){
        if(_currLine.find("\'") == std::string::npos && _currLine.find("\"") == std::string::npos){
            _currLine = _currLine.substr(_currLine.find('<')+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('e'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
           // std::cout << "NEW CUR LINE IS " << _currLine << std::endl;
            return _currLine;
        }
        else {
            _currLine = _currLine.substr(_currLine.find('<')+2, _currLine.length());
            _currLine = _currLine.substr(0, _currLine.find('e'));
            _currLine.erase(remove_if(_currLine.begin(), _currLine.end(), ::isspace), _currLine.end());
           // std::cout << "NEW CUR LINE IS " << _currLine << std::endl;
            return _currLine;
        }
    }
    return "Nothing being compared";

}

std::string parse_if_else_values(std::string _currLine, bool _parseIf){
    std::string _ifValue;
    std::string _elseValue;

    if(_parseIf){
        _ifValue = _currLine.substr(_currLine.find_first_of("=")+1, _currLine.find("if")-2);
        _ifValue.erase(remove_if(_ifValue.begin(), _ifValue.end(), ::isspace), _ifValue.end());
        return _ifValue;
    }
    else{
        _elseValue = _currLine.substr(_currLine.find_first_of("else")+4, _currLine.length());
        _elseValue.erase(remove_if(_elseValue.begin(), _elseValue.end(), ::isspace), _elseValue.end());
        return _elseValue;
    }
    return "No values";
}

void evaluate_comparison(classTemplate<std::string,std::string> &_dataTypes, classTemplate<std::string,std::string> &_strVars,
    classTemplate<std::string,int> &_intVars, std::string _varName, std::string _varCompared, std::string _currLine){
    if(_intVars.is_in_map(_varCompared)){
        std::string _comparedValue = parse_operand_comparison(_currLine);
        //std::cout << "compared variable is " << _comparedValue << std::endl;
        if(_currLine.find('>') != std::string::npos && _currLine.find(">=") == std::string::npos){
            if(_intVars.is_in_map(_comparedValue)){
                if(_intVars.get_value(_varCompared) > _intVars.get_value(_comparedValue)){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "integer");
                _intVars.insert_into_map(_varName, stoi(if_value_true));
                //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
                }
                else{
                    std::string if_value_true = parse_if_else_values(_currLine, false);
                    _dataTypes.insert_into_map(_varName, "integer");
                    _intVars.insert_into_map(_varName, stoi(if_value_true));
                    //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
                }
            }
            else if(_intVars.get_value(_varCompared) > stoi(_comparedValue)){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "integer");
                _intVars.insert_into_map(_varName, stoi(if_value_true));
                //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
            }
            else{
                std::string if_value_true = parse_if_else_values(_currLine, false);
                _dataTypes.insert_into_map(_varName, "integer");
                _intVars.insert_into_map(_varName, stoi(if_value_true));
                //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
            }
        }
        else if(_currLine.find('<') != std::string::npos && _currLine.find("<=") == std::string::npos){
            if(_intVars.is_in_map(_comparedValue)){
                if(_intVars.get_value(_varCompared) < _intVars.get_value(_comparedValue)){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "integer");
                _intVars.insert_into_map(_varName, stoi(if_value_true));
                //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
                }
                else{
                    std::string if_value_true = parse_if_else_values(_currLine, false);
                    _dataTypes.insert_into_map(_varName, "integer");
                    _intVars.insert_into_map(_varName, stoi(if_value_true));
                    //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
                }
            }
            else if(_intVars.get_value(_varCompared) < stoi(_comparedValue)){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "integer");
                _intVars.insert_into_map(_varName, stoi(if_value_true));
                //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
            }
            else{
                std::string if_value_true = parse_if_else_values(_currLine, false);
                _dataTypes.insert_into_map(_varName, "integer");
                _intVars.insert_into_map(_varName, stoi(if_value_true));
                //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
            }
        }
        else if(_currLine.find("==") != std::string::npos){
            if(_intVars.is_in_map(_comparedValue)){
                if(_intVars.get_value(_varCompared) == _intVars.get_value(_comparedValue)){
                    std::string if_value_true = parse_if_else_values(_currLine, true);
                    _dataTypes.insert_into_map(_varName, "integer");
                    _intVars.insert_into_map(_varName, stoi(if_value_true));
                    //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
                }
                else{
                    std::string if_value_true = parse_if_else_values(_currLine, false);
                    _dataTypes.insert_into_map(_varName, "integer");
                    _intVars.insert_into_map(_varName, stoi(if_value_true));
                    //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
                }
            }
            else if(_intVars.get_value(_varCompared) == stoi(_comparedValue)){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "integer");
                _intVars.insert_into_map(_varName, stoi(if_value_true));
                //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
            }
            else{
                std::string if_value_true = parse_if_else_values(_currLine, false);
                _dataTypes.insert_into_map(_varName, "integer");
                _intVars.insert_into_map(_varName, stoi(if_value_true));
                //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
            }
        }
        else if(_currLine.find(">=") != std::string::npos){
            if(_intVars.is_in_map(_comparedValue)){
                if(_intVars.get_value(_varCompared) >= _intVars.get_value(_comparedValue)){
                    std::string if_value_true = parse_if_else_values(_currLine, true);
                    _dataTypes.insert_into_map(_varName, "integer");
                    _intVars.insert_into_map(_varName, stoi(if_value_true));
                    //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
                }
                else{
                    std::string if_value_true = parse_if_else_values(_currLine, false);
                    _dataTypes.insert_into_map(_varName, "integer");
                    _intVars.insert_into_map(_varName, stoi(if_value_true));
                    //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
                }
            }
            else if(_intVars.get_value(_varCompared) >= stoi(_comparedValue)){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "integer");
                _intVars.insert_into_map(_varName, stoi(if_value_true));
                //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
            }
            else{
                std::string if_value_true = parse_if_else_values(_currLine, false);
                _dataTypes.insert_into_map(_varName, "integer");
                _intVars.insert_into_map(_varName, stoi(if_value_true));
                //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
            }
        }
        else if(_currLine.find("<=") != std::string::npos){
            if(_intVars.is_in_map(_comparedValue)){
                if(_intVars.get_value(_varCompared) <= _intVars.get_value(_comparedValue)){
                    std::string if_value_true = parse_if_else_values(_currLine, true);
                    _dataTypes.insert_into_map(_varName, "integer");
                    _intVars.insert_into_map(_varName, stoi(if_value_true));
                    //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
                }
                else{
                    std::string if_value_true = parse_if_else_values(_currLine, false);
                    _dataTypes.insert_into_map(_varName, "integer");
                    _intVars.insert_into_map(_varName, stoi(if_value_true));
                    //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
                }
            }
            else if(_intVars.get_value(_varCompared) <= stoi(_comparedValue)){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "integer");
                _intVars.insert_into_map(_varName, stoi(if_value_true));
                //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
            }
            else{
                std::string if_value_true = parse_if_else_values(_currLine, false);
                _dataTypes.insert_into_map(_varName, "integer");
                _intVars.insert_into_map(_varName, stoi(if_value_true));
                //std::cout << "The items in the int map are: " << _intVars.get_value(_varName) << std::endl;
            }
        }
    }
    else if(_strVars.is_in_map(_varCompared)){ //ask also if strings can be compared
        std::string _comparedValue = parse_operand_comparison(_currLine);
        //std::cout << "compared variable is " << _comparedValue << std::endl;
         if(_currLine.find('>') != std::string::npos && _currLine.find(">=") == std::string::npos){
            if(_strVars.is_in_map(_comparedValue)){
                if(_strVars.get_value(_varCompared) > _strVars.get_value(_comparedValue)){
                    std::string if_value_true = parse_if_else_values(_currLine, true);
                    _dataTypes.insert_into_map(_varName, "string");
                    _strVars.insert_into_map(_varName, if_value_true);
                    //std::cout << "The items in the data map are: " << _dataTypes.get_value(_varName) << std::endl;
                }
                else{
                    std::string if_value_true = parse_if_else_values(_currLine, false);
                    _dataTypes.insert_into_map(_varName, "string");
                    _strVars.insert_into_map(_varName, if_value_true);
                    //std::cout << "The items in the data map are: " << _dataTypes.get_value(_varName) << std::endl;
                }
            }
            else if(_strVars.get_value(_varCompared) > _strVars.get_value(_comparedValue)){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "string");
                _strVars.insert_into_map(_varName, if_value_true);
                //std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
            }
            else{
                std::string if_value_true = parse_if_else_values(_currLine, false);
                _dataTypes.insert_into_map(_varName, "string");
                _strVars.insert_into_map(_varName, if_value_true);
                //std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
            }
        }
        else if(_currLine.find('<') != std::string::npos && _currLine.find("<=") == std::string::npos){
            if(_strVars.is_in_map(_comparedValue)){
                if(_strVars.get_value(_varCompared) < _strVars.get_value(_comparedValue)){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "string");
                _strVars.insert_into_map(_varName, if_value_true);
                //std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
                }
                else{
                    std::string if_value_true = parse_if_else_values(_currLine, false);
                    _dataTypes.insert_into_map(_varName, "string");
                    _strVars.insert_into_map(_varName, if_value_true);
                    //std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
                }
            }
            else if(_strVars.get_value(_varCompared) < _comparedValue){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "string");
                _strVars.insert_into_map(_varName, if_value_true);
                //std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
            }
            else{
                std::string if_value_true = parse_if_else_values(_currLine, false);
                _dataTypes.insert_into_map(_varName, "string");
                _strVars.insert_into_map(_varName, if_value_true);
                //std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
            }
        }
        else if(_currLine.find("==") != std::string::npos){
            if(_strVars.is_in_map(_comparedValue)){
                if(_strVars.get_value(_varCompared) == _strVars.get_value(_comparedValue)){
                    std::string if_value_true = parse_if_else_values(_currLine, true);
                    _dataTypes.insert_into_map(_varName, "string");
                    _strVars.insert_into_map(_varName, if_value_true);
                    //std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
                }
                else{
                    std::string if_value_true = parse_if_else_values(_currLine, false);
                    _dataTypes.insert_into_map(_varName, "string");
                    _strVars.insert_into_map(_varName, if_value_true);
                   // std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
                }
            }
            else if(_strVars.get_value(_varCompared) == _comparedValue){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "string");
                _strVars.insert_into_map(_varName, if_value_true);
                //std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
            }
            else{
                std::string if_value_true = parse_if_else_values(_currLine, false);
                _dataTypes.insert_into_map(_varName, "string");
                _strVars.insert_into_map(_varName, if_value_true);
                //std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
            }
        }
        else if(_currLine.find(">=") != std::string::npos){
            if(_strVars.is_in_map(_comparedValue)){
                if(_strVars.get_value(_varCompared) >= _strVars.get_value(_comparedValue)){
                    std::string if_value_true = parse_if_else_values(_currLine, true);
                    _dataTypes.insert_into_map(_varName, "string");
                    _strVars.insert_into_map(_varName, if_value_true);
                   // std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
                }
                else{
                    std::string if_value_true = parse_if_else_values(_currLine, false);
                    _dataTypes.insert_into_map(_varName, "string");
                    _strVars.insert_into_map(_varName, if_value_true);
                   // std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
                }
            }
            else if(_strVars.get_value(_varCompared) >_comparedValue){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "string");
                _strVars.insert_into_map(_varName, if_value_true);
                //std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
            }
            else {
                std::string if_value_true = parse_if_else_values(_currLine, false);
                _dataTypes.insert_into_map(_varName, "string");
                _strVars.insert_into_map(_varName, if_value_true);
                //std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
            }
        }
        else if(_currLine.find("<=") != std::string::npos){
            if(_strVars.is_in_map(_comparedValue)){
                if(_strVars.get_value(_varCompared) <= _strVars.get_value(_comparedValue)){
                    std::string if_value_true = parse_if_else_values(_currLine, true);
                    _dataTypes.insert_into_map(_varName, "string");
                    _strVars.insert_into_map(_varName, if_value_true);
                   // std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
                }
                else{
                    std::string if_value_true = parse_if_else_values(_currLine, false);
                    _dataTypes.insert_into_map(_varName, "string");
                    _strVars.insert_into_map(_varName, if_value_true);
                   // std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
                }
            }
            else if(_strVars.get_value(_varCompared) >= _comparedValue){
                std::string if_value_true = parse_if_else_values(_currLine, true);
                _dataTypes.insert_into_map(_varName, "string");
                _strVars.insert_into_map(_varName, if_value_true);
               // std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
            }
            else{
                std::string if_value_true = parse_if_else_values(_currLine, false);
                _dataTypes.insert_into_map(_varName, "string");
                _strVars.insert_into_map(_varName, if_value_true);
               // std::cout << "The items in the string map are: " << _strVars.get_value(_varName) << std::endl;
            }
        }
    }
}

void evaluate_if_else_(classTemplate<std::string,std::string> &_dataTypes, classTemplate<std::string,std::string> &_strVars,
    classTemplate<std::string,int> &_intVars, std::string _currLine){
        //std::cout << _currLine << std::endl;
        std::string _varName = get_variable_name(_currLine);
        //std::cout << _varName << std::endl;
        std::string _varCompared = parse_compared_variable(_currLine);
        //std::cout << _varCompared << std::endl;
        evaluate_comparison(_dataTypes, _strVars, _intVars, _varName, _varCompared, _currLine);


    }