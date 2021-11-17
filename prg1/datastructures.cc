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

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::town_count()
{
    // Replace the line below with your implementation
    return towns_by_ds_vec.size();
}

void Datastructures::clear_all()
{
    // Replace the line below with your implementation
    if(!towns_by_ds.empty()){

        for (auto& town : towns_by_ds){
            delete town.second;

        }
        towns_by_ds_vec = {};
        towns_by_ds = {};

        qDebug() << towns_by_ds.size();
    }
}

bool Datastructures::add_town(TownID id, const Name& name, Coord coord, int tax)
{
    // Replace the line below with your implementation

    bool result = false;
    if (!towns_by_ds.count(id)){ // O(log(n))
        Town *town = new Town{id, name, coord, tax,{},{},{}};

        towns_by_ds[id] = town; // O(log(n))

        towns_by_ds_vec.push_back(town);  // O(1),
        result = true;
    }
    return result;

}

Name Datastructures::get_town_name(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    return towns_by_ds.at(id)->name_; // O(N) Ø(1)
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    return towns_by_ds.at(id)->coord_; // O(N) Ø(1)
}

int Datastructures::get_town_tax(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    return towns_by_ds.at(id)->tax_; // O(N) Ø(1)
}

std::vector<TownID> Datastructures::all_towns()
{
    // Replace the line below with your implementation
    std::vector<TownID> all_towns_id = {};
    for(auto town : towns_by_ds_vec){
        all_towns_id.push_back(town->id_);
    }
    return all_towns_id;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    // Replace the line below with your implementation
    std::vector<TownID> towns_same_name = {};

    for (auto& town : towns_by_ds){ // O(N)
        if (town.second->name_ == name){
            towns_same_name.push_back(town.first); // O(1)
        }
    }
    return towns_same_name;

}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    // Replace the line below with your implementation
    bool result = false;
    for (Town *town : towns_by_ds_vec){ // O(n) Ω(1)
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

    // Replace the line below with your implementation

    std::sort(towns_by_ds_vec.begin(), towns_by_ds_vec.end(),
              [](Town* l, Town* r){ // O(n log(n))
        return l->name_ < r->name_;
    });

    for (auto town : towns_by_ds_vec){ // O(n))
        towns_alpha.push_back(town->id_); // O(1)
    }
    return towns_alpha;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    // Replace the line below with your implementation

    std::vector<TownID> towns_coord = {};

    // Replace the line below with your implementation

    std::sort(towns_by_ds_vec.begin(), towns_by_ds_vec.end(),
              [this](Town* l, Town* r){ // O(n log(n))
        return find_distance(l->id_, {0,0}) < find_distance(r->id_, {0,0});

    });

    for (auto town : towns_by_ds_vec){ // O(n))
        towns_coord.push_back(town->id_); // O(1)
    }

    return towns_coord;
}


TownID Datastructures::min_distance()
{
    // Replace the line below with your implementation

    if(towns_by_ds_vec.size() == 0){return NO_TOWNID;}


    Town* result = *std::min_element(towns_by_ds_vec.begin(),towns_by_ds_vec.end(),
                                               [this](Town* l, Town* r){ // O(n)
            return find_distance(l->id_, {0,0}) < find_distance(r->id_, {0,0});
    });

    return result->id_;

}

TownID Datastructures::max_distance()
{
    // Replace the line below with your implementation

     if(towns_by_ds_vec.size() == 0){return NO_TOWNID;}

    Town* result = *std::max_element(towns_by_ds_vec.begin(),towns_by_ds_vec.end(),
                                               [this](Town* l, Town* r){ // O(n)
            return find_distance(l->id_, {0,0}) < find_distance(r->id_, {0,0});
    });

    return result->id_;
}

bool Datastructures::id_exists(TownID id){

    if (!towns_by_ds.empty()){

        if (std::find_if(towns_by_ds.begin(), towns_by_ds.end(), [id](std::pair<TownID, Town*> i){

                      return i.second->id_ == id; }) != towns_by_ds.end()){

            return true;
        }
    }

    return false;
}

bool Datastructures::mastercheck(Town* vassal, Town* master){

    // check if vassal is already paying taxes to another city
    if (vassal->master != nullptr){
        return false;
    }

    // Unique case, last master has no master. No loops
    if(master == nullptr){
        qDebug() << "nullptr found";
        return true;
    }

    // No loop or direct back-and-forth connection
    else if (master != vassal){

        return mastercheck(vassal, master->master);

    }
    // Vassalship would be a loop, so return false
    return false;


}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid){

    // Connect vassals and masters
    if(id_exists(vassalid) and id_exists(masterid)){

        if(mastercheck(towns_by_ds.at(vassalid), towns_by_ds.at(masterid))){
            towns_by_ds.at(vassalid)->master = towns_by_ds.at(masterid);
            towns_by_ds.at(masterid)->vassals.push_back(towns_by_ds.at(vassalid));
            towns_by_ds.at(masterid)->vassals_vec.push_back(vassalid);
            return true;
        }
    }

    return false;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(id_exists(id)){
        return towns_by_ds.at(id)->vassals_vec;

    }

    return {NO_TOWNID};

}


std::vector<TownID> Datastructures::get_masters(Town* town, std::vector<TownID> vec){

    if(town->master == nullptr){
        vec.push_back(town->id_);
        return vec;
    }

    vec.push_back(town->id_);
    return get_masters(town->master,vec);

}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    // Replace the line below with your implementation
    std::vector<TownID> taxer_path_vec = {};

    if (towns_by_ds.at(id)->master == nullptr){
        return {NO_TOWNID};
    }
    return get_masters(towns_by_ds.at(id), taxer_path_vec);
}

void Datastructures::change_master(Town* town){

    std::vector<Town*> &mv = town->master->vassals;
    std::vector<TownID> &mv_id = town->master->vassals_vec;

    for(Town* vassal: town->vassals){
        vassal->master = town->master;
        mv.push_back(vassal);
        mv_id.push_back(vassal->id_);

    }
    mv.erase(std::remove(mv.begin(),mv.end(),town),mv.end());
    mv_id.erase(std::remove(mv_id.begin(),mv_id.end(),town->id_),mv_id.end());
//    town->vassals = {};
//    town->master = nullptr;
}


bool Datastructures::remove_town(TownID id)
{

    if (id_exists(id)){
        qDebug() << "!!!";

        change_master(towns_by_ds.at(id));

        towns_by_ds_vec.erase(std::remove(towns_by_ds_vec.begin(),
                                          towns_by_ds_vec.end(), towns_by_ds.at(id)), towns_by_ds_vec.end());

        delete towns_by_ds.at(id);
        towns_by_ds.erase(id);
        qDebug() << towns_by_ds.size();
        qDebug() << towns_by_ds_vec.size();

//        qDebug() << "deleted";

        return true;
    }else{
        return false;
    }
}

int Datastructures::find_distance(TownID id, Coord c2){

    int x = towns_by_ds.at(id)->coord_.x;
    int y = towns_by_ds.at(id)->coord_.y;


    int dist = sqrt(pow((x-c2.x),2)+pow((y-c2.y),2));
    qDebug() << dist;
    return dist;

}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::vector<TownID> result = {};


    // O(n)
    for (Town* town: towns_by_ds_vec){

        result.push_back(town->id_);

    }

    // O(nlog(n))
    std::sort(result.begin(), result.end(), [this, &coord](TownID i, TownID j){

        return find_distance(i, coord) < find_distance(j, coord);
    });

    return result;
}

size_t Datastructures::rec_vassal_path(const Town* town, std::vector<TownID>& current_path, std::vector<TownID>& longest_path){

    if(town->vassals.empty()){
        return current_path.size();

    }
    qDebug() << current_path.size();
    for(Town* vassal : town->vassals){
        current_path.push_back(vassal->id_);

        if(rec_vassal_path(vassal, current_path, longest_path) > longest_path.size()){
            longest_path = current_path;
        }

    }
    current_path.pop_back();

    return current_path.size();
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    if (!id_exists(id)){
        return {NO_TOWNID};
    }
    std::vector<TownID> current_path = {id};
    std::vector<TownID> longest_path = {id};

    rec_vassal_path(towns_by_ds.at(id), current_path, longest_path);

    return longest_path;


}

double Datastructures::rec_net_tax(Town* town){


    if (town->vassals.empty()){
        return town->tax_;
    }

    int town_total_tax = 0;

    for (Town* t : town->vassals){

        double tax = rec_net_tax(t);
        town_total_tax += std::floor(0.1 * tax);
    }

    return town_total_tax + town->tax_;

}


int Datastructures::total_net_tax(TownID id)
{
    Town* target = towns_by_ds.at(id);
    if(!id_exists(id)){
        return NO_VALUE;
    }

    int net_tax = rec_net_tax(target);

    if (target->master != nullptr){
        std::cout << target->name_ << std::endl;
        net_tax = net_tax - std::floor(net_tax * 0.1);
    }
    return net_tax;
}
