# Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

yarp_prepare_plugin(segmentationimage TYPE SegmentationImageConverter
                                      INCLUDE SegmentationImage.h
                                      CATEGORY portmonitor
                                      DEPENDS "ENABLE_yarpcar_portmonitor")

if(NOT SKIP_segmentationimage)
  yarp_add_plugin(yarp_pm_segmentationimage)

  target_sources(yarp_pm_segmentationimage PRIVATE SegmentationImage.cpp
                                                   SegmentationImage.h)
  target_link_libraries(yarp_pm_segmentationimage PRIVATE YARP::YARP_os
                                                          YARP::YARP_sig)
  list(APPEND YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS YARP_os
                                                      YARP_sig)

  yarp_install(TARGETS yarp_pm_segmentationimage
               EXPORT YARP_${YARP_PLUGIN_MASTER}
               COMPONENT ${YARP_PLUGIN_MASTER}
               LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
               ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR}
               YARP_INI DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})

  set(YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS ${YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS} PARENT_SCOPE)

  set_property(TARGET yarp_pm_segmentationimage PROPERTY FOLDER "Plugins/Port Monitor")
endif()
