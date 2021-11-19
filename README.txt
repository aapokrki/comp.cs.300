prg1-game-of-taxes dokumentti
Aapo Kärki
aapo.karki@tuni.fi
H292001

Tavoitteena oli tehdä mahdollisimman tehokkaat funktiot oikeilla algoritmeilla
ja tietorakenteilla. Ensimmäinen versioni oli varsin hidas ja esimerkiksi
min- ja max distance funktiot eivät timeouttasivat melkein heti.

Älysin pohdinnan jälkeen, että tärkeintä tässä työssä on tehokkuus, 
eikä kurssin opit stl -algoritmeista. 
Niinpä poistin hitaat std::sort ja std::find funktiot ja lähdin hyödyntämään
tietorakenteiden ominaisuuksia. 

Esimerkiksi sorttaamisessa käytän map- tietorakenteiden ominaisuuksia hyödyksi.
Näin muutin jotkin funktiot O(n log(n)) tehokkuudesta jopa vakioaikaisiksi. 

Kompromissina ohjelma käyttää paljon enemmän muistia tehtävien suorittamiseen.
Omalla tietokoneellani tästä ei ole mitään ongelmaa, mutta 8gb tai alle muistia
taitaa olla liian vähän.

Myös esimerkiksi minmas_distance funktiot saavat tuloksensa suoraan eräästä mapista
johon lisätään tietoa aina kun uusi kaupunki on luotu. Nämä funktiot ovat
suuresti tehokkaampia, mutta add_town funktiossa on yksi logaritminen lisäys lisää. Tämä kompromissi on aika selkeä.

