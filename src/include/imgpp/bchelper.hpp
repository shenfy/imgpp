#ifndef IMGPP_BCHELPER_H
#define IMGPP_BCHELPER_H

#include "bcdesc.hpp"
namespace imgpp {
BCFormat KHRToBCFormat(int internal_format);
int BCFormatToKHR(BCFormat format);
}
#endif // IMGPP_BCFORMATHELPER_H
