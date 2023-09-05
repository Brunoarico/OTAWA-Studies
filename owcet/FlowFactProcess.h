#ifndef FLOWFACTPROCESS_H
#define FLOWFACTPROCESS_H

#include <vector>
#include <string>
#include <map>

class FlowFactProcess {
private:
    std::map<unsigned int, int> dict;

public:
    void load(const std::string& filename);
    void print();
    int get(unsigned int addr);
};  


#endif // FLOWFACTPROCESS_H
