#ifndef PARAMPARSER_H
#define PARAMPARSER_H

#include <QString>
#include <vector>
#include <QDebug>

struct ParsingException
{
    // variable initialization in constructor
    // : foo(value)
    ParsingException(QString msg_) : msg(msg_) {}
    QString msg;
};

class ParamParser
{
public:
    /// study point: const QString& ?? why??
    ParamParser(const QString& stringParams)
    {        parseParams(stringParams);
//        parseParams_withExceptoin(stringParams);
    }

    const std::vector<int>& getParams()
    {
        return params;
    }

private:    // don't care how data is processed, just get the result

    // "return" based exception handling
    void parseParams(const QString& stringParams)
    {
        QStringList paramsList = stringParams.split(",", QString::SkipEmptyParts);
        qDebug() << "parse" << stringParams << "to" << paramsList;

        bool success=false;
        for(int i=0; i<paramsList.size(); i++)
        {
            int value = paramsList[i].toInt(&success);
            if(!success)
            {
                qDebug() << "wrong param format" << paramsList[i];
                return;
            }
            params.push_back(value);
        }
    }

    // "throw" based exception handling
    void parseParams_withExceptoin(const QString& stringParams)
    {
        QStringList paramsList = stringParams.split(",", QString::SkipEmptyParts);
        qDebug() << "parse" << stringParams << "to" << paramsList;

        int idx=0;
        bool success=false;
        // study point: auto??
        for(auto& param: paramsList)
        {
            int value = param.toInt(&success);
            if(!success)
                throw ParsingException(QString("wrong param") + param);
            params.push_back(value);
            idx++;
        }
    }

    std::vector<int> params;
};

#endif // PARAMPARSER_H
