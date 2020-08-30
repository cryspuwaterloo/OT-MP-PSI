#ifndef __RECON_H__
#define __RECON_H__

#include <NTL/ZZ_p.h>
#include <NTL/ZZ.h>
#include "../global/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include "../global/psi_utils.h"

std::vector<vector<int>> recon_in_bin_x(std::vector<std::vector<Share>> shares, Context context, int m, int max_bin_size, int scheme, int* count);

#endif