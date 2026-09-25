#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <array>
#include <cstdint>

std::vector<int> removeDuplicates(const std::vector<int>& input)
{
    std::set<int> unique;
    std::vector<int> result;
    std::size_t prev_size = 0;
    for(auto& x : input )
    {
        unique.insert(x);
        if(prev_size != unique.size())
        {
            result.push_back(x);
            prev_size = unique.size();
        }
    }

    return result;

}

bool findTwoSum(
    const std::vector<int>& input,
    int target,
    int& index1,
    int& index2)
{
    // for(int i = 0;i<input.size() -1;i++)
    // {
    //     for(int j = i+1;j< input.size();j++)
    //     {
    //         if(input[i] + input[j] == target)
    //         {
    //             index1=i;
    //             index2=j;
    //             return true;
    //         }
    //     }
    // }


    std::unordered_map<int,std::size_t> store;

    for(std::size_t i = 0;i < input.size();i++)
    {
        int needed = target - input[i];

        auto it = store.find(needed);
        if(it != store.end())
        {
            index1 = it->second;
            index2 = i;
            return true;
        }
        else{
            store.insert({input[i],i});
        }
    }
    return false;

}


struct CanMessage
{
    uint32_t id;
    std::array<uint8_t, 8> data;
};

std::size_t filterMessages(const std::array<CanMessage, 32>& input,std::size_t inputCount, uint32_t requiredId,
    std::array<CanMessage, 32>& output){

        std::size_t numofelementswritten = 0;

        for(std::size_t i = 0;i < inputCount ;i++)
        {
            if(input[i].id == requiredId)
            {
                if(numofelementswritten  == output.size())
                {
                    break;
                }
                output[numofelementswritten] = input[i];
                ++numofelementswritten;
                
            }
        }

        return numofelementswritten;


    }

const CanMessage* findMessage(
    const std::array<CanMessage, 32>& messages,
    std::size_t count,
    uint32_t id)
{
    for(std::size_t i = 0;i<count;i++)
    {
        if(messages[i].id == id)
        {
            // return messages.data()+(i);
            return &messages[i];
        }
    }
    return nullptr;


}

int main()
{

    std::vector<int> input  = {5,3,2, 7, 11, 15};
    int target = 9;

    int index1=0;
    int index2=0;

     bool result = findTwoSum(input,target,index1,index2);

    std::cout<<"findtwo sum "<<result<<" index1 "<<index1<<"index2 "<<index2;


}