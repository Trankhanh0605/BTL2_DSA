#include "helper.h"

std::vector<float> *simpleEmbedding(const std::string &text)
{
    std::vector<float> *vec = new std::vector<float>();
    std::stringstream ss(text);
    float value;
    while (ss >> value)
    {
        vec->push_back(value);
    }
    return vec;
}

std::vector<float> *simpleEmbedding2(const std::string &text)
{
    std::vector<float> *vec = new std::vector<float>();
    std::stringstream ss(text);
    float value;
    while (ss >> value)
    {
        vec->push_back(value * 2.0f); // nhân 2 mỗi phần tử
    }
    return vec;
}

std::string printVectorRecord(const VectorRecord *record)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    oss << "ID=" << record->id
        << ", RawText=" << record->rawText
        << ", RawLength=" << record->rawLength
        << ", Vector=";

    if (record->vector)
    {
        oss << "[";
        for (int i = 0; i < record->vector->size(); ++i)
        {
            oss << (*record->vector)[i];
            if (i + 1 < record->vector->size())
                oss << ", ";
        }
        oss << "]";
    }

    oss << ", Distance=" << record->distanceFromReference
        << ", Norm=" << record->norm;

    return oss.str();
}

std::string printVectorRecordNoId(const VectorRecord *record)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    oss
        << "RawText=" << record->rawText
        << ", RawLength=" << record->rawLength
        << ", Vector=";

    if (record->vector)
    {
        oss << "[";
        for (int i = 0; i < record->vector->size(); ++i)
        {
            oss << (*record->vector)[i];
            if (i + 1 < record->vector->size())
                oss << ", ";
        }
        oss << "]";
    }

    oss << ", Distance=" << record->distanceFromReference
        << ", Norm=" << record->norm;

    return oss.str();
}

void printforeach(std::vector<float> &vec, int rawLength, std::string &rawText)
{
    std::cout << "rawLength: " << rawLength << ", rawText: " << rawText << ", Vector: [";
    for (size_t i = 0; i < vec.size(); i++)
    {
        std::cout << vec[i];
        if (i != vec.size() - 1)
            std::cout << ", ";
    }
    std::cout << "]\n";
}