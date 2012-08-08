
#include "data/ViewSetting.hpp"
#include "utils/dictionary.hpp"
#include "Conf.hpp" // we possibly need screen resolution data in the future here

using namespace psc;
using namespace data;

ViewSetting::ViewSetting(utils::map_any const& conf):
    x_offset_( conf.I("xoff") ),
    y_offset_( conf.I("yoff") ),
    z_offset_( 0 ),
    cube_size_( conf.I("csize") ),
    ats_x_( conf.I("garbage_src_x") ),
    ats_y_( conf.I("garbage_src_y") ),
    atf_x_( conf.I("garbage_ene_x") ),
    atf_y_( ats_y_ ),
    abl_btn_x_( conf.I("ability_btn_x") ),
    abl_btn_y_( conf.I("ability_btn_y") )
{
}

