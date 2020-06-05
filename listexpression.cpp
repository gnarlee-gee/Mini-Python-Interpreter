#include "HW4.h"

int main(int argc,char *argv[]){
    if(argc != 2){
        std::cout << "not enough arguments!\n";
        return 1;
    }
    
    std::fstream _ifs;
    open_read_file(argv[1],_ifs);
    std::string _currLine;
    int _lineNumber = 1;

    // * map for dataTypes, variables (string,ints,lists), and for error messages
    classTemplate<std::string,std::string> _dataTypes;
    classTemplate<std::string,std::string> _strVars;
    classTemplate<std::string,int> _intVars;
    classTemplate<std::string,std::string> _listVars;

    std::queue<std::string> _compiled_code;

    std::string datatype;
    std::string _var_name;
    std::string _raw_operands;
    std::string _processed_operands;
    std::string _re_formed_currLine;
    std::string _print_value;
    std::string _error_string;
    std::string _print_string;

    while(!_ifs.eof()){
        getline(_ifs,_currLine);
        
        if(_currLine.empty()){
            _compiled_code.push(_currLine);
            continue;
        }

        _error_string = "";
        _print_string = "";

        switch(type_of_operation(_currLine)){
            case 0:
                //std::cout << "Operation found!\n\n";
                _raw_operands = process_operation_line(_currLine);
                _processed_operands = process_operands(_raw_operands,_intVars,_listVars,_strVars,_dataTypes);
                
                if(_processed_operands.empty()){
                    _error_string = "#error";
                    break;
                }
                
                _var_name = get_variable_name(_currLine);
                _re_formed_currLine = _var_name + "=" + _processed_operands;

                insert_after_operation(_re_formed_currLine,_intVars,_listVars, _strVars,_dataTypes);

                break;
            case 1:
                _print_string = process_print(_currLine,_intVars,_listVars,_strVars,_dataTypes);
                break;
            case 2:
                evaluate_if_else_(_dataTypes, _strVars, _intVars, _currLine);
                break;
            case 3:
                //std::cout << "regular expression found!\n";
                _var_name = get_variable_name(_currLine);
                datatype = get_datatype(_currLine);
                if(_dataTypes.is_in_map(_var_name)){
                    if(_dataTypes.get_value(_var_name) != datatype){
                        //TO OUTPUT FILE
                        _error_string = "#error";
                    }
                    else{
                        _dataTypes.insert_into_map(_var_name,datatype);
                        help_insert_to_maps(_intVars, _listVars, _strVars, _dataTypes, _var_name, datatype, _currLine);
                    }
                }
                else{
                    _dataTypes.insert_into_map(_var_name,datatype);
                    help_insert_to_maps(_intVars, _listVars, _strVars, _dataTypes, _var_name, datatype, _currLine);
                    //std::cout << "We succesfuly added variable: " << _var_name << " into the map. The data type is currently: " << _dataTypes.get_value(_var_name) << std::endl;
                }
                // if(_listVars.is_in_map(_var_name)){
                //     std::cout << "The items in the list map are: " << _listVars.get_value(_var_name) << std::endl;
                // }
                // else if(_intVars.is_in_map(_var_name)){
                //     std::cout << "The items in the int map are: " << _intVars.get_value(_var_name) << std::endl;
                // }
                // else if(_strVars.is_in_map(_var_name)){
                //     std::cout << "The items in the string map are: " << _strVars.get_value(_var_name) << std::endl;
                // }
                break;
            default:
                std::cout << "This shouldn't pop up!\nLine cause trouble is: ";
                std::cout << _currLine << std::endl;
            }
                    if(!_error_string.empty()){
            _compiled_code.push(_error_string);
        }
        
        _compiled_code.push(_currLine);

        if(!_print_string.empty() && _error_string.empty()){
            _compiled_code.push(_print_string);
        }


        _lineNumber++;
    }
    _ifs.close();


    std::fstream _ofs;
    open_out_file(argv[1],_ofs);

    while(!_compiled_code.empty()){
        _ofs << _compiled_code.front() << std::endl;
        _compiled_code.pop();
    }
    _ofs.close();
}