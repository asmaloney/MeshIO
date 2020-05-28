# MeshIO Copyright © 2019 Andy Maloney <asmaloney@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause


target_sources( ${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/COLLADA.cpp
		${CMAKE_CURRENT_LIST_DIR}/glTF.cpp
		${CMAKE_CURRENT_LIST_DIR}/IFC.cpp
		${CMAKE_CURRENT_LIST_DIR}/MeshIO.cpp
		${CMAKE_CURRENT_LIST_DIR}/mioAbstractLoader.cpp
		${CMAKE_CURRENT_LIST_DIR}/mioUtils.cpp
)
