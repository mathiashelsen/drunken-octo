#include "finance.hpp"

void readDatafile(ifstream *file, vector<vector<double> *> *list)
{
    char_separator<char> sep("\t ");
    if(file->is_open())
    {
	string line;
	while( getline( *file, line ) )
	{
	    tokenizer<char_separator<char> > tokens(line, sep);
	    vector<double> *row = new vector<double>;
	    for( tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); ++it )
	    {
		row->push_back(atof(it->c_str()));
	    }
	    list->push_back(row);
	    //data->push_back( atof( line.c_str() ) );
	}
    }
}

