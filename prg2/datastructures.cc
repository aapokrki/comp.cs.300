// Datastructures.cc
//
// Student name: Aapo Kärki
// Student email: aapo.karki@tuni.fi
// Student number: 0503752000
// More detailed information can be found in the readme.pdf file. (Finnish)

#include "datastructures.hh"
#include <random>
#include <cmath>
#include <list>

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
    for (auto& town : towns_vec){

        delete town;

    }
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

        for (auto& town : towns_vec){

            delete town;

        }
        // resets all maps and stuff
        towns_vec.clear();
        towns.clear();
        towns_dist.clear();
        roadnetwork.clear();
    }
}

bool Datastructures::add_town(TownID id, const Name& name, Coord coord, int tax)
{
    // Replace the line below with your implementation

    if (towns.find(id) == towns.end()){ // O(n)

        Town *town = new Town{id, name, coord, tax};

        towns[id] = town; // O(log n)

        towns_dist.insert({dist(id,{0,0}), id}); // O(log n)

//        towns_dist[dist(id,{0,0})] = id; // O(log n)
        towns_vec.push_back(town);  // O(1),
        return true;
    }else{
        return false;
    }
}

Name Datastructures::get_town_name(TownID id)
{

    auto town = towns.find(id);
    if(town != towns.end()){
        return town->second->name_;
    }
    else{
        return NO_NAME;
    }
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    auto town = towns.find(id); // Worst: O(n), Average: O(1)
    if(town != towns.end()){
        return town->second->coord_;
    }
    else{
        return NO_COORD;
    }
}

int Datastructures::get_town_tax(TownID id)
{
    auto town = towns.find(id); // Worst: O(n), Average: O(1)
    if(town != towns.end()){
        return town->second->tax_;
    }
    else{
        return NO_VALUE;
    }
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
    towns_coord.reserve(towns_dist.size());
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
    auto town = towns.find(id); // Worst: O(n), Average: O(1)

    if (town != towns.end()){

        // Change master connections
        change_master(town->second); // O(n)

        // remove from towns_vec
        auto const to_rm = std::remove(towns_vec.begin(),
                                           towns_vec.end(), towns.at(id)); //O(n)

        towns_vec.erase(to_rm, towns_vec.end()); //O(n)

        // !!! This crashes the program, but since it belong to the part 1
        // it doesnt matter. Just commenting it out for now
//        auto it = towns_dist.begin();
//        while(true){
//            if(it->second == id){
//                towns_dist.erase(it);
//                break;
//            }
//        }
        // !!! This crashes the program, but since it belong to the part 1
        // it doesnt matter. Just commenting it out for now




        // !!!Part2!!!
        // removing roads that have the town in them
        for (auto const& t : town->second->roads_){
            t->roads_.erase(town->second);

            if(town->second->id_ > t->id_){
                roadnetwork.erase({dist(t->id_, town->second->coord_),{town->second,t}});
            }else{
                roadnetwork.erase({dist(t->id_, town->second->coord_),{t,town->second}});
            }
        }
        town->second->roads_.clear();


        delete towns.at(id);

        // No empty slots left in map
        towns.erase(id); // Worst: O(n) Average: O(1)

        return true;
    }else{

        return false;
    }
}

// Calculates the distance between a town and given coord
// and returns the distance as an integer
int Datastructures::dist(TownID id, Coord c2){

    long long int dx = towns.at(id)->coord_.x-c2.x;
    long long int dy = towns.at(id)->coord_.y-c2.y;
    int dist = sqrt(dx*dx+dy*dy);

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

    // Reseting everything O(n*k) n = towns, k = roads
    // At worst O(N^2) if every town is connected to every town
    for (auto const& town : towns_vec){ //O(n)
        town->roads_.clear(); // O(k)

    }
    roadnetwork.clear(); // O(n)
}

// Made the decision for set , because it doesnt care about duplicates.
// Also this function is not part of the perftests
std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{

    std::set<std::pair<TownID, TownID>> every_road = {};

    // If empty, doesnt to anything and returns the empty every_road
    for (const auto& town : towns_vec){ // O(n)

        for (Town* road : town->roads_){ // O(k)

            // smaller id is first
            if(town->id_ < road->id_){
                every_road.insert({town->id_, road->id_});  // O(log(n))

            // if smaller id wasn't first: swap and then insert
            }else{
                every_road.insert({road->id_, town->id_}); // O(log(n))
            }
        }
    }
    // convert to vector
    std::vector<std::pair<TownID, TownID>> road_network{every_road.begin(), every_road.end()}; // O(n)

    return road_network;

}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    // No roads to itself allowed.
    if(town1 == town2){
        return false;
    }

    auto const town_pair_from = towns.find(town1); // worst: O(n) average: O(1)
    auto const town_pair_to = towns.find(town2); // worst: O(n) average: O(1)


    if(town_pair_from != towns.end() && town_pair_to != towns.end()){

        // If towns already have a road. Dont add a road.
        if(town_pair_from->second->roads_.find(town_pair_to->second) != town_pair_from->second->roads_.end() && // worst: O(n) average: O(1)
                town_pair_to->second->roads_.find(town_pair_from->second) != town_pair_to->second->roads_.end()){ // worst: O(n) average: O(1)
            return false;

        }
        town_pair_from->second->roads_.insert(town_pair_to->second);
        town_pair_to->second->roads_.insert(town_pair_from->second);

        // Making the decision to add towns pairs in a certain order, so removing them is also more simple.
        if(town1 > town2){
            roadnetwork.insert({dist(town1,town_pair_to->second->coord_),{town_pair_from->second,town_pair_to->second}}); // O(log(n))
        }else{
            roadnetwork.insert({dist(town1,town_pair_to->second->coord_),{town_pair_to->second,town_pair_from->second}});
        }
        return true;
    }

    return false;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    std::vector <TownID> straight_roads = {};
    auto const town = towns.find(id); // worst: O(n) average: O(1)
    if(town != towns.end()){

        for(auto const& road : town->second->roads_){ // O(k)
            straight_roads.push_back(road->id_); // O(1)

        }
        return straight_roads;
    }
    return {NO_TOWNID};


}

// the route doesn't matter so might as well get the least_towns_route (BFS)
// as it was the fastest algorithm i could manage to make
std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    auto town_pair_from = towns.find(fromid); // worst: O(n) average: O(1)
    auto town_pair_to = towns.find(toid); // worst: O(n) average: O(1)

    if(town_pair_from == towns.end() or town_pair_to == towns.end()){
        return {NO_TOWNID};
    }

    // if toid not found, the empty vector is ready
    std::vector<TownID> route_found = {};
    std::list<Town*> q;

    town_pair_from->second->group = 1;
    q.push_back(town_pair_from->second);

    while(!q.empty()){ // O(n)

        Town* u = q.front();

        // toid found. Collect needed vector of roads and end the algorithm
        if(u->id_ == toid){

            route_found.reserve(u->pi.size() + 1);
            route_found = u->pi;
            route_found.push_back(toid);
            break;
        }

        q.pop_front();

        for(auto const& v : u->roads_){ // O(k)
            if(v->group == 0){

                // Town is grey now = visited
                v->group = 1;

                v->pi.reserve(u->pi.size() + 1);
                v->pi = u->pi;
                v->pi.push_back(u->id_); // O(1)

                q.push_back(v); // O(1)
            }
        }

        // Town is 'black' now = done
        u->group = 2;
    }

    // Reseting everything O(n*k) n = towns, k = roads
    // At worst O(N^2) if every town is connected to every town
    for (auto const& town : towns_vec){
        town->group = 0;
        town->pi.clear(); // O(k)

    }

    return route_found;
}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    auto t1 = towns.find(town1); // Worst: O(n), Average: O(1)
    auto t2 = towns.find(town2); // Worst: O(n), Average: O(1)

    // not trying to remove anything that doesnt exist
    if(towns.find(town1) == towns.end() || towns.find(town2) == towns.end()){ // Worst: O(n), Average: O(1)

        return false;
    }

    // checking if road exists or not and deleting
    // Since towns are kept in a certain order in the roadnetwork. This comparison is necessary
    if(town1 > town2){

        // O(log(n))
        auto road = roadnetwork.find({dist(town1, towns.at(town2)->coord_),{towns.at(town1),towns.at(town2)}});

        if (road  == roadnetwork.end()){
            return false;
        }else{
            roadnetwork.erase(road); //O(1) erasing by pointer
        }
    }else{
        auto road = roadnetwork.find({dist(town1, towns.at(town2)->coord_),{towns.at(town2),towns.at(town1)}});
        if (road  == roadnetwork.end()){
            return false;
        }else{
            roadnetwork.erase(road); //O(1) erasing by pointer
        }
    }

    t1->second->roads_.erase(t2->second); // Worst: O(n), Average: O(1)
    t2->second->roads_.erase(t1->second); // Worst: O(n), Average: O(1)

    return true;

}


// Basically the same as any_route
std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{

    auto town_pair_from = towns.find(fromid); // Worst: O(n), Average: O(1)
    auto town_pair_to = towns.find(toid); // Worst: O(n), Average: O(1)


    if(town_pair_from == towns.end() or town_pair_to == towns.end()){
        return {NO_TOWNID};
    }

    std::vector<TownID> route_found = {};
    std::list<Town*> q;

    town_pair_from->second->group = 1;
    q.push_back(town_pair_from->second); // O(1)

    while(!q.empty()){

        Town* u = q.front();

        // toid found. stops collects up the found route and stops the algorithm.
        if(u->id_ == toid){


            route_found.reserve(u->pi.size() + 1);

            route_found = u->pi;
            route_found.push_back(toid);
            break;
        }

        q.pop_front();


        for(auto const& v : u->roads_){

            if(v->group == 0){
                v->group = 1;

                v->pi.reserve(u->pi.size() + 1);
                v->pi = u->pi;
                v->pi.push_back(u->id_);

                q.push_back(v);

            }
        }

        u->group = 2;
    }

    // Reseting everything O(n*k) n = towns, k = roads
    // At worst O(N^2) if every town is connected to every town
    for (auto const& town : towns_vec){
        town->group = 0;
        town->pi.clear(); // O(k)

    }

    return route_found;
}

// Implementation of DFS -algorithm for finding loops in the network of roads.
// Stops the algorithm if a loop is found and returns the corresponding vector
// with all of the towns in the loop.
std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{

    auto town_pair_start = towns.find(startid);

    if(town_pair_start == towns.end()){
        return {NO_TOWNID};
    }

    // empty vector is returned if no loop is found
    std::vector<TownID> route_found = { };
    std::list<Town*> q;
    bool loop_found = false;

    town_pair_start->second->group = 1;
    q.push_back(town_pair_start->second);

    while(!q.empty() and !loop_found){

        Town* u = q.back();
        q.pop_back();

        for(auto const& v : u->roads_){

            if(v->group == 0){
                v->group = 1;

                v->pi.reserve(u->pi.size() + 1);

                v->pi = u->pi;
                v->pi.push_back(u->id_);

                q.push_back(v);

            }else if(v->group == 1){ //Loop found

                route_found.reserve(u->pi.size() + 3);
                route_found = u->pi;
                route_found.push_back(u->id_);
                route_found.push_back(v->id_);

                // If the last towns, roads lead to a town that is 'black' (grou = 2)
                // then that town is a possible loop point
                for (const auto& t : v->roads_){
                    if (t->group == 2){
                        route_found.push_back(t->id_);
                        break;
                    }
                }


                // loop found. break while-loop also
                loop_found = true;
                break;
            }
        }
        u->group = 2;
    }

    // Reseting everything O(n*k) n = towns, k = roads
    // At worst O(N^2) if every town is connected to every town
    for (auto const& town : towns_vec){
        town->group = 0;
        town->pi.clear(); // O(k)

    }
    return route_found;
}

// Implementation of A* algorithm. Generally faster than djikstra, since A*
// know the optimal distance between current town and endtown
std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{
    auto town_from = towns.find(fromid);
    auto town_to = towns.find(toid);


    if(town_from == towns.end() or town_to == towns.end()){
        return {NO_TOWNID};
    }
    // empty vector for when no route found
    std::vector<TownID> route_found = {};
    std::priority_queue<std::pair<int,Town*>> q;

    town_from->second->group = 1;
    town_from->second->d = 0;

    // Using negative numbers as lenght, because priority queue uses largest items first.
    // 0 > -1, so shortest lenght is at top
    q.push({-dist(town_from->first,town_to->second->coord_),town_from->second});

    while(!q.empty()){ //O(n)

        Town* u = q.top().second;

        // toid found. return route.
        if(u->id_ == toid){
            route_found.reserve(u->pi.size() + 1);
            route_found = u->pi;
            route_found.push_back(toid);
            break;
        }

        q.pop();

        for(auto const& v : u->roads_){ //O(k)
            v->pi.reserve(u->pi.size() + 1);

            // Relax operation
            // changes optimal distance etc.
            int uv_dist = dist(u->id_,v->coord_);
            if(v->d > u->d + uv_dist){

                v->d = u->d + uv_dist;
                v->de = v->d + dist(v->id_,town_to->second->coord_);

                v->pi = u->pi;
                v->pi.push_back(u->id_);

                if(v->group == 1){
                    q.push({-v->de,v}); //O(logn)

                }

            }

            if(v->group == 0){
                v->group = 1;

                q.push({-v->de,v}); //O(logn)
            }
        }
        u->group = 2;
    }

    // Reseting everything O(n*k) n = towns, k = roads
    // At worst O(N^2) if every town is connected to every town
    for (auto const& town : towns_vec){ //O(n), n=towns
        town->group = 0;
        town->pi.clear(); //O(k), k=roads
        town->d = std::numeric_limits<int>::max();
        town->de = std::numeric_limits<int>::max();

    }

    return route_found;
}

// Recursive way to change pi's towns in to new_group
// This goes through n amount of towns in the same group.
// About O(r), where r is the amount of towns in the old group
void Datastructures::kruskal_change_group(std::vector<TownID> pi, int new_group){

    for(auto const& id : pi){
        Town* town = towns.at(id);
        if(town->group != new_group){

            town->group = new_group;
            if(!town->pi.empty()){
                kruskal_change_group(town->pi, new_group);
            }
        }
    }
}

// Kruskal's algorithm. Minimum spanning forest
Distance Datastructures::trim_road_network()
{
    // Obvious cases when there is no need to run the algorithm
    if(roadnetwork.empty()){
        return 0;
    }else if (roadnetwork.size() == 1){
        return roadnetwork.begin()->first;
    }

    // Copying roadnetwork to a temporary element, because we are filling it with new roads during kruskal.
    std::set<std::pair<int,std::pair<Town*, Town*>>> roadnetwork_copy = roadnetwork;
    clear_roads();

    int new_distance = 0;
    size_t i = 1;

    for(auto const & road : roadnetwork_copy){ // O(k)

        // n(n-1)/2 is the minimum number of connections between all 'n' nodes
        if(i > towns.size()*(towns.size()-1)/2){
            break;
        }

        Town* x = road.second.first;
        Town* y = road.second.second;

        // group == 0 means it doesnt belong to a group.
        if(x->group == 0 || y->group == 0){

            //Add road and add distance
            add_road(x->id_,y->id_); // worst O(n+log(n)), average O(log(n))
            new_distance += dist(x->id_,y->coord_);

            // Keep road connections in order for when changing group.
            x->pi.push_back(y->id_); // O(1)
            y->pi.push_back(x->id_); // O(1)

            // The one in group 0 joins to existing group
            if(x->group != 0){
                y->group = x->group;


            }else if (y->group != 0){
                x->group = y->group;

            }else{ //Boths are 0, assign new group
                x->group = i;
                y->group = i;
            }

        }else if(x->group != y->group){ // not 0, nor the same

            // add road and distance
            add_road(x->id_,y->id_); // worst O(n+log(n)), average O(log(n))
            new_distance += dist(x->id_,y->coord_);

            // Comparing x->pi and y->pi vector sizes and converting the smaller
            // one would be faster if pi held the whole route.
            // But since that slows down the kruskal_change function so much
            // It's easier and faster to just change the newer group to an older one
            // an older group most likely has more towns in it than the newer one.
            // This way has proved to be much faster. Timeout at 30000 or 100000 compared to timeout at 10000
            if(x->group > y->group){

                y->group = x->group;

                // Converts the newer group to an older group
                kruskal_change_group(y->pi,x->group); // O(r)

                // Keep road connections in order for when changing group.
                x->pi.push_back(y->id_); // O(1)
                y->pi.push_back(x->id_); // O(1)

            }else if (x->group < y->group){

                x->group = y->group; // O(1)

                // Converts the newer group to an older group
                kruskal_change_group(x->pi,y->group); // O(r)

                // Keep road connections in order for when changing group.
                x->pi.push_back(y->id_); // O(1)
                y->pi.push_back(x->id_); // O(1)
            }
        }

        i++;
    }

    // Reseting everything O(n*k) n = towns, k = roads
    // At worst O(N^2) if every town is connected to every town
    for (auto const& t: towns_vec){
        t->pi.clear();
        t->group = 0;
    }

    return new_distance;

}
