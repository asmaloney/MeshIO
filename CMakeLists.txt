# MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

option( PLUGIN-3rdParty_MESH_IO "Install the MeshIO plugin" OFF )

if ( PLUGIN-3rdParty_MESH_IO )
	project( MeshIO )

	AddPlugin( NAME ${PROJECT_NAME} TYPE io )
	
	include_directories( BEFORE extern/assimp/include )

	add_subdirectory( include )
	add_subdirectory( src )
	
	# Turn off the assimp things we don't want
	set( BUILD_DOCS FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( BUILD_SHARED_LIBS FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_ANDROID_JNIIOSYSTEM FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_ASSIMP_TOOLS FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_COVERALLS FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_NONFREE_C4D_IMPORTER FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_SAMPLES FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_TESTS FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_INSTALL_PDB FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_NO_EXPORT TRUE CACHE INTERNAL "override ASSIMP flags" )
	
	# Turn off all importers and exporters, then enable only the ones we want
	set( ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE INTERNAL "override ASSIMP flags" )
	set( ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT FALSE CACHE INTERNAL "override ASSIMP flags" )
	
	add_subdirectory( extern/assimp EXCLUDE_FROM_ALL )
	
	ADD_ASSIMP_IMPORTER( COLLADA )
	ADD_ASSIMP_IMPORTER( GLTF )
	ADD_ASSIMP_IMPORTER( IFC )
	
	# See: https://github.com/asmaloney/MeshIO/issues/1
	if ( AMD64 )
		message( WARNING "The AMD64 option may cause link errors. If you run into link errors regarding 'inflate_fast', try turning this off." )
	endif()

	target_link_libraries( ${PROJECT_NAME} assimp )
endif()
