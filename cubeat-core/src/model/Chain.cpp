#include "model/Chain.hpp"

#include <cstdio>

using namespace psc;
using namespace model;

// 2014.12 Introducing power_factor_table to fine tune the power without figuring out the exact formula.
//         Based on the last formula of:
//         1) (step_amount - 1)*(stepnum)/2 + (stepnum-1)
//         2) (step_amount) + (stepnum-1)

static int const power_factor_table_[10][21] = {
      // chain   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20 ...
    /*cubes3*/{  0,  3,  5,  6,  8, 10, 12, 13, 15, 17, 19, 21, 23, 23, 23, 23, 23, 23, 23, 23},
    /*cubes4*/{  3,  5,  6,  8, 10, 12, 13, 15, 17, 19, 21, 23, 23, 23, 23, 23, 23, 23, 23, 23},
    /*cubes5*/{  4,  6,  8, 10, 12, 13, 15, 17, 19, 21, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},
    /*cubes6*/{  5,  7, 10, 12, 13, 15, 17, 19, 21, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},
    /*cubes7*/{  6,  9, 11, 13, 15, 17, 19, 21, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},
    /*cubes8*/{  7, 10, 13, 15, 17, 19, 21, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},
    /*cubes9*/{  8, 12, 14, 17, 19, 21, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},
   /*cubes10*/{  9, 13, 16, 18, 21, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},
   /*cubes11*/{ 10, 14, 17, 21, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},
   /*cubes12*/{ 11, 15, 19, 22, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23}
   // cubes more than 12 will resort to special case: always (step_amount) + (step-1)*3
};

void Chain::update_power_factor() {
    update_power_factor_step(chain_step_.size());
//        int new_step_amount = chain_step_.back();
//        if( new_step_amount < 3 ) return;
//        int stepnum = chain_step_.size();
//        if( stepnum == 1 ) {
//            // power_factor_ += (new_step_amount - 2);
//            if( new_step_amount > 3 ) {
//                power_step_.back() = (new_step_amount - 1);
//            }
//        }
//        else {
//            power_step_.back() = (new_step_amount - 1)*(stepnum)/2 + (stepnum-1);
//            //power_factor_ += (new_step_amount - 1)/2*(stepnum) + (stepnum-2);
//            //power_factor_ += (new_step_amount - 1)/2*(stepnum) + (stepnum-2)*2;
//            //power_factor_ += (new_step_amount - 2)*(stepnum);
//            //power_factor_ += (new_step_amount - 1)/2*(stepnum) + utils::power2(stepnum-2);
//            //power_factor_ += (new_step_amount - 1)*(stepnum) + utils::power2(stepnum-2);
//        }
}

void Chain::update_power_factor_step(int const& step) {
    int step_amount = chain_step_[step-1];
    //step: This is for specific step, not the last step of a chain
//    if( step == 1 ) {
//        if( step_amount > chain_amounts_ ) { // notice the strictly-greater sign.
//            power_step_[step-1] = step_amount;
//        } else {
//            power_step_[step-1] = 0;
//            // 2014.6 To cope with chain merge problem, it's totally possible for "weaker" chains to be
//            //        absorbed and cleaned out of its attack power, so have to consider it here.
//        }
//    }
//    else {
//        if( step_amount >= chain_amounts_ ) { // notice the greater-equal sign.
//            power_step_[step-1] = (step_amount) /* *(step)/2 */ + (step-1);
//        } else {
//            power_step_[step-1] = 0;
//        }
//    }
    if( step < 20 ) {
        if( step_amount >= 3 && step_amount <= 12 ) {
            power_step_[step-1] = power_factor_table_[step_amount-3][step-1];
            // step_amount-3 is to compensate for indexing from a C array
        }
        else if( step_amount >= 13 ) {
            power_step_[step-1] = (step_amount) + (step-1)*3;
        } else {
            power_step_[step-1] = 0;
            // 2014.6 To cope with chain merge problem, it's totally possible for "weaker" chains to be
            //        absorbed and cleaned out of its attack power, so have to consider it here.
        }
    } else {
        printf("model::Chain::update_power_factor_step: \n");
        printf("    something must be wrong, it's impossible to have step >= 20\n\n");
    }
}

void Chain::debugging_print()
{
    printf("Chain %x (updated indices: ", this);
    for( std::set<int>::iterator i = step_indices_.begin(), iend = step_indices_.end(); i != iend; ++i ) {
        printf("%d ", *i);
    }
    printf(")\n");
    for( size_t i = 0; i < chain_step_.size(); ++i ) {
        printf("  - step %d / %d cubes / power %d / extending %d: ", i+1, chain_step_[i], power_step_[i], static_cast<int>(is_step_extending_[i]));
        for( std::list< std::pair<int, int> >::iterator j = step_group_[i].begin(), jend = step_group_[i].end(); j != jend; ++j ) {
            printf("(%d,%d) ", j->first, j->second);
        }
        printf("\n");
    }
    printf("\n");
}
