# MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

option( PLUGIN-3rdParty_MESH_IO "Install the MeshIO plugin" OFF )

if ( PLUGIN-3rdParty_MESH_IO )
	# assimp requires at least 3.22, so put it here so it's obivous where it's coming from
	CMAKE_MINIMUM_REQUIRED( VERSION 3.22 )

	project( MeshIO )

	AddPlugin( NAME ${PROJECT_NAME} TYPE io )
	
	target_compile_features( ${PROJECT_NAME}
	    PRIVATE
	        cxx_std_17
	)

	set_target_properties( ${PROJECT_NAME}
		PROPERTIES
			CXX_EXTENSIONS NO
			EXPORT_COMPILE_COMMANDS ON
			POSITION_INDEPENDENT_CODE ON
			INTERPROCEDURAL_OPTIMIZATION ON
			INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF
	)

	include_directories( BEFORE extern/assimp/include )

	add_subdirectory( include )
	add_subdirectory( src )
	
	# Turn off the assimp things we don't want
	set( BUILD_SHARED_LIBS FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_ANDROID_JNIIOSYSTEM FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_ASSIMP_TOOLS FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_DOCS FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_DRACO FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_FRAMEWORK FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_NONFREE_C4D_IMPORTER FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_SAMPLES FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_TESTS FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_COVERALLS FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_DOUBLE_PRECISION FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_HUNTER_ENABLED FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_INSTALL FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_INSTALL_PDB FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_NO_EXPORT TRUE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_OPT_BUILD_PACKAGES TRUE CACHE INTERNAL "override ASSIMP flags" )
	
	# Turn off all importers and exporters, then enable only the ones we want
	set( ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT FALSE CACHE INTERNAL "override ASSIMP flags" )
	
	set( ASSIMP_BUILD_COLLADA_IMPORTER TRUE CACHE INTERNAL "override ASSIMP flags" FORCE )
	set( ASSIMP_BUILD_GLTF_IMPORTER TRUE CACHE INTERNAL "override ASSIMP flags" FORCE )
	set( ASSIMP_BUILD_IFC_IMPORTER TRUE CACHE INTERNAL "override ASSIMP flags" FORCE )
	
	# Linux needs zlib built with -fPIC, so build from source
	if ( UNIX AND NOT APPLE )
		set( ASSIMP_BUILD_ZLIB TRUE CACHE INTERNAL "override ASSIMP flags" )
	endif()
	
	add_subdirectory( extern/assimp EXCLUDE_FROM_ALL )
		
	# See: https://github.com/asmaloney/MeshIO/issues/1
	if ( AMD64 )
		message( WARNING "The AMD64 option may cause link errors. If you run into link errors regarding 'inflate_fast', try turning this off." )
	endif()

	target_link_libraries( ${PROJECT_NAME} assimp )
endif()
