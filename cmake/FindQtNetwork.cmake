# @file	    FindQtNetwork.cmake
# @brief	CMake script for finding QtNetwork.
# @author	WUZHUOBIN
# @version	1.0
# @since	Oct.26.2018
# @date 	Oct.26.2018
# @log
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			Copyright (c) WUZHUOBIN 											    #
#			All rights reserved.												    #
#			See Copyright.txt													    #
#			This software is distributed WITHOUT ANY WARRANTY; without even		    #
#			the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR	    #
#			PURPOSE.  See the above copyright notice for more information.		    #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Qt5Network
find_package(Qt5Network REQUIRED)
# LIBRARIES
list(
    APPEND
    PROJECT_LIBRARY
    Qt5::Network
)