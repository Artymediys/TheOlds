#ifndef LZW_STREAMBASE_DOT_H
#define LZW_STREAMBASE_DOT_H

#include <string>

namespace lzw {

template<typename T>
class input_symbol_stream
{
public :
    input_symbol_stream( T & );
    bool operator>>( char &c );
};

template<typename T>
class output_symbol_stream
{
public :
    output_symbol_stream( T &  );
    void operator<<( const std::string &s );
};

const unsigned int EOF_CODE = 256;

template<typename T>
class input_code_stream
{
public :
    input_code_stream( T &, unsigned int );
    bool operator>>( unsigned int &i );
};

template<typename T>
class output_code_stream 
{
public :
    output_code_stream( T &, unsigned int );
    void operator<<( const unsigned int i );
};

}; //namespace lzw

#endif //#ifndef LZW_STREAMBASE_DOT_H
