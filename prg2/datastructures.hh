// Datastructures.cc
//
// Student name: Aapo Kärki
// Student email: aapo.karki@tuni.fi
// Student number: 0503752000

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <unordered_set>
#include <set>
#include <queue>
#include <iostream>
// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

template<typename T>
class custom_priority_queue : public std::priority_queue<T, std::vector<T>>
{
public:
    bool remove(const T& value) {
        auto it = std::find(this->c.begin(), this->c.end(),value);
        if(it != this->c.end()){
//            std::cerr<< "tie poistetaan" <<std::endl;
            this->c.erase(it);
            std::make_heap(this->c.begin(), this->c.end(), this->comp);
            return true;
        }else{
//            std::cerr<< "ei loydy" <<std::endl;
            return false;
        }
    }
};


class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: // O(1)
    // Short rationale for estimate: .size() is constant
    unsigned int town_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop to delete towns
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: adding to map is O(log n), but it has to go through
    //                               the map every time for no duplicates, so O(n)
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: At worst O(N), Average O(1)
    // Short rationale for estimate: unordered_map.at is at Worst: O(n), Average: O(1)
    Name get_town_name(TownID id);

    // Estimate of performance: At worst O(N), Average O(1)
    // Short rationale for estimate: unordered_map.at is at Worst: O(n), Average: O(1)
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: At worst O(N), Average O(1)
    // Short rationale for estimate: unordered_map.at is at Worst: O(n), Average: O(1)
    int get_town_tax(TownID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: For loop goes through the whole map.
    //                               So it depends linearry on the amount of elements
    std::vector<TownID> all_towns();

    // Estimate of performance: O(N)
    // Short rationale for estimate: For loop goes through the whole map.
    //                               So it depends linearry on the amount of elements
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: Worst: O(n), Average: O(1)
    // Short rationale for estimate: multimap.find and .at are constant in average

    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: O(n log(n))
    // Short rationale for estimate: for loop with multimap insert
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop to push_back to vector.
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: // O(1)
    // Short rationale for estimate: Just calling already sorted data
    TownID min_distance();

    // Estimate of performance: // O(1)
    // Short rationale for estimate: Just calling already sorted data
    TownID max_distance();

    // Estimate of performance: O(n)
    // Short rationale for estimate: linear recursive function call and multimap.find
    //                               is at worst O(n), but average is O(1)
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: // O(n)
    // Short rationale for estimate: std::for_each is O(n) so the whole thing is O(n)
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance:O(n)
    // Short rationale for estimate: Recursion that calls itself as long as vassals have masters
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: Worst case O(n^2) Best case: Omega(1)
    // Short rationale for estimate: worst case for loop O(n) with worst case O(n) map.erase
    bool remove_town(TownID id);

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: for loop inserting to a multimap
    //                                  O(n) * O(log n) = O(n log n)
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance: O(n)?
    // Short rationale for estimate: Recursive function call in a for loop seems to be O(n^2)
    //                               but every town in the vassal tree is visited only once!
    //                               Therefore the complexity would be O(n), right?
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: O(n)?
    // Short rationale for estimate: Recursive function call in a for loop seems to be O(n^2)
    //                               but every town in the vassal tree is visited ONLY ONCE!
    //                               Therefore the complexity would be O(n), right?
    int total_net_tax(TownID id);



    // Phase 2 operations

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_roads();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_road_network();

private:
    struct Town{
        //Part 1
        TownID id_ = "";
        Name name_ = "";
        Coord coord_ = {};
        int tax_ = 0;

        Town* master_ = nullptr;
        std::vector<Town*> vassals_ = {};

        //Part 2
        std::unordered_set<Town*> roads_ = {};

        // 0 = not visited
        // 1 = visited aka grey
        // 2 done aka black
        // n > 2, just different groups. Used in trim_road_network kruskal
        int group = 0;

        int d = std::numeric_limits<int>::max();
        int de = std::numeric_limits<int>::max();

        std::vector<TownID> pi = {};

    };



    //Part 1
    std::vector<Town*> towns_vec = {};
    std::unordered_map<TownID,Town*> towns = {};
    std::multimap<int,TownID> towns_dist = {};
    bool mastercheck(Town* vassal, Town* master);
    std::vector<TownID> get_masters(Town* town, std::vector<TownID> vec);
    void change_master(Town *town);
    int dist(TownID id, Coord c2);
    unsigned long long rec_vassal_path(const Town *town, std::vector<TownID> &current_path, std::vector<TownID> &longest_path);
    double rec_net_tax(Town *town);

    //Part 2
    std::set<std::pair<int,std::pair<Town*, Town*>>> roadnetwork = {};
    void kruskal_change_group(std::vector<TownID> pi, int new_group);
};

#endif // DATASTRUCTURES_HH
