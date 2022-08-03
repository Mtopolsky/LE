//Last Epoch Beta 0.8.5
//rare item generation
//this will probably work only until next patch
//as it is expected to bring implementation of multiplayer
//and with its coming this will likely be patched/bricked

#include <iostream>
#include <fstream>

using namespace std;

string to_string(int src) {
    if (src == 0)
        return "0";

    string res = "";
    while (src != 0) {
        res += src%10 + 48;
        src /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}

int main() {
    fstream f("affix_table", ios::out);           
    int t1[21] = {0,1,2,16,17,18,32,33,34,48,49,50,64,65,66,80,81,82,96,97,98}; // affix tier table
    int stashtab_width = 12;    
    int stashtab_height = 17;  // 0,0 is bottom left corner of the tab
    int type = 21;             // item type (f.e helmet, belt, gloves, ring, etc...)
    int base = 0;              // item base (f.e copper ring, gold ring, coral ring, etc...)
    int rarity = 0;            // white - 0, magic - 1, exalted - 4, unique - 7, set item - 8, legendary - 9
    int iqual = 255;           // quality of implicit (min to max roll within tier)
    int non_rare = 0;          // no idea what this one does but seems like it doesnt affect rare items, todo: try it out on uniqe items
    int forg_pot = 1;          // crafting potential of the item
    int affix_count = 5;       // number of affixes on item, as far as i know this is uncapped (tested up to 10)
    int affix_tier = 0;        // more about these at the bottom of the page
    int affix_type = 0;        // todo: create table of affixes based on tier. 
    int affix_qual = 255;      // quality of affix (min to max roll within tier)
    int lp = 0;                // if item is unique sets LP, if items is legendary/set/rare seems to do nothing
    int tab_id = 40;           // stashtab where the item will be located after adding it
    int quant = 1;
    int format = 0;
    int test_val = 0;
    for(int z=40;z<45;z++)
        for (int x = 0; x < stashtab_width; x++) 
            for (int y = 0; y < stashtab_height; y++) {
                string out = "{\"itemData\":\"\",\"data\":[1,"
                        + to_string(type)
                        + ","
                        + to_string(base)
                        + ","
                        + to_string(rarity)
                        + ","
                        + to_string(iqual)
                        + ","
                        + to_string(iqual)
                        + ","
                        + to_string(non_rare)
                        + ","
                        + to_string(forg_pot)
                        + ","
                        + to_string(affix_count)
                        + ",";
                for (int i = 0; i < affix_count; i++) {
                    out += to_string(affix_tier)
                        + ","
                        + to_string(affix_type)
                        + ","
                        + to_string(affix_qual)
                        + ",";
                    affix_type++;
                    if (affix_type == 256) {
                        affix_type = 0;
                        test_val++;
                        affix_tier = t1[test_val];
                    }
                }

                out += to_string(lp)
                    + "],\"inventoryPosition\":{\"x\":"
                    + to_string(x)
                    + ",\"y\":"
                    + to_string(y)
                    + "},\"tabID\":"
                    + to_string(z)
                    + ",\"quantity\":"
                    + to_string(quant)
                    + ", \"formatVersion\":"
                    + to_string(format)
                    + "},";

                f << out;
            }

    f.close();
    return 0;
}

//these numbers go only up to 255 (1 byte size) but theres more affixes than that,
//so right now its solved through tier table (tier 0,1,2 are all tier 1 affixes, tier 96,97,98 are all tier 7 affixes etc...)
//theres space for 765(255*3) affixes with this distribution
// 
//tier 1 - 0 /1 /2  are three different affixes
//tier 2 - 16/17/18 are those same three affixes but rank higher
//tier 3 - 32/33/34
//tier 4 - 48/49/50
//tier 5 - 64/65/66
//tier 6 - 80/81/82
//tier 7 - 96/97/98
// 
//If affix doesnt have f.e. tier 7 and you set it to tier 7 by this generation, affix value still sits within its max tier.
//even though any affix can be risen to up to tier 7, not every affix has higher tiers than 1, thus rising them doesnt increase their numerical value
//this behavior can be usefull if you want to merge prefered modifier that has only tier(1-5) with unique item into legendary item 
//
//as per 0.8.5 item has to be exalted && it has to have at least one modifier on tier at least 6,
//with custom generation its possible f.e to generate rare item with tier6 affixes, this will not be able to merge with uniq item into legendary
//code as is now, will generate 5 stash tabs full of items that should have all possible combinations of affixes with all tiers, its basically ingame table of all affixes
//if you want table of all affixes outside of game, this guy has it completed already and much more -> https://lastepoch.tunklab.com/
//I am not affiliated with Eleventh Hour Games, custom item generation might be agaist their ToS, so generate items at your own risk.