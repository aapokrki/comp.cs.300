// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <iostream>

#include <random>
#include <QDebug>
#include <cmath>
#include <list>
#include <queue>
std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::town_count()
{
    // Replace the line below with your implementation
    return towns_vec.size();
}

void Datastructures::clear_all()
{
    // Replace the line below with your implementation
    if(!towns.empty()){

        for (auto& town : towns){

            delete town.second;
        }
        // resets all maps and stuff
        towns_vec = {};
        towns = {};
        towns_dist = {};


    }
}

bool Datastructures::add_town(TownID id, const Name& name, Coord coord, int tax)
{
    // Replace the line below with your implementation

    if (towns.find(id) == towns.end()){ // O(n)

        Town *town = new Town{id, name, coord, tax,{},{},{},{},{},{}};

        towns[id] = town; // O(log n)

        towns_dist[dist(id,{0,0})] = id; // O(log n)
        towns_vec.push_back(town);  // O(1),
        return true;
    }else{
        return false;
    }
}

Name Datastructures::get_town_name(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    return towns.at(id)->name_; // Worst: O(n), Average: O(1)
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    return towns.at(id)->coord_; // Worst: O(n), Average: O(1)
}

int Datastructures::get_town_tax(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    return towns.at(id)->tax_; // Worst: O(n), Average: O(1)
}

std::vector<TownID> Datastructures::all_towns()
{
    // Replace the line below with your implementation
    std::vector<TownID> all_towns_id = {};
    for(auto town : towns_vec){ // O(N)
        all_towns_id.push_back(town->id_); // O(1)
    }
    return all_towns_id;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{

    std::vector<TownID> towns_name = {};

    for (auto& town : towns){ // O(N)
        if (town.second->name_ == name){
            towns_name.push_back(town.first); // O(1)
        }
    }
    return towns_name;

}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    if (towns.find(id) != towns.end()){ // Worst: O(n), Average: O(1)
        towns.at(id)->name_ = newname; // Worst: O(n), Average: O(1)
        return true;
    }
    return false;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    std::vector<TownID> towns_alpha = {};
    std::multimap<Name, Town*> sort_map_alpha = {};

    for (Town* town : towns_vec){ // O(n)
        sort_map_alpha.insert(std::pair(town->name_, town));// O(log n)
    }

    for (const auto& pair : sort_map_alpha){ // O(n))
        towns_alpha.push_back(pair.second->id_); // O(1)
    }
    return towns_alpha;

}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    std::vector<TownID> towns_coord = {};

    for (const auto& town : towns_dist){ // O(n)
        towns_coord.push_back(town.second);
    }
    return towns_coord;
}


TownID Datastructures::min_distance()
{
    if(towns_vec.size() == 0){return NO_TOWNID;}
    return towns_dist.begin()->second; // O(1)

}


TownID Datastructures::max_distance()
{
    if(towns_vec.size() == 0){return NO_TOWNID;}
    return std::prev(towns_dist.end())->second; // O(1)
}

// checks for vassalship loops etc. illegal stuff
bool Datastructures::mastercheck(Town* vassal, Town* master)
{
    // check if vassal is already paying taxes to another city
    if (vassal->master_ != nullptr){
        return false;
    }

    // Unique case, last master has no master. No loops
    if(master == nullptr){

        return true;
    }

    // No loop or direct back-and-forth connection
    else if (master != vassal){

        return mastercheck(vassal, master->master_); // O(n)

    }
    // Vassalship would be a loop, so return false
    return false;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    // Connect vassals and masters
    // Worst: 2 * O(n), Average: 2 * O(1)
    if(towns.find(vassalid) != towns.end() and towns.find(masterid) != towns.end()){

        if(mastercheck(towns.at(vassalid), towns.at(masterid))){
            towns.at(vassalid)->master_ = towns.at(masterid);
            towns.at(masterid)->vassals_.push_back(towns.at(vassalid)); // O(n)
            return true;
        }
    }
    return false;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    std::vector<TownID> vassals_vec = {};
    if(towns.find(id) != towns.end()){
        std::for_each(towns.at(id)->vassals_.begin(), towns.at(id)->vassals_.end(),[&vassals_vec](Town* t){
            vassals_vec.push_back(t->id_); // O(n)
        });
        return vassals_vec;
    }
    return {NO_TOWNID};
}

//Recursively get all vassals masters' connections to masters
std::vector<TownID> Datastructures::get_masters(Town* town, std::vector<TownID> vec)
{

    if(town->master_ == nullptr){
        vec.push_back(town->id_);
        return vec;
    }

    vec.push_back(town->id_);
    return get_masters(town->master_,vec); //O(n)
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    // Replace the line below with your implementation
    std::vector<TownID> taxer_path_vec = {};

    if (towns.find(id) == towns.end()){ // Worst: O(n), Average: O(1)
        return {NO_TOWNID};
    }
    return get_masters(towns.at(id), taxer_path_vec); //O(n)
}

// changes towns vassals to the towns master
// or just removes connections if town doesn't have a master
void Datastructures::change_master(Town* town)
{
    if(town->master_ == nullptr){

        // Vassals no longer have a master
        for(Town* vassal: town->vassals_){ // O(n)
            vassal->master_ = nullptr;
        }
    }else{

        // mv = 'm'aster->'v'assals
        std::vector<Town*> &mv = town->master_->vassals_;

        // Change vassal connections accordingly
        for(Town* vassal: town->vassals_){ // O(n)
            vassal->master_ = town->master_;
            mv.push_back(vassal); // O(1)

        }
        // Remove town from its master's vassals
        mv.erase(std::remove(mv.begin(),mv.end(),town),mv.end()); // O(n)
    }
}

bool Datastructures::remove_town(TownID id)
{

    if (towns.find(id) != towns.end()){ // Worst: O(n), Average: O(1)

        // Change master connections
        change_master(towns.at(id)); // O(n)

        // remove from towns_vec
        auto const to_rm = std::remove(towns_vec.begin(),
                                           towns_vec.end(), towns.at(id)); //O(n)

        towns_vec.erase(to_rm, towns_vec.end()); //O(n)

        // remove from towns_dist
        for (auto const& pair : towns_dist){ // Worst: O(n)
            if(pair.second == id){
                towns_dist.erase(pair.first); // Worst: O(n) Average: O(1)
            }
        }

        delete towns.at(id);

        // No empty slots left in map
        towns.erase(id); // Worst: O(n) Average: O(1)

        return true;
    }else{

        // cmd_remove_town in mainprogram.cc call for the name before this function.
        // So the program crashes regardless
        return false;
    }
}

// Calculates the distance between a town and given coord
// and returns the distance as an integer
int Datastructures::dist(TownID id, Coord c2){

    int x = towns.at(id)->coord_.x;
    int y = towns.at(id)->coord_.y;
    int dist = sqrt(pow((x-c2.x),2)+pow((y-c2.y),2));

    return dist;

}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::vector<TownID> result = {};
    std::multimap<int, Town*> sort_nearest = {};

    // multimap keeps them in order of distance. No need to sort.
    for (Town* town: towns_vec){ // O(n)
        sort_nearest.insert(std::pair(dist(town->id_,coord), town)); // O(log n)
    }

    for(const auto& pair : sort_nearest){ // O(n)

        result.push_back(pair.second->id_); // O(1)
    }
    return result;
}

//Recursively get the longest vassal path.
unsigned long long Datastructures::rec_vassal_path
(const Town* town, std::vector<TownID>& current_path, std::vector<TownID>& longest_path)
{

    // Reached to the branch end, return current lenght
    if(town->vassals_.empty()){ // O(1)
        return current_path.size();
    }

    for(Town* vassal : town->vassals_){ // O(n)

        // Keep memory of which path is done
        current_path.push_back(vassal->id_); // O(1)

        // Check if current path is longer than previous longest path. Set current as longest
        //
        if(rec_vassal_path(vassal, current_path, longest_path) > longest_path.size()){ // O(n)
            longest_path = current_path;
        }
    }
    // pop away checked towns
    current_path.pop_back(); // O(1)

    return current_path.size();
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    if (towns.find(id) != towns.end()){ // Worst: O(n), Average: O(1)
        std::vector<TownID> current_path = {id};
        std::vector<TownID> longest_path = {id};

        rec_vassal_path(towns.at(id), current_path, longest_path); // O(n)

        return longest_path;
    }
    return {NO_TOWNID};
}

// Recursively get the net tax of one town
double Datastructures::rec_net_tax(Town* town)
{
    int town_total_tax = 0;

    if (town->vassals_.empty()){ // O(1)
        return town->tax_;
    }

    for (Town* t : town->vassals_){ // O(N)

        double vassal_tax = rec_net_tax(t); // O(n)
        town_total_tax += std::floor(0.1 * vassal_tax); // O(1)
    }
    return town_total_tax + town->tax_;
}

int Datastructures::total_net_tax(TownID id)
{
    if(towns.find(id) != towns.end()){ // Worst: O(n), Average: O(1)

        int net_tax = rec_net_tax(towns.at(id)); // O(N)

        // Town has to pay 10% to its master
        if (towns.at(id)->master_ != nullptr){ // O(1)
            net_tax = net_tax - std::floor(net_tax * 0.1); // O(1)
        }
        return net_tax;
    }
    return NO_VALUE;
}

//
// Phase 2 operations
//


void Datastructures::clear_roads()
{
    // Replace the line below with your implementation
    for (auto const& town : towns_vec){ //O(n)
        town->roads_ = {};
    }
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    // Replace the line below with your implementation
    std::set<Road> road_network_set;

    for (const auto& town : towns_vec){

//        road_network.insert(road_network.end(), town->roads_.begin(),town->roads_.end());
        for (TownID road : town->roads_){

            // smaller id is first
            if(town->id_ < road){
                road_network_set.insert({town->id_, road});

            // if smaller id wasn't first: swap and then insert
            }else{
                road_network_set.insert({road, town->id_});
            }

        }
    }
    std::vector<Road> road_network{road_network_set.begin(), road_network_set.end()};

    return road_network;

}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if (towns.find(town1) != towns.end() and towns.find(town2) != towns.end()){

//        towns.at(town1)->roads_.push_back({town1, town2});
//        towns.at(town2)->roads_.push_back({town2, town1});

        towns.at(town1)->roads_.insert(town2);
        towns.at(town2)->roads_.insert(town1);

        return true;
    }

    return false;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    std::vector <TownID> straight_roads = {};
    if(towns.find(id) != towns.end()){

        for(auto const& road : towns.at(id)->roads_){
            straight_roads.push_back(road);

        }
        return straight_roads;
    }
    return {NO_TOWNID};


}

// Since the route doesn't matter.
// BFS and DFS have similar time complexities, but BFS is better
// because the goal is never extremely deep. Which would suit DFS better
// This function has q act as a stack to show that this is just a route.
// not the shortest, not the longest. Just any route.
std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    if(towns.find(fromid) == towns.end() or towns.find(toid) == towns.end()){
        return {NO_TOWNID};
    }



    std::vector<TownID> route_found = {};


    std::list<TownID> q;

    towns.at(fromid)->visited = 1;
    q.push_back(fromid);

    while(!q.empty()){


        //.back for stack type
        //.front for queue type
        TownID u = q.back();


        //dequb
        std::cerr<<std::endl;
        std::cerr << u <<std::endl;
        for(auto const& t : towns.at(u)->pi){

            std::cerr << t << "->";
        }
        std::cerr<<std::endl;
        //dequb




        if(u == toid){
            route_found = towns.at(u)->pi;
            route_found.push_back(toid);
            break;
        }

        //.back for stack type
        //.front for queue type
        q.pop_back();

        for(auto const& road : towns.at(u)->roads_){
            Town* v = towns.at(road);
            if(v->visited == 0){
                v->visited = 1;
                v->d = towns.at(u)->d +1;

                v->pi = towns.at(u)->pi;
                v->pi.push_back(u);

                q.push_back(v->id_);
            }
        }
        towns.at(u)->visited = 2;
    }

    // Reseting everything
    for (auto const& town : towns){
        town.second->d = 0;
        town.second->visited = 0;
        town.second->pi = {};

    }

    return route_found;
}



bool Datastructures::remove_road(TownID town1, TownID town2)
{
    std::unordered_set<TownID>& t1 = towns.at(town1)->roads_;
    std::unordered_set<TownID>& t2 = towns.at(town2)->roads_;


    if(towns.find(town1) == towns.end() or towns.find(town2) == towns.end()){

        return false;
    }

    // Implemented unordered_set instead of vector just because of this function
    // remove if it was shit
    if(t1.find(town2) == t2.end() or t2.find(town1) == t1.end()){

        return false;
    }

    t1.erase(town2);
    t2.erase(town1);

    return true;
}


// Basically the same as any_route, but q acts as a literal queue. first in first out
std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    if(towns.find(fromid) == towns.end() or towns.find(toid) == towns.end()){
        return {NO_TOWNID};
    }



    std::vector<TownID> route_found = {};
    std::list<TownID> q;

    towns.at(fromid)->visited = 1;
    q.push_back(fromid);

    while(!q.empty()){


        //.back for stack type
        //.front for queue type
        TownID u = q.front();

        //dequb
        std::cerr<<std::endl;
        std::cerr << u <<std::endl;
        for(auto const& t : towns.at(u)->pi){

            std::cerr << t << "->";
        }
        std::cerr<<std::endl;
        //dequb

        if(u == toid){
            route_found = towns.at(u)->pi;
            route_found.push_back(toid);
            break;
        }

        //.back for stack type
        //.front for queue type
        q.pop_front();

        for(auto const& road : towns.at(u)->roads_){
            Town* v = towns.at(road);
            if(v->visited == 0){
                v->visited = 1;
                v->d = towns.at(u)->d +1;

                v->pi = towns.at(u)->pi;
                v->pi.push_back(u);


                q.push_back(v->id_);
            }
        }
        towns.at(u)->visited = 2;
    }

    // Reseting everything
    for (auto const& town : towns){
        town.second->d = 0;
        town.second->visited = 0;
        town.second->pi = {};

    }

    return route_found;
}

std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{

    if(towns.find(startid) == towns.end()){
        return {NO_TOWNID};
    }



    std::vector<TownID> route_found = {};
    std::list<TownID> q;
    bool loop_found = false;

    towns.at(startid)->visited = 1;
    q.push_back(startid);

    while(!q.empty() or !loop_found){


        //.back for stack type
        //.front for queue type
        TownID u = q.back();


        //.back for stack type
        //.front for queue type
        q.pop_back();

        for(auto const& road : towns.at(u)->roads_){
            Town* v = towns.at(road);
            if(v->visited == 0){
                v->visited = 1;
                v->d = towns.at(u)->d +1;

                v->pi = towns.at(u)->pi;
                v->pi.push_back(u);

                q.push_back(v->id_);
            }else if(v->visited == 1){

                route_found = towns.at(u)->pi;
                route_found.push_back(u);
                route_found.push_back(v->id_);

                for(TownID t : v->roads_){
                    if(towns.at(t)->visited){
                        route_found.push_back(t);
                        break;
                    }
                }

                loop_found = true;
                break;
            }
        }
        towns.at(u)->visited = 2;
    }

    // Reseting everything
    for (auto const& town : towns){
        town.second->d = 0;
        town.second->visited = 0;
        town.second->pi = {};

    }

    return route_found;

}

std::vector<TownID> Datastructures::shortest_route(TownID /*fromid*/, TownID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("shortest_route()");
}

Distance Datastructures::trim_road_network()
{
    // Replace the line below with your implementation
    throw NotImplemented("trim_road_network()");
}
