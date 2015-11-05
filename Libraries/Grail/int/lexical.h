//#This comes from Grail-3.3.5
//	This code copyright (c) by the Grail project.
//	No commercial use permitted without written consent.
//	January 1994



/* Added namespace grail, Brandon Stewart, July 2015 */

#ifndef LEXICAL_H
#define LEXICAL_H

namespace grail
{

template<> char	re<int>::re_star = '*';
template<> char	re<int>::re_plus = '|';
template<> char	re<int>::re_cat = '\0';
template<> char*	re<int>::re_estring = (char*)"\"\"";
template<> char	re<int>::re_lparen = '(';
template<> char	re<int>::re_rparen = ')';
template<> char*	re<int>::re_eset = (char*)"{}";
template<> char	re<int>::re_left_delimiter = ' ';
template<> char	re<int>::re_right_delimiter = ' ';
template<> char	re<int>::re_left_symbol_delimiter = '\0';
template<> char	re<int>::re_right_symbol_delimiter = '\0';

template<> char	string<re<int> >::separator = ' ';

}

#endif //LEXICAL_H
