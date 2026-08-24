#include <iostream>
#include <vector>
#include <ranges>
#include <map>
#include <algorithm>

struct Diagnostic
{
    int id;
    int severity;
}; 


int main()
{
    std::vector<int> rpm{
        1000, 2500, 4000, 5500, 6500, 7000, 3000
    };

    auto result = rpm 
                    | std::views::filter([](int rp){return rp >=4000;})
                    | std::views::transform([](int rp){ return rp /100;})
                    | std::views::take(3);

    for(auto x : result)
    {
        std::cout<<" rpm/100 "<<x<<std::endl;
    }

    std::vector<Diagnostic> diagnostics{
        {10, 2},
        {20, 5},
        {30, 1},
        {40, 4},
        {50, 3}
    };

    auto result_ids = diagnostics 
                    | std::views::filter([](Diagnostic& did){ return did.severity >= 3; })
                    | std::views::transform([](Diagnostic& did){did.severity += 1; return did.id;})
                    | std::views::take(2);
    
    for(auto x : result_ids)
    {
        std::cout<<"result "<<x<<std::endl;
    }

   

    std::map<int,Diagnostic> diagnostics_id{
        {10, {10, 2}},
        {20, {20, 5}},
        {30, {30, 1}},
        {40, {40, 4}},
        {50, {50, 3}}
    };

    auto result_map_id = diagnostics_id
                        | std::views::values
                        | std::views::filter([](Diagnostic& did){return did.severity >=3; })
                        | std::views::transform([](Diagnostic& did){ return did.id;})
                        | std::views::take(2);

   

    for(auto x : result_map_id)
    {
        std::cout<<"map id "<<x<<std::endl;
    }

    std::ranges::sort(diagnostics,std::ranges::greater{},&Diagnostic::severity);


     for(auto x : diagnostics)
    {
        std::cout<<"id "<<x.id<<"seviratity "<<x.severity<<std::endl;
    }

    auto max = std::ranges::max_element(diagnostics,{},&Diagnostic::severity);

    std::cout<<"Max element {"<<max->id<<","<<max->severity<<"}"<<std::endl;

    std::vector<Diagnostic> diagnostics_find{
    {10, 2},
    {20, 5},
    {30, 1},
    {40, 4},
    {50, 3}
    };

    auto it = std::ranges::find_if(diagnostics_find,[](Diagnostic& did){ return did.severity >=4 ; });
    if(it != diagnostics_find.end()) std::cout<<" find if id"<<it->id << " severity "<<it->severity<<std::endl;

    auto ele = std::ranges::find(diagnostics_find,40,&Diagnostic::id);
    if(ele != diagnostics_find.end()) std::cout<<" find if id"<<ele->id << " severity "<<ele->severity<<std::endl;

    if(std::ranges::contains(diagnostics_find,30,&Diagnostic::id)) std::cout<<" element 30 present"<<std::endl;
    else std::cout<<" element 30 not present"<<std::endl;

     if(std::ranges::contains(diagnostics_find,100,&Diagnostic::id)) std::cout<<" element 30 present"<<std::endl;
    else std::cout<<" element 100 not present"<<std::endl;

    if(std::ranges::all_of(diagnostics_find,[](const Diagnostic& did){ return did.severity<=5;})) std::cout<<"  all element severity less than 5"<<std::endl;
    else std::cout<<" some element severity grater than 5"<<std::endl;

    if(std::ranges::any_of(diagnostics_find,[](const Diagnostic& did){ return did.severity >= 5;})) std::cout<<"  some element severity grater than= 5"<<std::endl;
    else std::cout<<" all  element severity less than 5"<<std::endl;

     if(std::ranges::none_of(diagnostics_find,[](const Diagnostic& did){ return did.severity > 5;})) std::cout<<"  no element severity grater than 5"<<std::endl;
    else std::cout<<" some  element severity greater than 5"<<std::endl;

    std::vector<Diagnostic> critical;
    std::ranges::copy_if(diagnostics_find,std::back_inserter(critical),[](const Diagnostic& did){return did.severity >= 4;});

    for(const auto& x: critical) std::cout<<" critical "<<x.id << " sev "<<x.severity<<std::endl;

    std::vector<Diagnostic> task{
    {10, 2},
    {20, 5},
    {30, 1},
    {40, 4},
    {50, 3},
    {60, 5},
    {70, 2}
    };

    auto critical_4 = task
                      | std::views::filter([](const Diagnostic& did){return did.severity >= 4; })
                      | std::views::transform([](const Diagnostic& did){ return did.id;})
                      | std::views::take(3)
                      | std::ranges::to<std::vector>();

    std::ranges::sort(critical_4,std::ranges::greater{});
}

