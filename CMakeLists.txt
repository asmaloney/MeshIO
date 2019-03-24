# MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required( VERSION 3.0 )

option( PLUGIN-3rdParty_MESH_IO "Check to install the MeshIO plugin" OFF )

if ( PLUGIN-3rdParty_MESH_IO )
	project( MeshIO )

	include_directories( BEFORE extern/assimp/include )
	include_directories( src )

	add_subdirectory( src )
	
	# Turn off the assimp things we don't want
	set( BUILD_SHARED_LIBS FALSE CACHE INTERNAL "override ASSIMP flags" FORCE )
	set( ASSIMP_ANDROID_JNIIOSYSTEM FALSE CACHE INTERNAL "override ASSIMP flags" FORCE )
	set( ASSIMP_BUILD_ASSIMP_TOOLS FALSE CACHE INTERNAL "override ASSIMP flags" FORCE )
	set( ASSIMP_BUILD_NONFREE_C4D_IMPORTER FALSE CACHE INTERNAL "override ASSIMP flags" FORCE )
	set( ASSIMP_BUILD_TESTS FALSE CACHE INTERNAL "override ASSIMP flags" FORCE )
	set( ASSIMP_INSTALL_PDB FALSE CACHE INTERNAL "override ASSIMP flags" FORCE )
	set( ASSIMP_NO_EXPORT TRUE CACHE INTERNAL "override ASSIMP flags" FORCE )
	
	# Turn off all importers and exporters, then enable only the ones we want
	set( ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE INTERNAL "override ASSIMP flags" FORCE )
	set( ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT FALSE CACHE INTERNAL "override ASSIMP flags" FORCE )
	
	set( ASSIMP_BUILD_COLLADA_IMPORTER TRUE CACHE INTERNAL "override ASSIMP flags" FORCE )
	set( ASSIMP_BUILD_GLTF_IMPORTER TRUE CACHE INTERNAL "override ASSIMP flags" FORCE )
	set( ASSIMP_BUILD_IFC_IMPORTER TRUE CACHE INTERNAL "override ASSIMP flags" FORCE )
	
	add_subdirectory( extern/assimp )
	
	include( ../../CMakePluginTpl.cmake )

	target_link_libraries( ${PROJECT_NAME} assimp )
endif()
