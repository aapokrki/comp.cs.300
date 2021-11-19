// Datastructures.cc
//
// Student name: Aapo Kärki
// Student email: aapo.karki@tuni.fi
// Student number: H292001


#include "datastructures.hh"

#include <random>
#include <iostream>
#include <cmath>

#include <QDebug>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{

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
        towns_vec = {};
        towns = {};

//        qDebug() << towns_by_ds.size();
    }
}

bool Datastructures::add_town(TownID id, const Name& name, Coord coord, int tax)
{
    // Replace the line below with your implementation

    bool result = false;
    if (!towns.count(id)){ // O(log(n))
        Town *town = new Town{id, name, coord, tax,{},{}};

        towns[id] = town; // O(log(n))

        towns_vec.push_back(town);  // O(1),
        result = true;
    }
    return result;

}

Name Datastructures::get_town_name(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    return towns.at(id)->name_; // O(N) Ø(1)
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    return towns.at(id)->coord_; // O(N) Ø(1)
}

int Datastructures::get_town_tax(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    return towns.at(id)->tax_; // O(N) Ø(1)
}

std::vector<TownID> Datastructures::all_towns()
{
    // Replace the line below with your implementation
    std::vector<TownID> all_towns_id = {};
    for(auto town : towns_vec){
        all_towns_id.push_back(town->id_);
    }
    return all_towns_id;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    // Replace the line below with your implementation
    std::vector<TownID> towns_same_name = {};

    for (auto& town : towns){ // O(N)
        if (town.second->name_ == name){
            towns_same_name.push_back(town.first); // O(1)
        }
    }
    return towns_same_name;

}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{

    bool result = false;
    for (Town *town : towns_vec){ // O(n) Ω(1)
        if (town->id_ == id){ // O(1)
            town->name_ = newname; // O(1)
            result = true;
            break;
        }
    }
    return result;

}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    std::vector<TownID> towns_alpha = {};
    std::multimap<Name, Town*> sort_map_alpha = {};

    for (Town* town : towns_vec){

        sort_map_alpha.insert(std::pair(town->name_, town));

    }

    for (const auto& pair : sort_map_alpha){ // O(n))
        towns_alpha.push_back(pair.second->id_); // O(1)
    }
    return towns_alpha;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    // Replace the line below with your implementation

    std::vector<TownID> towns_coord = {};

    std::multimap<int, Town*> sort_map_dist = {};
    for (Town* town : towns_vec){

        sort_map_dist.insert(std::pair(dist(town->id_,{0,0}), town));

    }

    for (std::pair<int, Town*> town : sort_map_dist){ // O(n))
        towns_coord.push_back(town.second->id_); // O(1)
    }

    return towns_coord;
}


TownID Datastructures::min_distance()
{
    // Replace the line below with your implementation

    if(towns_vec.size() == 0){return NO_TOWNID;}


//    Town* result = *std::min_element(towns_by_ds_vec.begin(),towns_by_ds_vec.end(),
//                                               [this](Town* l, Town* r){ // O(n)
//            return find_distance(l->id_, {0,0}) < find_distance(r->id_, {0,0});
//    });

    std::map<int, Town*> sort_min = {};
    for (Town* town : towns_vec){

        sort_min.insert(std::pair(dist(town->id_,{0,0}), town));

    }

    return sort_min.begin()->second->id_;

}

TownID Datastructures::max_distance()
{
    // Replace the line below with your implementation

     if(towns_vec.size() == 0){return NO_TOWNID;}

//    Town* result = *std::max_element(towns_by_ds_vec.begin(),towns_by_ds_vec.end(),
//                                               [this](Town* l, Town* r){ // O(n)
//            return find_distance(l->id_, {0,0}) < find_distance(r->id_, {0,0});
//    });
     std::map<int, Town*> sort_max = {};
     for (Town* town : towns_vec){ // O(n)

         sort_max.insert(std::pair(dist(town->id_,{0,0}), town)); //O(log(n))

     }

     return std::prev(sort_max.end())->second->id_;
}


bool Datastructures::mastercheck(Town* vassal, Town* master){

    // check if vassal is already paying taxes to another city
    if (vassal->master_ != nullptr){
        return false;
    }

    // Unique case, last master has no master. No loops
    if(master == nullptr){
//        qDebug() << "nullptr found";
        return true;
    }

    // No loop or direct back-and-forth connection
    else if (master != vassal){

        return mastercheck(vassal, master->master_);

    }
    // Vassalship would be a loop, so return false
    return false;


}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid){

    // Connect vassals and masters
    if(towns.find(vassalid) != towns.end() and towns.find(masterid) != towns.end()){

        if(mastercheck(towns.at(vassalid), towns.at(masterid))){
            towns.at(vassalid)->master_ = towns.at(masterid);
            towns.at(masterid)->vassals_.push_back(towns.at(vassalid));

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
            vassals_vec.push_back(t->id_);
        });
        return vassals_vec;
    }

    return {NO_TOWNID};
}


std::vector<TownID> Datastructures::get_masters(Town* town, std::vector<TownID> vec){

    if(town->master_ == nullptr){
        vec.push_back(town->id_);
        return vec;
    }

    vec.push_back(town->id_);
    return get_masters(town->master_,vec);

}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    // Replace the line below with your implementation
    std::vector<TownID> taxer_path_vec = {};

    if (towns.find(id) == towns.end()){
        return {NO_TOWNID};
    }
    return get_masters(towns.at(id), taxer_path_vec);
}

void Datastructures::change_master(Town* town){


    if(town->master_ == nullptr){

        // Vassals no longer have a master
        for(Town* vassal: town->vassals_){
            vassal->master_ = nullptr;
        }
    }else{

        // mv = master->vassals
        std::vector<Town*> &mv = town->master_->vassals_;

        // Change vassal connections accordingly
        for(Town* vassal: town->vassals_){
            vassal->master_ = town->master_;
            mv.push_back(vassal);

        }
        // Remove town from its master's vassals
        mv.erase(std::remove(mv.begin(),mv.end(),town),mv.end());
    }

}


bool Datastructures::remove_town(TownID id)
{

    if (towns.find(id) != towns.end()){

        // Change master connections
        change_master(towns.at(id));


        towns_vec.erase(std::remove(towns_vec.begin(),
                                          towns_vec.end(), towns.at(id)), towns_vec.end());

        delete towns.at(id);

        // No empty slots in map
        towns.erase(id);

        return true;
    }else{

        // cmd_remove_town in mainprogram.cc call for the name before this function.
        // So the program crashes regardless
        return false;
    }
}

/*
 * Calculates the distance between a town and given coord
 * and returns the distance as an integer
*/
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

    // O(n)
    for (Town* town: towns_vec){

        sort_nearest.insert(std::pair(dist(town->id_,coord), town));

    }

    // O(n)
    for(const auto& pair : sort_nearest){

        result.push_back(pair.second->id_);
    }
    return result;
}

size_t Datastructures::rec_vassal_path(const Town* town, std::vector<TownID>& current_path, std::vector<TownID>& longest_path){

    if(town->vassals_.empty()){
        return current_path.size();

    }

    for(Town* vassal : town->vassals_){
        current_path.push_back(vassal->id_);

        // Check if current path is longer than previous longest path. Set current as longest
        if(rec_vassal_path(vassal, current_path, longest_path) > longest_path.size()){
            longest_path = current_path;
        }

    }
    current_path.pop_back();

    return current_path.size();
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    if (towns.find(id) != towns.end()){
        std::vector<TownID> current_path = {id};
        std::vector<TownID> longest_path = {id};

        rec_vassal_path(towns.at(id), current_path, longest_path);

        return longest_path;
    }


    return {NO_TOWNID};
}

double Datastructures::rec_net_tax(Town* town){


    if (town->vassals_.empty()){
        return town->tax_;
    }

    int town_total_tax = 0;

    for (Town* t : town->vassals_){

        double tax = rec_net_tax(t);
        town_total_tax += std::floor(0.1 * tax);
    }

    return town_total_tax + town->tax_;
}

int Datastructures::total_net_tax(TownID id)
{

    if(towns.find(id) != towns.end()){

        int net_tax = rec_net_tax(towns.at(id));

        // Town has to pay 10% to its master
        if (towns.at(id)->master_ != nullptr){
            net_tax = net_tax - std::floor(net_tax * 0.1);
        }
        return net_tax;
    }

    return NO_VALUE;
}
