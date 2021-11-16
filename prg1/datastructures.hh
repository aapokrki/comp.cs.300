// Datastructures.hh
//
// Student name: Aapo Kärki
// Student email: aapo.karki@tuni.fi
// Student number: H292001

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <set>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>


#include <map>

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

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:
    // Short rationale for estimate:
    unsigned int town_count();

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_all();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: O(N) Ø(1)
    // Short rationale for estimate: Output is searched from an unordered_map with a key value "id". Therefore O(N) Ø(1)
    Name get_town_name(TownID id);

    // Estimate of performance: O(N) Ø(1)
    // Short rationale for estimate: Output is searched from an unordered_map with a key value "id". Therefore O(N) Ø(1)
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: O(N) Ø(1)
    // Short rationale for estimate: Output is searched from an unordered_map with a key value "id". Therefore O(N) Ø(1)
    int get_town_tax(TownID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Returns instantly the made vector in private
    std::vector<TownID> all_towns();

    // Estimate of performance: O(N)
    // Short rationale for estimate: For loop goes through the whole map.
    //                               So it depends linearry on the amount of elements
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Most likely constant. Extremely rare worst case O(N^2)
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: O(n log(n)) + O(n)
    // Short rationale for estimate:
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: O(n log(n)) + O(n)
    // Short rationale for estimate:
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID min_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID max_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_town(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int total_net_tax(TownID id);



private:
    struct Town{
        TownID id_ = "";
        Name name_ = "";
        Coord coord_ = {};
        int tax_ = 0;

        Town* master = nullptr;
        std::vector<Town*> vassals;
        std::vector<TownID> vassals_vec;
    };

    bool id_exists(TownID id);

    bool mastercheck(Town* vassal, Town* master);
    std::vector<TownID> get_masters(Town* town, std::vector<TownID> vec);

    void change_master(Town *town);
    int find_distance(TownID id, Coord c2);

    // Add stuff needed for your class implementation here


    std::vector <Town*> towns_by_ds_vec = {};

    std::unordered_map<TownID,Town*> towns_by_ds = {};



    size_t rec_vassal_path(const Town *town, std::vector<TownID> &current_path, std::vector<TownID> &longest_path);
};

#endif // DATASTRUCTURES_HH
