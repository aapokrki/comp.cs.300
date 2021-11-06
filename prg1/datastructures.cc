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
    // Write any cleanup you need here
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
        Datastructures *town = new Datastructures;
        town->id_ = id;
        town->name_ = name;
        town->coord_ = coord;

        town->dist_ = sqrt(pow(coord.x,2)+pow(coord.y,2));
        town->tax_ = tax;

        towns_by_ds[id] = town; // O(log(n))

        towns_by_ds_vec.push_back(town);  // O(1)
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
    for (Datastructures *town : towns_by_ds_vec){ // O(n) Ω(1)
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
              [](Datastructures* l, Datastructures* r){ // O(n log(n))
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
              [](Datastructures* l, Datastructures* r){ // O(n log(n))
        return l->dist_ < r->dist_;

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


    Datastructures* result = *std::min_element(towns_by_ds_vec.begin(),towns_by_ds_vec.end(),
                                               [](Datastructures* l, Datastructures* r){ // O(n)
        return l->dist_ < r->dist_;
    });

    return result->id_;

}

TownID Datastructures::max_distance()
{
    // Replace the line below with your implementation

     if(towns_by_ds_vec.size() == 0){return NO_TOWNID;}

    Datastructures* result = *std::max_element(towns_by_ds_vec.begin(),towns_by_ds_vec.end(),
                                               [](Datastructures* l, Datastructures* r){ // O(n)
        return l->dist_ < r->dist_;
    });

    return result->id_;
}

bool Datastructures::add_vassalship(TownID /*vassalid*/, TownID /*masterid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("add_vassalship()");
}

std::vector<TownID> Datastructures::get_town_vassals(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("get_town_vassals()");
}

std::vector<TownID> Datastructures::taxer_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("taxer_path()");
}

bool Datastructures::remove_town(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_town()");
}

std::vector<TownID> Datastructures::towns_nearest(Coord /*coord*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("towns_nearest()");
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("longest_vassal_path()");
}

int Datastructures::total_net_tax(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("total_net_tax()");
}
