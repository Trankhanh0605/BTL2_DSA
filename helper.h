#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "src/VectorStore.h"

// Khai báo hàm, không cài đặt ở đây
std::vector<float> *simpleEmbedding(const std::string &text);
std::vector<float> *simpleEmbedding2(const std::string &text);
std::string printVectorRecord(const VectorRecord *record);
void printforeach(std::vector<float> &vec, int rawLength, std::string &rawText);
std::string printVectorRecordNoId(const VectorRecord *record);

#endif // HELPER_H