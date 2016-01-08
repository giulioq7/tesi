#include "networkmodel.h"

NetworkModel::NetworkModel()
{
     count = 0;
}

std::string NetworkModel::not_trans(std::string operand)
{
    string res_expr = "(";
    for(unsigned int i=0; i<components.size();i++)
    {
        for(unsigned int j=0; j<components.at(i).model->trans.size() ; j++)
        {
            string t = components.at(i).model->trans.at(j).name;
            string c = components.at(i).name;
            string str(t); str.append("("); str.append(c); str.append(")");
            if(str != operand)
            {
                pair<string,string> p = make_pair(t,c);
                if(conv_str_int.find(p) == conv_str_int.end())
                {
                    conv_str_int[p] = count;
                    conv_int_str[count] = p;
                    count++;
                }
                stringstream ss;
                ss << conv_str_int[p];
                res_expr.append(ss.str());
                res_expr.append("|");
            }

        }
    }

    //removes last pipe "|"
    res_expr.resize(res_expr.size()-1);
    res_expr.append(")");

    return res_expr;
}


NetComponent *NetworkModel::find_component(std::string id)
{
    NetComponent* ref = NULL;
    for(vector<NetComponent>::iterator it = components.begin(); it != components.end(); it++)
    {
        if((*it).name == id)
        {
            ref = &(*it);
            break;
        }
    }
    return ref;
}
