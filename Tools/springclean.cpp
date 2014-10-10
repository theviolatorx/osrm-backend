/*

Copyright (c) 2013, Project OSRM, Dennis Luxen, others
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list
of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <cstdio>

#include "../DataStructures/SharedMemoryFactory.h"
#include "../Server/DataStructures/SharedDataType.h"
#include "../Util/SimpleLogger.h"

void delete_region(const SharedDataType region)
{
    if (SharedMemory::RegionExists(region) && !SharedMemory::Remove(region))
    {
        const std::string name = [&]
        {
            switch (region)
            {
            case CURRENT_REGIONS:
                return "CURRENT_REGIONS";
            case LAYOUT_1:
                return "LAYOUT_1";
            case DATA_1:
                return "DATA_1";
            case LAYOUT_2:
                return "LAYOUT_2";
            case DATA_2:
                return "DATA_2";
            case LAYOUT_NONE:
                return "LAYOUT_NONE";
            default: // DATA_NONE:
                return "DATA_NONE";
            }
        }();

        SimpleLogger().Write(logWARNING) << "could not delete shared memory region " << name;
    }
}

// find all existing shmem regions and remove them.
void springclean()
{
    SimpleLogger().Write() << "spring-cleaning all shared memory regions";
    delete_region(DATA_1);
    delete_region(LAYOUT_1);
    delete_region(DATA_2);
    delete_region(LAYOUT_2);
    delete_region(CURRENT_REGIONS);
}

int main()
{
    LogPolicy::GetInstance().Unmute();
    SimpleLogger().Write() << "ATTENTION! BE CAREFUL!";
    SimpleLogger().Write() << "----------------------";
    SimpleLogger().Write() << "This tool may put osrm-routed into an undefined state!";
    SimpleLogger().Write() << "By typing 'Y' you acknowledge that you know what your are doing.";
    SimpleLogger().Write() << "\n\nDo you want to purge all shared memory allocated by osrm-datastore? [type 'Y' to confirm]";

    const auto c = getchar();
    if (c != 'Y')
    {
        SimpleLogger().Write() << "aborted.";
        return 0;
    }
    springclean();
    return 0;
}
